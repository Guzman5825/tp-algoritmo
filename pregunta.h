#ifndef PREGUNTA_H_INCLUDED
#define PREGUNTA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listaCircular.h"
#include "lista_simple.h"
#include "respuesta.h"
#define MAX_OPCIONES 4
#define MAX_CARACTERES_PREGUNTA 300//ojo con este maximo depende de las preguntas
#define MAX_CARACTERES_OPCIONES 200 //lo mismo este si se rompe tal vez es por que no tiene suficiente espacio

typedef struct
{
    char pregunta[MAX_CARACTERES_PREGUNTA];
    size_t dificultad;
    char opciones[MAX_OPCIONES][ MAX_CARACTERES_OPCIONES ];
    char opcionCorrecta;
    int orden;
    tListaC respuestas;
}
tPregunta;

void verOpcionesPreguntas (const tPregunta *pre );
int filtraXDificultad ( const void *dato1, const void *dato2 );
void aleatorizarRespuestaCorrecta ( tPregunta *pre );
int cmpOrdenPregunta( const void* p1, const void *p2 );
int ModificarElOrdenPregunta(void* n, void* d);
int vaciarRespuestas(void* d, void* d2);

#endif // PREGUNTA_H_INCLUDED
