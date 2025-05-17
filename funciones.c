#include <stdio.h>
#include <string.h>
#include "funciones.h"

#define MAX_PRODUCTOS 5
#define MAX_COMPONENTES 10
#define TASA_IMPUESTO_BASE 16.0 // Bienes y servicios utilizados en la producción

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

int mostrarMenu()
{
    int opc;
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

    scanf("%d", &opc);
    return opc;
}

// opcion1
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
                len--;
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
        fflush(stdin);

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

    } while (val != 1 || cantidad <= 0);

    strcpy(componentes[*numComponentes], nombre);
    cantidades[*numComponentes] = cantidad;
    (*numComponentes)++;

    printf("\nComponente registrado correctamente.\n");
}

// opcion2
void mostrarComponentes(char componentes[][30], float cantidades[], int numComponentes)
{
    if (numComponentes == 0)
    {
        printf("\nNo hay componentes registrados en el inventario.\n");
        return;
    }

    printf("\n--- INVENTARIO DE COMPONENTES ---\n");
    printf("Componente\tCantidad\n");
    printf("----------------------------------------\n");

    for (int i = 0; i < numComponentes; i++)
    {
        printf("%s\t\t%.2f\n", componentes[i], cantidades[i]);
    }
}

// opcion3
void obtenerNombreProducto(char nombre[], char nombres[][30], int numProductos)
{
    int len, existe;

    do
    {
        printf("Nombre del producto: ");
        fgets(nombre, 30, stdin);
        len = strlen(nombre);

        // Eliminar el salto de línea si está presente
        if (nombre[len - 1] == '\n')
        {
            nombre[len - 1] = '\0';
            len--; // Reducir longitud correctamente
        }

        if (len == 0)
        {
            printf("Error: El nombre no puede estar vacío.\n");
            existe = 1;
        }
        else
        {
            // Verificar si el nombre ya existe
            existe = 0;
            for (int i = 0; i < numProductos; i++)
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
}

float obtenerTiempoFabricacion()
{
    float tiempo;
    int val;

    do
    {
        printf("Tiempo de fabricación (horas): ");
        val = scanf("%f", &tiempo);
        fflush(stdin);

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

    } while (val != 1 || tiempo <= 0);
    return tiempo;
}

float obtenerCostoPorUnidad()
{
    float costo;
    int val;

    do
    {
        printf("Costo por unidad ($): ");
        val = scanf("%f", &costo);
        fflush(stdin);

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

    } while (val != 1 || costo <= 0);
    return costo;
}

void obtenerRegistroSanitario(char registro[])
{
    int len;

    do
    {
        printf("Registro sanitario: ");
        fgets(registro, 30, stdin);
        len = strlen(registro);

        if (registro[len - 1] == '\n')
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
}

void mostrarComponentesDisponibles(char componentes[][30], float cantidades[], int numComponentes)
{
    printf("\nComponentes disponibles en inventario:\n");
    for (int i = 0; i < numComponentes; i++)
    {
        printf("%d. %s (%.2f unidades disponibles)\n", i + 1, componentes[i], cantidades[i]);
    }
}

void obtenerRequerimientosComponentes(int requerimientosComp[][MAX_COMPONENTES], int numProducto,
                                      char componentes[][30], int numComponentes)
{
    int cantidadComp;
    int val;

    printf("\nIngrese la cantidad requerida de cada componente para este producto:\n");
    for (int i = 0; i < numComponentes; i++)
    {
        do
        {
            printf("%s: ", componentes[i]);
            val = scanf("%d", &cantidadComp);
            fflush(stdin);

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

            requerimientosComp[numProducto][i] = cantidadComp;
            break;

        } while (val != 1 || cantidadComp < 0);
    }
}

// opcion4
int verificarFactibilidad(char nombres[][30], float tiempos[], char registros[][30],
                          float costos[], int requerimientosComp[][MAX_COMPONENTES],
                          int indice, int cantidad, float tiempoDisponible,
                          char componentes[][30], float cantidades[], int numComponentes,
                          float *tiempoTotal, float *costoTotal, float *impuestoTotal,
                          float *totalConImpuesto, float *tiempoFaltante,
                          int componentesFaltantes[], int *hayFaltantes)
{
    *tiempoTotal = tiempos[indice] * cantidad;
    *costoTotal = costos[indice] * cantidad;
    *impuestoTotal = (*costoTotal) * (TASA_IMPUESTO_BASE / 100.0);
    *totalConImpuesto = (*costoTotal) + (*impuestoTotal);

    int factible = 1;
    *tiempoFaltante = 0;
    *hayFaltantes = 0;

    if (*tiempoTotal > tiempoDisponible)
    {
        *tiempoFaltante = (*tiempoTotal) - tiempoDisponible;
        factible = 0;
    }

    for (int i = 0; i < numComponentes; i++)
    {
        componentesFaltantes[i] = 0;
        int requerido = requerimientosComp[indice][i] * cantidad;
        if (requerido > cantidades[i])
        {
            componentesFaltantes[i] = requerido - cantidades[i];
            *hayFaltantes = 1;
            factible = 0;
        }
    }

    return factible;
}

void imprimirFactura(char nombres[][30], char registros[][30], int indice, int cantidad,
                     float tiempos[], float tiempoTotal, float costos[],
                     float costoTotal, float impuestoTotal, float totalConImpuesto,
                     char componentes[][30], float cantidades[], int requerimientosComp[][MAX_COMPONENTES],
                     int componentesFaltantes[], int numComponentes)
{
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
            printf("\t- %s\t%d unidades", componentes[i], totalRequerido);
            printf("\n");
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
}

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

// opcion6
void eliminarProducto(char nombres[][30], float tiempos[], char registros[][30],
                      float costos[], int requerimientosComp[][MAX_COMPONENTES],
                      int *numProductos)
{
    char nombreEliminar[30], continuar;
    int indice, len;

    printf("\n--- ELIMINAR PRODUCTO ---\n");

    // Mostrar productos disponibles
    printf("Productos disponibles:\n");
    for (int i = 0; i < *numProductos; i++)
    {
        printf("%d. %s\n", i + 1, nombres[i]);
    }

    printf("\nIngrese el nombre del producto a eliminar: ");
    fgets(nombreEliminar, 30, stdin);
    len = strlen(nombreEliminar);
    if (nombreEliminar[len - 1] == '\n')
    {
        nombreEliminar[len - 1] = '\0';
        len--;
    }

    indice = buscarProducto(nombres, *numProductos, nombreEliminar);

    if (indice == -1)
    {
        printf("Error: Producto no encontrado.\n");
        return;
    }

    // Confirmación antes de eliminar
    do
    {
        printf("¿Está seguro de que desea eliminar '%s'? (S/N): ", nombreEliminar);
        fflush(stdin);
        scanf(" %c", &continuar);

        if (continuar != 'N' && continuar != 'n' && continuar != 'S' && continuar != 's')
        {
            printf("Ingrese una opción válida.\n");
        }
    } while (continuar != 'N' && continuar != 'n' && continuar != 'S' && continuar != 's');

    if (continuar == 'N' || continuar == 'n')
    {
        printf("\nOperación cancelada. El producto no fue eliminado.\n");
        return;
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

    (*numProductos)--;

    printf("\nProducto '%s' eliminado correctamente.\n", nombreEliminar);
}

// opcion7
void editarComponentesProducto(char nombres[][30], int requerimientosComp[][MAX_COMPONENTES], 
                             int numProductos, char componentes[][30], float cantidades[], 
                             int numComponentes)
{
    char nombreProducto[30];
    int indiceProducto;
    int val, len;
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
    len = strlen(nombreProducto);
    if (nombreProducto[len - 1] == '\n')
    {
        nombreProducto[len - 1] = '\0';
        len--;
    }
   
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
            fflush(stdin);
            
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

// opcion8

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
    fflush(stdin);
    
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
        fflush(stdin);     
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
