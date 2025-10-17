//***************************************************************************************************************************/
//*****************************************************Anulacion*************************************************************/
//***************************************************************************************************************************/

#include "anulacion.h"
#include "compra.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define archivo_compras "compras.dat"

void anularCompra(unsigned int referencia) {
    printf("-----------------------------------ANULACION DE COMPRA-----------------------------------\n");
    printf("presione 'e' en cualquier momento para volver al menu principal\n");
    FILE *archivo = fopen(archivo_compras, "r+b");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de compras.\n");
        return;
    }

    Compra compra;
    int encontrada = 0;
    long posicion = 0;

    while (fread(&compra, sizeof(Compra), 1, archivo) == 1) {
        if (compra.referencia == referencia) {
            encontrada = 1;
            break;
        }
        posicion++;
    }

    if (!encontrada) {
        printf("No se encontro una compra con la referencia %u.\n", referencia);
        fclose(archivo);
        clear_input_buffer();
        return;
    }

    if (strcmp(compra.estado, "Anulada") == 0) {
        printf("La compra con referencia %u ya ha sido anulada previamente.\n", referencia);
        fclose(archivo);
        return;
    }

    // Limpiar buffer de entrada por si quedaron restos de lecturas previas
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);

    // Verificación de identidad con reintentos
    char panVerificacion[5];
    char cvvVerificacion[10];
    char entrada[32];

    // Pide 4 últimos dígitos del PAN
    while (1) {
        printf("PAN (4 ultimos) o 'e' para salir: ");
        if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
            fclose(archivo);
            return;
        }
        entrada[strcspn(entrada, "\r\n")] = '\0';
        if (strlen(entrada) == 1 && (entrada[0] == 'e' || entrada[0] == 'E')) {
            fclose(archivo);
            return;
        }
        if (strlen(entrada) != 4) {
            printf("Entrada invalida: debe ingresar exactamente 4 digitos.\n");
            continue;
        }
        int ok = 1;
        for (size_t i = 0; i < 4; i++) {
            if (!isdigit((unsigned char)entrada[i])) { ok = 0; break; }
        }
        if (!ok) {
            printf("Entrada invalida: solo se permiten digitos.\n");
            continue;
        }
        strncpy(panVerificacion, entrada, 5);
        panVerificacion[4] = '\0';
        break;
    }

    // Pide CVV (la longitud debe coincidir con la almacenada)
    size_t cvvEsperado = strlen(compra.cvv);
    while (1) {
        printf("CVV (o 'e' para salir): ");
        if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
            fclose(archivo);
            return;
        }
        entrada[strcspn(entrada, "\r\n")] = '\0';
        if (strlen(entrada) == 1 && (entrada[0] == 'e' || entrada[0] == 'E')) {
            fclose(archivo);
            return;
        }
        if (strlen(entrada) != cvvEsperado) {
            printf("CVV invalido: debe tener %zu digitos.\n", cvvEsperado);
            continue;
        }
        int ok = 1;
        for (size_t i = 0; i < strlen(entrada); i++) {
            if (!isdigit((unsigned char)entrada[i])) { ok = 0; break; }
        }
        if (!ok) {
            printf("CVV invalido: solo se permiten digitos.\n");
            continue;
        }
        strncpy(cvvVerificacion, entrada, sizeof(cvvVerificacion));
        cvvVerificacion[sizeof(cvvVerificacion)-1] = '\0';
        break;
    }

    int panLen = strlen(compra.PAN);
    if (panLen < 4 ||
        strncmp(compra.PAN + panLen - 4, panVerificacion, 4) != 0 ||
        strcmp(compra.cvv, cvvVerificacion) != 0) {
        printf("Los datos proporcionados no coinciden con los de la compra. Anulacion cancelada.\n");
        fclose(archivo);
        return;
    }

    strcpy(compra.estado, "Anulada");

    // Mover el puntero al registro correcto y sobrescribir
    fseek(archivo, (long)(posicion * sizeof(Compra)), SEEK_SET);
    fwrite(&compra, sizeof(Compra), 1, archivo);
    fflush(archivo);

    printf("Compra con referencia %u anulada exitosamente.\n", referencia);

    fclose(archivo);
}