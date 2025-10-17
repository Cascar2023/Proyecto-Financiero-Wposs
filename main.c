#include <stdio.h>
#include <stdlib.h>
#include "compra.h"
#include "anulacion.h"
#include "reporte.h"
#include "reimpresion.h"
#include "cierre.h"
#include "utils.h"

int main(){
    int opcion;
    char entrada[32];
    do{
        printf("Menu de procesos financieros (0 para salir):\n");
        printf("1. Procesar compra\n");
        printf("2. Anulacion\n");
        printf("3. Cierre\n");
        printf("4. Reimpresion\n");
        printf("5. Reporte totales\n");

        /* leer la opcion con fgets y validarla: no permitir ENTER vacio ni entradas no numéricas */
        while (1) {
            printf("Seleccione una opcion: ");
            if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
                /* EOF o error: repetir */
                printf("Entrada no valida. Intente nuevamente.\n");
                continue;
            }
            entrada[strcspn(entrada, "\r\n")] = '\0'; /* eliminar salto de linea */
            if (entrada[0] == '\0') {
                printf("Entrada vacia no permitida. Intente nuevamente.\n");
                continue;
            }
            char *endptr;
            long val = strtol(entrada, &endptr, 10);
            if (endptr == entrada || *endptr != '\0') {
                printf("Entrada invalida. Debe ingresar un numero.\n");
                continue;
            }
            opcion = (int)val;
            break;
        }

        switch(opcion){
            case 1:
                procesarCompra();
                printf("Presione Enter para continuar...");
                clear_input_buffer(); // Limpiar buffer
                clear_screen(); // Limpiar pantalla
                break;
            case 2:
                printf("************************************************\n");
                printf("Anulacion...\n");
                {
                    unsigned int referencia;
                    /* Leer referencia con validación para evitar ENTER vacío */
                    while (1) {
                        printf("Ingrese la referencia de la compra a anular: ");
                        if (fgets(entrada, sizeof(entrada), stdin) == NULL) continue;
                        entrada[strcspn(entrada, "\r\n")] = '\0';
                        if (entrada[0] == '\0') { printf("Entrada vacia no permitida.\n"); continue; }
                        char *endptr;
                        unsigned long v = strtoul(entrada, &endptr, 10);
                        if (endptr == entrada || *endptr != '\0') { printf("Referencia invalida.\n"); continue; }
                        referencia = (unsigned int)v;
                        break;
                    }
                    anularCompra(referencia);
                }
                printf("Presione Enter para continuar...");
                clear_input_buffer(); // Limpiar buffer
                clear_screen(); // Limpiar pantalla
                break;
            case 3:
                printf("************************************************\n");
                printf("Cierre...\n");
                cierreBancario();
                printf("Presione Enter para continuar...");
                clear_input_buffer(); // Limpiar buffer
                clear_screen(); // Limpiar pantalla
                break;
            case 4:
                printf("************************************************\n");
                printf("Reimpresion...\n");
                reimprimir();
                printf("Presione Enter para continuar...");
                clear_input_buffer(); // Limpiar buffer
                clear_screen(); // Limpiar pantalla
                break;
            case 5:
                printf("************************************************\n");
                printf("Reporte totales...\n");
                reporteTotales();
                printf("Presione Enter para continuar...");
                clear_input_buffer(); // Limpiar buffer
                clear_screen(); // Limpiar pantalla
                break;
            case 0:
                printf("Saliendo del programa.\n");
                break;
            default:
                printf("Opcion invalida. Intente de nuevo.\n");
        }
    } while (opcion != 0);
    return 0;
}