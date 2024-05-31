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
    juego->cantRondas=3;    ///luego pedir por config.txt
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

int contestarPregunta(const void* d, void* d2){
    const tPregunta *pregunta=d;
    tJuego *juego=d2;

    printf("%s\n",pregunta->pregunta);
    verOpcionesPreguntas(pregunta);
    obtenerRespuestaDeTecladoTemporizado(&(juego->respuestas[juego->jugadorActual][juego->rondaActual].respuesta),
                                         &(juego->respuestas[juego->jugadorActual][juego->rondaActual].tiempo),
                                         juego->tiempoLimite);
    printf("Su respuesta es %c y tardo %.4f\n",juego->respuestas[juego->jugadorActual][juego->rondaActual].respuesta,
           juego->respuestas[juego->jugadorActual][juego->rondaActual].tiempo);


    juego->rondaActual++;

    return 1;
}

int juegaJugador(const void* d, void* d2){
    const tJugador *jugador=d;
    tJuego *juego=d2;

    printf("Turno del jugador: %s\n", jugador->nombre);
    juego->rondaActual=0;
    recorrerLista(&juego->listaPreguntas,contestarPregunta,juego);

    system("cls");

    juego->jugadorActual++;
    return 1;
}

int iniciarJuego(tJuego *juego){

    juego->jugadorActual=0;
    recorrerLista(&juego->listaJugadores,juegaJugador,juego);

    return TODO_OK;
}


int calcularResultadosYimprimir(tJuego *juego){
    //puntajesJugadores
    tPregunta pre;
    int i,j,k;
    int hayDuplicadosMejoresTiempo;
    int puntajesJugadores[MAX_CANT_JUGADORES];
    double mejorTiempo;

    ////////////////////aca se calcula los puntajes
    for(k=0;k<juego->cantJug;k++)
        puntajesJugadores[k]=0;

    for(i=0;i<juego->cantRondas;i++){
        determinarMejorTiempoYTiempoDuplicado(juego->respuestas,juego->cantJug,i,
                                                  &mejorTiempo,&hayDuplicadosMejoresTiempo);
        verDatoDeListaEnPos( &juego->listaPreguntas, &pre, sizeof( tPregunta ),i);
        for(j=0;j< juego->cantJug;j++){
            juego->respuestas[j][i].puntaje=calcularPuntaje(juego->respuestas[j][i].respuesta,pre.opcionCorrecta,juego->respuestas[j][i].tiempo,
                                                            juego->tiempoLimite,mejorTiempo,hayDuplicadosMejoresTiempo); //i=ronda actual
            puntajesJugadores[j]+=juego->respuestas[j][i].puntaje;
        }
    }
    system("cls");
    //////////////////aca se imprimir
    puts("resultados.");
    printf("preguntas / jugadores:            ");
    tJugador jugadorActual;
    for(j=0;j< juego->cantJug;j++){
        verDatoDeListaEnPos( &juego->listaJugadores, &jugadorActual, sizeof( tPregunta ),j);
        printf("%.10s ",jugadorActual.nombre);
    }
    puts("");

    for(i=0;i<juego->cantRondas;i++){
        verDatoDeListaEnPos( &juego->listaPreguntas, &pre, sizeof( tPregunta ),i);
        printf("%.30s :",pre.pregunta);
        for(j=0;j< juego->cantJug;j++)
            printf(" %c:%2.1f:%2d   ",juego->respuestas[j][i].respuesta,
                   juego->respuestas[j][i].tiempo,juego->respuestas[j][i].puntaje);
        puts("");
    }
    printf("puntajes totales: ");
    for(j=0;j< juego->cantJug;j++)
        printf("%d   ",puntajesJugadores[j]);

    ///despues se dice quienes son los ganadores a los ganadores
    return TODO_OK;
}

void cerrarJuego(tJuego *juego){
    vaciarLista(&juego->listaJugadores);
    vaciarLista(&juego->listaMejorRes);
    vaciarLista(&juego->listaPreguntas);
}
