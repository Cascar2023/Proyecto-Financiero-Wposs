#include "reimpresion.h"
#include "compra.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_COMPRAS 20
#define MAX_LINEA 256

// ...existing code...
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
    printf("----------------------------------------REIMPRESION-----------------------------------------------\n");
    printf("%-10s %-22s %-18s %-6s %-8s %-10s %-10s\n",
        "Monto", "PAN", "Franquicia", "CVV", "Venc.", "Ref.", "Estado");
    printf("---------------------------------------------------------------------------------------------\n");
}

void imprimirCompraEnTabla(const Compra *compra) {
    char panMascara[25];
    strncpy(panMascara, compra->PAN, sizeof(panMascara));
    panMascara[sizeof(panMascara)-1] = '\0';
    enmascararPAN(panMascara);

    printf("%-10.2lf %-22s %-18s %-6s %-8s %-10u %-10s\n",
        compra->montoDolares,
        panMascara,
        compra->franquicia,
        compra->cvv,
        compra->fechaVencimiento,
        compra->referencia,
        compra->estado);
}

// Comparador para ordenar por referencia descendente (mayor primero)
static int cmp_referencia_desc(const void *a, const void *b) {
    const Compra *A = a;
    const Compra *B = b;
    if (A->referencia < B->referencia) return 1;
    if (A->referencia > B->referencia) return -1;
    return 0;
}

void reimprimir() {
    Compra compras[MAX_COMPRAS];
    FILE *archivo = fopen("compras.dat", "rb");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo de compras.\n");
        return;
    }

    int contador = 0;
    while (contador < MAX_COMPRAS && fread(&compras[contador], sizeof(Compra), 1, archivo) == 1) {
        contador++;
    }
    fclose(archivo);

    if (contador == 0) {
        printf("No hay compras registradas.\n");
        return;
    }

    // Ordenar por referencia descendente para mostrar primero la compra más reciente
    qsort(compras, contador, sizeof(Compra), cmp_referencia_desc);

    int pos = 0; // ahora 0 = compra más reciente
    char linea[16];

    do {
        clear_screen();             // uso portable
        imprimirTablaHeader();
        imprimirCompraEnTabla(&compras[pos]);

        printf("\n");
        if (pos > 0) printf("<- [a] ");
        else printf("        ");
        printf("Transaccion %d de %d", pos + 1, contador);
        if (pos < contador - 1) printf(" [d] ->");
        printf("\nPresiona 'a' (izquierda), 'd' (derecha), o 'q' para salir: ");

        if (fgets(linea, sizeof(linea), stdin) == NULL) break;
        if (linea[0] == '\n' || linea[0] == '\0') continue; // no aceptar ENTER vacío
        if(strlen(linea) > 2) continue; //no aceptar mas de 1 caracter

        char opcion = linea[0];

        if ((opcion == 'a' || opcion == 'A') && pos > 0) pos--;
        else if ((opcion == 'd' || opcion == 'D') && pos < contador - 1) pos++;
        else if (opcion == 'q' || opcion == 'Q') break;
        /* cualquier otro input se ignora y se vuelve a mostrar la vista */
    } while (1);
}