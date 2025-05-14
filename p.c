/**
 * Sistema de Optimización de Producción para Componentes Electrónicos
 * 
 * Este programa permite gestionar la planificación de producción
 * para una fábrica de componentes electrónicos, calculando tiempos,
 * recursos necesarios y verificando la factibilidad de producción.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PRODUCTOS 5
#define MAX_NOMBRE 50

// Prototipos de funciones
void ingresarProductos(char nombres[][MAX_NOMBRE], float* tiempos, float* recursos, 
                     int* cantidades, int* numProductos);
void calcularRequerimientos(float* tiempos, float* recursos, int* cantidades, 
                         int numProductos, float* tiempoTotal, float* recursosTotal);
int verificarFactibilidad(float tiempoTotal, float recursosTotal, 
                       float tiempoDisponible, float recursosDisponibles);
void buscarProducto(char nombres[][MAX_NOMBRE], int numProductos, 
                  char* buscar, int* indice);
void editarProducto(char nombres[][MAX_NOMBRE], float* tiempos, 
                  float* recursos, int* cantidades, int numProductos);
void eliminarProducto(char nombres[][MAX_NOMBRE], float* tiempos, 
                    float* recursos, int* cantidades, int* numProductos, int indice);
void mostrarProductos(char nombres[][MAX_NOMBRE], float* tiempos, 
                    float* recursos, int* cantidades, int numProductos);
void limpiarBuffer();

int main() {
    // Declaración de variables y arreglos
    char nombres[MAX_PRODUCTOS][MAX_NOMBRE];
    float tiempos[MAX_PRODUCTOS];
    float recursos[MAX_PRODUCTOS];
    int cantidades[MAX_PRODUCTOS];
    int numProductos = 0;
    int opcion;
    float tiempoTotal = 0, recursosTotal = 0;
    float tiempoDisponible, recursosDisponibles;
    char nombreBuscar[MAX_NOMBRE];
    int indiceBusqueda;
    
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
        printf("7. Salir\n");
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
                ingresarProductos(nombres, tiempos, recursos, cantidades, &numProductos);
                break;
                
            case 2:
                if (numProductos == 0) {
                    printf("Error: No hay productos registrados.\n");
                } else {
                    calcularRequerimientos(tiempos, recursos, cantidades, 
                                          numProductos, &tiempoTotal, &recursosTotal);
                    printf("\nRequerimientos totales calculados:\n");
                    printf("- Tiempo total necesario: %.2f horas\n", tiempoTotal);
                    printf("- Recursos totales necesarios: %.2f unidades\n", recursosTotal);
                }
                break;
                
            case 3:
                if (numProductos == 0) {
                    printf("Error: No hay productos registrados.\n");
                } else {
                    // Calcular primero los requerimientos si no se ha hecho
                    if (tiempoTotal == 0 && recursosTotal == 0) {
                        calcularRequerimientos(tiempos, recursos, cantidades, 
                                             numProductos, &tiempoTotal, &recursosTotal);
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
                        printf("Faltaría: %.2f horas y/o %.2f unidades de recursos.\n", 
                              (tiempoTotal > tiempoDisponible) ? tiempoTotal - tiempoDisponible : 0,
                              (recursosTotal > recursosDisponibles) ? recursosTotal - recursosDisponibles : 0);
                    }
                }
                break;
                
            case 4:
                if (numProductos == 0) {
                    printf("Error: No hay productos registrados.\n");
                } else {
                    editarProducto(nombres, tiempos, recursos, cantidades, numProductos);
                    // Resetear cálculos previos cuando se edita un producto
                    tiempoTotal = 0;
                    recursosTotal = 0;
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
                        eliminarProducto(nombres, tiempos, recursos, cantidades, &numProductos, indiceBusqueda);
                        printf("Producto '%s' eliminado correctamente.\n", nombreBuscar);
                        // Resetear cálculos previos cuando se elimina un producto
                        tiempoTotal = 0;
                        recursosTotal = 0;
                    } else {
                        printf("Error: Producto no encontrado.\n");
                    }
                }
                break;
                
            case 6:
                if (numProductos == 0) {
                    printf("No hay productos registrados.\n");
                } else {
                    mostrarProductos(nombres, tiempos, recursos, cantidades, numProductos);
                }
                break;
                
            case 7:
                printf("\nSaliendo del sistema. ¡Hasta pronto!\n");
                break;
                
            default:
                printf("Opción inválida. Intente nuevamente.\n");
        }
        
        if (opcion != 7) {
            printf("\nPresione Enter para continuar...");
            getchar();
        }
        
    } while (opcion != 7);
    
    return 0;
}

/**
 * Ingresa información de productos al sistema
 */
