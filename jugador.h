#ifndef JUGADOR_H_INCLUDED
#define JUGADOR_H_INCLUDED

#include <stdio.h>
#define MAX_CADENA 30
#define MAX_TRIVIAS 101

typedef struct
{
    char nombre [MAX_CADENA];
    int orden;
    char opcionEligida[MAX_TRIVIAS];
    double tiempoTardado[MAX_TRIVIAS];
}
tJugador;

int verJugador ( const void* dato, void *stream );
int cmpJugadorXOrdenMenAMay ( const void* j1, const void *j2 );
int cmpJugadorXOrdenMayAMen ( const void* j1, const void *j2 );

#endif // JUGADOR_H_INCLUDED
