#include "juego.h"

void crearJuego(tJuego* juego){
    crearLista( &juego->listaPreguntas );
    crearLista(&juego->listaJugadores);
    crearLista( &juego->listaMejorRes );
    juego->cantJug=0;
    juego->nivelEligido=0;
    juego->tiempoLimite=0;
    srand(time(NULL));
}

int cargarJuego(tJuego* juego){
    juego->tiempoLimite=10; ///luego pedir por config.txt
    juego->cantRondas=2;    ///luego pedir por config.txt
    juego->nivelEligido=1;  ///esto se debe pedir aparte

    cargarJugadores(juego);
    puts("cargando preguntas...");

    cargarPreguntas( &juego->listaPreguntas, "https://664d06f4ede9a2b5565273e6.mockapi.io/PREGUNTAS",
                     juego->nivelEligido, juego->cantRondas );

    return 0;
}

int cargarJugadores ( tJuego *juego )
{
    tJugador jugador;

    puts("Ingrese el nombre de un jugador o FIN si ya ingreso todos los nombres!!!!");
    fflush(stdin);
    gets(jugador.nombre);// se podria usar el fgets pero debemos eliminar el \n del final

    while( strcmpi( "FIN", jugador.nombre ) != 0 )
    {
        jugador.orden = rand();//se podria tambien con un map asignarle un orden y luego ordenar la lista por el orden
        jugador.puntajeTotal=0;
        insertarEnListaOrdenadoConDuplicado( &juego->listaJugadores, &jugador, sizeof( tJugador ),cmpJugadorXOrdenMenAMay );
        juego->cantJug++;
        puts("Ingrese el nombre de un jugador");
        fflush(stdin);
        gets( jugador.nombre );
    }

    int orden=0;
    mapLista(&juego->listaJugadores,ModificarElOrdenJugador,&orden);

    return TODO_OK;
}

static size_t write_callback(void *respuesta, size_t tamDatos, size_t cantDatos, tJsontxt *datosUsuario) {

    size_t tamNuevo = tamDatos * cantDatos ;

    datosUsuario->cadenaJSON = realloc( datosUsuario->cadenaJSON, datosUsuario->tamCadena + tamNuevo + 1 );
    if( ! datosUsuario->cadenaJSON )
        return 0; //no puedo agrandar la cadena

    memcpy( datosUsuario->cadenaJSON + datosUsuario->tamCadena, respuesta, tamNuevo );
    datosUsuario->tamCadena += tamNuevo;

    datosUsuario->cadenaJSON[ datosUsuario->tamCadena ] = '\0';

    return tamNuevo;
}

int inicializarJsonTxt ( tJsontxt *soli )
{
    soli->cadenaJSON = malloc(1);
    if( !soli->cadenaJSON )
        return 0;//no hhay memoria suficiente
    soli->tamCadena = 0;
    soli->cadenaJSON[0] = '\0';
    return 1;
}

int cargarPreguntas ( t_Lista *lista, const char *urlAPI, size_t nivelDifucultad, size_t cantRaunds )
{
    CURL *curl;
    CURLcode coderes;
    cJSON *jsonPreguntas;
    tJsontxt jsonRes;
    tPregunta pregunta;
    int i, cantElem;



    if( ! inicializarJsonTxt( &jsonRes )  )
        return 0;//No tengo donde almacenar la respuesta

    curl = curl_easy_init();
    if( ! curl )
        return 0; //no se pudo inicializar una instancia de curl, no voy a poder realizar la consulta
    curl_easy_setopt( curl, CURLOPT_URL, urlAPI );//le decimos la ruta del api
    curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, write_callback );//como va a manejar cada paquete de datos, la funcion callback
    curl_easy_setopt( curl, CURLOPT_WRITEDATA, &jsonRes ); //lo que necesita la funcion callback
    curl_easy_setopt( curl, CURLOPT_SSL_VERIFYPEER, 0L); //por un tema de verificacion, INVESTIGAR!!

    coderes = curl_easy_perform( curl ); //realizamos la solicitud

    if( coderes != CURLE_OK )
    {
        printf( "Error al realizar la solicitud: %s\n", curl_easy_strerror( coderes ) );
        //deberia finalizar el json de buffer
        return 0; //por el error de la consulta
    }

    jsonPreguntas = cJSON_Parse(jsonRes.cadenaJSON);
    for( i=0; i < cJSON_GetArraySize(jsonPreguntas); i++ )
    {
        parsearPregunta( &pregunta, cJSON_GetArrayItem( jsonPreguntas, i ) );
        if(pregunta.dificultad==nivelDifucultad){
            pregunta.orden=rand();
            aleatorizarRespuestaCorrecta( &pregunta );
            crearLista(&(pregunta.respuestas)); //dado que siempre se usara esto para crear la listas de preguntas
            insertarEnListaOrdenadoConDuplicado( lista, &pregunta, sizeof(tPregunta),cmpOrdenPregunta);
        }
    }

    cJSON_Delete( jsonPreguntas );//liberamos el cjson, tiene una implementacion con memoria dinamica
    curl_easy_cleanup( curl ); //terminamos la solicitud

    cantElem = lista_Filter(lista, filtraXDificultad, &nivelDifucultad);    //esto hay que cambiar ya que esta filtrdo
    while( cantElem > cantRaunds )
    {
        eliminarDeListaEnPos( lista, (rand()+95) % cantElem );//elimino cualquiera hasta tener la cantidad correcta
        cantElem--;
    }
    int orden=0;
    mapLista(lista,ModificarElOrdenPregunta,&orden);
    return 1;
}

