#ifndef ARCHIVO_H_INCLUDED
#define ARCHIVO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINEA_CONFIG 100
#define ERROR_APERTURA_ARCHIVO 0
#define ERROR 0
#define TODO_OK 1

int cargarConfiguracionDeTxt(int* tiempoLimite,int* cantRondas);

#endif // ARCHIVO_H_INCLUDED
