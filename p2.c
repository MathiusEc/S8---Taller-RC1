#include <stdio.h>
#include <string.h>

#define MAX_PRODUCTOS 5
#define MAX_COMPONENTES 10
#define TASA_IMPUESTO_BASE 15.0 // Bienes y servicios utilizados en la producción

// Prototipos de funciones
void mostrarMenu();
void limpiarBuffer();
void ingresarComponente(char componentes[][30], float cantidades[], int *numComponentes);
void mostrarComponentes(char componentes[][30], float cantidades[], int numComponentes);
void ingresarProducto(char nombres[][30], float tiempos[], char registros[][30], 
                     float costos[], int requerimientosComp[][MAX_COMPONENTES], 
                     int *numProductos, char componentes[][30], float cantidades[], int numComponentes);
void verificarFactibilidad(char nombres[][30], float tiempos[], char registros[][30], 
                         float costos[], int requerimientosComp[][MAX_COMPONENTES], 
                         int indice, int cantidad, float tiempoDisponible, 
                         char componentes[][30], float cantidades[], int numComponentes);
int buscarComponente(char componentes[][30], int numComponentes, char *nombre);
int buscarProducto(char nombres[][30], int numProductos, char *nombre);
void eliminarProducto(char nombres[][30], float tiempos[], char registros[][30], 
                    float costos[], int requerimientosComp[][MAX_COMPONENTES], 
                    int *numProductos);
void editarComponentesProducto(char nombres[][30], int requerimientosComp[][MAX_COMPONENTES], 
                             int numProductos, char componentes[][30], float cantidades[], 
                             int numComponentes);
void aumentarInventario(char componentes[][30], float cantidades[], int numComponentes);
void ejecutarProduccion(char nombres[][30], float tiempos[], char registros[][30], 
                      float costos[], int requerimientosComp[][MAX_COMPONENTES], 
                      int indice, int cantidad, char componentes[][30], float cantidades[], 
                      int numComponentes);

