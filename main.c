#include <stdio.h>
#include <string.h>
#include "funciones.h"

#define MAX_PRODUCTOS 5

int main(int argc, char *argv[])
{
    int opci, val;
    char nombres[MAX_PRODUCTOS][30];
    float tiempos[MAX_PRODUCTOS];
    float recursos[MAX_PRODUCTOS];
    float costos[MAX_PRODUCTOS];
    char registros[MAX_PRODUCTOS][30];
    int numProductos = 0;

    do
    {
        opci = menu();
        val = scanf("%d", &opci);
        fflush(stdin);
        if (val != 1)
        {
            printf("Error: Entrada inválida. Intente nuevamente.\n");
            fflush(stdin);
            continue;
        }
        
        switch (opci)
        {
        case 1:
            if (numProductos >= MAX_PRODUCTOS)
            {
                printf("Error: Ya se alcanzó el límite máximo de productos (%d).\n", MAX_PRODUCTOS);
                break;
            }

            printf("\n--- REGISTRO DE PRODUCTOS ---\n");
            printf("Ingrese la información del producto #%d:\n", numProductos + 1);

            char nombre[30];
            leerNombreProducto(nombres, numProductos, nombre);
            strcpy(nombres[numProductos], nombre);

            tiempos[numProductos] = leerTiempoFabricacion();
            recursos[numProductos] = leerRecursosNecesarios();
            costos[numProductos] = leerCostoUnidad();

            leerRegistroSanitario(registros[numProductos]);

            numProductos++;
            printf("\nProducto registrado correctamente.\n");
            break;
            break;

        default:
            break;
        }
    } while (opci != 6 || val != 0);

    return 0;
}