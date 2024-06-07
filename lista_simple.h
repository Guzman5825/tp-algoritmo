#ifndef LISTA_SIMPLE_H_INCLUDED
#define LISTA_SIMPLE_H_INCLUDED
#include <string.h>
#include <stdlib.h> //la uno por que contiene a malloc
#include <stddef.h> //la incluyo por el tipo de dato size_t, es lo correcto
#define MENOR(x,y) ( (x)<(y)? (x) : (y) )

typedef struct  s_nodo
{
    void *dato;
    size_t tamDato;
    struct s_nodo *siguiente;
}
t_Nodo;

typedef t_Nodo* t_Lista;

typedef int(*Comparacion)(const void*,const void* );//politica (elemNodo, elemExterno)
typedef int(*Accion)( const void*, void* );
typedef int(*Accion2)( void*, void* );
typedef int(*Accion3)( const void*,const void* );

void crearLista ( t_Lista *lista );

int listaVacia ( t_Lista *lista );
void vaciarLista ( t_Lista *lista );
void recorrerLista (t_Lista *lista, Accion acc, void *datoAccion  );
void mapLista(t_Lista *lista, Accion2 acc, void *datoAccion  );

int insertarEnListaAlPrincipioConDuplicados ( t_Lista *lista, const void *elem, size_t tamElem );
int insertarEnListaAlFinalConDuplicados ( t_Lista *lista, const void *elem, size_t tamElem );
int insertarEnListaOrdenadoConDuplicado ( t_Lista *lista, const void *elem, size_t tamElem, Comparacion cmp );

int insertarEnListaAlPrincipioSinDuplicados ( t_Lista *lista, const void *elem, size_t tamElem, Comparacion cmp, void *datosAccion, Accion acc );
int insertarEnListaAlFinalSinDuplicados ( t_Lista *lista, const void *elem, size_t tamElem, Comparacion cmp, Accion3 acc );
int insertarEnListaOrdenadoSinDuplicados ( t_Lista *lista, const void *elem, size_t tamElem, Comparacion cmp, void *datosAccion, Accion2 acc );

int insertarEnListaEnPos ( t_Lista *lista, const void *elem, size_t tamElem, size_t posAInsert );

int verDatoDeListaEnPos( t_Lista *lista, void *destino, size_t tamElem,size_t pos );
int sacarDeListaEnPos( t_Lista *lista, void *destino, size_t tamElem,size_t pos );
int eliminarDeListaEnPos( t_Lista *lista, size_t pos );

int buscarEnListaSegunCmp ( t_Lista *lista, Comparacion cmp, t_Lista **elejido  );//-1 si no hay menor/mayor, 0 si hay un unico menor/mayor, 1 si existe mas de un  menor/mayor
void ordenarLista( t_Lista *lista, Comparacion cmp );

int buscarDatoEnListaDesordenadaPorClave ( const t_Lista *lista, size_t tamELem, void *clave, Comparacion cmp );

size_t lista_Filter ( t_Lista *lista, Comparacion cmp, const void *base );

#endif // LISTA_SIMPLE_H_INCLUDED
