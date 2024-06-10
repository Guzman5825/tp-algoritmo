#include "juego.h"

void crearJuego(tJuego* juego){
    crearLista( &juego->listaPreguntas );
    crearListaC(&juego->listaJugadores);
    juego->cantJug=0;
    juego->nivelEligido=0;
    juego->tiempoLimite=0;
    srand(time(NULL));
}


int cargarJuego(tJuego* juego){

    cargarConfiguracionDeTxt(&juego->tiempoLimite,&juego->cantRondas);

    cargarJugadores(juego);
    if(juego->cantJug==0){
        puts("no se han ingresado jugadores");
        return 0;
    }

    cargarDificultad(juego);

    system("cls");
    puts("\ncargando preguntas...");

    cargarPreguntas( &juego->listaPreguntas, "https://664d06f4ede9a2b5565273e6.mockapi.io/PREGUNTAS",
                     juego->nivelEligido, juego->cantRondas );

    return 1;
}

int cargarJugadores ( tJuego *juego )
{
    tJugador jugador={"",0,0};
    char nombreLargo[2000];
    t_Lista jugadores;

    crearLista(&jugadores);

    system("cls");
    puts("Ingrese el nombre de un jugador o FIN si ya ingreso todos los nombres. no mas de 20 caracteres por nombre");

    obtenerTextoNoVacioDeTeclado(nombreLargo);
    strncpy(jugador.nombre, nombreLargo, 20);
    jugador.nombre[20]='\0';

    while( strcmpi( "FIN", jugador.nombre ) != 0 )
    {
        jugador.orden = rand();
        insertarEnListaOrdenadoConDuplicado( &jugadores, &jugador, sizeof( tJugador ),cmpJugadorXOrdenMenAMay );
        juego->cantJug++;
        puts("Ingrese el nombre de un jugador");
        obtenerTextoNoVacioDeTeclado(nombreLargo);
        strncpy(jugador.nombre, nombreLargo, 20);
        jugador.nombre[20]='\0';
    }


    while(!listaVacia(&jugadores)){
        verDatoDeListaEnPos(&jugadores,&jugador,sizeof(tJugador),0);
        eliminarDeListaEnPos(&jugadores,0);
        insertarEnSiguiente(&juego->listaJugadores,&jugador,sizeof(tJugador));
    }

    int orden=0;
    mapListaC(&juego->listaJugadores,ModificarElOrdenJugador,&orden);

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
            crearListaC(&(pregunta.respuestas)); //dado que siempre se usara esto para crear la listas de preguntas
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
    fflush(stdin);
    return 1;
}

void cargarDificultad(tJuego *lista){
    int nivel=0;
    system("cls");
    printf("Ingrese nivel de dificultad\n1:Baja \n2:Media \n3:Alta\n");
    nivel=obtenerRespuestaDeTecladoEntre('1','3');
    lista->nivelEligido=nivel-'0';
}

void parsearPregunta ( tPregunta *destinoPregun, cJSON *origen )
{
    cJSON *valor;
    valor = cJSON_GetObjectItem( origen, "pregunta" );
    strcpy( destinoPregun->pregunta, valor->valuestring+1 );
    valor = cJSON_GetObjectItem( origen, "resp_correcta" );
    strcpy( (destinoPregun->opciones)[0], valor->valuestring);
    valor = cJSON_GetObjectItem( origen, "opcion_1" );
    strcpy( (destinoPregun->opciones)[1], valor->valuestring);
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

    respuesta.ordenJugador=juego->jugadorActual;
    printf("%s\n",pregunta->pregunta);
    verOpcionesPreguntas(pregunta);

    obtenerRespuestaDeTecladoTemporizado(&(respuesta.respuesta),&tiempoTranscurrido,juego->tiempoLimite);
    respuesta.tiempo=(int)tiempoTranscurrido; //solo se queda con la parte ententera del double

    printf("Su respuesta es %c y tardo %2d segundos \n",respuesta.respuesta,respuesta.tiempo);

    insertarEnSiguiente(&pregunta->respuestas,&respuesta,sizeof(respuesta));

    return 1;
}

int juegaJugador(void* d, void* d2){
    tJugador *jugador=d;
    tJuego *juego=d2;

    //limpiar pantalla
    system("cls");
    printf("ahora es el turno del jugador: %s\n", jugador->nombre);
    puts("ingrese cualquier tecla si esta listo para jugar...");
    getch();
    system("cls");

    printf("Turno del jugador: %s\n", jugador->nombre);
    mapLista(&juego->listaPreguntas,contestarPregunta,juego);

    puts("turno terminado,ingrese cualquier tecla para continuar...");
    getch();
    system("cls");

    juego->jugadorActual++;
    return 1;
}

int ordenarPosiciones(void* d, void* d2){
    tPregunta *pregunta=d;
    tRespuesta resp;
    resp.ordenJugador=0;
    buscarPorClaveYaccionarEnListaC(&pregunta->respuestas,&resp,sizeof(tRespuesta),cmpOrdenJugador,NULL,NULL);
    return 1;
}



