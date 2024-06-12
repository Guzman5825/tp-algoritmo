#include "archivo.h"

int cargarConfiguracionDeTxt(int* tiempoLimite,int* cantRondas){
    FILE *pfConfig;
    char linea[MAX_LINEA_CONFIG];

    if(!(pfConfig = fopen("config.txt", "rt")))
    {
        puts("no se pudo abrir el archivo config.txt");
        return ERROR_APERTURA_ARCHIVO;
    }

    if(fgets(linea, MAX_LINEA_CONFIG, pfConfig))
        sscanf(linea,"Rounds: %d",cantRondas);

    if(fgets(linea, MAX_LINEA_CONFIG, pfConfig))
        sscanf(linea,"Tiempo por round: %d",tiempoLimite);

    fclose(pfConfig);

    if(*cantRondas<4 || *cantRondas>7){
        puts("Error: cantidad de rondas, supero el limite permitido, ingrese un cantidad mayor 4 a 7 rondas");
        return ERROR;
    }

    if(*tiempoLimite<=0){
        puts("Error: tiempo limite invalido");
        return ERROR;
    }

    return TODO_OK;
}
