#include <stdio.h>
#include <string.h>
#include "funciones.h"

#define MAX_PRODUCTOS 5
#define MAX_COMPONENTES 10
#define TASA_IMPUESTO_BASE 16.0 // Bienes y servicios utilizados en la producción

int main(int argc, char *argv[])
{
    char componentes[MAX_COMPONENTES][30];
    float cantidades[MAX_COMPONENTES];
    int numComponentes = 0;

    char nombres[MAX_PRODUCTOS][30];
    float tiempos[MAX_PRODUCTOS];
    float costos[MAX_PRODUCTOS];
    char registros[MAX_PRODUCTOS][30];
    int requerimientosComp[MAX_PRODUCTOS][MAX_COMPONENTES];
    int numProductos = 0;

    int opci;

    do
    {
        opci = mostrarMenu();
        fflush(stdin);

        switch (opci)
        {
            case 1:
                ingresarComponente(componentes, cantidades, &numComponentes);
                break;

            case 2:
                mostrarComponentes(componentes, cantidades, numComponentes);
                break;

            case 3:
            {
                if (numComponentes == 0)
                {
                    printf("\nError: Primero debe ingresar componentes al inventario.\n");
                    break;
                }

                if (numProductos >= MAX_PRODUCTOS)
                {
                    printf("\nError: Ya se alcanzó el límite máximo de productos (%d).\n", MAX_PRODUCTOS);
                    break;
                }
                
                char nombre[30];
                float tiempo, costo;
                char registro[30];

                printf("\n--- REGISTRO DE PRODUCTO ---\n");

                for (int j = 0; j < numComponentes; j++)
                {
                    requerimientosComp[numProductos][j] = 0;
                }

                obtenerNombreProducto(nombre, nombres, numProductos);

                tiempo = obtenerTiempoFabricacion();

                costo = obtenerCostoPorUnidad();

                obtenerRegistroSanitario(registro);

                mostrarComponentesDisponibles(componentes, cantidades, numComponentes);

                obtenerRequerimientosComponentes(requerimientosComp, numProductos, componentes, numComponentes);

                strcpy(nombres[numProductos], nombre);
                tiempos[numProductos] = tiempo;
                costos[numProductos] = costo;
                strcpy(registros[numProductos], registro);

                numProductos++;

                printf("\nProducto registrado correctamente.\n");
                break;
            }
        }
    } while (opci != 9);

    return 0;
}
