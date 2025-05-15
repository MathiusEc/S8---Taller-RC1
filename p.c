/**
 * Sistema de Producción para Componentes Electrónicos
 * 
 * Este programa permite gestionar la planificación de producción
 * para una fábrica de componentes electrónicos, calculando tiempos,
 * recursos necesarios y verificando la factibilidad de producción.
 * 
 * Versión mejorada con factura detallada y cálculo de impuestos dinámico.
 */

#include <stdio.h>
#include <string.h>

#define MAX_PRODUCTOS 5
#define MAX_NOMBRE 50
#define MAX_REGISTRO 10
#define TASA_IMPUESTO_BASE 16.0  // Porcentaje de IVA base

// Prototipos de funciones
void ingresarProductos(char nombres[][MAX_NOMBRE], float tiempos[], float recursos[], 
                     float costos[], char registros[][MAX_REGISTRO], int* numProductos);
void calcularRequerimientos(float tiempo, float recurso, 
                         int cantidad, float costo, float impuesto,
                         float* tiempoTotal, float* recursosTotal, float* costoTotal, 
                         float* impuestoTotal);
void verificarFactibilidad(char nombres[][MAX_NOMBRE], float tiempos[], float recursos[], 
                        float costos[], char registros[][MAX_REGISTRO], int indice, int cantidad, 
                        float tiempoDisponible, float recursosDisponibles, float impuesto);
void buscarProducto(char nombres[][MAX_NOMBRE], int numProductos, 
                  char buscar[], int* indice);
void editarProducto(char nombres[][MAX_NOMBRE], float tiempos[], 
                  float recursos[], float costos[], char registros[][MAX_REGISTRO], 
                  int numProductos);
void eliminarProducto(char nombres[][MAX_NOMBRE], float tiempos[], 
                    float recursos[], float costos[], char registros[][MAX_REGISTRO], 
                    int* numProductos, int indice);
void mostrarProductos(char nombres[][MAX_NOMBRE], float tiempos[], 
                    float recursos[], float costos[], char registros[][MAX_REGISTRO], 
                    int numProductos);
void imprimirLinea(int longitud, char caracter);

