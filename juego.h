#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include <locale.h>
#include <stdio.h>
#include <windows.h>
#include <process.h>
#include <curl/curl.h>
#include <time.h>
#include <conio.h>
#include "lista_simple.h"
#include "listaCircular.h"
#include "cJSON.h"
#include "input.h"
#include "jugador.h"
#include "pregunta.h"
#include "respuesta.h"
#include "informe.h"
#include "archivo.h"
#define MAX_CANT_JUGADORES 100
#define MAX_CANT_RONDAS 100

typedef struct{
    t_Lista listaPreguntas;
    tListaC listaJugadores;
    size_t cantJug;
    int nivelEligido;
    int tiempoLimite;
    int cantRondas;
    tRespuesta respuestas[MAX_CANT_JUGADORES][MAX_CANT_RONDAS];
    int jugadorActual;
}tJuego;

typedef struct
{
    char *cadenaJSON;
    size_t tamCadena;
}
tJsontxt;

typedef struct
{
    int maximaPuntuacion;
    int tiempoLimite;
    int mejorTiempo;
    int existeTiempoMejorDuplicado;
    char respuestaCorrecta;
    tListaC jugadores;
}
tContexto;

int menu();

void crearJuego(tJuego *juego);
int cargarJuego(tJuego *juego);
int cargarJugadores ( tJuego *lista );
int cargarPreguntas ( t_Lista *lista, const char *urlAPI, size_t nivelDificultad, size_t cantRaunds );
void cargarDificultad(tJuego *lista);
void parsearPregunta ( tPregunta *destinoPregun, cJSON *origen );
int iniciarJuego(tJuego *juego);
int calcularResultadosYimprimir(tJuego *juego);
void cerrarJuego(tJuego *juego);
void generarInforme(tJuego *juego,int puntuacioMax);

#endif // JUEGO_H_INCLUDED
