#include "jugador.h"

int verJugador ( const void* dato, void *stream )
{
    tJugador *jugador = (tJugador*)dato;
    fprintf((FILE*)stream,"%-30s\t%-7d\n", jugador->nombre, jugador->orden);
    return 1;
}

int mostrarJugarConOrdenReal( void* dato, void *stream )
{
    tJugador *jugador = (tJugador*)dato;
    fprintf((FILE*)stream,"--turno:%d nombre:%s \n", jugador->orden+1,jugador->nombre);
    return 1;
}

int cmpJugadorXOrdenMenAMay ( const void* j1, const void *j2 )
{
    tJugador *jExistente = (tJugador*)j1;
    tJugador *nuevoJ = (tJugador*)j2;

    return jExistente->orden - nuevoJ->orden;
}

int cmpJugadorXOrdenMayAMen ( const void* j1, const void *j2 )
{
    tJugador *jExistente = (tJugador*)j1;
    tJugador *nuevoJ = (tJugador*)j2;

    return  nuevoJ->orden - jExistente->orden;
}

int ModificarElOrdenJugador( void* n, void* d){
    tJugador *jugador=n;
    int* orden=d;
    jugador->orden=*orden;
    (*orden)++;
    return 1;
}

int imprimirJugador ( void* dato, void *stream )
{
    tJugador *jugador = (tJugador*)dato;
    fprintf((FILE*)stream,"%-20.20s", jugador->nombre);
    return 1;
}

int imprimirPuntajeTotalJugador ( void* dato, void *stream )
{
    tJugador *jugador = (tJugador*)dato;
    fprintf((FILE*)stream,"%10d   ", jugador->puntajeTotal);
    return 1;
}

int sumarPuntos( void* d,void* d2){
    tJugador * j = (tJugador*)d;
    int *n=d2;
    j->puntajeTotal+=*n;
    return 1;
}

int obtenerMaximaPuntuacion( void* d,void* d2){
    tJugador * j = (tJugador*)d;
    int *n=d2;
    if(j->puntajeTotal>*n)
        *n=j->puntajeTotal;
    return 1;
}

int imprimirGanadores(void* d,void* d2){
    tJugador * j = (tJugador*)d;
    int *n=d2;

    if(*n==j->puntajeTotal)
        printf("%s  ",j->nombre);

    return 1;
}

