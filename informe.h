#ifndef INFORME_H_INCLUDED
#define INFORME_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listaCircular.h"
#include "lista_simple.h"
#include "juego.h"
#include "jugador.h"

int imprimirEnArchivoPregunta(void*,void*);
int imprimirJugadorEnArchivo(void*,void*);
int puntosPorPreguntaParaArchivo(void*,void*);
int imprimirPuntosPorPreguntaEnArchivo(void*,void*);
void ganadoresEnArchivo(tListaC*lista,FILE*pa,int puntajeMaximo);
void imprimirGanadoresEnArchivo(const void*dato,FILE*pa);
int cmpJugaPuntMax(const void*elem,int punt);

#endif // INFORME_H_INCLUDED
