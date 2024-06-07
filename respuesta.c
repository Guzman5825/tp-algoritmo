#include "respuesta.h"

int mostrarRespuesta(void* d, void* d2){
    tRespuesta * r=d;
    printf("'%c' %2ds %2d   ",r->respuesta,r->tiempo,r->puntaje);
    return 1;
}


int calcularPuntaje(char respuestaJugador,char respuestaCorrecta,int tiempoRespuesta,
                    int tiempoLimite,int mejorTiempo,int existeDuplicadoMejorTiempo){
    if(tiempoRespuesta>=tiempoLimite)
        return 0;
    if(respuestaJugador!=respuestaCorrecta)
        return -2;
    if(tiempoRespuesta>mejorTiempo)
        return 1;
    if(existeDuplicadoMejorTiempo)
        return 2;
    return 3;
}
