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
                
                if (val != 1 || cantidad <= 0)
                {
                    printf("Error: La cantidad debe ser un número mayor que cero.\n");
                    break;
                }

                printf("Ingrese el tiempo disponible (horas): ");
                val = scanf("%f", &tiempoDisponible);
                limpiarBuffer();
                
                if (val != 1 || tiempoDisponible <= 0)
                {
                    printf("Error: El tiempo disponible debe ser un número mayor que cero.\n");
                    break;
                }

                verificarFactibilidad(nombres, tiempos, registros, costos, requerimientosComp,
                                    indice, cantidad, tiempoDisponible, 
                                    componentes, cantidades, numComponentes);
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
            
        case 6: // Salir
            printf("\nSaliendo del sistema. ¡Hasta pronto!\n");
            break;
            
        default:
            printf("\nOpción inválida. Intente nuevamente.\n");
        }
        
        if (opcion != 6)
        {
            printf("\nPresione Enter para continuar...");
            getchar();
        }
        
    } while (opcion != 6);

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
    printf("6. Salir\n");
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
    
    // Leer nombre del componente
    do
    {
        printf("Nombre del componente: ");
        fgets(nombre, 30, stdin);
        len = strlen(nombre);
        
        if (len > 0 && nombre[len - 1] == '\n')
        {
            nombre[len - 1] = '\0';
            len--;
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
            for (int i = 0; i < *numComponentes; i++)
            {
                if (strcmp(componentes[i], nombre) == 0)
                {
                    printf("Error: Ya existe un componente con ese nombre.\n");
                    printf("Si desea actualizar la cantidad, use la opción correspondiente.\n");
                    existe = 1;
                    break;
                }
            }
        }
    } while (existe == 1);
    
    // Leer cantidad disponible
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
        }
    } while (cantidad <= 0 || val != 1);
    
    // Guardar el componente
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
    
    // Inicializar requerimientos de componentes a 0
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
        
        if (len > 0 && nombre[len - 1] == '\n')
        {
            nombre[len - 1] = '\0';
            len--;
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
        }
    } while (tiempo <= 0 || val != 1);
    
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
        }
    } while (costo <= 0 || val != 1);
    
    // Leer registro sanitario
    char registro[30];
    do
    {
        printf("Registro sanitario: ");
        fgets(registro, 30, stdin);
        len = strlen(registro);
        
        if (len > 0 && registro[len - 1] == '\n')
        {
            registro[len - 1] = '\0';
            len--;
        }
        
        if (len == 0)
        {
            printf("Error: El registro sanitario no puede estar vacío.\n");
        }
    } while (len == 0);
    
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
    
    (*numProductos)++;
    
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
    int componentesFaltantes[MAX_COMPONENTES] = {0};
    int hayFaltantes = 0;
    
    for (int i = 0; i < numComponentes; i++)
    {
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

