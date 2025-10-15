#include "cierre.h"
#include "reporte.h"
#include <stdio.h>
#include <string.h>

void cierreBancario() {
    reporteTotales();
    char opcion;
    printf("Desea proceder con el cierre bancario? Esto eliminara todas las transacciones (s para si/n para no): \n");
    scanf(" %c", &opcion);
    if (opcion != 's' && opcion != 'S' && opcion != 'n' && opcion != 'N') {
        printf("Opcion invalida. Por favor ingrese 's' para si o 'n' para no.\n");
        return;
    }
    if (opcion == 's' || opcion == 'S') {
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