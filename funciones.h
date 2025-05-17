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

int verificarFactibilidad(char nombres[][30], float tiempos[], char registros[][30],
                          float costos[], int requerimientosComp[][MAX_COMPONENTES],
                          int indice, int cantidad, float tiempoDisponible,
                          char componentes[][30], float cantidades[], int numComponentes,
                          float *tiempoTotal, float *costoTotal, float *impuestoTotal, 
                          float *totalConImpuesto, float *tiempoFaltante,
                          int componentesFaltantes[], int *hayFaltantes);
void imprimirFactura(char nombres[][30], char registros[][30], int indice, int cantidad,
                     float tiempos[], float tiempoTotal, float costos[],
                     float costoTotal, float impuestoTotal, float totalConImpuesto,
                     char componentes[][30], float cantidades[], int requerimientosComp[][MAX_COMPONENTES],
                     int componentesFaltantes[], int numComponentes);

void ejecutarProduccion(char nombres[][30], float tiempos[], char registros[][30], 
                      float costos[], int requerimientosComp[][MAX_COMPONENTES], 
                      int indice, int cantidad, char componentes[][30], float cantidades[], 
                      int numComponentes);


void eliminarProducto(char nombres[][30], float tiempos[], char registros[][30],
                      float costos[], int requerimientosComp[][MAX_COMPONENTES],
                      int *numProductos);

void editarComponentesProducto(char nombres[][30], int requerimientosComp[][MAX_COMPONENTES], 
                             int numProductos, char componentes[][30], float cantidades[], 
                             int numComponentes);

void aumentarInventario(char componentes[][30], float cantidades[], int numComponentes);


            