int main() {
    // Declaración de variables y arreglos
    char nombres[MAX_PRODUCTOS][MAX_NOMBRE];
    char registros[MAX_PRODUCTOS][MAX_REGISTRO];
    float tiempos[MAX_PRODUCTOS];
    float recursos[MAX_PRODUCTOS];
    float costos[MAX_PRODUCTOS];
    int numProductos = 0;
    int opcion;
    float tiempoDisponible, recursosDisponibles;
    char nombreBuscar[MAX_NOMBRE];
    int indiceBusqueda;
    int cantidad;
    
    printf("=== SISTEMA DE PRODUCCIÓN DE COMPONENTES ELECTRÓNICOS ===\n\n");
    
    // Bucle principal del programa
    do {
        printf("\nMENU PRINCIPAL:\n");
        printf("1. Ingresar productos\n");
        printf("2. Verificar factibilidad de producción\n");
        printf("3. Editar un producto\n");
        printf("4. Eliminar un producto\n");
        printf("5. Mostrar todos los productos\n");
        printf("6. Salir\n");
        printf("Seleccione una opción: ");
        
        // Validar entrada del usuario
        if (scanf("%d", &opcion) != 1) {
            printf("Entrada inválida. Ingrese un número.\n");
            fflush(stdin);
            continue;
        }
        fflush(stdin);
        
        // Procesar opción seleccionada
        switch (opcion) {
            case 1:
                ingresarProductos(nombres, tiempos, recursos, costos, 
                                registros, &numProductos);
                break;
                
            case 2:
                if (numProductos == 0) {
                    printf("Error: No hay productos registrados.\n");
                } else {
                    printf("\nIngrese el nombre del producto a verificar: ");
                    fgets(nombreBuscar, MAX_NOMBRE, stdin);
                    nombreBuscar[strcspn(nombreBuscar, "\n")] = '\0'; // Eliminar salto de línea
                    
                    indiceBusqueda = -1;
                    buscarProducto(nombres, numProductos, nombreBuscar, &indiceBusqueda);
                    
                    if (indiceBusqueda == -1) {
                        printf("Error: Producto no encontrado.\n");
                        break;
                    }
                    
                    printf("\nIngrese la cantidad a producir: ");
                    scanf("%d", &cantidad);
                    fflush(stdin);
                    
                    if (cantidad <= 0) {
                        printf("Error: La cantidad debe ser mayor que cero.\n");
                        break;
                    }
                    
                    printf("\nIngrese el tiempo disponible (horas): ");
                    scanf("%f", &tiempoDisponible);
                    fflush(stdin);
                    
                    printf("Ingrese los recursos disponibles (unidades): ");
                    scanf("%f", &recursosDisponibles);
                    fflush(stdin);
                    
                    // Definir el impuesto según la cantidad
                    float impuesto;
                    if (cantidad < 10) {
                        impuesto = 16.0; // IVA estándar para cantidades pequeñas
                    } else if (cantidad < 50) {
                        impuesto = 12.0; // Impuesto reducido para cantidades medianas
                    } else {
                        impuesto = 8.0;  // Impuesto mínimo para grandes cantidades
                    }
                    
                    printf("\nImpuesto aplicable: %.1f%% (basado en la cantidad)\n", impuesto);
                    
                    verificarFactibilidad(nombres, tiempos, recursos, costos, 
                                       registros, indiceBusqueda, cantidad, tiempoDisponible, 
                                       recursosDisponibles, impuesto);
                }
                break;
                
            case 3:
                if (numProductos == 0) {
                    printf("Error: No hay productos registrados.\n");
                } else {
                    editarProducto(nombres, tiempos, recursos, costos, 
                                 registros, numProductos);
                }
                break;
                
            case 4:
                if (numProductos == 0) {
                    printf("Error: No hay productos registrados.\n");
                } else {
                    printf("\nIngrese el nombre del producto a eliminar: ");
                    fgets(nombreBuscar, MAX_NOMBRE, stdin);
                    nombreBuscar[strcspn(nombreBuscar, "\n")] = '\0'; // Eliminar salto de línea
                    
                    indiceBusqueda = -1;
                    buscarProducto(nombres, numProductos, nombreBuscar, &indiceBusqueda);
                    
                    if (indiceBusqueda != -1) {
                        eliminarProducto(nombres, tiempos, recursos, costos, 
                                       registros, &numProductos, indiceBusqueda);
                        printf("Producto '%s' eliminado correctamente.\n", nombreBuscar);
                    } else {
                        printf("Error: Producto no encontrado.\n");
                    }
                }
                break;
                
            case 5:
                if (numProductos == 0) {
                    printf("No hay productos registrados.\n");
                } else {
                    mostrarProductos(nombres, tiempos, recursos, costos, 
                                   registros, numProductos);
                }
                break;
                
            case 6:
                printf("\nSaliendo del sistema. ¡Hasta pronto!\n");
                break;
                
            default:
                printf("Opción inválida. Intente nuevamente.\n");
        }
        
        if (opcion != 6) {
            printf("\nPresione Enter para continuar...");
            getchar();
        }
        
    } while (opcion != 6);
    
    return 0;
}

/**
 * Imprime una línea de caracteres
 */
void imprimirLinea(int longitud, char caracter) {
    int i;
    for (i = 0; i < longitud; i++) {
        printf("%c", caracter);
    }
    printf("\n");
}

/**
 * Ingresa información de productos al sistema
 */
void ingresarProductos(char nombres[][MAX_NOMBRE], float tiempos[], float recursos[], 
                     float costos[], char registros[][MAX_REGISTRO], int* numProductos) {
    int j, encontrado;
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
            fflush(stdin);
            continue;
        }
        fflush(stdin);
        break;
    } while (1);
    
    // Ingresar recursos necesarios
    do {
        printf("Recursos necesarios por unidad: ");
        if (scanf("%f", &recursos[*numProductos]) != 1 || recursos[*numProductos] <= 0) {
            printf("Error: Ingrese un valor numérico positivo.\n");
            fflush(stdin);
            continue;
        }
        fflush(stdin);
        break;
    } while (1);
    
    // Ingresar costo por unidad
    do {
        printf("Costo por unidad ($): ");
        if (scanf("%f", &costos[*numProductos]) != 1 || costos[*numProductos] <= 0) {
            printf("Error: Ingrese un valor numérico positivo.\n");
            fflush(stdin);
            continue;
        }
        fflush(stdin);
        break;
    } while (1);
    
    // Ingresar registro sanitario simplificado
    printf("Registro sanitario: ");
    fgets(registros[*numProductos], MAX_REGISTRO, stdin);
    registros[*numProductos][strcspn(registros[*numProductos], "\n")] = '\0'; // Eliminar salto de línea
    
    (*numProductos)++;
    printf("\nProducto registrado correctamente.\n");
}