int main()
{
    int opcion = 0;
    int val;
    
    // Arreglos para componentes
    char componentes[MAX_COMPONENTES][30];
    float cantidades[MAX_COMPONENTES];
    int numComponentes = 0;
    
    // Arreglos para productos
    char nombres[MAX_PRODUCTOS][30];
    float tiempos[MAX_PRODUCTOS];
    char registros[MAX_PRODUCTOS][30];
    float costos[MAX_PRODUCTOS];
    int requerimientosComp[MAX_PRODUCTOS][MAX_COMPONENTES]; // Cantidad de cada componente que requiere el producto
    int numProductos = 0;

    do
    {
        mostrarMenu();
        val = scanf("%d", &opcion);
        limpiarBuffer();
        
        if (val != 1)
        {
            printf("Error: Entrada inválida. Intente nuevamente.\n");
            continue;
        }

        switch (opcion)
        {
        case 1: // Ingresar componente al inventario
            ingresarComponente(componentes, cantidades, &numComponentes);
            break;

        case 2: // Mostrar inventario de componentes
            mostrarComponentes(componentes, cantidades, numComponentes);
            break;

        case 3: // Ingresar producto
            if (numComponentes == 0)
            {
                printf("\nError: Primero debe ingresar componentes al inventario.\n");
                break;
            }
            
            ingresarProducto(nombres, tiempos, registros, costos, requerimientosComp, 
                           &numProductos, componentes, cantidades, numComponentes);
            break;

        case 4: // Verificar factibilidad de producción
            if (numProductos == 0)
            {
                printf("\nError: No hay productos registrados.\n");
                break;
            }
            
            if (numComponentes == 0)
            {
                printf("\nError: No hay componentes en el inventario.\n");
                break;
            }
            
            {
                char nombreBuscar[30];
                int indice;
                int cantidad;
                float tiempoDisponible;
                char respuesta;

                printf("\nIngrese el nombre del producto a verificar: ");
                fgets(nombreBuscar, 30, stdin);
                nombreBuscar[strcspn(nombreBuscar, "\n")] = '\0';

                indice = buscarProducto(nombres, numProductos, nombreBuscar);
                
                if (indice == -1)
                {
                    printf("Error: Producto no encontrado.\n");
                    break;
                }

                printf("Ingrese la cantidad a producir: ");
                val = scanf("%d", &cantidad);
                limpiarBuffer();
                
                if (val != 1)
                {
                    printf("Error: La cantidad debe ser un número.\n");
                    break;
                }
                
                if (cantidad <= 0)
                {
                    printf("Error: La cantidad debe ser mayor que cero.\n");
                    break;
                }

                printf("Ingrese el tiempo disponible (horas): ");
                val = scanf("%f", &tiempoDisponible);
                limpiarBuffer();
                
                if (val != 1)
                {
                    printf("Error: El tiempo debe ser un número.\n");
                    break;
                }
                
                if (tiempoDisponible <= 0)
                {
                    printf("Error: El tiempo disponible debe ser mayor que cero.\n");
                    break;
                }

                verificarFactibilidad(nombres, tiempos, registros, costos, requerimientosComp,
                                    indice, cantidad, tiempoDisponible, 
                                    componentes, cantidades, numComponentes);
                
                // Preguntar si desea ejecutar la producción
                printf("\n¿Desea ejecutar la producción? (S/N): ");
                scanf("%c", &respuesta);
                limpiarBuffer();
                
                if (respuesta == 'S' || respuesta == 's')
                {
                    // Verificar nuevamente la factibilidad antes de ejecutar
                    int factible = 1;
                    float tiempoTotal = tiempos[indice] * cantidad;
                    
                    if (tiempoTotal > tiempoDisponible)
                    {
                        factible = 0;
                    }
                    
                    for (int i = 0; i < numComponentes; i++)
                    {
                        int requerido = requerimientosComp[indice][i] * cantidad;
                        if (requerido > cantidades[i])
                        {
                            factible = 0;
                            break;
                        }
                    }
                    
                    if (factible == 1)
                    {
                        ejecutarProduccion(nombres, tiempos, registros, costos, requerimientosComp,
                                         indice, cantidad, componentes, cantidades, numComponentes);
                    }
                    else
                    {
                        printf("\nNo se puede ejecutar la producción debido a recursos insuficientes.\n");
                    }
                }
            }
            break;
            
        case 5: // Mostrar productos
            if (numProductos == 0)
            {
                printf("\nNo hay productos registrados.\n");
                break;
            }
            
            printf("\n--- LISTADO DE PRODUCTOS ---\n");
            printf("%-20s %-15s %-15s %-15s\n", "Nombre", "Tiempo(h)", "Costo($)", "Reg. Sanitario");
            printf("------------------------------------------------------------------\n");
            
            for (int i = 0; i < numProductos; i++)
            {
                printf("%-20s %-15.2f %-15.2f %-15s\n", 
                      nombres[i], tiempos[i], costos[i], registros[i]);
                
                printf("  Componentes requeridos:\n");
                for (int j = 0; j < numComponentes; j++)
                {
                    if (requerimientosComp[i][j] > 0)
                    {
                        printf("    - %-15s: %d unidades\n", componentes[j], requerimientosComp[i][j]);
                    }
                }
                printf("\n");
            }
            break;
            
        case 6: // Eliminar producto
            if (numProductos == 0)
            {
                printf("\nNo hay productos registrados para eliminar.\n");
                break;
            }
            
            eliminarProducto(nombres, tiempos, registros, costos, requerimientosComp, &numProductos);
            break;
            
        case 7: // Editar componentes de un producto
            if (numProductos == 0)
            {
                printf("\nNo hay productos registrados para editar.\n");
                break;
            }
            
            if (numComponentes == 0)
            {
                printf("\nNo hay componentes en el inventario.\n");
                break;
            }
            
            editarComponentesProducto(nombres, requerimientosComp, numProductos, 
                                    componentes, cantidades, numComponentes);
            break;
            
        case 8: // Aumentar inventario
            if (numComponentes == 0)
            {
                printf("\nNo hay componentes en el inventario para aumentar.\n");
                break;
            }
            
            aumentarInventario(componentes, cantidades, numComponentes);
            break;
            
        case 9: // Salir
            printf("\nSaliendo del sistema. ¡Hasta pronto!\n");
            break;
            
        default:
            printf("\nOpción inválida. Intente nuevamente.\n");
        }
        
        if (opcion != 9)
        {
            printf("\nPresione Enter para continuar...");
            getchar();
        }
        
    } while (opcion != 9);

    return 0;
}

