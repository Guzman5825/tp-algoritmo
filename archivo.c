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

    if(*cantRondas>33 || *cantRondas<1){
        puts("error, cantidad de rondas, supero el limite permitido, ingrese un cantidad menor 34 y mayor a 0");
        return ERROR;
    }

    return TODO_OK;
}
