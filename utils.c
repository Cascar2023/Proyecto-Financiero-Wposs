#include "utils.h"

void clear_input_buffer(void) {
    int c = getchar();
    if (c == '\n' || c == EOF) {
        return; // no había nada pendiente (o ya era solo un '\n')
    }
    // Si llegamos aquí, consumimos el resto hasta el '\n' o EOF
    while ((c = getchar()) != '\n' && c != EOF) {
        /* vacía */
    }
}