int iniciarJuego(tJuego *juego){
    ///mostrar Lista de Jugadores y orden en como lo responde    !!!!!!!!!!!!!!!
    system("cls");
    puts("INFORMACION PARA JUGAR");
    puts("orden de los jugadores:");
    mapListaC(&juego->listaJugadores,mostrarJugarConOrdenReal,stdout);
    printf("cantidad de preguntas: %d\n",juego->cantRondas);
    printf("tiempo limite por pregunta:%d segundos\n",juego->tiempoLimite);
    puts("ingrese cualquier tecla para continuar...");
    getch();

    juego->jugadorActual=0;
    mapListaC(&juego->listaJugadores,juegaJugador,juego);
    mapLista(&juego->listaPreguntas,ordenarPosiciones,NULL);    //para que la lista circular apunte al primer turno y quede un paralalismo

    return TODO_OK;
}
/**-------------------------------------------------------------------*/
//lista de respuestas
int calcularPuntajeDeRespuesta(void* d, void* d2){
    tRespuesta* respuesta =d;
    tContexto * c =d2;
    respuesta->puntaje=calcularPuntaje(respuesta->respuesta,c->respuestaCorrecta,respuesta->tiempo,
                                     c->tiempoLimite,c->mejorTiempo,c->existeTiempoMejorDuplicado);

    tJugador j;
    j.orden=respuesta->ordenJugador;
    buscarPorClaveYaccionarEnListaC(&c->jugadores,&j,sizeof(tJugador),cmpJugadorXOrdenMenAMay,&respuesta->puntaje,sumarPuntos);

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

int calcularPuntajesDeTodasRespuestas(void* d, void* d2){
    tPregunta* pregunta =d;
    tContexto* c=d2;
    c->respuestaCorrecta=pregunta->opcionCorrecta;
    c->mejorTiempo=100000;

    mapListaC(&pregunta->respuestas,mejorTiempoValido,c);
    mapListaC(&pregunta->respuestas,calcularPuntajeDeRespuesta,c);

    return 1;
}

int calcularResultadosYimprimir(tJuego *juego){
    tContexto c;
    c.jugadores=juego->listaJugadores;
    c.tiempoLimite=juego->tiempoLimite;
    c.maximaPuntuacion=INT_MIN;

    mapLista(&juego->listaPreguntas,calcularPuntajesDeTodasRespuestas,&c);
    system("cls");
/*
    puts("resultados.");
    printf("preguntas / jugadores:                                                            ");
    mapListaC(&juego->listaJugadores,imprimirJugador,stdout);
    puts("");
    mapLista(&juego->listaPreguntas,mostrarPreguntaYimprimirRespuesta,NULL);

    printf("puntajes totales:                                                        ");   ///puntajes totales ,despues lo veo
    mapListaC(&juego->listaJugadores,imprimirPuntajeTotalJugador,stdout);
*/

    system("cls");
    puts("juego terminado , ingrese cualquier tecla para mostrar el/los ganador/ganadores");
    getch();
    system("cls");

    mapListaC(&juego->listaJugadores,obtenerMaximaPuntuacion,&(c.maximaPuntuacion) );
    printf("ganadores/ganadores:");
    mapListaC(&juego->listaJugadores,imprimirGanadores,&(c.maximaPuntuacion));
    printf("\npuntuacion ganadora: %d\n",c.maximaPuntuacion);
    puts("para mas detalle lea el informe generado...");
    generarInforme(juego,&c);
    puts("ingrese cualquier tecla para finalizar la partida");
    getch();
    return TODO_OK;
}

void cerrarJuego(tJuego *juego){
    system("cls");
    puts("\ncerrando juego...");
    vaciarListaC(&juego->listaJugadores);
    mapLista(&juego->listaPreguntas,vaciarRespuestas,NULL);
    vaciarLista(&juego->listaPreguntas);
}

int menu(){
    char eleccion;
    printf("[A] Jugar\n[B] Salir\n");
    eleccion=obtenerRespuestaDeTecladoEntre('A','B');
    if(eleccion=='B')
        return 0;
    return 1;
}

void generarInforme(tJuego*juego,tContexto *c){
    char nombreArchivo[100];
    obtenerNombreDeArchivoConFecha(nombreArchivo,sizeof(nombreArchivo));
    FILE*pa=fopen(nombreArchivo,"wt");
    if (pa == NULL) {
        perror("Error al abrir el archivo");
        return;
    }
    c->archivo=pa;

    mapLista(&juego->listaPreguntas,imprimirEnArchivoPregunta,pa);
    fprintf(pa,"Respuestas Jugadores:\n");
    mapLista(&juego->listaPreguntas,puntosPorPreguntaParaArchivo,c);
    fprintf(pa,"puntajes Totales de cada Jugador:\n");
    mapListaC(&juego->listaJugadores,imprimirJugadorEnArchivo,pa);
    fprintf(pa,"ganadores/ganadores:\n");
    ganadoresEnArchivo(&juego->listaJugadores,pa,c->maximaPuntuacion);
    fprintf(pa,"puntaje ganador:%d\n",c->maximaPuntuacion);
    fclose(pa);
}


