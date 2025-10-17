#include "compra.h"
#include "utils.h"
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

#define MAX_COMPRAS 20
#define referencia_inicial 1000
#define archivo_compras "compras.dat"

//***************************************************************************************************************************/
//***************************************************Procesar Compra*********************************************************/
//***************************************************************************************************************************/

// ...existing code...

void procesarCompra() {
    printf("-----------------------------------PROCESAR COMPRA-----------------------------------\n");
    printf("Presione 'e' para retornar al menu principal en cualquier momento.\n");
    Compra compra;
    FILE *archivo = fopen(archivo_compras, "rb");
    unsigned int referenciaActual = referencia_inicial - 1;

    if (archivo != NULL) {
        Compra temp;
        while (fread(&temp, sizeof(Compra), 1, archivo) == 1) {
            if (temp.referencia > referenciaActual) {
                referenciaActual = temp.referencia;
            }
        }
        fclose(archivo);
        compra.referencia = referenciaActual + 1;
    } else {
        compra.referencia = referencia_inicial;
    }

    // Monto en dólares
    do {
        printf("Ingrese el monto en dolares de su compra (max 10 digitos y 2 decimales): \n");
        char montoStr[20];
        int valido = 1;
        fgets(montoStr, sizeof(montoStr), stdin);
        montoStr[strcspn(montoStr, "\n")] = 0;
        if (strcmp(montoStr, "e") == 0 || strcmp(montoStr, "E") == 0) {
            printf("Retornando al menu principal.\n");
            return;
        }

        // Verifica que solo tenga números y máximo un punto
        int puntos = 0, digitosAntes = 0, digitosDespues = 0;
        for (int i = 0; montoStr[i]; i++) {
            if (montoStr[i] == '.') {
                puntos++;
                continue;
            }
            if (!isdigit((unsigned char)montoStr[i])) {
                valido = 0;
                break;
            }
            if (puntos == 0) digitosAntes++;
            else digitosDespues++;
        }
        if (!valido || puntos > 1) {
            printf("Monto invalido. Debe ingresar solo numeros y como maximo dos decimales.\n");
            continue;
        }
        if (digitosAntes > 10) {
            printf("El monto debe tener como maximo 10 digitos antes del punto decimal.\n");
            continue;
        }
        if (digitosDespues > 2) {
            printf("El monto debe tener como maximo 2 decimales.\n");
            continue;
        }
        compra.montoDolares = atof(montoStr);
        if (compra.montoDolares <= 0) {
            printf("El monto de la compra debe ser mayor a cero.\n");
            continue;
        }
        if (compra.montoDolares > 9999999999.99) {
            printf("El monto de la compra excede el limite permitido de $9,999,999,999.99.\n");
            continue;
        }
        break;
    } while (1);

    // PAN de la tarjeta
    do {
        printf("Ingrese el PAN de su tarjeta (solo numeros): \n");
        fgets(compra.PAN, sizeof(compra.PAN), stdin);
        compra.PAN[strcspn(compra.PAN, "\n")] = 0;
        if (strcmp(compra.PAN, "e") == 0 || strcmp(compra.PAN, "E") == 0) {
            printf("Retornando al menu principal.\n");
            return;
        }
        limpiarEspacios(compra.PAN);
        limpiarCaracteresNoNumericos(compra.PAN);

        if (!esPANValido(compra.PAN)) {
            printf("PAN invalido. Debe tener entre 13 y 19 digitos y pasar el algoritmo de Luhn.\n");
        } else {
            break;
        }
    } while (1);

    // Identificar franquicia
    do {
        identificarFranquicia(compra.PAN, compra.franquicia);
        if (strcmp(compra.franquicia, "Desconocida") == 0) {
            printf("Franquicia de tarjeta desconocida o no soportada.\n");
            // Volver a pedir el PAN
            do {
                printf("Ingrese nuevamente el PAN de su tarjeta (solo numeros): \n");
                fgets(compra.PAN, sizeof(compra.PAN), stdin);
                compra.PAN[strcspn(compra.PAN, "\n")] = 0;
                limpiarEspacios(compra.PAN);
                limpiarCaracteresNoNumericos(compra.PAN);
            } while (!esPANValido(compra.PAN));
        } else {
            break;
        }
    } while (1);

    // CVV de la tarjeta
    do {
        printf("Ingrese el CVV de su tarjeta: \n");
        fgets(compra.cvv, sizeof(compra.cvv), stdin);
        compra.cvv[strcspn(compra.cvv, "\n")] = 0;
        if (strcmp(compra.cvv, "e") == 0 || strcmp(compra.cvv, "E") == 0) {
            printf("Retornando al menu principal.\n");
            return;
        }
        if (!validarCVV(compra.cvv, compra.franquicia)) {
            printf("CVV invalido para la franquicia %s.\n", compra.franquicia);
        } else {
            break;
        }
        clear_input_buffer();
    } while (1);

    // Fecha de vencimiento
    do {
        printf("Ingrese la fecha de vencimiento de su tarjeta (MM/AA): \n");
        fgets(compra.fechaVencimiento, sizeof(compra.fechaVencimiento), stdin);
        compra.fechaVencimiento[strcspn(compra.fechaVencimiento, "\n")] = 0;
        if (strcmp(compra.fechaVencimiento, "e") == 0 || strcmp(compra.fechaVencimiento, "E") == 0) {
            printf("Retornando al menu principal.\n");
            return;
        }
        clear_input_buffer();

        if(compra.fechaVencimiento[0] < '0' || compra.fechaVencimiento[0] > '9' || 
            compra.fechaVencimiento[1] < '0' || compra.fechaVencimiento[1] > '9' || 
            compra.fechaVencimiento[2] != '/' || 
            compra.fechaVencimiento[3] < '2' || compra.fechaVencimiento[3] > '9' || 
            compra.fechaVencimiento[4] < '0' || compra.fechaVencimiento[4] > '9' ||
            compra.fechaVencimiento[5] != '\0') {
            printf("Fecha de vencimiento invalida. Formato correcto MM/AA.\n");
            continue;
        }

        int mesVenc = (compra.fechaVencimiento[0] - '0') * 10 + (compra.fechaVencimiento[1] - '0');
        int anioVenc = (compra.fechaVencimiento[3] - '0') * 10 + (compra.fechaVencimiento[4] - '0') + 2000;
        if (mesVenc < 1 || mesVenc > 12) {
            printf("Mes invalido.\n");
            continue;
        }

        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        if (anioVenc < (tm.tm_year + 1900) || (anioVenc == (tm.tm_year + 1900) && mesVenc < (tm.tm_mon + 1))) {
            printf("La tarjeta ha expirado.\n");
            continue;
        }
        break;
    } while (1);

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

// ...existing code...

// Función para guardar la compra en un archivo
void guardarCompra(const Compra *compra) {
    FILE *archivo = fopen(archivo_compras, "ab");
    if (archivo == NULL) {
        printf("Error al abrir el archivo para guardar la compra.\n");
        return;
    }
    fwrite(compra, sizeof(Compra), 1, archivo);
    fclose(archivo);
    printf("Compra guardada en el archivo compras.dat\n");
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
        size_t largoPAN = strlen(pan);
        while (*src) {
            if (isdigit((unsigned char)*src)) {
                *dst++ = *src;
            }
            src++;
        }
        *dst = '\0';
        if (largoPAN != strlen(pan)) {
            printf("Procurar ingresar solo numeros en el PAN, el PAN ingresado se le borrara los caracteres no numericos y se tomara asi.\n");
        }
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
    size_t len = strlen(pan);

    // Visa: empieza con 4 y tiene 13 a 16 dígitos
    if (len >= 1 && pan[0] == '4' && (len >= 13 && len <= 16)) {
        strcpy(franquicia, "Visa");
    }
    // Mastercard: empieza con 51–55 o 2221–2720 y tiene 16 dígitos
    else if (
        (len >= 2 && pan[0] == '5' && pan[1] >= '1' && pan[1] <= '5' && len == 16) ||
        (len >= 4 && len == 16 && ({ char buf[5]; memcpy(buf, pan, 4); buf[4] = '\0'; int v = atoi(buf); v >= 2221 && v <= 2720; }))
    ) {
        strcpy(franquicia, "Mastercard");
    }
    // American Express: empieza con 34 o 37 y tiene 15 dígitos
    else if (len >= 2 && ((strncmp(pan, "34", 2) == 0) || (strncmp(pan, "37", 2) == 0)) && len == 15) {
        strcpy(franquicia, "American Express");
    }
    // UnionPay: empieza con 62 y suele tener 16-19 dígitos
    else if (len >= 2 && strncmp(pan, "62", 2) == 0 && (len >= 16 && len <= 19)) {
        strcpy(franquicia, "UnionPay");
    }
    // Discover: empieza con 6011, 65 o 622126–622925 y tiene 16 dígitos
    else if (
        (len >= 4 && strncmp(pan, "6011", 4) == 0 && len == 16) ||
        (len >= 2 && strncmp(pan, "65", 2) == 0 && len == 16) ||
        (len >= 6 && len == 16 && ({ char buf[7]; memcpy(buf, pan, 6); buf[6] = '\0'; int v = atoi(buf); v >= 622126 && v <= 622925; }))
    ) {
        strcpy(franquicia, "Discover");
    }
    // JCB: 3528–3589, típicamente 16 dígitos
    else if (len >= 4 && len == 16 && ({ char buf[5]; memcpy(buf, pan, 4); buf[4] = '\0'; int v = atoi(buf); v >= 3528 && v <= 3589; })) {
        strcpy(franquicia, "JCB");
    }
    // Diners Club: 300–305, 36, 38 o 39 — tradicionalmente 14 dígitos
    else if (
        (len >= 3 && len == 14 && ({ char buf3[4]; memcpy(buf3, pan, 3); buf3[3]='\0'; int v3 = atoi(buf3); v3 >= 300 && v3 <= 305; })) ||
        (len >= 2 && len == 14 && (strncmp(pan, "36", 2) == 0 || strncmp(pan, "38", 2) == 0 || strncmp(pan, "39", 2) == 0))
    ) {
        strcpy(franquicia, "Diners Club");
    }
    // Maestro: puede empezar con 50, o 56-69, longitudes variadas (12-19)
    else if (
        len >= 2 && ((strncmp(pan, "50", 2) == 0) ||
                    (pan[0] == '5' && pan[1] >= '6' && pan[1] <= '9') ||
                    (pan[0] == '6' && (pan[1] >= '0' && pan[1] <= '9')))
        && (len >= 12 && len <= 19)
    ) {
        strcpy(franquicia, "Maestro");
    }
    // Si no coincide con ninguna
    else {
        strcpy(franquicia, "Desconocida");
    }
}

// Valida el CVV según la franquicia
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
        strcmp(franquicia, "Discover") == 0 ||
        strcmp(franquicia, "JCB") == 0 ||
        strcmp(franquicia, "Diners Club") == 0 ||
        strcmp(franquicia, "Maestro") == 0 ||
        strcmp(franquicia, "UnionPay") == 0) {
        // Estas franquicias usan típicamente 3 dígitos
        return (len == 3) ? 1 : 0;
    }
    else if (strcmp(franquicia, "American Express") == 0 ||
            strcmp(franquicia, "Amex") == 0) {
        // Amex usa 4 dígitos (CID)
        return (len == 4) ? 1 : 0;
    }
    else {
        // Franquicia desconocida: rechazar
        return 0;
    }
}

