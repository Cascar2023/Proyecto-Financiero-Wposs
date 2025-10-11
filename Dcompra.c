#include "compra.h"
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

#define MAX_COMPRAS 20
#define referencia_inicial 1000
#define archivo_compras "compras.txt"

//***************************************************************************************************************************/
//***************************************************Procesar Compra*********************************************************/
//***************************************************************************************************************************/

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

    // Solicitar y validar datos de la compra

    // Monto en dólares
    printf("ingrese el monto en dolares de su compra: \n");
    scanf("%lf", &compra.montoDolares);

    // Validar monto
    if (compra.montoDolares <= 0) {
        printf("El monto de la compra debe ser mayor a cero.\n");
        return;
    }

    // PAN de la tarjeta
    printf("ingrese el PAN de su tarjeta (Por favor, solo ingrese numeros): \n");
    getchar();  // Limpiar el buffer
    fgets(compra.PAN, sizeof(compra.PAN), stdin);
    compra.PAN[strcspn(compra.PAN, "\n")] = 0; // Eliminar salto de línea
    limpiarEspacios(compra.PAN);
    limpiarCaracteresNoNumericos(compra.PAN);

    // Validar PAN con Luhn y longitud
    if (!esPANValido(compra.PAN)) {
        printf("PAN invalido. Debe tener entre 13 y 19 digitos y pasar el algoritmo de Luhn.\n");
        return;
    }

    // Identificar franquicia segun PAN (Visa, Mastercard, Amex, Discover) otra sera rechazada
    identificarFranquicia(compra.PAN, compra.franquicia);
    if (strcmp(compra.franquicia, "Desconocida") == 0) {
        printf("Franquicia de tarjeta desconocida o no soportada.\n");
        return;
    }



    // CVV de la tarjeta
    printf("ingrese el CVV de su tarjeta: \n");
    fgets(compra.cvv, sizeof(compra.cvv), stdin);
    compra.cvv[strcspn(compra.cvv, "\n")] = 0;


    // Validar CVV segun franquicia
    if (!validarCVV(compra.cvv, compra.franquicia)) {
        printf("CVV invalido para la franquicia %s.\n", compra.franquicia);
        return;
    }


    // Fecha de vencimiento de la tarjeta
    printf("ingrese la fecha de vencimiento de su tarjeta (MM/AA): \n");
    fgets(compra.fechaVencimiento, sizeof(compra.fechaVencimiento), stdin);
    compra.fechaVencimiento[strcspn(compra.fechaVencimiento, "\n")] = 0; // Eliminar salto de línea

    // Validar formato de fecha MM/AA
    if(compra.fechaVencimiento[0] < '0' || compra.fechaVencimiento[0] > '9' || 
        compra.fechaVencimiento[1] < '0' || compra.fechaVencimiento[1] > '9' || 
        compra.fechaVencimiento[2] != '/' || 
        compra.fechaVencimiento[3] < '2' || compra.fechaVencimiento[3] > '9' || 
        compra.fechaVencimiento[4] < '0' || compra.fechaVencimiento[4] > '9' ||
        compra.fechaVencimiento[5] != '\0') {
        printf("Fecha de vencimiento invalida. Formato correcto MM/AA.\n");
        return;
    }

    //Verificar el mes 
    int mesVenc = (compra.fechaVencimiento[0] - '0') * 10 + (compra.fechaVencimiento[1] - '0');
    int anioVenc = (compra.fechaVencimiento[3] - '0') * 10 + (compra.fechaVencimiento[4] - '0') + 2000;
    if (mesVenc < 1 || mesVenc > 12) {
        printf("Mes invalido.\n");
        return;
    }

    // Verificar si la tarjeta ha expirado
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    if (anioVenc < (tm.tm_year + 1900) || (anioVenc == (tm.tm_year + 1900) && mesVenc < (tm.tm_mon + 1))) {
        printf("La tarjeta ha expirado.\n");
        return;
    }

    // Verificar limite de compras
    if (compra.referencia >= referencia_inicial + MAX_COMPRAS) {
        printf("Se ha alcanzado el maximo de compras permitidas.\n");
        return;
    }

    strcpy(compra.estado, "Vigente");

    // Guardar compra en archivo
    guardarCompra(&compra);
    printf("Compra procesada y guardada con exito.\n");
}

