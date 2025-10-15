#ifndef REIMPRESION_H
#define REIMPRESION_H
#include "compra.h"

char compras[20]; // Array para almacenar las compras

void reimprimir();
void enmascararPAN(char *linea);
void imprimirTablaHeader();
void imprimirCompraEnTabla(const Compra *compra);

#endif
