/**
 * Sistema de Optimización de Producción para Componentes Electrónicos
 * 
 * Este programa permite gestionar la planificación de producción
 * para una fábrica de componentes electrónicos, calculando tiempos,
 * recursos necesarios y verificando la factibilidad de producción.
 * 
 * Versión mejorada con impuestos, registros sanitarios y más funcionalidades.
 */

#include <stdio.h>
#include <string.h>

#define MAX_PRODUCTOS 5
#define MAX_NOMBRE 50
#define MAX_REGISTRO 20
#define TASA_IMPUESTO_BASE 16.0  // Porcentaje de IVA base

// Prototipos de funciones
void ingresarProductos(char nombres[][MAX_NOMBRE], float tiempos[], float recursos[], 
                     int cantidades[], float impuestos[], char registros[][MAX_REGISTRO],
                     int prioridades[], int* numProductos);
void calcularRequerimientos(float tiempos[], float recursos[], int cantidades[], 
                         float impuestos[], int numProductos, float* tiempoTotal, 
                         float* recursosTotal, float* impuestoTotal);
int verificarFactibilidad(float tiempoTotal, float recursosTotal, 
                       float tiempoDisponible, float recursosDisponibles);
void buscarProducto(char nombres[][MAX_NOMBRE], int numProductos, 
                  char buscar[], int* indice);
void editarProducto(char nombres[][MAX_NOMBRE], float tiempos[], 
                  float recursos[], int cantidades[], float impuestos[],
                  char registros[][MAX_REGISTRO], int prioridades[], int numProductos);
void eliminarProducto(char nombres[][MAX_NOMBRE], float tiempos[], 
                    float recursos[], int cantidades[], float impuestos[],
                    char registros[][MAX_REGISTRO], int prioridades[], 
                    int* numProductos, int indice);
void mostrarProductos(char nombres[][MAX_NOMBRE], float tiempos[], 
                    float recursos[], int cantidades[], float impuestos[],
                    char registros[][MAX_REGISTRO], int prioridades[], int numProductos);
void limpiarBuffer();
void optimizarProduccion(char nombres[][MAX_NOMBRE], float tiempos[], 
                       float recursos[], int cantidades[], int prioridades[], 
                       int numProductos, float tiempoDisponible, float recursosDisponibles);
void calcularCostoTotal(float recursos[], int cantidades[], float impuestos[], 
                      int numProductos, float* costoSinImpuesto, float* impuestoTotal, 
                      float* costoTotal);
void verificarRegistrosSanitarios(char registros[][MAX_REGISTRO], int numProductos);
int validarRegistroSanitario(char registro[]);

