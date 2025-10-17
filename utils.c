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

void clear_screen(void) {
#ifdef _WIN32
    system("cls");
#else
    /* primero intenta clear (comando POSIX) */
    if (system("clear") != 0) {
        /* fallback usando secuencia ANSI (si el terminal la soporta) */
        printf("\x1b[2J\x1b[H");
        fflush(stdout);
    }
#endif
}