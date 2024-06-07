#include "lista_simple.h"

void crearLista ( t_Lista *lista )
{
    *lista = NULL;

    return;
}

int listaVacia ( t_Lista *lista )
{
    return ! *lista;
}

void vaciarLista ( t_Lista *lista )
{
    t_Nodo *nodoProxAElim;

    while ( * lista )
    {
        nodoProxAElim = *lista;
        *lista = nodoProxAElim->siguiente;
        free( nodoProxAElim->dato );
        free( nodoProxAElim );
    }

}

void recorrerLista (t_Lista *lista, Accion acc, void *datoAccion  )
{
    while ( * lista )
    {
        acc( (*lista)->dato, datoAccion );
        lista = & (*lista)->siguiente;
    }
}

void mapLista(t_Lista *lista, Accion2 acc, void *datoAccion  )
{
    while ( * lista )
    {
        acc( (*lista)->dato, datoAccion );
        lista = & (*lista)->siguiente;
    }
}

int insertarEnListaAlPrincipioConDuplicados ( t_Lista *lista, const void *elem, size_t tamElem )
{
    t_Nodo *nuevoElem;

    nuevoElem = malloc( sizeof(t_Nodo) );
    if( ! nuevoElem )
        return 0;

    nuevoElem->dato = malloc( tamElem );
    if( ! nuevoElem->dato )
    {
        free( nuevoElem );
        return 0;
    }

    nuevoElem->tamDato = tamElem;
    memcpy( nuevoElem->dato, elem, tamElem );
    nuevoElem->siguiente = *lista;
    *lista = nuevoElem;

    return 1;
}

int insertarEnListaAlFinalConDuplicados ( t_Lista *lista, const void *elem, size_t tamElem )
{
    t_Nodo *nuevoElem;

    while ( * lista )
    {
        lista = & (*lista)->siguiente;
    }

    nuevoElem = malloc( sizeof(t_Nodo) );
    if( ! nuevoElem )
        return 0;

    nuevoElem->dato = malloc( tamElem );
    if( ! nuevoElem->dato )
    {
        free( nuevoElem );
        return 0;
    }

    nuevoElem->tamDato = tamElem;
    memcpy( nuevoElem->dato, elem, tamElem );
    nuevoElem->siguiente = NULL;

    *lista = nuevoElem;

    return 1;
}

int insertarEnListaOrdenadoConDuplicado ( t_Lista *lista, const void *elem, size_t tamElem, Comparacion cmp )
{
    t_Nodo *nuevoElem;

    while ( *lista && cmp( (*lista)->dato, elem ) < 0 )
    {
        lista = & (*lista)->siguiente;
    }

    nuevoElem = malloc( sizeof(t_Nodo) );
    if( ! nuevoElem )
        return 0;

    nuevoElem->dato = malloc( tamElem );
    if( ! nuevoElem->dato )
    {
        free( nuevoElem );
        return 0;
    }

    nuevoElem->tamDato = tamElem;
    memcpy( nuevoElem->dato, elem, tamElem );

    nuevoElem->siguiente = *lista;
    *lista = nuevoElem;

    return 1;
}

int insertarEnListaAlPrincipioSinDuplicados ( t_Lista *lista, const void *elem, size_t tamElem, Comparacion cmp, void *datosAccion, Accion acc )
{
    t_Lista *punLisTemp = lista;
    t_Nodo *nuevoElem;

    while ( *punLisTemp )
    {
        if( cmp( (*punLisTemp)->dato, elem ) == 0 )
        {
            if( acc )
                acc( (*punLisTemp)->dato, datosAccion );

            return 0;
        }

        punLisTemp = & (*punLisTemp)->siguiente;
    }

    nuevoElem = malloc( sizeof(t_Nodo) );
    if( ! nuevoElem )
        return 0;

    nuevoElem->dato = malloc( tamElem );
    if( ! nuevoElem->dato )
    {
        free( nuevoElem );
        return 0;
    }

    nuevoElem->tamDato = tamElem;
    memcpy( nuevoElem->dato, elem, tamElem );
    nuevoElem->siguiente = *lista;
    *lista = nuevoElem;

    return 1;
}

int insertarEnListaAlFinalSinDuplicados ( t_Lista *lista, const void *elem, size_t tamElem, Comparacion cmp, Accion3 acc )
{
    t_Nodo *nuevoElem;

    while ( * lista && cmp( elem, (*lista)->dato ) != 0 )
    {
        lista = & (*lista)->siguiente;
    }



    if( * lista )
    {
        if( acc )
            acc( (*lista)->dato , elem );
        return 0;
    }

    nuevoElem = malloc( sizeof(t_Nodo) );
    if( ! nuevoElem )
        return 0;

    nuevoElem->dato = malloc( tamElem );
    if( ! nuevoElem->dato )
    {
        free( nuevoElem );
        return 0;
    }

    nuevoElem->tamDato = tamElem;
    memcpy( nuevoElem->dato, elem, tamElem );
    nuevoElem->siguiente = NULL;

    *lista = nuevoElem;

    return 1;
}

