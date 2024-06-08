#ifndef RESPUESTA_H_INCLUDED
#define RESPUESTA_H_INCLUDED

#include <stdio.h>

typedef struct
{
    int ordenJugador;
    char respuesta;
    int tiempo;
    int puntaje;
}
tRespuesta;

int mostrarRespuesta(void* d, void* d2);
int cmpOrdenJugador( const void* r1, const void *r2 );
int calcularPuntaje(char respuestaJugador,char respuestaCorrecta,int tiempoRespuesta,
                    int tiempoLimite,int mejorTiempo,int existeDuplicadoMejorTiempo);

#endif // RESPUESTA_H_INCLUDED