void parsearPregunta ( tPregunta *destinoPregun, cJSON *origen )
{
    cJSON *valor;
    valor = cJSON_GetObjectItem( origen, "pregunta" );
    strcpy( destinoPregun->pregunta, valor->valuestring );
    valor = cJSON_GetObjectItem( origen, "resp_correcta" );
    strcpy( (destinoPregun->opciones)[0], valor->valuestring);
    valor = cJSON_GetObjectItem( origen, "opcion_1" );
    strcpy( (destinoPregun->opciones)[1], valor->valuestring );
    valor = cJSON_GetObjectItem( origen, "opcion_2" );
    strcpy( (destinoPregun->opciones)[2], valor->valuestring);
    valor = cJSON_GetObjectItem( origen, "opcion_3" );
    strcpy( (destinoPregun->opciones)[3], valor->valuestring);
    valor = cJSON_GetObjectItem( origen, "nivel" );
    destinoPregun->dificultad = valor->valueint;
}

int contestarPregunta(void* d, void* d2){
    tPregunta *pregunta=d;
    tJuego *juego=d2;
    double tiempoTranscurrido;
    tRespuesta respuesta;

    respuesta.ordenJugador=juego->jugadorActual;    ///aca guardamos el orden
    printf("%s\n",pregunta->pregunta);
    verOpcionesPreguntas(pregunta);
    obtenerRespuestaDeTecladoTemporizado(&(respuesta.respuesta),&tiempoTranscurrido,juego->tiempoLimite);
    respuesta.tiempo=(int)tiempoTranscurrido; //solo se queda con la parte ententera del double

    printf("Su respuesta es %c y tardo %2d segundos \n",respuesta.respuesta,respuesta.tiempo);

    insertarEnListaAlFinalConDuplicados(&pregunta->respuestas,&respuesta,sizeof(respuesta));

    return 1;
}

int juegaJugador(void* d, void* d2){
    tJugador *jugador=d;
    tJuego *juego=d2;

    printf("Turno del jugador: %s\n", jugador->nombre);
    mapLista(&juego->listaPreguntas,contestarPregunta,juego);

    puts("turno terminado,ingrese nueva tecla para continuar");
    getchar();
    system("cls");

    juego->jugadorActual++;
    return 1;
}

int iniciarJuego(tJuego *juego){

    juego->jugadorActual=0;
    mapLista(&juego->listaJugadores,juegaJugador,juego);

    return TODO_OK;
}
/**-------------------------------------------------------------------*/
//lista de respuestas
int calcularPuntajeDeRespuesta(void* d, void* d2){
    tRespuesta* respuesta =d;
    tContexto * c =d2;
    respuesta->puntaje=calcularPuntaje(respuesta->respuesta,c->respuestaCorrecta,respuesta->tiempo,
                                     c->tiempoLimite,c->mejorTiempo,c->existeTiempoMejorDuplicado);
    ///aca cambiarlo por una lista circular o lista circular doble

    tJugador j;
    j.orden=respuesta->ordenJugador;
    insertarEnListaOrdenadoSinDuplicados(&c->jugadores,&j,sizeof(tJugador),
                                         cmpJugadorXOrdenMenAMay,&respuesta->puntaje,sumarPuntos);
    return 1;
}

int mejorTiempoValido(void* d, void* d2){
    tRespuesta* respuesta =d;
    tContexto *c =d2;
    if(respuesta->respuesta==c->respuestaCorrecta){ ///solo tomamos los tiempos y duplicados de aquella respuestas validas
        if(respuesta->tiempo<c->mejorTiempo){
        c->mejorTiempo=respuesta->tiempo;
        c->existeTiempoMejorDuplicado=0;
        }else{
        if(respuesta->tiempo==c->mejorTiempo)
            c->existeTiempoMejorDuplicado=1;
        }
    }

    return 1;
}

int calcularPuntajesDeRondas(void* d, void* d2){
    tPregunta* pregunta =d;
    tContexto* c=d2;
    c->respuestaCorrecta=pregunta->opcionCorrecta;
    c->mejorTiempo=100000;

    mapLista(&pregunta->respuestas,mejorTiempoValido,c);
    mapLista(&pregunta->respuestas,calcularPuntajeDeRespuesta,c);

    return 1;
}

int calcularResultadosYimprimir(tJuego *juego){
    tContexto c;
    c.jugadores=juego->listaJugadores;
    c.tiempoLimite=juego->tiempoLimite;

    mapLista(&juego->listaPreguntas,calcularPuntajesDeRondas,&c);
    system("cls");

    puts("resultados.");
    printf("preguntas / jugadores:                   ");
    mapLista(&juego->listaJugadores,imprimirJugador,stdout);
    puts("");
    mapLista(&juego->listaPreguntas,mostrarPreguntaYimprimirRespuesta,NULL);

    printf("puntajes totales:                        ");   ///puntajes totales ,despues lo veo
    mapLista(&juego->listaJugadores,imprimirPuntajeTotalJugador,stdout);

    return TODO_OK;
}

void cerrarJuego(tJuego *juego){
    vaciarLista(&juego->listaJugadores);
    vaciarLista(&juego->listaMejorRes);
    vaciarLista(&juego->listaPreguntas);
}
