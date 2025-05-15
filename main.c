#include <stdio.h>
#include <string.h>
#include "funciones.h"

#define MAX_PRODUCTOS 5
#define TASA_IMPUESTO_BASE 15 // Bienes y servicios utlizados en la producción

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

        case 2:
            if (numProductos == 0)
            {
                printf("\n\tNo hay productos registrados.\n");
                break;
            }
            {
                char nombreBuscar[30];
                int indice = -1;
                int cantidad;
                float tiempoDisponible, recursosDisponibles;

                printf("\n\tIngrese el nombre del producto a verificar: ");
                fflush(stdin);
                fgets(nombreBuscar, 30, stdin);
                nombreBuscar[strcspn(nombreBuscar, "\n")] = '\0';

                // Buscar el índice del producto
                for (int i = 0; i < numProductos; i++)
                {
                    if (strcmp(nombres[i], nombreBuscar) == 0)
                    {
                        indice = i;
                        break;
                    }
                }

                if (indice == -1)
                {
                    printf("\tProducto no encontrado.\n");
                    break;
                }

                printf("Ingrese la cantidad a producir: ");
                scanf("%d", &cantidad);
                fflush(stdin);

                printf("Ingrese el tiempo disponible (horas): ");
                scanf("%f", &tiempoDisponible);
                fflush(stdin);

                printf("Ingrese los recursos disponibles (unidades): ");
                scanf("%f", &recursosDisponibles);
                fflush(stdin);

                verificarFactibilidad(
                    nombres, tiempos, recursos, costos, registros,
                    indice, cantidad, tiempoDisponible, recursosDisponibles, TASA_IMPUESTO_BASE);
            }
            break;
            
        default:
            break;
        }
    } while (opci != 6 || val != 0);

    return 0;
}