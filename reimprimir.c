#include "reimpresion.h"
#include "compra.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_COMPRAS 20
#define MAX_LINEA 256

void enmascararPAN(char *pan) {
    int len = strlen(pan);
    if (len >= 8) {
        char panMascara[25];
        strncpy(panMascara, pan, 4);
        for (int j = 4; j < len - 4; j++) {
            panMascara[j] = '*';
        }
        strncpy(panMascara + len - 4, pan + len - 4, 4);
        panMascara[len] = '\0';
        strcpy(pan, panMascara);
    }
}

void imprimirTablaHeader() {
    printf("%-10s %-22s %-12s %-6s %-8s %-10s %-10s\n",
        "Monto", "PAN", "Franquicia", "CVV", "Venc.", "Ref.", "Estado");
    printf("---------------------------------------------------------------------------------------------\n");
}

void imprimirCompraEnTabla(const Compra *compra) {
    char panMascara[25];
    strncpy(panMascara, compra->PAN, sizeof(panMascara));
    panMascara[sizeof(panMascara)-1] = '\0';
    enmascararPAN(panMascara);

    printf("%-10.2lf %-22s %-12s %-6s %-8s %-10u %-10s\n",
        compra->montoDolares,
        panMascara,
        compra->franquicia,
        compra->cvv,
        compra->fechaVencimiento,
        compra->referencia,
        compra->estado);
}

void reimprimir() {
    Compra compras[MAX_COMPRAS];
    FILE *archivo = fopen("compras.dat", "rb");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo de compras.\n");
        return;
    }

    int contador = 0;
    while (fread(&compras[contador], sizeof(Compra), 1, archivo) == 1 && contador < MAX_COMPRAS) {
        contador++;
    }
    fclose(archivo);

    if (contador == 0) {
        printf("No hay compras registradas.\n");
        return;
    }

    int pos = contador - 1;
    char opcion;
    do {
        system("cls");
        imprimirTablaHeader();
        imprimirCompraEnTabla(&compras[pos]);

        printf("\n");
        if (pos > 0) printf("<- [a] ");
        else printf("        ");
        printf("Transaccion %d de %d", contador - pos, contador);
        if (pos < contador - 1) printf(" [d] ->");
        printf("\nPresiona 'a' (izquierda), 'd' (derecha), o 'q' para salir: ");

        opcion = getchar();
        while (opcion == '\n') opcion = getchar();

        if (opcion == 'a' && pos > 0) pos--;
        else if (opcion == 'd' && pos < contador - 1) pos++;
    } while (opcion != 'q');
}