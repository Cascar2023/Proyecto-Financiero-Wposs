#include "cierre.h"
#include "reporte.h"
#include <stdio.h>
#include <string.h>

void cierreBancario() {
    printf("-----------------------------------CIERRE BANCARIO-----------------------------------\n");
    reporteTotales();

    char opcion[4];

    while (1) {
        printf("Desea proceder con el cierre bancario? Esto eliminara todas las transacciones (s para si/n para no): ");
        if (fgets(opcion, sizeof(opcion), stdin) == NULL) {
            // Entrada no válida, limpiar buffer y reintentar
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            printf("Entrada invalida. Intente nuevamente.\n");
            continue;
        }

        // Eliminar salto de linea y espacios finales
        opcion[strcspn(opcion, "\r\n")] = '\0';

        if (strlen(opcion) != 1) {
            printf("Opcion invalida, debe ingresar solo un caracter ('s' o 'n').\n");
            continue;
        }

        char c = opcion[0];
        if (c == 's' || c == 'S') {
            FILE *archivo = fopen("compras.dat", "wb"); // Abrir en modo binario y truncar
            if (archivo != NULL) {
                fclose(archivo);
                printf("Cierre bancario realizado con exito.\n");
            } else {
                printf("Error al abrir el archivo de compras para el cierre.\n");
            }
            break;
        } else if (c == 'n' || c == 'N') {
            printf("Cierre bancario cancelado.\n");
            break;
        } else {
            printf("Opcion invalida. Por favor ingrese 's' para si o 'n' para no.\n");
            continue;
        }
    }
}