#include <stdio.h>
#include "compra.h"
#include "anulacion.h"

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
                getchar(); // Limpiar buffer
                printf("Presione Enter para continuar...");
                getchar(); // Esperar a que el usuario presione una tecla
                system("cls"); // Limpiar pantalla (funciona en Windows)
                break;
            case 2:
                printf("Anulacion...\n");
                unsigned int referencia;
                printf("Ingrese la referencia de la compra a anular: ");
                scanf("%u", &referencia);
                anularCompra(referencia);
                getchar(); // Limpiar buffer
                printf("Presione Enter para continuar...");
                getchar(); // Esperar a que el usuario presione una tecla
                system("cls"); // Limpiar pantalla (funciona en Windows)
                break;
            case 3:
                printf("Cierre...\n");
                getchar(); // Limpiar buffer
                printf("Presione Enter para continuar...");
                getchar(); // Esperar a que el usuario presione una tecla
                system("cls"); // Limpiar pantalla (funciona en Windows)
                break;
            case 4:
                printf("Reimpresion...\n");
                reimprimir();
                getchar(); // Limpiar buffer
                printf("Presione Enter para continuar...");
                getchar(); // Esperar a que el usuario presione una tecla
                system("cls"); // Limpiar pantalla (funciona en Windows)
                break;
            case 5:
                printf("Reporte totales...\n");
                getchar(); // Limpiar buffer
                printf("Presione Enter para continuar...");
                getchar(); // Esperar a que el usuario presione una tecla
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