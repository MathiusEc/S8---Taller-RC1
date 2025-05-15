#include <stdio.h>
#include <string.h>
#include "funciones.h"

#define MAX_PRODUCTOS 5
#define TASA_IMPUESTO_BASE 16.0 // Porcentaje de IVA base

int menu()
{
    int opc;
    printf("\n==============================\n");
    printf("\t MENU PRINCIPAL\n");
    printf(" 1. Ingresar productos\n");
    printf(" 2. Verificar factibilidad de producción\n");
    printf(" 3. Editar un producto\n");
    printf(" 4. Eliminar un producto\n");
    printf(" 5. Mostrar todos los productos\n");
    printf(" 6. Salir\n");
    printf("==============================\n");
    printf(">> ");

    return opc;
}

int productoExiste(char nombres[][30], int numProductos, char *producto)
{
    for (int i = 0; i < numProductos; i++)
    {
        if (strcmp(nombres[i], producto) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void leerNombreProducto(char nombres[][30], int numProductos, char *nombre)
{
    int f, len;

    do
    {
        printf("Nombre del producto: ");
        fgets(nombre, 30, stdin);
        len = strlen(nombre);

        if (len > 0 && nombre[len - 1] == '\n')
        {
            nombre[len - 1] = '\0'; // Eliminar el salto de línea
            len--;
        }

        if (len == 0)
        {
            printf("Error: El nombre no puede estar vacío.\n");
            f = 0;
        }
        else if (productoExiste(nombres, numProductos, nombre) == 1)
        {
            printf("Error: Ya existe un producto con ese nombre.\n");
            f = 0;
        }
        else
        {
            f = 1;
        }

    } while (f == 0);
}

float leerTiempoFabricacion()
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
            fflush(stdin);
            continue;
        }

        if (tiempo <= 0)
        {
            printf("Error: El tiempo debe ser mayor que cero.\n");
        }
    } while (tiempo <= 0 || val != 1);
    return tiempo;
}

float leerRecursosNecesarios()
{
    float recursos;
    int val;
    do
    {
        printf("Recursos necesarios (unidades): ");
        val = scanf("%f", &recursos);
        fflush(stdin);

        if (val != 1)
        {
            printf("Error: Ingrese un valor numérico.\n");
            fflush(stdin);
            continue;
        }

        if (recursos <= 0)
        {
            printf("Error: Los recursos deben ser mayores que cero.\n");
        }
    } while (recursos <= 0 || val != 1);
    return recursos;
}

float leerCostoUnidad()
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
            fflush(stdin);
            continue;
        }
        if (costo <= 0)
        {
            printf("Error: El costo debe ser mayor que cero.\n");
        }
    } while (costo <= 0 || val != 1);
    return costo;
}

void leerRegistroSanitario(char *registro)
{
    int len;
    do
    {
        printf("Registro sanitario: ");
        fflush(stdin);
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
}

void calcularRequerimientos(float tiempo, float recurso, int cantidad, float costo, float impuesto,
                            float *tiempoTotal, float *recursosTotal, float *costoTotal, float *impuestoTotal)
{
    *tiempoTotal = tiempo * cantidad;
    *recursosTotal = recurso * cantidad;

    float costoProd = costo * cantidad; // Calcula el costo total sin impuestos
    *impuestoTotal = costoProd * (impuesto / 100.0f); // Calcula el impuesto
    *costoTotal = costoProd + *impuestoTotal; // Suma el impuesto al costo total
}

void verificarFactibilidad(char nombres[][30], float tiempos[], float recursos[], float costos[], char registros[][30],
                           int indice, int cantidad, float tiempoDisponible, float recursosDisponibles, float impuesto)
{
    float tiempoTotal = 0, recursosTotal = 0, costoTotal = 0, impuestoTotal = 0;
    
    // Calcular los requerimientos totales
    calcularRequerimientos(tiempos[indice], recursos[indice], cantidad, costos[indice], 
                           impuesto, &tiempoTotal, &recursosTotal, &costoTotal, &impuestoTotal);

    int factible = 1;
    float tiempoFaltante = 0;
    float recursosFaltantes = 0;

    // Comparaciones explícitas de disponibilidad
    if (tiempoTotal > tiempoDisponible)
    {
        tiempoFaltante = tiempoTotal - tiempoDisponible;
        factible = 0;
    }

    if (recursosTotal > recursosDisponibles)
    {
        recursosFaltantes = recursosTotal - recursosDisponibles;
        factible = 0;
    }

    // Impresión de la factura proyectada
    printf("\n\tFACTURA PROYECTADA\n");
    printf("\tProducto:\t\t%s\n", nombres[indice]);
    printf("\tRegistro Sanitario:\t%s\n", registros[indice]);
    printf("\tCantidad:\t\t%d\n", cantidad);
    printf("\tPrecio Unitario:\t$%.2f\n", costos[indice]);
    printf("\tTiempo por Unidad:\t%.2f horas\n", tiempos[indice]);
    printf("\tTiempo Total:\t\t%.2f horas\n", tiempoTotal);
    printf("\tRecursos por Unidad:\t%.2f unidades\n", recursos[indice]);
    printf("\tRecursos Totales:\t%.2f unidades\n", recursosTotal);

    float costoProd = costos[indice] * cantidad; // Se calcula aquí para asegurarse de que está definido correctamente
    printf("\tSubtotal:\t\t$%.2f\n", costoProd);
    printf("\tTasa de Impuesto:\t%.1f%%\n", impuesto);
    printf("\tImpuesto:\t\t$%.2f\n", impuestoTotal);
    printf("\tTOTAL:\t\t\t$%.2f\n", costoTotal);

    // Evaluación explícita de factibilidad
    if (factible == 1) 
    {
        printf("\n\t¡PRODUCCIÓN FACTIBLE!\n");
        printf("\tEs posible cumplir con la producción con los recursos disponibles.\n");
    } 
    else 
    {
        printf("\n\t¡PRODUCCIÓN NO FACTIBLE!\n");
        
        if (tiempoFaltante > 0.0f) 
        {
            printf("\tTIEMPO INSUFICIENTE:\n");
            printf("\t- Tiempo Disponible:\t%.2f horas\n", tiempoDisponible);
            printf("\t- Tiempo Necesario:\t%.2f horas\n", tiempoTotal);
            printf("\t- Tiempo Faltante:\t%.2f horas\n", tiempoFaltante);
        }

        if (recursosFaltantes > 0.0f) 
        {
            printf("\tRECURSOS INSUFICIENTES:\n");
            printf("\t- Recursos Disponibles:\t%.2f unidades\n", recursosDisponibles);
            printf("\t- Recursos Necesarios:\t%.2f unidades\n", recursosTotal);
            printf("\t- Recursos Faltantes:\t%.2f unidades\n", recursosFaltantes);
        }
    }
}
