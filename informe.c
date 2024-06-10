#include "informe.h"

int imprimirEnArchivoPregunta(void*preg,void*pa){
    tPregunta*pregunta=(tPregunta*)preg;
    FILE *archivo = (FILE *)pa;
    fprintf(archivo, "%d)Pregunta: %s\n",pregunta->orden+1, pregunta->pregunta);
    fprintf(archivo, "Opciones:\n");
    for (int i = 0; i < MAX_OPCIONES; i++) {
        fprintf(archivo, "  Opción %d: %s\n", i + 1, pregunta->opciones[i]);
    }
    fprintf(archivo, "Respuesta Correcta: %c\n", pregunta->opcionCorrecta);
    return 1;
}

int puntosPorPreguntaParaArchivo(void*preg,void*pa){
    tPregunta*pregunta=(tPregunta*)preg;
    FILE *archivo = (FILE *)pa;
    fprintf(archivo, "\nRespuesta de la pregunta N°%d:", pregunta->orden+1);
    mapListaC(&pregunta->respuestas,imprimirPuntosPorPreguntaEnArchivo,pa);
    return 1;
}

int imprimirPuntosPorPreguntaEnArchivo(void*elem,void*pa){
    tRespuesta*resp=(tRespuesta*)elem;
    FILE *archivo = (FILE *)pa;
    fprintf(archivo, "\nJugador Nro %d\t", resp->ordenJugador);
    fprintf(archivo, "respuesta %c\t", resp->respuesta);
    fprintf(archivo, "puntos %d\t", resp->puntaje);
    fprintf(archivo, "tiempo en responder %d segundos", resp->tiempo);
    return 1;
}

int imprimirJugadorEnArchivo(void *jug, void *pa) {
    tJugador *jugador = (tJugador *)jug;
    FILE *archivo = (FILE *)pa;
    fprintf(archivo, "\nPuntaje total %d del jugador: %s", jugador->puntajeTotal,jugador->nombre);
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
    fprintf(pa, "\nGanador: %s", jug->nombre);
}
