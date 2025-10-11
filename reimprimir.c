#include "reimpresion.h"
#include <stdio.h>
#include <string.h>

#define MAX_COMPRAS 20
#define MAX_LINEA 256

void reimprimir() {
    char compras[MAX_COMPRAS][MAX_LINEA]; // Array para almacenar las compras
    printf("Las compras registradas empezando desde la ultima son las siguientes:\n");
    FILE *archivo = fopen("compras.txt", "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo de compras.\n");
        return;
    }
    char linea[MAX_LINEA];
    int contador = 0;
    while (fgets(linea, sizeof(linea), archivo) && contador < MAX_COMPRAS) {
        strcpy(compras[contador], linea);
        contador++;
    }
    fclose(archivo);

    for (int i = contador - 1; i >= 0; i--) {
        printf("%s", compras[i]);
    }
}