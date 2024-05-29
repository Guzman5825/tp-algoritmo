#include "input.h"

int obtenerRespuestaDeTecladoTemporizado(char* respuesta,double *tiempoRespuesta,int tiempoLimite)
{
    *respuesta='-';
    clock_t start_time=clock();
    *tiempoRespuesta = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    while ( *tiempoRespuesta<tiempoLimite && !esOpcionValida(*respuesta))
    {
        usleep(1000);  // es necesario para no forzar al procesador
        if (_kbhit())
            *respuesta = toupper(_getch());
        *tiempoRespuesta = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    }

    return TODO_OK;
}

