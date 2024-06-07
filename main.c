#include "juego.h"
int main()
{
    tJuego juego;
    crearJuego(&juego);
    cargarJuego(&juego);
    iniciarJuego(&juego);
    calcularResultadosYimprimir(&juego);
    cerrarJuego(&juego);
    return 0;
}
