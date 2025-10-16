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
    do{
        printf("Menu de procesos financieros (0 para salir):\n");
        printf("1. Procesar compra\n");
        printf("2. Anulacion\n");
        printf("3. Cierre\n");
        printf("4. Reimpresion\n");
        printf("5. Reporte totales\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        switch(opcion){
            case 1:
                clear_input_buffer(); // Limpiar buffer antes de procesar compra
                procesarCompra();
                printf("Presione Enter para continuar...");
                clear_input_buffer(); // Limpiar buffer
                system("cls"); // Limpiar pantalla (funciona en Windows)
                break;
            case 2:
                clear_input_buffer();
                printf("************************************************\n");
                printf("Anulacion...\n");
                unsigned int referencia;
                printf("Ingrese la referencia de la compra a anular: ");
                scanf("%u", &referencia);
                anularCompra(referencia);
                printf("Presione Enter para continuar...");
                clear_input_buffer(); // Limpiar buffer
                system("cls"); // Limpiar pantalla (funciona en Windows)
                break;
            case 3:
                clear_input_buffer();
                printf("************************************************\n");
                printf("Cierre...\n");
                cierreBancario();
                printf("Presione Enter para continuar...");
                clear_input_buffer(); // Limpiar buffer
                system("cls"); // Limpiar pantalla (funciona en Windows)
                break;
            case 4:
                clear_input_buffer();
                printf("************************************************\n");
                printf("Reimpresion...\n");
                reimprimir();
                printf("Presione Enter para continuar...");
                clear_input_buffer(); // Limpiar buffer
                system("cls"); // Limpiar pantalla (funciona en Windows)
                break;
            case 5:
                clear_input_buffer();
                printf("************************************************\n");
                printf("Reporte totales...\n");
                reporteTotales();
                printf("Presione Enter para continuar...");
                clear_input_buffer(); // Limpiar buffer
                system("cls"); // Limpiar pantalla (funciona en Windows)
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