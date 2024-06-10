#include "informe.h"

int imprimirEnArchivoPregunta(void*preg,void*pa){
    tPregunta*pregunta=(tPregunta*)preg;
    FILE *archivo = (FILE *)pa;
    fprintf(archivo, "Pregunta N°%d: %s\n",pregunta->orden+1, pregunta->pregunta);
    fprintf(archivo, "Opciones:\n");
    for (int i = 0; i < MAX_OPCIONES; i++) {
        fprintf(archivo, "  Opción %d: %s\n", i + 1, pregunta->opciones[i]);
    }
    fprintf(archivo, "Respuesta Correcta: %c\n", pregunta->opcionCorrecta);
    return 1;
}

int puntosPorPreguntaParaArchivo(void*preg,void*pa){
    tPregunta*pregunta=(tPregunta*)preg;
    tContexto *c=pa;
    fprintf(c->archivo, "Respuesta de la pregunta N°%d:\n", pregunta->orden+1);
    mapListaC(&pregunta->respuestas,imprimirPuntosPorPreguntaEnArchivo,pa);
    return 1;
}

int imprimirPuntosPorPreguntaEnArchivo(void*elem,void*pa){
    tRespuesta*resp=(tRespuesta*)elem;
    tContexto *c=pa;
    tJugador j;
    j.orden=resp->ordenJugador;

    //fprintf(c->archivo, "\nJugador Nro %d\t:", resp->ordenJugador+1);
    fprintf(c->archivo, "Jugador ");
    buscarPorClaveYaccionarEnListaC(&c->jugadores,&j,sizeof(tJugador),cmpJugadorXOrdenMenAMay,
                                    c->archivo,imprimirJugador);
    fprintf(c->archivo, "  resp:'%c'", resp->respuesta);
    fprintf(c->archivo, "  tiempo:%3ds", resp->tiempo);
    fprintf(c->archivo, "  puntos:%1d\n", resp->puntaje);
    return 1;
}

int imprimirJugadorEnArchivo(void *jug, void *pa) {
    tJugador *jugador = (tJugador *)jug;
    FILE *archivo = (FILE *)pa;
    fprintf(archivo, "Puntaje total %d del jugador: %s\n", jugador->puntajeTotal,jugador->nombre);
    return 1;
}

void ganadoresEnArchivo(tListaC*lista,FILE*pa,int puntajeMaximo){
    tNodo*primero = *lista;
    while( *lista && (*lista)->sig!=primero){
        if (cmpJugaPuntMax((*lista)->dato,puntajeMaximo)==0)
            imprimirGanadoresEnArchivo((*lista)->dato,pa);
        lista = & (*lista)->sig;
    }
    if ( *lista)
        if (cmpJugaPuntMax((*lista)->dato,puntajeMaximo)==0)
            imprimirGanadoresEnArchivo((*lista)->dato,pa);
}

int cmpJugaPuntMax(const void*elem,int punt){
    tJugador*jug=(tJugador*)elem;
    return jug->puntajeTotal - punt;
}

void imprimirGanadoresEnArchivo(const void*dato,FILE*pa){
    tJugador*jug=(tJugador*)dato;
    fprintf(pa, "--Ganador: %s\n", jug->nombre);
}
