#include "juego.h"

int main()
{
    setlocale(LC_CTYPE,"spanish");

    tJuego juego;
    crearJuego(&juego);

    if(menu() && cargarJuego(&juego)){
        iniciarJuego(&juego);
        calcularResultadosYimprimir(&juego);
    }
    cerrarJuego(&juego);
    return 0;
}
