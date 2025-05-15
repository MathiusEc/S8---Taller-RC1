

int menu();

//opcion 1
int productoExiste(char nombres[][30], int numProductos, char *producto);
void leerNombreProducto(char nombres[][30], int numProductos, char *nombre);
float leerTiempoFabricacion();
float leerRecursosNecesarios();
float leerCostoUnidad();
void leerRegistroSanitario(char *registro);

//opcion 2
void calcularRequerimientos(float tiempo, float recurso, int cantidad, float costo, float impuesto,
                            float *tiempoTotal, float *recursosTotal, float *costoTotal, float *impuestoTotal);
void verificarFactibilidad(char nombres[][30], float tiempos[], float recursos[], float costos[], char registros[][30],
                           int indice, int cantidad, float tiempoDisponible, float recursosDisponibles, float impuesto);
  
//opcion 3