void mostrarMenu()
{
    printf("\n==============================\n");
    printf("      MENU PRINCIPAL\n");
    printf("==============================\n");
    printf("1. Ingresar componente al inventario\n");
    printf("2. Mostrar inventario de componentes\n");
    printf("3. Ingresar producto\n");
    printf("4. Verificar factibilidad de producción\n");
    printf("5. Mostrar todos los productos\n");
    printf("6. Eliminar producto\n");
    printf("7. Editar componentes de un producto\n");
    printf("8. Aumentar inventario de componentes\n");
    printf("9. Salir\n");
    printf("==============================\n");
    printf("Seleccione una opción: ");
}

void limpiarBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void ingresarComponente(char componentes[][30], float cantidades[], int *numComponentes)
{
    if (*numComponentes >= MAX_COMPONENTES)
    {
        printf("\nError: Ya se alcanzó el límite máximo de componentes (%d).\n", MAX_COMPONENTES);
        return;
    }

    char nombre[30];
    int len;
    int existe;
    float cantidad;
    int val;

    printf("\n--- REGISTRO DE COMPONENTE ---\n");

    do
    {
        printf("Nombre del componente: ");
        fgets(nombre, 30, stdin);
        len = strlen(nombre);
        
        if (len > 0)
        {
            if (nombre[len - 1] == '\n')
            {
                nombre[len - 1] = '\0';
                len = len - 1;
            }
        }
        
        if (len == 0)
        {
            printf("Error: El nombre no puede estar vacío.\n");
            existe = 1;
        }
        else
        {
            existe = 0;
            for (int i = 0; i < *numComponentes; i++)
            {
                if (strcmp(componentes[i], nombre) == 0)
                {
                    printf("Error: Ya existe un componente con ese nombre.\n");
                    printf("Si desea actualizar la cantidad, use la opción 8 del menú.\n");
                    existe = 1;
                    break;
                }
            }
        }
    } while (existe == 1);

    do
    {
        printf("Cantidad disponible: ");
        val = scanf("%f", &cantidad);
        limpiarBuffer();
        
        if (val != 1)
        {
            printf("Error: Ingrese un valor numérico.\n");
            continue;
        }
        
        if (cantidad <= 0)
        {
            printf("Error: La cantidad debe ser mayor que cero.\n");
            continue;
        }
        
        break;
        
    } while (1);

    strcpy(componentes[*numComponentes], nombre);
    cantidades[*numComponentes] = cantidad;
    (*numComponentes)++;
    
    printf("\nComponente registrado correctamente.\n");
}

void mostrarComponentes(char componentes[][30], float cantidades[], int numComponentes)
{
    if (numComponentes == 0)
    {
        printf("\nNo hay componentes registrados en el inventario.\n");
        return;
    }
    
    printf("\n--- INVENTARIO DE COMPONENTES ---\n");
    printf("%-25s %-15s\n", "Componente", "Cantidad");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < numComponentes; i++)
    {
        printf("%-25s %-15.2f\n", componentes[i], cantidades[i]);
    }
}

int buscarComponente(char componentes[][30], int numComponentes, char *nombre)
{
    for (int i = 0; i < numComponentes; i++)
    {
        if (strcmp(componentes[i], nombre) == 0)
        {
            return i;
        }
    }
    return -1;
}

