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

float leerTiempoFabricacion() {
    float tiempo;
    int val;
    do {
        printf("Tiempo de fabricación (horas): ");
        val = scanf("%f", &tiempo);
        fflush(stdin);
        if (val != 1) {
            printf("Error: Ingrese un valor numérico.\n");
            fflush(stdin);
            continue;
        }

        if (tiempo <= 0) {
            printf("Error: El tiempo debe ser mayor que cero.\n");
        }
    } while (tiempo <= 0 || val != 1);
    return tiempo;
}

float leerRecursosNecesarios() {
    float recursos;
    int val;
    do {
        printf("Recursos necesarios (unidades): ");
        val = scanf("%f", &recursos);
        fflush(stdin);

        if (val != 1) {
            printf("Error: Ingrese un valor numérico.\n");
            fflush(stdin);
            continue;
        }
        
        if (recursos <= 0) {
            printf("Error: Los recursos deben ser mayores que cero.\n");
        }
    } while (recursos <= 0 || val != 1);
    return recursos;
}

float leerCostoUnidad() {
    float costo;
    int val;
    do {
        printf("Costo por unidad ($): ");
        val = scanf("%f", &costo);
        fflush(stdin);
        if (val != 1) {
            printf("Error: Ingrese un valor numérico.\n");
            fflush(stdin);
            continue;
        }
        if (costo <= 0) {
            printf("Error: El costo debe ser mayor que cero.\n");
        }
    } while (costo <= 0 || val != 1);
    return costo;
}

void leerRegistroSanitario(char *registro) {
    int len;
    do {
        printf("Registro sanitario: ");
        fflush(stdin);
        fgets(registro, 30, stdin);
        len = strlen(registro);
        if (len > 0 && registro[len - 1] == '\n') {
            registro[len - 1] = '\0';
            len--;
        }
        if (len == 0) {
            printf("Error: El registro sanitario no puede estar vacío.\n");
        }
    } while (len == 0);
}
