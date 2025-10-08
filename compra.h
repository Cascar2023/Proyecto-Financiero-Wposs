typedef struct {
    double montoDolares;
    char PAN[20];
    int cvv;
    char fechaVencimiento[6];
    unsigned int referencia;
} Compra;

void procesarCompra();
void guardarCompra(const Compra *compra);