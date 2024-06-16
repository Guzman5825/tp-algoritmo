#include "input.h"

int obtenerRespuestaDeTecladoTemporizado(char* respuesta,double *tiempoRespuesta,int tiempoLimite)
{
    *respuesta='-';
    clock_t start_time=clock();
    *tiempoRespuesta = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    while ( *tiempoRespuesta<tiempoLimite && !esOpcionValida(*respuesta))
    {
        usleep(1000);  // es necesario para no forzar al procesador
        if (_kbhit())
            *respuesta = toupper(_getch());
        *tiempoRespuesta = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    }
    return TODO_OK;
}

char obtenerRespuestaDeTecladoEntre(char letraMenor,char letraMayor){
    char opcionEligida='-';
    while( !(opcionEligida>=letraMenor && opcionEligida<=letraMayor) ) {
        usleep(1000);
        if (kbhit())
            opcionEligida =  toupper(_getch());
    }

    return opcionEligida;
}

int obtenerTextoNoVacioDeTecladoYLimitado(char* TextoDestino,int limiteCaracter){
    char *p;
    if(TextoDestino==NULL)
        return 0;
    do{
        fflush(stdin);
        fgets( TextoDestino, limiteCaracter, stdin );
        if(*TextoDestino=='\n')
            puts("no se permite texto vacio, ingrese un texto valido...");
    }while(*TextoDestino=='\n');
    p=strchr(TextoDestino,'\n');
    if(p!=NULL && *p=='\n')
        *p='\0';
    return 1;
}

int obtenerNombreDeArchivoConFecha(char* textoDestino,unsigned tamTextoDestino){
    time_t tiempoTranscurrido = time(NULL);
    struct tm *fechaHora = localtime(&tiempoTranscurrido);
    snprintf(textoDestino, tamTextoDestino, "informe-juego_%4d-%02d-%02d-%02d-%02d.txt",
             (fechaHora->tm_year + 1900), (fechaHora->tm_mon + 1), fechaHora->tm_mday, fechaHora->tm_hour, fechaHora->tm_min);
    return 1;
}
