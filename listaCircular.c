#include "listaCircular.h"

void crearListaC ( tListaC *l ){
    *l=NULL;
}

int listaVaciaC ( tListaC *lc ){
    return *lc==NULL;
}

int posicionarseEnSiguiente(tListaC *lc){
    if(*lc!=NULL)
        return 0;
    *lc=(*lc)->sig;

    return 1;
}

int insertarEnSiguiente( tListaC *lc, const void *elem, size_t tamElem){
    tNodo *nue;

    nue = malloc( sizeof(tNodo) );
    if( ! nue )
        return 0;

    nue->dato = malloc( tamElem );
    if( ! nue->dato )
    {
        free( nue );
        return 0;
    }

    nue->tamDato = tamElem;
    memcpy( nue->dato, elem, tamElem );

    if(*lc==NULL){
        nue->sig=nue;
    }else{
        nue->sig=(*lc)->sig;
        (*lc)->sig=nue;
    }

    *lc = nue;

    return 1;
}

int buscarPorClaveYaccionarEnListaC( tListaC *lc, const void *elem, size_t tamElem, Cmp cmp, void *datosAccion, Accion2 acc ){
    tNodo* primero=*lc, *act=*lc;

    if(act==NULL)
        return 0;

    while ( act && act->sig!=primero && cmp(act->dato,elem)!=0 )
        act = act->sig;

    if(cmp(act->dato,elem)!=0 ){
        if(act==primero)  //significa que es el primero, y no encontro
            return 0;
        else{           //caso contrario, me falta ver el ultimo
            act = act->sig;
            if(cmp(act->dato,elem)!=0)
                return 0;
        }
    }

    if(acc)
        acc(act->dato,datosAccion);

    *lc=act;

    return 1;
}

void mapListaC(tListaC *lista, Accion2 acc, void *datoAccion  )
{
    tNodo* primero=*lista;

    while ( *lista && (*lista)->sig!=primero )
    {
        acc( (*lista)->dato, datoAccion );
        lista = & (*lista)->sig;
    }
    if ( *lista)
        acc( (*lista)->dato, datoAccion );
}



void vaciarListaC ( tListaC *lc ){
    tNodo* elim;
    if (*lc==NULL)
        return;
    while(*lc!=NULL && (*lc)!=(*lc)->sig ){
        elim=(*lc)->sig;
        (*lc)->sig=elim->sig;
        free(elim->dato);
        free(elim);
    }

    if(*lc!=NULL && *lc==(*lc)->sig ){
        free((*lc)->dato);
        free(*lc);
    }

    *lc=NULL;
}
