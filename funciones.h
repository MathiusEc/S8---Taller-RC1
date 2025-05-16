#define MAX_PRODUCTOS 5
#define MAX_COMPONENTES 10
#define TASA_IMPUESTO_BASE 16.0 // Bienes y servicios utilizados en la producción

int mostrarMenu();

void ingresarComponente(char componentes[][30], float cantidades[], int *numComponentes);
void mostrarComponentes(char componentes[][30], float cantidades[], int numComponentes);
void ingresarProducto(char nombres[][30], float tiempos[], char registros[][30], 
                      float costos[], int requerimientosComp[][MAX_COMPONENTES], 
                      int *numProductos, char componentes[][30], float cantidades[], int numComponentes);