#ifndef COMPRA_H
#define COMPRA_H

typedef struct {
    double montoDolares;
    char PAN[20];
    char franquicia[20];
    char cvv[5];
    char fechaVencimiento[6];
    unsigned int referencia;
    char estado[15];
} Compra;

void procesarCompra();
void guardarCompra(const Compra *compra);
int esPANValido(const char *pan);
void identificarFranquicia(const char *pan, char *franquicia);
int validarCVV(const char *cvv, const char *franquicia);

#endif // COMPRA_H