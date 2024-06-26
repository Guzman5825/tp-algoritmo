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
, pero no se hizo por un tema de escalabilidad, ya que tener separado las respuestas de los jugadores y el procesamiento de los datos
permite mayor flexibilidad a la hora de hacer cambios.
---mientras se imprime las preguntas, se podria imprimir las respuesta de cada preguntas a su vez
nota: como minimo tengo que recorrer las rondas 2 veces, uno para calcular el mejor tiempo ,y el otro para calcular los puntajes
nota2:el problemas se penso como programacion orientado objeto, osea tengo una lista de preguntas y en cada pregunta una lista de respuesta
*/
