#include "compra.h"
#include <stdio.h>
#include <time.h>

#define MAX_COMPRAS 20
#define referencia_inicial 1000
#define archivo_compras "compras.txt"

void procesarCompra() {
    Compra compra;
    FILE *archivo = fopen(archivo_compras, "r");
    unsigned int referenciaActual = referencia_inicial - 1; // Para que la primera sea 1000 si no hay nada

    if (archivo != NULL) {
        char linea[256];
        unsigned int refTemp;
        while (fgets(linea, sizeof(linea), archivo) != NULL) {
            if (sscanf(linea, "%*[^R]Referencia: %u", &refTemp) == 1) {
                if (refTemp > referenciaActual) {
                    referenciaActual = refTemp;
                }
            }
        }
        fclose(archivo);
        compra.referencia = referenciaActual + 1;
    } else {
        compra.referencia = referencia_inicial;
    }

    printf("ingrese el monto en dolares de su compra: \n");
    scanf("%lf", &compra.montoDolares);

    if (compra.montoDolares <= 0) {
        printf("El monto de la compra debe ser mayor a cero.\n");
        return;
    }

    printf("ingrese el PAN de su tarjeta: \n");
    getchar();  // Limpiar el buffer
    fgets(compra.PAN, sizeof(compra.PAN), stdin);
    compra.PAN[strcspn(compra.PAN, "\n")] = 0; // Eliminar salto de línea
    printf("ingrese el CVV de su tarjeta: \n");
    scanf("%d", &compra.cvv);
    getchar();  // Limpiar el buffer

    if(compra.cvv < 100 || compra.cvv > 9999) {
        printf("CVV invalido. Debe ser un numero de 3 o 4 digitos.\n");
        return;
    }

    printf("ingrese la fecha de vencimiento de su tarjeta (MM/AA): \n");
    fgets(compra.fechaVencimiento, sizeof(compra.fechaVencimiento), stdin);
    compra.fechaVencimiento[strcspn(compra.fechaVencimiento, "\n")] = 0; // Eliminar salto de línea

    if(compra.fechaVencimiento[0] < '0' || compra.fechaVencimiento[0] > '1' || 
        compra.fechaVencimiento[1] < '0' || compra.fechaVencimiento[1] > '2' || 
        compra.fechaVencimiento[2] != '/' || 
        compra.fechaVencimiento[3] < '2' || compra.fechaVencimiento[3] > '9' || 
        compra.fechaVencimiento[4] < '0' || compra.fechaVencimiento[4] > '9') {
        printf("Fecha de vencimiento invalida. Formato correcto MM/AA.\n");
        return;
    }



    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int mesVenc = (compra.fechaVencimiento[0] - '0') * 10 + (compra.fechaVencimiento[1] - '0');
    int anioVenc = (compra.fechaVencimiento[3] - '0') * 10 + (compra.fechaVencimiento[4] - '0') + 2000;
    if (anioVenc < (tm.tm_year + 1900) || (anioVenc == (tm.tm_year + 1900) && mesVenc < (tm.tm_mon + 1))) {
        printf("La tarjeta ha expirado.\n");
        return;
    }

    if (compra.referencia >= referencia_inicial + MAX_COMPRAS) {
        printf("Se ha alcanzado el maximo de compras permitidas.\n");
        return;
    }

    guardarCompra(&compra);
    printf("Compra procesada y guardada con exito.\n");
}

void guardarCompra(const Compra *compra) {
    FILE *archivo = fopen("compras.txt", "a+t");
    if (archivo == NULL) {
        printf("Error al abrir el archivo para guardar la compra.\n");
        return;
    }
    fprintf(archivo, "Monto: %.2f, PAN: %s, CVV: %d, Vencimiento: %s, Referencia: %u\n",
        compra->montoDolares, compra->PAN, compra->cvv, compra->fechaVencimiento, compra->referencia);
    fclose(archivo);
    printf("Compra guardada en el archivo compras.txt\n");
}