/**
 * Calcula los requerimientos totales de tiempo, recursos, costos e impuestos
 */
void calcularRequerimientos(float tiempo, float recurso, 
                         int cantidad, float costo, float impuesto,
                         float* tiempoTotal, float* recursosTotal, float* costoTotal, 
                         float* impuestoTotal) {
    *tiempoTotal = tiempo * cantidad;
    *recursosTotal = recurso * cantidad;
    
    float costoProd = costo * cantidad;
    *impuestoTotal = costoProd * (impuesto / 100.0);
    
    *costoTotal = costoProd + *impuestoTotal;
}

/**
 * Verifica si es posible cumplir con la producción dados los límites
 * y genera una factura con los costos e impuestos
 */
void verificarFactibilidad(char nombres[][MAX_NOMBRE], float tiempos[], float recursos[], 
                        float costos[], char registros[][MAX_REGISTRO], int indice, int cantidad, 
                        float tiempoDisponible, float recursosDisponibles, float impuesto) {
    float tiempoTotal = 0, recursosTotal = 0, costoTotal = 0, impuestoTotal = 0;
    
    // Calcular requerimientos
    calcularRequerimientos(tiempos[indice], recursos[indice], cantidad, costos[indice], 
                         impuesto, &tiempoTotal, &recursosTotal, &costoTotal, &impuestoTotal);
    
    int factible = 1;
    float tiempoFaltante = 0;
    float recursosFaltantes = 0;
    
    if (tiempoTotal > tiempoDisponible) {
        tiempoFaltante = tiempoTotal - tiempoDisponible;
        factible = 0;
    }
    
    if (recursosTotal > recursosDisponibles) {
        recursosFaltantes = recursosTotal - recursosDisponibles;
        factible = 0;
    }
    
    // Imprimir factura bonita
    int anchoFactura = 60;
    
    printf("\n");
    imprimirLinea(anchoFactura, '*');
    printf("*%*s%*s*\n", anchoFactura/2 + 10, "FACTURA PROYECTADA", anchoFactura/2 - 10, "");
    imprimirLinea(anchoFactura, '*');
    printf("* %-20s %-35s *\n", "Producto:", nombres[indice]);
    printf("* %-20s %-35s *\n", "Registro Sanitario:", registros[indice]);
    imprimirLinea(anchoFactura, '-');
    printf("* %-20s %-35d *\n", "Cantidad:", cantidad);
    printf("* %-20s $%-34.2f *\n", "Precio Unitario:", costos[indice]);
    imprimirLinea(anchoFactura, '-');
    printf("* %-20s %-35.2f *\n", "Tiempo por Unidad:", tiempos[indice]);
    printf("* %-20s %-35.2f *\n", "Tiempo Total:", tiempoTotal);
    printf("* %-20s %-35.2f *\n", "Recursos por Unidad:", recursos[indice]);
    printf("* %-20s %-35.2f *\n", "Recursos Totales:", recursosTotal);
    imprimirLinea(anchoFactura, '-');
    printf("* %-20s $%-34.2f *\n", "Subtotal:", costoTotal - impuestoTotal);
    printf("* %-20s %%%-34.1f *\n", "Tasa de Impuesto:", impuesto);
    printf("* %-20s $%-34.2f *\n", "Impuesto:", impuestoTotal);
    imprimirLinea(anchoFactura, '=');
    printf("* %-20s $%-34.2f *\n", "TOTAL:", costoTotal);
    imprimirLinea(anchoFactura, '*');
    
    if (factible) {
        printf("\n");
        imprimirLinea(anchoFactura, '#');
        printf("#%*s%*s#\n", anchoFactura/2 + 10, "¡PRODUCCIÓN FACTIBLE!", anchoFactura/2 - 10, "");
        printf("#%*s%*s#\n", anchoFactura/2 + 15, "Es posible cumplir con la producción", anchoFactura/2 - 15, "");
        printf("#%*s%*s#\n", anchoFactura/2 + 13, "con los recursos disponibles", anchoFactura/2 - 13, "");
        imprimirLinea(anchoFactura, '#');
    } else {
        printf("\n");
        imprimirLinea(anchoFactura, '!');
        printf("!%*s%*s!\n", anchoFactura/2 + 11, "¡PRODUCCIÓN NO FACTIBLE!", anchoFactura/2 - 11, "");
        imprimirLinea(anchoFactura, '!');
        
        if (tiempoFaltante > 0) {
            printf("! %-56s !\n", "TIEMPO INSUFICIENTE:");
            printf("! %-20s %-35.2f !\n", "- Tiempo Disponible:", tiempoDisponible);
            printf("! %-20s %-35.2f !\n", "- Tiempo Necesario:", tiempoTotal);
            printf("! %-20s %-35.2f !\n", "- Tiempo Faltante:", tiempoFaltante);
            imprimirLinea(anchoFactura, '-');
        }
        
        if (recursosFaltantes > 0) {
            printf("! %-56s !\n", "RECURSOS INSUFICIENTES:");
            printf("! %-20s %-35.2f !\n", "- Recursos Disponibles:", recursosDisponibles);
            printf("! %-20s %-35.2f !\n", "- Recursos Necesarios:", recursosTotal);
            printf("! %-20s %-35.2f !\n", "- Recursos Faltantes:", recursosFaltantes);
            imprimirLinea(anchoFactura, '-');
        }
        
        // Calcular posibles impuestos adicionales por producción urgente
        float impuestoUrgencia = (costoTotal - impuestoTotal) * 0.05; // 5% adicional por urgencia
        float totalUrgente = costoTotal + impuestoUrgencia;
        
        printf("! %-56s !\n", "COSTOS ADICIONALES POR PRODUCCIÓN URGENTE:");
        printf("! %-20s $%-34.2f !\n", "- Impuesto Urgencia:", impuestoUrgencia);
        printf("! %-20s $%-34.2f !\n", "- TOTAL con Urgencia:", totalUrgente);
        imprimirLinea(anchoFactura, '!');
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
                  float recursos[], float costos[], char registros[][MAX_REGISTRO], 
                  int numProductos) {
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
    printf("3. Editar costo por unidad (actual: $%.2f)\n", costos[indice]);
    printf("4. Editar registro sanitario (actual: %s)\n", registros[indice]);
    printf("Seleccione dato a editar: ");
    
    if (scanf("%d", &opcionEditar) != 1) {
        printf("Entrada inválida. Operación cancelada.\n");
        fflush(stdin);
        return;
    }
    fflush(stdin);
    
    switch (opcionEditar) {
        case 1:
            do {
                printf("Nuevo tiempo de fabricación: ");
                if (scanf("%f", &tiempos[indice]) != 1 || tiempos[indice] <= 0) {
                    printf("Error: Ingrese un valor numérico positivo.\n");
                    fflush(stdin);
                    continue;
                }
                fflush(stdin);
                break;
            } while (1);
            break;
            
        case 2:
            do {
                printf("Nuevos recursos necesarios: ");
                if (scanf("%f", &recursos[indice]) != 1 || recursos[indice] <= 0) {
                    printf("Error: Ingrese un valor numérico positivo.\n");
                    fflush(stdin);
                    continue;
                }
                fflush(stdin);
                break;
            } while (1);
            break;
            
        case 3:
            do {
                printf("Nuevo costo por unidad: ");
                if (scanf("%f", &costos[indice]) != 1 || costos[indice] <= 0) {
                    printf("Error: Ingrese un valor numérico positivo.\n");
                    fflush(stdin);
                    continue;
                }
                fflush(stdin);
                break;
            } while (1);
            break;
            
        case 4:
            printf("Nuevo registro sanitario: ");
            fgets(registros[indice], MAX_REGISTRO, stdin);
            registros[indice][strcspn(registros[indice], "\n")] = '\0'; // Eliminar salto de línea
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
                    float recursos[], float costos[], char registros[][MAX_REGISTRO], 
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
        costos[i] = costos[i + 1];
    }
    
    (*numProductos)--; // Reducir el contador de productos
}

/**
 * Muestra todos los productos registrados
 */
void mostrarProductos(char nombres[][MAX_NOMBRE], float tiempos[], 
                    float recursos[], float costos[], char registros[][MAX_REGISTRO], 
                    int numProductos) {
    int i;
    
    printf("\n--- LISTADO DE PRODUCTOS ---\n");
    imprimirLinea(70, '-');
    printf("%-15s %-10s %-10s %-10s %-15s\n", 
          "Nombre", "Tiempo(h)", "Recursos", "Costo($)", "Reg. Sanitario");
    imprimirLinea(70, '-');
    
    for (i = 0; i < numProductos; i++) {
        printf("%-15s %-10.2f %-10.2f %-10.2f %-15s\n", 
              nombres[i], tiempos[i], recursos[i], costos[i], registros[i]);
    }
    
    imprimirLinea(70, '-');
}

