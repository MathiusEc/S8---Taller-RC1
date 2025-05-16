#define MAX_PRODUCTOS 5
#define MAX_COMPONENTES 10
#define TASA_IMPUESTO_BASE 16.0 // Bienes y servicios utilizados en la producción

int buscarComponente(char componentes[][30], int numComponentes, char *nombre);
int buscarProducto(char nombres[][30], int numProductos, char *nombre);

int mostrarMenu();

void ingresarComponente(char componentes[][30], float cantidades[], int *numComponentes);
void mostrarComponentes(char componentes[][30], float cantidades[], int numComponentes);

void obtenerNombreProducto(char nombre[], char nombres[][30], int numProductos);
float obtenerTiempoFabricacion();
float obtenerCostoPorUnidad();
void obtenerRegistroSanitario(char registro[]);
void mostrarComponentesDisponibles(char componentes[][30], float cantidades[], int numComponentes);
void obtenerRequerimientosComponentes(int requerimientosComp[][MAX_COMPONENTES], int numProducto,
                                      char componentes[][30], int numComponentes);