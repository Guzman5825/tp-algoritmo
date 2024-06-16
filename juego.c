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

    if(!cargarConfiguracionDeTxt(&juego->tiempoLimite,&juego->cantRondas)){
        puts("ingrese cualquier tecla para continuar...");
        getch();
        return ERROR_CARGA;
    }

    cargarJugadores(&juego->listaJugadores,&juego->cantJug);

    cargarDificultad(&juego->nivelEligido);

    system("cls");
    puts("\ncargando preguntas...");

    if(!cargarPreguntas( &juego->listaPreguntas, "https://664d06f4ede9a2b5565273e6.mockapi.io/PREGUNTAS",
                     juego->nivelEligido, juego->cantRondas )){
        puts("ingrese cualquier tecla para continuar...");
        getch();
        return ERROR_CARGA;
        }

    return TODO_OK;
}

int cargarJugadores ( tListaC* listaJugadores,size_t * cantJug )
{
    tJugador jugador={"",0,0};
    t_Lista jugadores;
    int orden=0;

    crearLista(&jugadores);

    system("cls");
    *cantJug = solicitarJugadores(&jugadores);
    while( *cantJug == 0 )
    {
        puts("No ah registrado jugadores, por favor ingrese al menos un jugador");
        *cantJug = solicitarJugadores(&jugadores);
    }

    while(!listaVacia(&jugadores)){
        sacarDeListaEnPos(&jugadores,&jugador,sizeof(tJugador),0);
        jugador.orden=orden;
        insertarEnSiguiente(listaJugadores,&jugador,sizeof(tJugador));
        orden++;
    }

    return TODO_OK;
}

size_t solicitarJugadores (t_Lista *lista)
{
    tJugador jugador={"",0,0};
    size_t cantJug = 0;

    printf("Ingrese el nombre de un jugador de hasta %d caracteres o FIN si ya ingreso todos los nombres!!!!\n",MAX_CARACTERES_NOMBRE);

    obtenerTextoNoVacioDeTecladoYLimitado(jugador.nombre,MAX_CARACTERES_NOMBRE);

    while( strcmpi( "FIN", jugador.nombre ) != 0  )
    {
        jugador.orden = rand();
        insertarEnListaOrdenadoConDuplicado(lista, &jugador, sizeof( tJugador ),cmpJugadorXOrdenMenAMay );
        cantJug++;
        printf("Ingrese el nombre de un jugador de hasta %d caracteres o FIN si ya ingreso todos los nombres!!!!\n",MAX_CARACTERES_NOMBRE);
        obtenerTextoNoVacioDeTecladoYLimitado(jugador.nombre,MAX_CARACTERES_NOMBRE);
    }
    return cantJug;
}

///dado que envia por partes ,entonces se llama esta funcion varias veces hasta que cargue toda la informacion
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
    int i, cantElem=0;

    crearListaC(&(pregunta.respuestas)); //dado que siempre se usara esto para crear la listas de preguntas

    if( ! inicializarJsonTxt( &jsonRes )  ){
        puts("error de inicializcion de json");
        return 0;//No tengo donde almacenar la respuesta
    }


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
            cantElem++;
        }
    }

    cJSON_Delete( jsonPreguntas );//liberamos el cjson, tiene una implementacion con memoria dinamica
    curl_easy_cleanup( curl ); //terminamos la solicitud

    while( cantElem > cantRaunds )
    {
        eliminarDeListaEnPos(lista,0); //elimino los primeros, total la probabilidad sigue siendo la misma
        cantElem--;
    }

    int orden=0;
    mapLista(lista,ModificarElOrdenPregunta,&orden);
    fflush(stdin);

    return TODO_OK;
}

void cargarDificultad(int *nivelElegido){
    int nivel=0;
    system("cls");
    printf("Ingrese nivel de dificultad\n1:Baja \n2:Media \n3:Alta\n");
    nivel=obtenerRespuestaDeTecladoEntre('1','3');
    *nivelElegido=nivel-'0';
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
    respuesta.tiempo=(int)tiempoTranscurrido;

    printf("Su respuesta es %c y tardo %2d segundos \n",respuesta.respuesta,respuesta.tiempo);

    insertarEnSiguiente(&pregunta->respuestas,&respuesta,sizeof(respuesta));

    return TODO_OK;
}

