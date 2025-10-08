#include <stdio.h>
#include "compra.h"

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
                printf("Procesando compra...\n");
                procesarCompra();
                break;
            case 2:
                printf("Anulacion...\n");
                // Llamar a la funcion de anulacion
                break;
            case 3:
                printf("Cierre...\n");
                // Llamar a la funcion de cierre
                break;
            case 4:
                printf("Reimpresion...\n");
                // Llamar a la funcion de reimpresion
                break;
            case 5:
                printf("Reporte totales...\n");
                // Llamar a la funcion de reporte totales
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