#ifndef LISTACIRCULAR_H_INCLUDED
#define LISTACIRCULAR_H_INCLUDED

#include <string.h>
#include <stdlib.h> //la uno por que contiene a malloc
#include <stddef.h> //la incluyo por el tipo de dato size_t, es lo correcto
#define MENOR(x,y) ( (x)<(y)? (x) : (y) )

typedef struct  sNodo{
    void *dato;
    size_t tamDato;
    struct sNodo *sig;
}
tNodo;

typedef tNodo* tListaC;

typedef int(*Cmp)(const void*,const void* );//politica (elemNodo, elemExterno)
typedef int(*Accion)( const void*, void* );
typedef int(*Accion2)( void*, void* );

void crearListaC ( tListaC *l );
int listaVaciaC ( tListaC *lc );
int posicionarseEnSiguiente(tListaC *lc);
int insertarEnSiguiente( tListaC *lista, const void *elem, size_t tamElem);
void mapListaC(tListaC *lista, Accion2 acc, void *datoAccion );
int buscarPorClaveYaccionarEnListaC( tListaC *lista, const void *elem, size_t tamElem, Cmp cmp, void *datosAccion, Accion2 acc );
void vaciarListaC ( tListaC *lc );


#endif // LISTACIRCULAR_H_INCLUDED