int main() {
    // Declaración de variables y arreglos
    char nombres[MAX_PRODUCTOS][MAX_NOMBRE];
    char registros[MAX_PRODUCTOS][MAX_REGISTRO];
    float tiempos[MAX_PRODUCTOS];
    float recursos[MAX_PRODUCTOS];
    float impuestos[MAX_PRODUCTOS];
    int cantidades[MAX_PRODUCTOS];
    int prioridades[MAX_PRODUCTOS];
    int numProductos = 0;
    int opcion;
    float tiempoTotal = 0, recursosTotal = 0, impuestoTotal = 0;
    float tiempoDisponible, recursosDisponibles;
    char nombreBuscar[MAX_NOMBRE];
    int indiceBusqueda;
    float costoSinImpuesto = 0, costoTotal = 0;
    
    printf("=== SISTEMA DE OPTIMIZACIÓN DE PRODUCCIÓN ===\n\n");
    
    // Bucle principal del programa
    do {
        printf("\nMENU PRINCIPAL:\n");
        printf("1. Ingresar productos\n");
        printf("2. Calcular requerimientos totales\n");
        printf("3. Verificar factibilidad de producción\n");
        printf("4. Editar un producto\n");
        printf("5. Eliminar un producto\n");
        printf("6. Mostrar todos los productos\n");
        printf("7. Optimizar plan de producción\n");
        printf("8. Calcular costo total con impuestos\n");
        printf("9. Verificar validez de registros sanitarios\n");
        printf("10. Salir\n");
        printf("Seleccione una opción: ");
        
        // Validar entrada del usuario
        if (scanf("%d", &opcion) != 1) {
            printf("Entrada inválida. Ingrese un número.\n");
            limpiarBuffer();
            continue;
        }
        limpiarBuffer();
        
        // Procesar opción seleccionada
        switch (opcion) {
            case 1:
                ingresarProductos(nombres, tiempos, recursos, cantidades, 
                                impuestos, registros, prioridades, &numProductos);
                break;
                
            case 2:
                if (numProductos == 0) {
                    printf("Error: No hay productos registrados.\n");
                } else {
                    calcularRequerimientos(tiempos, recursos, cantidades, impuestos,
                                          numProductos, &tiempoTotal, &recursosTotal, &impuestoTotal);
                    printf("\nRequerimientos totales calculados:\n");
                    printf("- Tiempo total necesario: %.2f horas\n", tiempoTotal);
                    printf("- Recursos totales necesarios: %.2f unidades\n", recursosTotal);
                    printf("- Impuesto total a pagar: %.2f\n", impuestoTotal);
                }
                break;
                
            case 3:
                if (numProductos == 0) {
                    printf("Error: No hay productos registrados.\n");
                } else {
                    // Calcular primero los requerimientos si no se ha hecho
                    if (tiempoTotal == 0 && recursosTotal == 0) {
                        calcularRequerimientos(tiempos, recursos, cantidades, impuestos,
                                             numProductos, &tiempoTotal, &recursosTotal, &impuestoTotal);
                    }
                    
                    printf("\nIngrese el tiempo disponible (horas): ");
                    scanf("%f", &tiempoDisponible);
                    limpiarBuffer();
                    
                    printf("Ingrese los recursos disponibles (unidades): ");
                    scanf("%f", &recursosDisponibles);
                    limpiarBuffer();
                    
                    if (verificarFactibilidad(tiempoTotal, recursosTotal, 
                                           tiempoDisponible, recursosDisponibles)) {
                        printf("\n¡FACTIBLE! Es posible cumplir con la demanda con los recursos disponibles.\n");
                    } else {
                        printf("\n¡NO FACTIBLE! No es posible cumplir con la demanda con los recursos disponibles.\n");
                        
                        // Mostrar faltantes sin usar operador ternario
                        float tiempoFaltante = 0;
                        float recursosFaltantes = 0;
                        
                        if (tiempoTotal > tiempoDisponible) {
                            tiempoFaltante = tiempoTotal - tiempoDisponible;
                        }
                        
                        if (recursosTotal > recursosDisponibles) {
                            recursosFaltantes = recursosTotal - recursosDisponibles;
                        }
                        
                        printf("Faltaría: %.2f horas y/o %.2f unidades de recursos.\n", 
                              tiempoFaltante, recursosFaltantes);
                    }
                }
                break;
                
            case 4:
                if (numProductos == 0) {
                    printf("Error: No hay productos registrados.\n");
                } else {
                    editarProducto(nombres, tiempos, recursos, cantidades, 
                                 impuestos, registros, prioridades, numProductos);
                    // Resetear cálculos previos cuando se edita un producto
                    tiempoTotal = 0;
                    recursosTotal = 0;
                    impuestoTotal = 0;
                }
                break;
                
            case 5:
                if (numProductos == 0) {
                    printf("Error: No hay productos registrados.\n");
                } else {
                    printf("\nIngrese el nombre del producto a eliminar: ");
                    fgets(nombreBuscar, MAX_NOMBRE, stdin);
                    nombreBuscar[strcspn(nombreBuscar, "\n")] = '\0'; // Eliminar salto de línea
                    
                    indiceBusqueda = -1;
                    buscarProducto(nombres, numProductos, nombreBuscar, &indiceBusqueda);
                    
                    if (indiceBusqueda != -1) {
                        eliminarProducto(nombres, tiempos, recursos, cantidades, 
                                       impuestos, registros, prioridades, 
                                       &numProductos, indiceBusqueda);
                        printf("Producto '%s' eliminado correctamente.\n", nombreBuscar);
                        // Resetear cálculos previos cuando se elimina un producto
                        tiempoTotal = 0;
                        recursosTotal = 0;
                        impuestoTotal = 0;
                    } else {
                        printf("Error: Producto no encontrado.\n");
                    }
                }
                break;
                
            case 6:
                if (numProductos == 0) {
                    printf("No hay productos registrados.\n");
                } else {
                    mostrarProductos(nombres, tiempos, recursos, cantidades, 
                                   impuestos, registros, prioridades, numProductos);
                }
                break;
                
            case 7:
                if (numProductos == 0) {
                    printf("Error: No hay productos registrados.\n");
                } else {
                    printf("\nIngrese el tiempo disponible (horas): ");
                    scanf("%f", &tiempoDisponible);
                    limpiarBuffer();
                    
                    printf("Ingrese los recursos disponibles (unidades): ");
                    scanf("%f", &recursosDisponibles);
                    limpiarBuffer();
                    
                    optimizarProduccion(nombres, tiempos, recursos, cantidades, 
                                      prioridades, numProductos, 
                                      tiempoDisponible, recursosDisponibles);
                }
                break;
                
            case 8:
                if (numProductos == 0) {
                    printf("Error: No hay productos registrados.\n");
                } else {
                    calcularCostoTotal(recursos, cantidades, impuestos, numProductos, 
                                     &costoSinImpuesto, &impuestoTotal, &costoTotal);
                    
                    printf("\n--- CÁLCULO DE COSTOS ---\n");
                    printf("Costo total sin impuestos: $%.2f\n", costoSinImpuesto);
                    printf("Impuesto total a pagar: $%.2f\n", impuestoTotal);
                    printf("Costo total con impuestos: $%.2f\n", costoTotal);
                }
                break;
                
            case 9:
                if (numProductos == 0) {
                    printf("Error: No hay productos registrados.\n");
                } else {
                    verificarRegistrosSanitarios(registros, numProductos);
                }
                break;
                
            case 10:
                printf("\nSaliendo del sistema. ¡Hasta pronto!\n");
                break;
                
            default:
                printf("Opción inválida. Intente nuevamente.\n");
        }
        
        if (opcion != 10) {
            printf("\nPresione Enter para continuar...");
            getchar();
        }
        
    } while (opcion != 10);
    
    return 0;
}