int juegaJugador(void* d, void* d2){
    tJugador *jugador=d;
    tJuego *juego=d2;
    juego->jugadorActual=jugador->orden;        ///le envio el orden para que luego la pregunta pueda tener el orden
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

    return TODO_OK;
}

int ordenarPosiciones(void* d, void* d2){
    tPregunta *pregunta=d;
    tRespuesta resp;
    resp.ordenJugador=0;
    buscarPorClaveYaccionarEnListaC(&pregunta->respuestas,&resp,sizeof(tRespuesta),cmpOrdenJugador,NULL,NULL);
    return TODO_OK;
}



int iniciarJuego(tJuego *juego){
    system("cls");
    puts("INFORMACION PARA JUGAR");
    puts("orden de los jugadores:");
    mapListaC(&juego->listaJugadores,mostrarJugarConOrdenReal,stdout);
    printf("cantidad de preguntas: %d\n",juego->cantRondas);
    printf("tiempo limite por pregunta:%d segundos\n",juego->tiempoLimite);
    puts("ingrese cualquier tecla para continuar...");
    getch();

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
    buscarPorClaveYaccionarEnListaC(&c->jugadores,&j,sizeof(tJugador),cmpJugadorXOrdenMenAMay,
                                    &respuesta->puntaje,sumarPuntos);

    return TODO_OK;
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

    return TODO_OK;
}

int calcularPuntajesDeTodasRespuestas(void* d, void* d2){
    tPregunta* pregunta =d;
    tContexto* c=d2;
    c->respuestaCorrecta=pregunta->opcionCorrecta;
    c->mejorTiempo=INT_MAX;

    mapListaC(&pregunta->respuestas,mejorTiempoValido,c);
    mapListaC(&pregunta->respuestas,calcularPuntajeDeRespuesta,c);

    return TODO_OK;
}

int calcularResultadosYimprimir(tJuego *juego){
    tContexto c;
    c.jugadores=juego->listaJugadores;
    c.tiempoLimite=juego->tiempoLimite;
    c.maximaPuntuacion=INT_MIN;

    mapLista(&juego->listaPreguntas,calcularPuntajesDeTodasRespuestas,&c);

    system("cls");
    puts("juego terminado , ingrese cualquier tecla para mostrar el/los ganador/ganadores");
    getch();
    system("cls");

    mapListaC(&juego->listaJugadores,obtenerMaximaPuntuacion,&(c.maximaPuntuacion) );
    printf("ganadores/ganadores:");
    mapListaC(&juego->listaJugadores,imprimirGanadores,&(c.maximaPuntuacion));
    printf("\npuntuacion ganadora: %d\n",c.maximaPuntuacion);
    puts("para mas detalle lea el informe generado...");
    generarInforme(juego,&c,juego);
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
    return TODO_OK;
}

void generarInforme(tJuego*juego,tContexto *c,tJuego *j){
    char nombreArchivo[MAX_NOMBRE_ARCH];
    obtenerNombreDeArchivoConFecha(nombreArchivo,sizeof(nombreArchivo));
    FILE*pa=fopen(nombreArchivo,"wt");
    if (pa == NULL) {
        perror("Error al abrir el archivo");
        return;
    }
    c->archivo=pa;

    fprintf(pa,"condiciones del juego:\n");
    fprintf(pa,"-cantidad de jugadores:%d\n",(int)j->cantJug);
    fprintf(pa,"-cantidad de preguntas:%d\n",(int)j->cantRondas);
    fprintf(pa,"-dificultad elegida:");
    fprintf(pa,nivelAtexto(j->nivelEligido) );
    fprintf(pa,"\n");
    fprintf(pa,"-tiempo limite por pregunta: %d segundos\n",j->tiempoLimite);

    fprintf(pa,"\nPreguntas:\n");
    mapLista(&juego->listaPreguntas,imprimirEnArchivoPregunta,pa);
    fprintf(pa,"\nRespuestas Jugadores:\n");
    mapLista(&juego->listaPreguntas,puntosPorPreguntaParaArchivo,c);
    fprintf(pa,"\nPuntajes Totales de cada Jugador:\n");
    mapListaC(&juego->listaJugadores,imprimirJugadorEnArchivo,pa);
    fprintf(pa,"\nGanadores/ganadores:\n");
    ganadoresEnArchivo(&juego->listaJugadores,pa,c->maximaPuntuacion);
    fprintf(pa,"Puntaje ganador:%d\n",c->maximaPuntuacion);
    fclose(pa);
}
