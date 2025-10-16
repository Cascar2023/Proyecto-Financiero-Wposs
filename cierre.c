#include "cierre.h"
#include "reporte.h"
#include <stdio.h>
#include <string.h>

void cierreBancario() {
    printf("-----------------------------------CIERRE BANCARIO-----------------------------------\n");
    reporteTotales();
    char opcion[20];
    //Se hace el reporte y se pregunta si se desea proceder con el cierre
    printf("Desea proceder con el cierre bancario? Esto eliminara todas las transacciones (s para si/n para no): \n");
    gets(opcion);
    if (strlen(opcion) > 1) {
        printf("Opcion invalida, debe ingresar solo un caracter. Por favor ingrese 's' para si o 'n' para no.\n");
        return;
    }
    if (strcmp(opcion, "s") != 0 && strcmp(opcion, "S") != 0 && strcmp(opcion, "n") != 0 && strcmp(opcion, "N") != 0) {
        printf("Opcion invalida. Por favor ingrese 's' para si o 'n' para no.\n");
        return;
    }
    if (strcmp(opcion, "s") == 0 || strcmp(opcion, "S") == 0) {
        FILE *archivo = fopen("compras.dat", "wb"); // Abrir en modo binario y truncar
        if (archivo != NULL) {
            fclose(archivo);
            printf("Cierre bancario realizado con exito.\n");
        } else {
            printf("Error al abrir el archivo de compras para el cierre.\n");
        }
    } else {
        printf("Cierre bancario cancelado.\n");
    }
}