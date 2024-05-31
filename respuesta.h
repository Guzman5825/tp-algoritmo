#ifndef RESPUESTA_H_INCLUDED
#define RESPUESTA_H_INCLUDED

typedef struct
{
    char respuesta;
    double tiempo;
    int puntaje;
}
tRespuesta;

int determinarMejorTiempoYTiempoDuplicado(tRespuesta respuestas[][100],int cantJugadores, int rondaActual,double* mejorTiempo,int* existenDuplicado);
int calcularPuntaje(char respuestaJugador,char respuestaCorrecta,double tiempoRespuesta,
                    int tiempoLimite,double mejorTiempo,int existeDuplicadoMejorTiempo);

#endif // RESPUESTA_H_INCLUDED
