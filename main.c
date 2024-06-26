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

/**
---se puso un comentario a todo los return con numero
---tambien para el URL
---en vez de ingresar por clave en una lista, se podria ingresar aleatoreamente por posicion y directamente en lista circular, agregando una nueva funcion en listaC
---se podria utilizar mas variables en memoria, ejemplo, cada vez que se conteste la pregunta, se podria obtener el mejor tiempo de cada ronda
---mientras se imprime las preguntas, se podria imprimir las respuesta de cada preguntas a su vez

*/
