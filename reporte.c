#include "reporte.h"
#include <stdio.h>
#include <string.h>

void reporteTotales() {
    printf("-----------------------------------REPORTE TOTAL DE COMPRAS-----------------------------------\n");
    FILE *archivo = fopen("compras.dat", "rb");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo de compras.\n");
        return;
    }

    int contador = 0;
    int contadorAnuladas = 0;
    int contadorVigentes = 0;
    double montoTotal = 0.0;
    Compra compra;

    while (fread(&compra, sizeof(Compra), 1, archivo) == 1) {
        contador++;
        // Contar las compras anuladas y vigentes
        if (strcmp(compra.estado, "Anulada") == 0)
            contadorAnuladas++;
        else if (strcmp(compra.estado, "Vigente") == 0){
            contadorVigentes++;
            montoTotal += compra.montoDolares;
        }
    }
    fclose(archivo);

    printf("Total de compras: %d\n", contador);
    printf("Compras vigentes: %d\n", contadorVigentes);
    printf("Compras anuladas: %d\n", contadorAnuladas);
    printf("Monto total: %.2lf\n", montoTotal);
}