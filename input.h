#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include <stdio.h>
#include <ctype.h>
#include <conio.h>
#include <time.h>
#include <unistd.h>

#define TODO_OK 1

#define esOpcionValida(x) ( (x>='A' && x<='D') ? (1) : (0) )

int obtenerRespuestaDeTecladoTemporizado(char* respuesta,double *tiempoRespuesta,int tiempoLimite);

#endif // INPUT_H_INCLUDED
