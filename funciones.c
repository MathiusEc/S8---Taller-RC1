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
void obtenerNombreProducto(char nombre[], char nombres[][30], int numProductos) {
    int len, existe;

    do {
        printf("Nombre del producto: ");
        fgets(nombre, 30, stdin);
        len = strlen(nombre);

        // Eliminar el salto de línea si está presente
        if (len > 0 && nombre[len - 1] == '\n') {
            nombre[len - 1] = '\0';
            len--;  // Reducir longitud correctamente
        }

        if (len == 0) {
            printf("Error: El nombre no puede estar vacío.\n");
            existe = 1;
        } else {
            // Verificar si el nombre ya existe
            existe = 0;
            for (int i = 0; i < numProductos; i++) {
                if (strcmp(nombres[i], nombre) == 0) {
                    printf("Error: Ya existe un producto con ese nombre.\n");
                    existe = 1;
                    break;
                }
            }
        }
    } while (existe == 1);
}

float obtenerTiempoFabricacion() {
    float tiempo;
    int val;
    
    do {
        printf("Tiempo de fabricación (horas): ");
        val = scanf("%f", &tiempo);
        fflush(stdin);
        
        if (val != 1) {
            printf("Error: Ingrese un valor numérico.\n");
            continue;
        }
        
        if (tiempo <= 0) {
            printf("Error: El tiempo debe ser mayor que cero.\n");
            continue;
        }
        
        break;
        
    } while (val != 1 || tiempo <= 0);
    return tiempo;
}

float obtenerCostoPorUnidad() {
    float costo;
    int val;
    
    do {
        printf("Costo por unidad ($): ");
        val = scanf("%f", &costo);
        fflush(stdin);
        
        if (val != 1) {
            printf("Error: Ingrese un valor numérico.\n");
            continue;
        }
        
        if (costo <= 0) {
            printf("Error: El costo debe ser mayor que cero.\n");
            continue;
        }
        
        break;
        
    } while (val != 1 || costo <= 0);
    return costo;
}

void obtenerRegistroSanitario(char registro[]) {
    int len;
    
    do {
        printf("Registro sanitario: ");
        fgets(registro, 30, stdin);
        len = strlen(registro);
        
        if (len > 0) {
            if (registro[len - 1] == '\n') {
                registro[len - 1] = '\0';
                len = len - 1;
            }
        }
        
        if (len == 0) {
            printf("Error: El registro sanitario no puede estar vacío.\n");
            continue;
        }
        
        break;
        
    } while (1);
}

void mostrarComponentesDisponibles(char componentes[][30], float cantidades[], int numComponentes) {
    printf("\nComponentes disponibles en inventario:\n");
    for (int i = 0; i < numComponentes; i++) {
        printf("%d. %s (%.2f unidades disponibles)\n", i + 1, componentes[i], cantidades[i]);
    }
}

void obtenerRequerimientosComponentes(int requerimientosComp[][MAX_COMPONENTES], int numProducto,
                                      char componentes[][30], int numComponentes) {
    int cantidadComp;
    int val;
    
    printf("\nIngrese la cantidad requerida de cada componente para este producto:\n");
    for (int i = 0; i < numComponentes; i++) {
        do {
            printf("%s: ", componentes[i]);
            val = scanf("%d", &cantidadComp);
            fflush(stdin);
            
            if (val != 1) {
                printf("Error: Ingrese un valor numérico.\n");
                continue;
            }
            
            if (cantidadComp < 0) {
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
    
    // Verificar tiempo
    if (*tiempoTotal > tiempoDisponible)
    {
        *tiempoFaltante = (*tiempoTotal) - tiempoDisponible;
        factible = 0;
    }
    
    // Verificar componentes
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



void mostrarResultadoFactibilidad(int factible, float tiempoDisponible, float tiempoTotal, 
                                 float tiempoFaltante, int hayFaltantes, 
                                 char componentes[][30], int componentesFaltantes[], int numComponentes)
{
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