int buscarProducto(char nombres[][30], int numProductos, char *nombre)
{
    for (int i = 0; i < numProductos; i++)
    {
        if (strcmp(nombres[i], nombre) == 0)
        {
            return i;
        }
    }
    return -1;
}

void ingresarProducto(char nombres[][30], float tiempos[], char registros[][30], 
                     float costos[], int requerimientosComp[][MAX_COMPONENTES], 
                     int *numProductos, char componentes[][30], float cantidades[], int numComponentes)
{
    if (*numProductos >= MAX_PRODUCTOS)
    {
        printf("\nError: Ya se alcanzó el límite máximo de productos (%d).\n", MAX_PRODUCTOS);
        return;
    }

    char nombre[30];
    int len;
    int existe;
    float tiempo, costo;
    int val;
    int cantidadComp;

    printf("\n--- REGISTRO DE PRODUCTO ---\n");
    
    for (int j = 0; j < numComponentes; j++)
    {
        requerimientosComp[*numProductos][j] = 0;
    }
    
    // Leer nombre del producto
    do
    {
        printf("Nombre del producto: ");
        fgets(nombre, 30, stdin);
        len = strlen(nombre);
        
        if (len > 0)
        {
            if (nombre[len - 1] == '\n')
            {
                nombre[len - 1] = '\0';
                len = len--;
            }
        }
        
        if (len == 0)
        {
            printf("Error: El nombre no puede estar vacío.\n");
            existe = 1;
        }
        else
        {
            // Verificar si ya existe
            existe = 0;
            for (int i = 0; i < *numProductos; i++)
            {
                if (strcmp(nombres[i], nombre) == 0)
                {
                    printf("Error: Ya existe un producto con ese nombre.\n");
                    existe = 1;
                    break;
                }
            }
        }
    } while (existe == 1);
    
    // Leer tiempo de fabricación
    do
    {
        printf("Tiempo de fabricación (horas): ");
        val = scanf("%f", &tiempo);
        limpiarBuffer();
        
        if (val != 1)
        {
            printf("Error: Ingrese un valor numérico.\n");
            continue;
        }
        
        if (tiempo <= 0)
        {
            printf("Error: El tiempo debe ser mayor que cero.\n");
            continue;
        }
        
        break;
        
    } while (1);
    
    // Leer costo por unidad
    do
    {
        printf("Costo por unidad ($): ");
        val = scanf("%f", &costo);
        limpiarBuffer();
        
        if (val != 1)
        {
            printf("Error: Ingrese un valor numérico.\n");
            continue;
        }
        
        if (costo <= 0)
        {
            printf("Error: El costo debe ser mayor que cero.\n");
            continue;
        }
        
        break;
        
    } while (1);
    
    // Leer registro sanitario
    char registro[30];
    do
    {
        printf("Registro sanitario: ");
        fgets(registro, 30, stdin);
        len = strlen(registro);
        
        if (len > 0)
        {
            if (registro[len - 1] == '\n')
            {
                registro[len - 1] = '\0';
                len = len - 1;
            }
        }
        
        if (len == 0)
        {
            printf("Error: El registro sanitario no puede estar vacío.\n");
            continue;
        }
        
        break;
        
    } while (1);
    
    // Mostrar componentes disponibles
    printf("\nComponentes disponibles en inventario:\n");
    for (int i = 0; i < numComponentes; i++)
    {
        printf("%d. %s (%.2f unidades disponibles)\n", i + 1, componentes[i], cantidades[i]);
    }
    
    // Leer requerimientos de componentes
    printf("\nIngrese la cantidad requerida de cada componente para este producto:\n");
    for (int i = 0; i < numComponentes; i++)
    {
        do
        {
            printf("%s: ", componentes[i]);
            val = scanf("%d", &cantidadComp);
            limpiarBuffer();
            
            if (val != 1)
            {
                printf("Error: Ingrese un valor numérico.\n");
                continue;
            }
            
            if (cantidadComp < 0)
            {
                printf("Error: La cantidad no puede ser negativa.\n");
                continue;
            }
            
            requerimientosComp[*numProductos][i] = cantidadComp;
            break;
            
        } while (1);
    }
    
    // Guardar el producto
    strcpy(nombres[*numProductos], nombre);
    tiempos[*numProductos] = tiempo;
    costos[*numProductos] = costo;
    strcpy(registros[*numProductos], registro);
    
    (*numProductos) = (*numProductos) + 1;
    
    printf("\nProducto registrado correctamente.\n");
}

