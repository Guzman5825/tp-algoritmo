#ifndef JUGADOR_H_INCLUDED
#define JUGADOR_H_INCLUDED

#include <stdio.h>
#define MAX_CADENA 30
#define MAX_TRIVIAS 101

typedef struct
{
    char nombre [MAX_CADENA];
    int orden;
    int puntajeTotal;
}
tJugador;

int verJugador ( const void* dato, void *stream );
int mostrarJugarConOrdenReal( void* dato, void *stream );
int cmpJugadorXOrdenMenAMay ( const void* j1, const void *j2 );
int cmpJugadorXOrdenMayAMen ( const void* j1, const void *j2 );
int ModificarElOrdenJugador(void* n, void* d);
int imprimirJugador ( void* dato, void *stream );
int imprimirPuntajeTotalJugador ( void* dato, void *stream );
int sumarPuntos( void* d,void* d2);
int obtenerMaximaPuntuacion( void* d,void* d2);
int imprimirGanadores(void* d,void* d2);
#endif // JUGADOR_H_INCLUDED
