#ifndef COMPRA_H
#define COMPRA_H

typedef struct {
    double montoDolares;
    char PAN[21];
    char franquicia[20];
    char cvv[6];
    char fechaVencimiento[6];
    unsigned int referencia;
    char estado[15];
} Compra;

void procesarCompra();
void guardarCompra(const Compra *compra);
void limpiarEspacios(char *pan);
void limpiarCaracteresNoNumericos(char *pan);
int esPANValido(const char *pan);
void identificarFranquicia(const char *pan, char *franquicia);
int validarCVV(const char *cvv, const char *franquicia);

#endif // COMPRA_H