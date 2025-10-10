//***************************************************************************************************************************/
//*****************************************************Anulacion*************************************************************/
//***************************************************************************************************************************/

#include "anulacion.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define archivo_compras "compras.txt"

void anularCompra(unsigned int referencia) {
    FILE *archivo = fopen(archivo_compras, "r+");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de compras.\n");
        return;
    }

    char linea[256];
    long posicion = 0;
    int encontrada = 0;

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        unsigned int refTemp;
        if (sscanf(linea, "%*[^R]Referencia: %u", &refTemp) == 1 && refTemp == referencia) {
            encontrada = 1;
            break;
        }
        posicion = ftell(archivo);
    }

    if (!encontrada) {
        printf("No se encontro una compra con la referencia %u.\n", referencia);
        fclose(archivo);
        return;
    }

    // Mover el puntero al inicio de la línea encontrada
    fseek(archivo, posicion, SEEK_SET);

    // Leer la línea completa
    if (fgets(linea, sizeof(linea), archivo) != NULL) {
        // Buscar "Estado: " y cambiar su valor de 'Vigente' a 'Anulada'
        char *estadoPtr = strstr(linea, "Vigente");
        if (estadoPtr != NULL && strncmp(estadoPtr, "Anulada", 7) == 0) {
            printf("La compra con referencia %u ya ha sido anulada previamente.\n", referencia);
            fclose(archivo);
            return;
        }
        if (estadoPtr != NULL && strlen(estadoPtr) >= 7) { // "Estado: X" tiene al menos 7 caracteres
            printf("Antes de anular la compra, indiquenos los 4 ultimos digitos del PAN y su CVV para verificar su identidad:\n");
            char panVerificacion[20];
            char cvvVerificacion[5];
            printf("PAN: ");
            scanf("%19s", panVerificacion);
            printf("CVV: ");
            scanf("%4s", cvvVerificacion);

            if(strstr(linea, panVerificacion) == NULL || strstr(linea, cvvVerificacion) == NULL) {
                printf("Los datos proporcionados no coinciden con los de la compra. Anulacion cancelada.\n");
                fclose(archivo);
                return;
            }

            strcpy(estadoPtr, "Anulada"); // Cambiar el estado a 'Anulada'

            // Mover el puntero de nuevo al inicio de la línea para sobrescribir
            fseek(archivo, posicion, SEEK_SET);
            fputs(linea, archivo);
            fflush(archivo); // Asegurar que los cambios se escriban en el archivo
            printf("Compra con referencia %u anulada exitosamente.\n", referencia);
        } else {
            printf("No se pudo encontrar el estado en la linea de la compra.\n");
        }
    } else {
        printf("Error al leer la linea de la compra.\n");
    }

    fclose(archivo);
}