void verificarFactibilidad(char nombres[][30], float tiempos[], char registros[][30], 
                         float costos[], int requerimientosComp[][MAX_COMPONENTES], 
                         int indice, int cantidad, float tiempoDisponible, 
                         char componentes[][30], float cantidades[], int numComponentes)
{
    float tiempoTotal = tiempos[indice] * cantidad;
    float costoTotal = costos[indice] * cantidad;
    float impuestoTotal = costoTotal * (TASA_IMPUESTO_BASE / 100.0);
    float totalConImpuesto = costoTotal + impuestoTotal;
    
    int factible = 1;
    float tiempoFaltante = 0;
    
    // Verificar tiempo
    if (tiempoTotal > tiempoDisponible)
    {
        tiempoFaltante = tiempoTotal - tiempoDisponible;
        factible = 0;
    }
    
    // Verificar componentes
    int componentesFaltantes[MAX_COMPONENTES];
    int hayFaltantes = 0;
    
    for (int i = 0; i < numComponentes; i++)
    {
        componentesFaltantes[i] = 0;
        int requerido = requerimientosComp[indice][i] * cantidad;
        if (requerido > cantidades[i])
        {
            componentesFaltantes[i] = requerido - cantidades[i];
            hayFaltantes = 1;
            factible = 0;
        }
    }
    
    // Imprimir factura proyectada
    printf("\n=================================================\n");
    printf("            FACTURA PROYECTADA\n");
    printf("=================================================\n");
    printf("Producto:           %s\n", nombres[indice]);
    printf("Registro Sanitario: %s\n", registros[indice]);
    printf("Cantidad:           %d\n", cantidad);
    printf("-------------------------------------------------\n");
    printf("Tiempo por Unidad:  %.2f horas\n", tiempos[indice]);
    printf("Tiempo Total:       %.2f horas\n", tiempoTotal);
    printf("-------------------------------------------------\n");
    printf("Componentes requeridos:\n");
    
    for (int i = 0; i < numComponentes; i++)
    {
        if (requerimientosComp[indice][i] > 0)
        {
            int totalRequerido = requerimientosComp[indice][i] * cantidad;
            printf("  - %-15s: %d unidades", componentes[i], totalRequerido);
            
            if (totalRequerido > cantidades[i])
            {
                printf(" (INSUFICIENTE: faltan %d)\n", componentesFaltantes[i]);
            }
            else
            {
                printf("\n");
            }
        }
    }
    
    printf("-------------------------------------------------\n");
    printf("Precio Unitario:    $%.2f\n", costos[indice]);
    printf("Subtotal:           $%.2f\n", costoTotal);
    printf("Tasa de Impuesto:   %.1f%%\n", TASA_IMPUESTO_BASE);
    printf("Impuesto:           $%.2f\n", impuestoTotal);
    printf("=================================================\n");
    printf("TOTAL:              $%.2f\n", totalConImpuesto);
    printf("=================================================\n");
    
    // Evaluación de factibilidad
    if (factible == 1)
    {
        printf("\n¡PRODUCCIÓN FACTIBLE!\n");
        printf("Es posible cumplir con la producción con los recursos disponibles.\n");
    }
    else
    {
        printf("\n¡PRODUCCIÓN NO FACTIBLE!\n");
        
        if (tiempoFaltante > 0)
        {
            printf("\nTIEMPO INSUFICIENTE:\n");
            printf("- Tiempo Disponible:  %.2f horas\n", tiempoDisponible);
            printf("- Tiempo Necesario:   %.2f horas\n", tiempoTotal);
            printf("- Tiempo Faltante:    %.2f horas\n", tiempoFaltante);
        }
        
        if (hayFaltantes == 1)
        {
            printf("\nCOMPONENTES INSUFICIENTES:\n");
            for (int i = 0; i < numComponentes; i++)
            {
                if (componentesFaltantes[i] > 0)
                {
                    printf("- %s: faltan %d unidades\n", componentes[i], componentesFaltantes[i]);
                }
            }
        }
    }
}