/**
 * Ingresa información de productos al sistema
 */
void ingresarProductos(char nombres[][MAX_NOMBRE], float tiempos[], float recursos[], 
                     int cantidades[], float impuestos[], char registros[][MAX_REGISTRO],
                     int prioridades[], int* numProductos) {
    int i, j, encontrado;
    char buffer[MAX_NOMBRE];
    
    if (*numProductos >= MAX_PRODUCTOS) {
        printf("Error: Ya se alcanzó el límite máximo de productos (%d).\n", MAX_PRODUCTOS);
        return;
    }
    
    printf("\n--- REGISTRO DE PRODUCTOS ---\n");
    printf("Ingrese la información del producto #%d:\n", *numProductos + 1);
    
    // Ingresar nombre del producto
    do {
        printf("Nombre del producto: ");
        fgets(buffer, MAX_NOMBRE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Eliminar salto de línea
        
        if (strlen(buffer) == 0) {
            printf("Error: El nombre no puede estar vacío.\n");
            continue;
        }
        
        // Verificar si el nombre ya existe
        encontrado = 0;
        for (j = 0; j < *numProductos; j++) {
            if (strcmp(nombres[j], buffer) == 0) {
                encontrado = 1;
                break;
            }
        }
        
        if (encontrado) {
            printf("Error: Ya existe un producto con ese nombre.\n");
        } else {
            strcpy(nombres[*numProductos], buffer);
            break;
        }
    } while (1);
    
    // Ingresar tiempo de fabricación
    do {
        printf("Tiempo de fabricación por unidad (horas): ");
        if (scanf("%f", &tiempos[*numProductos]) != 1 || tiempos[*numProductos] <= 0) {
            printf("Error: Ingrese un valor numérico positivo.\n");
            limpiarBuffer();
            continue;
        }
        limpiarBuffer();
        break;
    } while (1);
    
    // Ingresar recursos necesarios
    do {
        printf("Recursos necesarios por unidad: ");
        if (scanf("%f", &recursos[*numProductos]) != 1 || recursos[*numProductos] <= 0) {
            printf("Error: Ingrese un valor numérico positivo.\n");
            limpiarBuffer();
            continue;
        }
        limpiarBuffer();
        break;
    } while (1);
    
    // Ingresar cantidad demandada
    do {
        printf("Cantidad demandada: ");
        if (scanf("%d", &cantidades[*numProductos]) != 1 || cantidades[*numProductos] <= 0) {
            printf("Error: Ingrese un valor entero positivo.\n");
            limpiarBuffer();
            continue;
        }
        limpiarBuffer();
        break;
    } while (1);
    
    // Ingresar porcentaje de impuesto (nuevo)
    do {
        printf("Porcentaje de impuesto (%%): ");
        if (scanf("%f", &impuestos[*numProductos]) != 1 || impuestos[*numProductos] < 0) {
            printf("Error: Ingrese un valor numérico no negativo.\n");
            limpiarBuffer();
            continue;
        }
        limpiarBuffer();
        break;
    } while (1);
    
    // Ingresar registro sanitario (nuevo)
    do {
        printf("Registro sanitario (formato RS-XXXXX): ");
        fgets(registros[*numProductos], MAX_REGISTRO, stdin);
        registros[*numProductos][strcspn(registros[*numProductos], "\n")] = '\0'; // Eliminar salto de línea
        
        if (!validarRegistroSanitario(registros[*numProductos])) {
            printf("Error: Formato inválido. Debe ser RS-XXXXX donde X son dígitos.\n");
            continue;
        }
        break;
    } while (1);
    
    // Ingresar prioridad de producción (nuevo)
    do {
        printf("Prioridad de producción (1-5, donde 1 es máxima prioridad): ");
        if (scanf("%d", &prioridades[*numProductos]) != 1 || 
            prioridades[*numProductos] < 1 || prioridades[*numProductos] > 5) {
            printf("Error: Ingrese un valor entre 1 y 5.\n");
            limpiarBuffer();
            continue;
        }
        limpiarBuffer();
        break;
    } while (1);
    
    (*numProductos)++;
    printf("\nProducto registrado correctamente.\n");
}

/**
 * Calcula los requerimientos totales de tiempo, recursos e impuestos
 */
void calcularRequerimientos(float tiempos[], float recursos[], int cantidades[], 
                         float impuestos[], int numProductos, float* tiempoTotal, 
                         float* recursosTotal, float* impuestoTotal) {
    int i;
    *tiempoTotal = 0;
    *recursosTotal = 0;
    *impuestoTotal = 0;
    
    for (i = 0; i < numProductos; i++) {
        *tiempoTotal += tiempos[i] * cantidades[i];
        *recursosTotal += recursos[i] * cantidades[i];
        *impuestoTotal += (recursos[i] * cantidades[i]) * (impuestos[i] / 100.0);
    }
}

/**
 * Verifica si es posible cumplir con la producción dados los límites
 */
int verificarFactibilidad(float tiempoTotal, float recursosTotal, 
                       float tiempoDisponible, float recursosDisponibles) {
    if (tiempoTotal <= tiempoDisponible && recursosTotal <= recursosDisponibles) {
        return 1; // Factible
    } else {
        return 0; // No factible
    }
}

/**
 * Busca un producto por nombre y devuelve su índice
 */
void buscarProducto(char nombres[][MAX_NOMBRE], int numProductos, 
                  char buscar[], int* indice) {
    int i;
    *indice = -1;
    
    for (i = 0; i < numProductos; i++) {
        if (strcmp(nombres[i], buscar) == 0) {
            *indice = i;
            return;
        }
    }
}

/**
 * Permite editar la información de un producto existente
 */
void editarProducto(char nombres[][MAX_NOMBRE], float tiempos[], 
                  float recursos[], int cantidades[], float impuestos[],
                  char registros[][MAX_REGISTRO], int prioridades[], int numProductos) {
    char nombreBuscar[MAX_NOMBRE];
    int indice = -1;
    int opcionEditar;
    
    printf("\n--- EDICIÓN DE PRODUCTO ---\n");
    printf("Ingrese el nombre del producto a editar: ");
    fgets(nombreBuscar, MAX_NOMBRE, stdin);
    nombreBuscar[strcspn(nombreBuscar, "\n")] = '\0'; // Eliminar salto de línea
    
    buscarProducto(nombres, numProductos, nombreBuscar, &indice);
    
    if (indice == -1) {
        printf("Error: Producto no encontrado.\n");
        return;
    }
    
    printf("\nProducto encontrado: %s\n", nombres[indice]);
    printf("1. Editar tiempo de fabricación (actual: %.2f)\n", tiempos[indice]);
    printf("2. Editar recursos necesarios (actual: %.2f)\n", recursos[indice]);
    printf("3. Editar cantidad demandada (actual: %d)\n", cantidades[indice]);
    printf("4. Editar porcentaje de impuesto (actual: %.2f%%)\n", impuestos[indice]);
    printf("5. Editar registro sanitario (actual: %s)\n", registros[indice]);
    printf("6. Editar prioridad (actual: %d)\n", prioridades[indice]);
    printf("Seleccione dato a editar: ");
    
    if (scanf("%d", &opcionEditar) != 1) {
        printf("Entrada inválida. Operación cancelada.\n");
        limpiarBuffer();
        return;
    }
    limpiarBuffer();
    
    switch (opcionEditar) {
        case 1:
            do {
                printf("Nuevo tiempo de fabricación: ");
                if (scanf("%f", &tiempos[indice]) != 1 || tiempos[indice] <= 0) {
                    printf("Error: Ingrese un valor numérico positivo.\n");
                    limpiarBuffer();
                    continue;
                }
                limpiarBuffer();
                break;
            } while (1);
            break;
            
        case 2:
            do {
                printf("Nuevos recursos necesarios: ");
                if (scanf("%f", &recursos[indice]) != 1 || recursos[indice] <= 0) {
                    printf("Error: Ingrese un valor numérico positivo.\n");
                    limpiarBuffer();
                    continue;
                }
                limpiarBuffer();
                break;
            } while (1);
            break;
            
        case 3:
            do {
                printf("Nueva cantidad demandada: ");
                if (scanf("%d", &cantidades[indice]) != 1 || cantidades[indice] <= 0) {
                    printf("Error: Ingrese un valor entero positivo.\n");
                    limpiarBuffer();
                    continue;
                }
                limpiarBuffer();
                break;
            } while (1);
            break;
            
        case 4:
            do {
                printf("Nuevo porcentaje de impuesto: ");
                if (scanf("%f", &impuestos[indice]) != 1 || impuestos[indice] < 0) {
                    printf("Error: Ingrese un valor numérico no negativo.\n");
                    limpiarBuffer();
                    continue;
                }
                limpiarBuffer();
                break;
            } while (1);
            break;
            
        case 5:
            do {
                printf("Nuevo registro sanitario (formato RS-XXXXX): ");
                fgets(registros[indice], MAX_REGISTRO, stdin);
                registros[indice][strcspn(registros[indice], "\n")] = '\0'; // Eliminar salto de línea
                
                if (!validarRegistroSanitario(registros[indice])) {
                    printf("Error: Formato inválido. Debe ser RS-XXXXX donde X son dígitos.\n");
                    continue;
                }
                break;
            } while (1);
            break;
            
        case 6:
            do {
                printf("Nueva prioridad (1-5): ");
                if (scanf("%d", &prioridades[indice]) != 1 || 
                    prioridades[indice] < 1 || prioridades[indice] > 5) {
                    printf("Error: Ingrese un valor entre 1 y 5.\n");
                    limpiarBuffer();
                    continue;
                }
                limpiarBuffer();
                break;
            } while (1);
            break;
            
        default:
            printf("Opción inválida. No se realizaron cambios.\n");
            return;
    }
    
    printf("Producto actualizado correctamente.\n");
}

/**
 * Elimina un producto y reorganiza los arreglos
 */
void eliminarProducto(char nombres[][MAX_NOMBRE], float tiempos[], 
                    float recursos[], int cantidades[], float impuestos[],
                    char registros[][MAX_REGISTRO], int prioridades[], 
                    int* numProductos, int indice) {
    int i;
    
    if (indice < 0 || indice >= *numProductos) {
        return; // Índice fuera de rango
    }
    
    // Mover todos los elementos una posición hacia atrás desde el índice a eliminar
    for (i = indice; i < *numProductos - 1; i++) {
        strcpy(nombres[i], nombres[i + 1]);
        strcpy(registros[i], registros[i + 1]);
        tiempos[i] = tiempos[i + 1];
        recursos[i] = recursos[i + 1];
        cantidades[i] = cantidades[i + 1];
        impuestos[i] = impuestos[i + 1];
        prioridades[i] = prioridades[i + 1];
    }
    
    (*numProductos)--; // Reducir el contador de productos
}

/**
 * Muestra todos los productos registrados
 */
void mostrarProductos(char nombres[][MAX_NOMBRE], float tiempos[], 
                    float recursos[], int cantidades[], float impuestos[],
                    char registros[][MAX_REGISTRO], int prioridades[], int numProductos) {
    int i;
    
    printf("\n--- LISTADO DE PRODUCTOS ---\n");
    printf("%-15s %-10s %-10s %-10s %-10s %-15s %-10s\n", 
          "Nombre", "Tiempo(h)", "Recursos", "Demanda", "Impuesto", "Reg. Sanitario", "Prioridad");
    printf("---------------------------------------------------------------------------------\n");
    
    for (i = 0; i < numProductos; i++) {
        printf("%-15s %-10.2f %-10.2f %-10d %-10.2f %-15s %-10d\n", 
              nombres[i], tiempos[i], recursos[i], cantidades[i], 
              impuestos[i], registros[i], prioridades[i]);
    }
}

/**
 * Limpia el buffer de entrada
 */
void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * Optimiza el plan de producción según prioridades y recursos disponibles
 */
void optimizarProduccion(char nombres[][MAX_NOMBRE], float tiempos[], 
                       float recursos[], int cantidades[], int prioridades[], 
                       int numProductos, float tiempoDisponible, float recursosDisponibles) {
    int i, j;
    int indicesMayor[MAX_PRODUCTOS];
    int cantidadesOptimizadas[MAX_PRODUCTOS];
    float tiempoUsado = 0;
    float recursosUsados = 0;
    
    // Inicializar arreglos
    for (i = 0; i < numProductos; i++) {
        indicesMayor[i] = i;
        cantidadesOptimizadas[i] = 0;
    }
    
    // Ordenar productos por prioridad (1 es máxima prioridad)
    for (i = 0; i < numProductos - 1; i++) {
        for (j = 0; j < numProductos - i - 1; j++) {
            if (prioridades[indicesMayor[j]] > prioridades[indicesMayor[j + 1]]) {
                int temp = indicesMayor[j];
                indicesMayor[j] = indicesMayor[j + 1];
                indicesMayor[j + 1] = temp;
            }
        }
    }
    
    printf("\n--- PLAN DE PRODUCCIÓN OPTIMIZADO ---\n");
    printf("Optimizando según prioridades y recursos disponibles...\n\n");
    
    // Asignar recursos según prioridad
    for (i = 0; i < numProductos; i++) {
        int idx = indicesMayor[i];
        int maxUnidades = cantidades[idx]; // Demanda máxima
        int unidadesPosibles = 0;
        
        // Calcular cuántas unidades podemos producir con el tiempo disponible
        if (tiempos[idx] > 0) {
            int porTiempo = (int)((tiempoDisponible - tiempoUsado) / tiempos[idx]);
            if (porTiempo < maxUnidades) {
                maxUnidades = porTiempo;
            }
        }
        
        // Calcular cuántas unidades podemos producir con los recursos disponibles
        if (recursos[idx] > 0) {
            int porRecursos = (int)((recursosDisponibles - recursosUsados) / recursos[idx]);
            if (porRecursos < maxUnidades) {
                maxUnidades = porRecursos;
            }
        }
        
        // Si no podemos producir ninguna unidad, continuamos con el siguiente producto
        if (maxUnidades <= 0) {
            continue;
        }
        
        // Asignar unidades
        cantidadesOptimizadas[idx] = maxUnidades;
        tiempoUsado += tiempos[idx] * maxUnidades;
        recursosUsados += recursos[idx] * maxUnidades;
    }
    
    // Mostrar plan optimizado
    printf("%-15s %-15s %-15s %-15s %-15s\n", 
          "Producto", "Prioridad", "Demanda", "Optimizado", "% Satisfecho");
    printf("-----------------------------------------------------------------------\n");
    
    for (i = 0; i < numProductos; i++) {
        float porcentaje = 0;
        if (cantidades[i] > 0) {
            porcentaje = ((float)cantidadesOptimizadas[i] / cantidades[i]) * 100;
        }
        
        printf("%-15s %-15d %-15d %-15d %-15.2f%%\n", 
              nombres[i], prioridades[i], cantidades[i], 
              cantidadesOptimizadas[i], porcentaje);
    }
    
    printf("\nResumen de recursos:\n");
    printf("- Tiempo disponible: %.2f horas, Utilizado: %.2f horas (%.2f%%)\n", 
          tiempoDisponible, tiempoUsado, (tiempoUsado / tiempoDisponible) * 100);
    printf("- Recursos disponibles: %.2f unidades, Utilizados: %.2f unidades (%.2f%%)\n", 
          recursosDisponibles, recursosUsados, (recursosUsados / recursosDisponibles) * 100);
}

/**
 * Calcula el costo total incluyendo impuestos
 */
void calcularCostoTotal(float recursos[], int cantidades[], float impuestos[], 
                      int numProductos, float* costoSinImpuesto, float* impuestoTotal, 
                      float* costoTotal) {
    int i;
    *costoSinImpuesto = 0;
    *impuestoTotal = 0;
    
    for (i = 0; i < numProductos; i++) {
        float costoProducto = recursos[i] * cantidades[i];
        float impuestoProducto = costoProducto * (impuestos[i] / 100.0);
        
        *costoSinImpuesto += costoProducto;
        *impuestoTotal += impuestoProducto;
    }
    
    *costoTotal = *costoSinImpuesto + *impuestoTotal;
}

/**
 * Verifica la validez de los registros sanitarios
 */
void verificarRegistrosSanitarios(char registros[][MAX_REGISTRO], int numProductos) {
    int i;
    int validos = 0;
    int invalidos = 0;
    
    printf("\n--- VERIFICACIÓN DE REGISTROS SANITARIOS ---\n");
    
    for (i = 0; i < numProductos; i++) {
        if (validarRegistroSanitario(registros[i])) {
            printf("Producto %d: Registro %s - VÁLIDO\n", i + 1, registros[i]);
            validos++;
        } else {
            printf("Producto %d: Registro %s - INVÁLIDO (No cumple formato RS-XXXXX)\n", 
                  i + 1, registros[i]);
            invalidos++;
        }
    }
    
    printf("\nResumen: %d registros válidos, %d registros inválidos\n", validos, invalidos);
}

/**
 * Valida el formato de un registro sanitario
 */
int validarRegistroSanitario(char registro[]) {
    int i;
    int longitud = strlen(registro);
    
    // Verificar longitud mínima (RS- + al menos 1 dígito)
    if (longitud < 4) {
        return 0;
    }
    
    // Verificar prefijo RS-
    if (registro[0] != 'R' || registro[1] != 'S' || registro[2] != '-') {
        return 0;
    }
    
    // Verificar que el resto sean dígitos
    for (i = 3; i < longitud; i++) {
        if (registro[i] < '0' || registro[i] > '9') {
            return 0;
        }
    }
    
    return 1;
}