// Función para guardar la compra en un archivo
void guardarCompra(const Compra *compra) {
    FILE *archivo = fopen("compras.txt", "a+t");
    if (archivo == NULL) {
        printf("Error al abrir el archivo para guardar la compra.\n");
        return;
    }
    // Guardar en formato legible
    fprintf(archivo, "Monto: %.2f, PAN: %s, Franquicia: %s, CVV: %s, Vencimiento: %s, Referencia: %u, Estado: %s\n",
        compra->montoDolares, compra->PAN, compra->franquicia, compra->cvv, compra->fechaVencimiento, compra->referencia, compra->estado);
    fclose(archivo);
    printf("Compra guardada en el archivo compras.txt\n");
}

// Función para limpiar espacios en blanco de un PAN
    void limpiarEspacios(char *pan) {
        char *src = pan, *dst = pan;
        while (*src) {
            if (!isspace((unsigned char)*src)) {
                *dst++ = *src;
            }
            src++;
        }
        *dst = '\0';
    }

// Función para eliminar caracteres no numéricos de un PAN
    void limpiarCaracteresNoNumericos(char *pan) {
        char *src = pan, *dst = pan;
        while (*src) {
            if (isdigit((unsigned char)*src)) {
                *dst++ = *src;
            }
            src++;
        }
        *dst = '\0';
    }

// Valida el PAN usando el algoritmo de Luhn y verifica longitud y que no sean todos los digitos iguales entre si
int esPANValido(const char *pan) {
    int len = strlen(pan);
    int suma = 0, par = 0;

    // Verifica longitud
    if (len < 13 || len > 19) {
        return 0;
    }

    // Verifica que solo sean dígitos y no todos iguales
    char primer = pan[0];
    int todosIguales = 1;
    for (int i = 0; i < len; i++) {
        if (!isdigit((unsigned char)pan[i])) return 0;
        if (pan[i] != primer) todosIguales = 0;
    }

    // Rechaza PANs con todos los dígitos iguales (ej: 0000..., 1111..., etc.)
    if (todosIguales) return 0;

    // Algoritmo de Luhn
    for (int i = len - 1; i >= 0; i--) {
        int digito = pan[i] - '0';
        if (par) {
            digito *= 2;
            if (digito > 9) digito -= 9;
        }
        suma += digito;
        par = !par;
    }

    return (suma % 10 == 0);
}

// Identifica la franquicia de la tarjeta segun el PAN
void identificarFranquicia(const char *pan, char *franquicia) {
    // Visa: empieza con 4 y tiene 13 a 16 dígitos
    if (pan[0] == '4' && (strlen(pan) >= 13 && strlen(pan) <= 16)) {
        strcpy(franquicia, "Visa");
    }
    // Mastercard: empieza con 51–55 o 2221–2720 y tiene 16 dígitos
    else if (
        ((pan[0] == '5' && pan[1] >= '1' && pan[1] <= '5') && strlen(pan) == 16) ||
        ((strncmp(pan, "2221", 4) >= 0 && strncmp(pan, "2720", 4) <= 0) && strlen(pan) == 16)
    ) {
        strcpy(franquicia, "Mastercard");
    }
    // American Express: empieza con 34 o 37 y tiene 15 dígitos
    else if (((strncmp(pan, "34", 2) == 0) || (strncmp(pan, "37", 2) == 0)) && strlen(pan) == 15) {
        strcpy(franquicia, "American Express");
    }
    // Discover: empieza con 6011, 65 o 622126–622925 y tiene 16 dígitos
    else if (
        ((strncmp(pan, "6011", 4) == 0) ||
        (strncmp(pan, "65", 2) == 0) ||
        ((strncmp(pan, "622126", 6) >= 0 && strncmp(pan, "622925", 6) <= 0)))
        && strlen(pan) == 16
    ) {
        strcpy(franquicia, "Discover");
    }
    // Si no coincide con ninguna
    else {
        strcpy(franquicia, "Desconocida");
    }
}

// Valida el CVV segun la franquicia
int validarCVV(const char *cvv, const char *franquicia) {
    if (cvv == NULL || franquicia == NULL) return 0;

    size_t len = strlen(cvv);

    // Debe contener solo dígitos
    if (len == 0) return 0;
    for (size_t i = 0; i < len; ++i) {
        if (!isdigit((unsigned char)cvv[i])) return 0;
    }

    // Validación por franquicia
    if (strcmp(franquicia, "Visa") == 0 ||
        strcmp(franquicia, "Mastercard") == 0 ||
        strcmp(franquicia, "Discover") == 0) {
        // Estas franquicias usan 3 dígitos
        return (len == 3) ? 1 : 0;
    }
    else if (strcmp(franquicia, "American Express") == 0 ||
            strcmp(franquicia, "Amex") == 0) {
        // Amex usa 4 dígitos (CID)
        return (len == 4) ? 1 : 0;
    }
    else {
        return 0;
    }
}