// Función para ejecutar la producción y descontar componentes del inventario
void ejecutarProduccion(char nombres[][30], float tiempos[], char registros[][30], 
                      float costos[], int requerimientosComp[][MAX_COMPONENTES], 
                      int indice, int cantidad, char componentes[][30], float cantidades[], 
                      int numComponentes)
{
    // Descontar componentes del inventario
    for (int i = 0; i < numComponentes; i++)
    {
        int requerido = requerimientosComp[indice][i] * cantidad;
        cantidades[i] = cantidades[i] - requerido;
    }
    
    float costoTotal = costos[indice] * cantidad;
    float impuestoTotal = costoTotal * (TASA_IMPUESTO_BASE / 100.0);
    float totalConImpuesto = costoTotal + impuestoTotal;
    
    printf("\n=================================================\n");
    printf("         PRODUCCIÓN EJECUTADA CON ÉXITO\n");
    printf("=================================================\n");
    printf("Producto:           %s\n", nombres[indice]);
    printf("Cantidad producida: %d\n", cantidad);
    printf("Costo total:        $%.2f\n", totalConImpuesto);
    printf("=================================================\n");
    
    printf("\nComponentes utilizados y descontados del inventario:\n");
    for (int i = 0; i < numComponentes; i++)
    {
        if (requerimientosComp[indice][i] > 0)
        {
            int utilizado = requerimientosComp[indice][i] * cantidad;
            printf("- %s: %d unidades (Quedan: %.2f)\n", 
                  componentes[i], utilizado, cantidades[i]);
        }
    }
    
    printf("\nInventario actualizado correctamente.\n");
}

// Función para eliminar un producto
void eliminarProducto(char nombres[][30], float tiempos[], char registros[][30], 
                    float costos[], int requerimientosComp[][MAX_COMPONENTES], 
                    int *numProductos)
{
    char nombreEliminar[30];
    int indice;
    
    printf("\n--- ELIMINAR PRODUCTO ---\n");
    
    // Mostrar productos disponibles
    printf("Productos disponibles:\n");
    for (int i = 0; i < *numProductos; i++)
    {
        printf("%d. %s\n", i + 1, nombres[i]);
    }
    
    printf("\nIngrese el nombre del producto a eliminar: ");
    fgets(nombreEliminar, 30, stdin);
    nombreEliminar[strcspn(nombreEliminar, "\n")] = '\0';
    
    indice = buscarProducto(nombres, *numProductos, nombreEliminar);
    
    if (indice == -1)
    {
        printf("Error: Producto no encontrado.\n");
        return;
    }
    
    // Confirmar eliminación
    char confirmar;
    printf("¿Está seguro de eliminar el producto '%s'? (S/N): ", nombres[indice]);
    scanf("%c", &confirmar);
    limpiarBuffer();
    
    if (confirmar != 'S')
    {
        if (confirmar != 's')
        {
            printf("Operación cancelada.\n");
            return;
        }
    }
    
    // Eliminar producto moviendo los elementos posteriores una posición hacia atrás
    for (int i = indice; i < *numProductos - 1; i++)
    {
        strcpy(nombres[i], nombres[i + 1]);
        tiempos[i] = tiempos[i + 1];
        strcpy(registros[i], registros[i + 1]);
        costos[i] = costos[i + 1];
        
        for (int j = 0; j < MAX_COMPONENTES; j++)
        {
            requerimientosComp[i][j] = requerimientosComp[i + 1][j];
        }
    }
    
    (*numProductos) = (*numProductos) - 1;
    
    printf("\nProducto '%s' eliminado correctamente.\n", nombreEliminar);
}