int insertarEnListaOrdenadoSinDuplicados ( t_Lista *lista, const void *elem, size_t tamElem,
                                           Comparacion cmp, void *datosAccion, Accion2 acc )
{
    t_Nodo *nuevoElem;
    int resulCmp = -1;

    while ( *lista && ( resulCmp = cmp( (*lista)->dato, elem ) ) < 0 )
    {
        lista = & (*lista)->siguiente;
    }

    if( resulCmp == 0 )
    {
        if( acc )
            acc( (*lista)->dato, datosAccion );
        return 0;
    }

    nuevoElem = malloc( sizeof(t_Nodo ) );
    if( !nuevoElem )
        return 0;

    nuevoElem->dato = malloc( tamElem );
    if( !nuevoElem->dato )
    {
        free( nuevoElem );
        return 0;
    }

    nuevoElem->tamDato = tamElem;
    memcpy( nuevoElem->dato, elem, tamElem );

    nuevoElem->siguiente = *lista;
    *lista = nuevoElem;

    return 1;
}

int insertarEnListaEnPos ( t_Lista *lista, const void *elem, size_t tamElem, size_t posAInsert )
{
    t_Nodo *nuevoElem;

    if( posAInsert < 0 )
        return 0; //posiciones negativas, no esta definida la funcion

    while ( *lista && posAInsert )
    {
        posAInsert--;
        lista = & (*lista)->siguiente;
    }

    nuevoElem = malloc( sizeof(t_Nodo) );
    if( ! nuevoElem )
        return 0;
    nuevoElem->dato = malloc( tamElem );
    if( ! nuevoElem->dato )
    {
        free( nuevoElem );
        return 0;
    }

    nuevoElem->tamDato = tamElem;
    memcpy( nuevoElem->dato, elem, tamElem );

    nuevoElem->siguiente = *lista;
    *lista = nuevoElem;

    return 1;
}

int verDatoDeListaEnPos( t_Lista *lista, void *destino, size_t tamElem,size_t pos )
{

    if( ! *lista )
        return 0;//lista vacia

    while (* lista && pos )
    {
        lista = & (*lista)->siguiente;
        pos--;
    }

    if( pos )
        return 0; //posicion invalida

    memcpy( destino, (*lista)->dato, MENOR( tamElem, (*lista)->tamDato ) );

    return 1;
}

int sacarDeListaEnPos( t_Lista *lista, void *destino, size_t tamElem,size_t pos )
{
    t_Nodo *nodoAElim;

    if( ! *lista )
        return 0;//lista vacia

    while (* lista && pos )
    {
        lista = & (*lista)->siguiente;
        pos--;
    }

    if( pos )
        return 0; //posicion invalida

    memcpy( destino, (*lista)->dato, MENOR( tamElem, (*lista)->tamDato ) );
    nodoAElim = *lista;
    *lista = nodoAElim->siguiente;

    free( nodoAElim->dato );
    free( nodoAElim );

    return 1;
}

int eliminarDeListaEnPos( t_Lista *lista, size_t pos ) //si no existe la posicion no elimino nada
{
    t_Nodo *nodoAElim;

    if( ! *lista )
        return 0;

    while (* lista && pos )
    {
        lista = & (*lista)->siguiente;
        pos--;
    }

    if( pos )
        return 0; //posicion invalida

    nodoAElim = *lista;
    *lista = nodoAElim->siguiente;

    free( nodoAElim->dato );
    free( nodoAElim );

    return 1;
}

int buscarEnListaSegunCmp ( t_Lista *lista, Comparacion cmp, t_Lista **elejido  ) //retorna 0 si no tenia elementos la lista por ende no busco nada
{

    *elejido = lista;//Tomo el primer elemento como menor

    while ( * lista )
    {
        lista = & (*lista)->siguiente;
        if ( cmp( (*lista)->dato, (**elejido)->dato ) < 0 )
        {
            *elejido = lista;
        }

    }

    return  (*elejido) ?  1 :  0; //si no havia nada en la lista, no encontre nada,
}

void ordenarLista( t_Lista *lista, Comparacion cmp )
{
    //El ordenamiento consiste en seleccionar el menor elemento y colocarlo en su respectiva posicion, ordenamiento por seleccion
    //No es el metodo mas eficiente,
    t_Lista *menor;
    t_Nodo *auxMenor;

    while ( *lista )
    {
        buscarEnListaSegunCmp( lista, cmp, &menor );
        auxMenor = *menor;
        *menor = (*menor)->siguiente;
        auxMenor->siguiente = *lista;
        *lista = auxMenor;
        lista = & (*lista)->siguiente;
    }

}

int buscarDatoEnListaDesordenadaPorClave ( const t_Lista *lista, size_t tamELem, void *clave, Comparacion cmp )
{
    while ( *lista )
    {
        if( cmp( clave, (*lista)->dato ) == 0 )
        {
            memcpy( clave, (*lista)->dato, MENOR( tamELem, (*lista)->tamDato ) );
            return 1;
        }

        lista = & (*lista)->siguiente;
    }

    return 0;
}

size_t lista_Filter ( t_Lista *lista, Comparacion cmp, const void *base )
{
    t_Nodo *nodoElim;
    size_t cantElem = 0;

    while( *lista )
    {
        if( cmp( (*lista)->dato, base ) != 1 ) //si es 1 indica que cumple la condicion
        {
            nodoElim = *lista;
            *lista = nodoElim->siguiente;
            free(nodoElim->dato);
            free(nodoElim);

        }
        else
        {
            lista = & (*lista)->siguiente;
            cantElem++;
        }

    }
    return cantElem;
}
