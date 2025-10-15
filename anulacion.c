//***************************************************************************************************************************/
//*****************************************************Anulacion*************************************************************/
//***************************************************************************************************************************/

#include "anulacion.h"
#include "compra.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define archivo_compras "compras.dat"

void anularCompra(unsigned int referencia) {
    FILE *archivo = fopen(archivo_compras, "r+b");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de compras.\n");
        return;
    }

    Compra compra;
    int encontrada = 0;
    long posicion = 0;

    while (fread(&compra, sizeof(Compra), 1, archivo) == 1) {
        if (compra.referencia == referencia) {
            encontrada = 1;
            break;
        }
        posicion++;
    }

    if (!encontrada) {
        printf("No se encontro una compra con la referencia %u.\n", referencia);
        fclose(archivo);
        return;
    }

    if (strcmp(compra.estado, "Anulada") == 0) {
        printf("La compra con referencia %u ya ha sido anulada previamente.\n", referencia);
        fclose(archivo);
        return;
    }

    // Verificación de identidad
    printf("Antes de anular la compra, indique los 4 ultimos digitos del PAN y su CVV para verificar su identidad:\n");
    char panVerificacion[5];
    char cvvVerificacion[6];
    printf("PAN (4 ultimos): ");
    scanf("%4s", panVerificacion);
    printf("CVV: ");
    scanf("%5s", cvvVerificacion);

    int panLen = strlen(compra.PAN);
    if (strncmp(compra.PAN + panLen - 4, panVerificacion, 4) != 0 ||
        strcmp(compra.cvv, cvvVerificacion) != 0) {
        printf("Los datos proporcionados no coinciden con los de la compra. Anulacion cancelada.\n");
        fclose(archivo);
        return;
    }

    strcpy(compra.estado, "Anulada");

    // Mover el puntero al registro correcto y sobrescribir
    fseek(archivo, posicion * sizeof(Compra), SEEK_SET);
    fwrite(&compra, sizeof(Compra), 1, archivo);
    fflush(archivo);

    printf("Compra con referencia %u anulada exitosamente.\n", referencia);

    fclose(archivo);
}