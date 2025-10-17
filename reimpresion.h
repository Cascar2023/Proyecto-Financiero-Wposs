#ifndef REIMPRESION_H
#define REIMPRESION_H

#include "compra.h"
#include <stddef.h>

/* Interfaz de reimpresión — no definir variables globales aquí. */

/* Muestra la vista interactiva de reimpresión */
void reimprimir(void);

/* Enmascara el PAN contenido en el buffer pasado (modifica el string) */
void enmascararPAN(char *pan);

/* Imprime el encabezado de la tabla de reimpresión */
void imprimirTablaHeader(void);

/* Imprime una compra formateada en la tabla */
void imprimirCompraEnTabla(const Compra *compra);

#endif /* REIMPRESION_H */