// Función para editar los componentes de un producto
void editarComponentesProducto(char nombres[][30], int requerimientosComp[][MAX_COMPONENTES], 
                             int numProductos, char componentes[][30], float cantidades[], 
                             int numComponentes)
{
    char nombreProducto[30];
    int indiceProducto;
    int val;
    int cantidadComp;
    
    printf("\n--- EDITAR COMPONENTES DE PRODUCTO ---\n");
    
    // Mostrar productos disponibles
    printf("Productos disponibles:\n");
    for (int i = 0; i < numProductos; i++)
    {
        printf("%d. %s\n", i + 1, nombres[i]);
    }
    
    printf("\nIngrese el nombre del producto a editar: ");
    fgets(nombreProducto, 30, stdin);
    nombreProducto[strcspn(nombreProducto, "\n")] = '\0';
    
    indiceProducto = buscarProducto(nombres, numProductos, nombreProducto);
    
    if (indiceProducto == -1)
    {
        printf("Error: Producto no encontrado.\n");
        return;
    }
    
    printf("\nComponentes actuales del producto '%s':\n", nombres[indiceProducto]);
    for (int i = 0; i < numComponentes; i++)
    {
        printf("- %s: %d unidades\n", componentes[i], requerimientosComp[indiceProducto][i]);
    }
    
    printf("\nIngrese las nuevas cantidades para cada componente:\n");
    
    for (int i = 0; i < numComponentes; i++)
    {
        do
        {
            printf("%s (actual: %d): ", componentes[i], requerimientosComp[indiceProducto][i]);
            val = scanf("%d", &cantidadComp);
            limpiarBuffer();
            
            if (val != 1)
            {
                printf("Error: Ingrese un valor numérico.\n");
                continue;
            }
            
            if (cantidadComp < 0)
            {
                printf("Error: La cantidad no puede ser negativa.\n");
                continue;
            }
            
            requerimientosComp[indiceProducto][i] = cantidadComp;
            break;
            
        } while (1);
    }
    
    printf("\nComponentes del producto '%s' actualizados correctamente.\n", nombres[indiceProducto]);
}

// Función para aumentar el inventario de componentes
void aumentarInventario(char componentes[][30], float cantidades[], int numComponentes)
{
    int opcion;
    int val;
    float cantidad;
    
    printf("\n--- AUMENTAR INVENTARIO DE COMPONENTES ---\n");
    
    // Mostrar componentes disponibles
    printf("Componentes disponibles:\n");
    for (int i = 0; i < numComponentes; i++)
    {
        printf("%d. %s (%.2f unidades disponibles)\n", i + 1, componentes[i], cantidades[i]);
    }
    
    printf("\nSeleccione el componente a aumentar (1-%d): ", numComponentes);
    val = scanf("%d", &opcion);
    limpiarBuffer();
    
    if (val != 1)
    {
        printf("Error: Entrada inválida.\n");
        return;
    }
    
    if (opcion < 1)
    {
        printf("Error: Opción inválida.\n");
        return;
    }
    
    if (opcion > numComponentes)
    {
        printf("Error: Opción inválida.\n");
        return;
    }
    
    int indice = opcion--;
    
    do
    {
        printf("Cantidad a añadir a '%s' (actual: %.2f): ", componentes[indice], cantidades[indice]);
        val = scanf("%f", &cantidad);
        limpiarBuffer();
        
        if (val != 1)
        {
            printf("Error: Ingrese un valor numérico.\n");
            continue;
        }
        
        if (cantidad <= 0)
        {
            printf("Error: La cantidad debe ser mayor que cero.\n");
            continue;
        }
        
        break;
        
    } while (1);
    
    cantidades[indice] = cantidades[indice] + cantidad;
    
    printf("\nInventario de '%s' aumentado correctamente. Nueva cantidad: %.2f\n", 
          componentes[indice], cantidades[indice]);
}