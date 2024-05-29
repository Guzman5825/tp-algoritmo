#include "respuesta.h"


int determinarMejorTiempoYTiempoDuplicado(tRespuesta respuestas[][100],int cantJugadores,
                                        int rondaActual,double* mejorTiempo,int* existenDuplicado){
    *mejorTiempo=100000.0;
    *existenDuplicado=0;
    for(int i=0;i<cantJugadores;i++){
        if( respuestas[i][rondaActual].tiempo<*mejorTiempo){
            *mejorTiempo=respuestas[i][rondaActual].tiempo;
            *existenDuplicado=0;
        }else
            if( respuestas[i][rondaActual].tiempo==*mejorTiempo)
                *existenDuplicado=1;
    }
    return 1;
}

int calcularPuntaje(char respuestaJugador,char respuestaCorrecta,double tiempoRespuesta,
                    int tiempoLimite,double mejorTiempo,int existeDuplicadoMejorTiempo){
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
