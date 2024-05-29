#include "juego.h"

typedef struct
{
    double menorTiempo;
    size_t raund;
    size_t unico; //simula un booleano
}
tRespuestaSeleccionada; //se podria encontrar un nombre mejor? enlistamos el menor tiempo de cada raund

int cmpRespuestaXRaund ( const void *r1, const void *r2 );
int quedarseElMEnorTiempo ( const void *r1, void *r2 );



void inicioDeJuego (  t_Lista *listaJug, t_Lista *listaPre, t_Lista *listaRes, size_t cantJug, size_t cantRaunds,double tiempoRaund );


void obtenerMejoresRespuestas ( t_Lista *listaPreguntas, t_Lista *listaRespuestas, t_Lista *listaMejRes, size_t cantJug,size_t cantRaunds, double tiempoMaximo );
int obtenerRespuestaConTimeout(char *respuesta, double tiempoMaximo, double *tiempoParcial);
void calcularPuntajesXRaund ( t_Lista *listaPreguntas, t_Lista *listaRespuestas, t_Lista *listaMejRes, size_t cantJug, size_t cantRaunds,double tiempoRaund );


int main()
{
    tJuego juego;
    crearJuego(&juego);
    cargarJuego(&juego);

    iniciarJuego(&juego);
//    inicioDeJuego( &(juego.listaJugadores), &(juego.listaPreguntas), &(juego.listaRespuestas), (juego.cantJug), 4, (double)10 );
    obtenerMejoresRespuestas( &(juego.listaPreguntas), &(juego.listaPreguntas), &(juego.listaMejorRes), (juego.cantJug),4,10.0 );
    calcularPuntajesXRaund( &(juego.listaPreguntas), &(juego.listaRespuestas), &(juego.listaMejorRes), (juego.cantJug), 4, 10.0 );
    //la idea es tener una lista de tiempos menores de cada raund, para luego sacar el puntaje con los
    //parametros que dice el tp de de si son 3 o 2 o 1 puntos, como debemos informar lo que respondio
    //y cual fue el puntje en ese raund entonces guardo eso, solo faltaria hacer la funcion que informe eso
    //Aca deberia estar los destruir lista y destruir el buffer del json, que uso para levantar los datos, que creo que me olvide XD


    return 0;
}


int esRespuestaCorrecta ( tPregunta *pre, tRespuestaJug *res )
{
    return pre->opcionCorrecta == res->respuesta; //comparamos 2 char, 2 enteros en si
}

int esTiempoCorrecto ( tRespuestaJug *res, double tiempoLimite )
{
    return res->tiempo <= tiempoLimite; //considero que si responde en el ultimo segundo sigue en juego, podria no ser asi
}

int cmpRespuestaXRaund ( const void *r1, const void *r2 )
{
    tRespuestaSeleccionada *menorActual = (tRespuestaSeleccionada*)r1;
    tRespuestaSeleccionada *menorCandidato = (tRespuestaSeleccionada*)r2;

    return menorActual->raund - menorCandidato->raund;
}

int quedarseElMEnorTiempo ( const void *r1, void *r2 )
{
    tRespuestaSeleccionada *menorActual = (tRespuestaSeleccionada*)r1;
    tRespuestaSeleccionada *menorCandidato = (tRespuestaSeleccionada*)r2;

    if( menorCandidato->menorTiempo < menorActual->menorTiempo )
        menorActual->menorTiempo = menorCandidato->menorTiempo;
    if( menorCandidato->menorTiempo == menorActual->menorTiempo )
        menorActual->unico = 0;
    return 1;
}


void obtenerMejoresRespuestas ( t_Lista *listaPreguntas, t_Lista *listaRespuestas, t_Lista *listaMejRes, size_t cantJug,size_t cantRaunds, double tiempoMaximo )
{
    size_t i;//vamos a insertar sin duplicados(de cada raund solo puede haver un unico tiempo menor si existiera)
    tPregunta pre;
    tRespuestaJug res;
    tRespuestaSeleccionada mejRes;

    for( i=0; i < cantRaunds*cantJug; i++ )
    {
        verDatoDeListaEnPos( listaRespuestas, &res, sizeof( tRespuestaJug ), i);
        verDatoDeListaEnPos( listaPreguntas, &pre, sizeof( tPregunta ), i % cantRaunds );//el index para segurarnos el paraleleismo
        if( esRespuestaCorrecta( &pre, &res ) && esTiempoCorrecto( &res, tiempoMaximo ) )
        {
            mejRes.menorTiempo = res.tiempo;
            mejRes.raund = i % cantRaunds;
            mejRes.unico = 1;//por defecto el primer tiempo es unico
            insertarEnListaAlFinalSinDuplicados( listaMejRes, &mejRes, sizeof(tRespuestaSeleccionada),cmpRespuestaXRaund, quedarseElMEnorTiempo);
            //El insertar de arriba parece mas un insertar agrupando, terminamos obteniendo una lista de los menores tiempos en cada raund
            //y sabemos si son unicos o no, sirve para ver si sumamos 2 puntos o 3
        }
    }
}


void calcularPuntajesXRaund ( t_Lista *listaPreguntas, t_Lista *listaRespuestas, t_Lista *listaMejRes, size_t cantJug, size_t cantRaunds,double tiempoRaund )
{
    tPregunta pre;
    tRespuestaJug res;
    tRespuestaSeleccionada mejRes;
    size_t i;

    for( i=0; i < cantJug * cantRaunds; i++ )
    {
        sacarDeListaEnPos( listaRespuestas, &res, sizeof(tRespuestaJug), i );
        verDatoDeListaEnPos( listaPreguntas, &pre, sizeof( tPregunta ), i % cantRaunds );
        mejRes.raund = i % cantRaunds;

        if( esTiempoCorrecto( &res, tiempoRaund ) )
        {
            if( esRespuestaCorrecta( &pre, &res ) )
            {
                buscarDatoEnListaDesordenadaPorClave( listaMejRes, sizeof(tRespuestaSeleccionada), &mejRes, cmpRespuestaXRaund );
                if( res.tiempo == mejRes.menorTiempo )//si respondio rapido le sumamos +3 o +2, en la lista de arriba solo estan los tiempos menores de respuestas correctas
                {
                    if( mejRes.unico )
                        res.puntaje = 3;
                    else
                        res.puntaje = 2;
                }
                else
                    res.puntaje = 1;//respondio bien pero no rapido
            }
            else
                res.puntaje = -2;
        }
        insertarEnListaEnPos( listaRespuestas, &res,sizeof(tRespuestaJug), i );

    }
}