void ingresarProductos(char nombres[][MAX_NOMBRE], float* tiempos, float* recursos, 
                     int* cantidades, int* numProductos) {
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
    
    (*numProductos)++;
    printf("\nProducto registrado correctamente.\n");
}

/**
 * Calcula los requerimientos totales de tiempo y recursos
 */
void calcularRequerimientos(float* tiempos, float* recursos, int* cantidades, 
                         int numProductos, float* tiempoTotal, float* recursosTotal) {
    int i;
    *tiempoTotal = 0;
    *recursosTotal = 0;
    
    for (i = 0; i < numProductos; i++) {
        *tiempoTotal += tiempos[i] * cantidades[i];
        *recursosTotal += recursos[i] * cantidades[i];
    }
}

/**
 * Verifica si es posible cumplir con la producción dados los límites
 */
int verificarFactibilidad(float tiempoTotal, float recursosTotal, 
                       float tiempoDisponible, float recursosDisponibles) {
    return (tiempoTotal <= tiempoDisponible && recursosTotal <= recursosDisponibles);
}

/**
 * Busca un producto por nombre y devuelve su índice
 */
void buscarProducto(char nombres[][MAX_NOMBRE], int numProductos, 
                  char* buscar, int* indice) {
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
void editarProducto(char nombres[][MAX_NOMBRE], float* tiempos, 
                  float* recursos, int* cantidades, int numProductos) {
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
            
        default:
            printf("Opción inválida. No se realizaron cambios.\n");
            return;
    }
    
    printf("Producto actualizado correctamente.\n");
}

/**
 * Elimina un producto y reorganiza los arreglos
 */
void eliminarProducto(char nombres[][MAX_NOMBRE], float* tiempos, 
                    float* recursos, int* cantidades, int* numProductos, int indice) {
    int i;
    
    if (indice < 0 || indice >= *numProductos) {
        return; // Índice fuera de rango
    }
    
    // Mover todos los elementos una posición hacia atrás desde el índice a eliminar
    for (i = indice; i < *numProductos - 1; i++) {
        strcpy(nombres[i], nombres[i + 1]);
        tiempos[i] = tiempos[i + 1];
        recursos[i] = recursos[i + 1];
        cantidades[i] = cantidades[i + 1];
    }
    
    (*numProductos)--; // Reducir el contador de productos
}

/**
 * Muestra todos los productos registrados
 */
void mostrarProductos(char nombres[][MAX_NOMBRE], float* tiempos, 
                    float* recursos, int* cantidades, int numProductos) {
    int i;
    
    printf("\n--- LISTADO DE PRODUCTOS ---\n");
    printf("%-20s %-15s %-15s %-15s\n", "Nombre", "Tiempo (h)", "Recursos", "Demanda");
    printf("----------------------------------------------------------------------\n");
    
    for (i = 0; i < numProductos; i++) {
        printf("%-20s %-15.2f %-15.2f %-15d\n", 
              nombres[i], tiempos[i], recursos[i], cantidades[i]);
    }
}

/**
 * Limpia el buffer de entrada
 */
void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}