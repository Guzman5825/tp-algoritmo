#ifndef PREGUNTA_H_INCLUDED
#define PREGUNTA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_OPCIONES 4
#define MAX_CARACTERES_PREGUNTA 90//ojo con este maximo depende de las preguntas
#define MAX_CARACTERES_OPCIONES 80 //lo mismo este si se rompe tal vez es por que no tiene suficiente espacio

typedef struct
{
    char pregunta[MAX_CARACTERES_PREGUNTA];
    size_t dificultad;
    char opciones[MAX_OPCIONES][ MAX_CARACTERES_OPCIONES ];
    char opcionCorrecta;
}
tPregunta;

void verOpcionesPreguntas ( tPregunta *pre );
int filtraXDificultad ( const void *dato1, const void *dato2 );
void aleatorizarRespuestaCorrecta ( tPregunta *pre );
#endif // PREGUNTA_H_INCLUDED
