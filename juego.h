#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include <stdio.h>
#include "lista_simple.h"
#include <curl/curl.h>
#include "cJSON.h"
#include <windows.h>
#include <process.h>
#include <time.h>
#include <conio.h>
#include "input.h"
#include "jugador.h"
#include "pregunta.h"
#include "respuesta.h"

typedef struct{
    t_Lista listaPreguntas;
    t_Lista listaJugadores;
    t_Lista listaRespuestas; //almacenamos los datos de cada partida correspondiente
    t_Lista listaMejorRes;
    size_t cantJug;
    int nivelEligido;
    int tiempoLimite;
    int cantRondas;
}tJuego;

typedef struct
{
    char *cadenaJSON;
    size_t tamCadena;
}
tJsontxt;



void crearJuego(tJuego *juego);
int cargarJuego(tJuego *juego);
int cargarJugadores ( tJuego *lista );
int cargarPreguntas ( t_Lista *lista, const char *urlAPI, size_t nivelDificultad, size_t cantRaunds );



void parsearPregunta ( tPregunta *destinoPregun, cJSON *origen );


int iniciarJuego(tJuego *juego);
int calcularResultado();
void cerrarJuego(tJuego *juego);


#endif // JUEGO_H_INCLUDED
