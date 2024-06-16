#include "pregunta.h"

void verOpcionesPreguntas (const tPregunta *pre )
{
    size_t i;

    for( i=0; i < MAX_OPCIONES; i++ )
    {
        printf( "%c)%s\n",(char) i+'A', (pre->opciones)[i] );
    }
}

int filtraXDificultad ( const void *dato1, const void *dato2 )
{
    tPregunta *pregu  = (tPregunta*) dato1;
    return pregu->dificultad == *(size_t*)dato2;
}

void aleatorizarRespuestaCorrecta ( tPregunta *pre )
{
    char temporal[MAX_CARACTERES_OPCIONES];

    //por defectos la respuesta correcta es siempre la A
    pre->opcionCorrecta = rand() % MAX_OPCIONES;
    strcpy( temporal, (pre->opciones)[0] );
    strcpy( (pre->opciones)[0],(pre->opciones)[(int)pre->opcionCorrecta] );
    strcpy( (pre->opciones)[(int)pre->opcionCorrecta], temporal );
    pre->opcionCorrecta += 'A';
    //es un intercambio barato, se puede mejorar, solo intercambiamos a la correcta para que no siempre sea la misam
    //depende de rand obvio
}

int cmpOrdenPregunta( const void* p1, const void *p2 ){
    const tPregunta* pre1=p1;
    const tPregunta* pre2=p2;
    return pre1->orden-pre2->orden;
}

int ModificarElOrdenPregunta( void* n, void* d){
    tPregunta *pregunta=n;
    int* orden=d;
    pregunta->orden=*orden;
    (*orden)++;
    return 1;
}

int vaciarRespuestas(void* d, void* d2){
    tPregunta *pregunta=d;
    vaciarListaC(&pregunta->respuestas);
    return 1;
}
