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

#define MAX_NOMBRE_ARCH 100
#define ERROR_CARGA 0
#define TODO_OK 1
#define nivelAtexto(x) ( x==1 ? "FACIL" : (x==2? "MEDIO":"DIFICIL" ) )

typedef struct{
    t_Lista listaPreguntas;
    tListaC listaJugadores;
    size_t cantJug;
    int nivelEligido;
    int tiempoLimite;
    int cantRondas;
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
    FILE* archivo;
}
tContexto;

int menu();
void crearJuego(tJuego *juego);
int cargarJuego(tJuego *juego);
int cargarJugadores ( tListaC* listaJugadores,size_t * cantJug );
size_t solicitarJugadores (t_Lista *lista);
int cargarPreguntas ( t_Lista *lista, const char *urlAPI, size_t nivelDificultad, size_t cantRaunds );
void cargarDificultad(int *nivelElegido);
void parsearPregunta ( tPregunta *destinoPregun, cJSON *origen );
int iniciarJuego(tJuego *juego);
int calcularResultadosYimprimir(tJuego *juego);
void cerrarJuego(tJuego *juego);
void generarInforme(tJuego*juego,tContexto *c,tJuego *j);

#endif // JUEGO_H_INCLUDED
