#include "juego.h"
int main()
{
    tJuego juego;
    crearJuego(&juego);
    cargarJuego(&juego);
    iniciarJuego(&juego);
    calcularResultadosYimprimir(&juego);
    cerrarJuego(&juego);
    //la idea es tener una lista de tiempos menores de cada raund, para luego sacar el puntaje con los
    //parametros que dice el tp de de si son 3 o 2 o 1 puntos, como debemos informar lo que respondio
    //y cual fue el puntje en ese raund entonces guardo eso, solo faltaria hacer la funcion que informe eso
    //Aca deberia estar los destruir lista y destruir el buffer del json, que uso para levantar los datos, que creo que me olvide XD
    ///edi: quisas tenga que hacer lote de pruebas, y probar , despues lo hago jaja

    return 0;
}
