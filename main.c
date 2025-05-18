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
        case 4:
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

            char nombreBuscar[30];
            int componentesFaltantes[MAX_COMPONENTES];
            int hayFaltantes = 0;
            int indice, cantidad, len, val;
            float tiempoDisponible, tiempoTotal, costoTotal, impuestoTotal, totalConImpuesto;
            float tiempoFaltante;
            char respuesta;

            printf("\nIngrese el nombre del producto a verificar: ");
            fgets(nombreBuscar, 30, stdin);
            len = strlen(nombreBuscar);
            if (nombreBuscar[len - 1] == '\n')
            {
                nombreBuscar[len - 1] = '\0';
                len--;
            }

            indice = buscarProducto(nombres, numProductos, nombreBuscar);

            if (indice == -1)
            {
                printf("Error: Producto no encontrado.\n");
                break;
            }

            printf("Ingrese la cantidad a producir: ");
            val = scanf("%d", &cantidad);
            fflush(stdin);

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
            fflush(stdin);

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

            verificarFactibilidad(nombres, tiempos, registros, costos,
                                  requerimientosComp, indice, cantidad, tiempoDisponible,
                                  componentes, cantidades, numComponentes,
                                  &tiempoTotal, &costoTotal, &impuestoTotal,
                                  &totalConImpuesto, &tiempoFaltante,
                                  componentesFaltantes, &hayFaltantes);

            // Preguntar si desea ejecutar la producción
            do
            {
                printf("\n¿Desea ejecutar la producción? (S/N): ");
                scanf(" %c", &respuesta);

                if (respuesta == 'N' || respuesta == 'n')
                {
                    printf("Proceso terminado.\n");
                    break;
                }

                if (respuesta != 'S' && respuesta != 's')
                {
                    printf("Ingrese una opción válida (S/N).\n");
                }

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
                        imprimirFactura(nombres, registros, indice, cantidad,
                                        tiempos, tiempoTotal, costos,
                                        costoTotal, impuestoTotal, totalConImpuesto,
                                        componentes, cantidades, requerimientosComp,
                                        componentesFaltantes, numComponentes);

                        ejecutarProduccion(nombres, tiempos, registros, costos, requerimientosComp,
                                           indice, cantidad, componentes, cantidades, numComponentes);
                    }
                    else
                    {
                        printf("\nNo se puede ejecutar la producción debido a recursos insuficientes.\n");
                        printf("Faltantes:\n");
                        for (int i = 0; i < numComponentes; i++)
                        {
                            int requerido = requerimientosComp[indice][i] * cantidad;
                            if (requerido > cantidades[i])
                            {
                                printf("- %s: faltan %.2f unidades\n", componentes[i], requerido - cantidades[i]);
                            }
                        }
                    }  
                }
            } while (respuesta != 'S' && respuesta != 's');
            break;

        case 5:
            if (numProductos == 0)
            {
                printf("\nNo hay productos registrados.\n");
                break;
            }

            printf("\n================= LISTA DE PRODUCTOS =================\n");
            for (int i = 0; i < numProductos; i++)
            {
                printf("--------------------------------------------------------\n");
                printf("Producto:           %s\n", nombres[i]);
                printf("Tiempo fabricación: %.2f horas\n", tiempos[i]);
                printf("Costo por unidad:   $%.2f\n", costos[i]);
                printf("Registro sanitario: %s\n", registros[i]);
                printf("Componentes requeridos:\n");
                int tieneComponentes = 0;
                for (int j = 0; j < numComponentes; j++)
                {
                    if (requerimientosComp[i][j] > 0)
                    {
                        printf("    - %s: %d unidades\n", componentes[j], requerimientosComp[i][j]);
                        tieneComponentes = 1;
                    }
                }
            }
                break;
            case 6:
                if (numProductos == 0)
                {
                    printf("\nNo hay productos registrados para eliminar.\n");
                    break;
                }

                eliminarProducto(nombres, tiempos, registros, costos, requerimientosComp, &numProductos);
                break;

            case 7:
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

            case 8:
                if (numComponentes == 0)
                {
                    printf("\nNo hay componentes en el inventario para aumentar.\n");
                    break;
                }

                aumentarInventario(componentes, cantidades, numComponentes);
                break;

            case 9:
                printf("\nSaliendo del sistema. ¡Hasta pronto!\n");
                break;
            default:
                printf("\nOpción inválida. Intente nuevamente.\n");
            }
    } while (opci != 9);

    return 0;
}
