#include <stdio.h>
#include <string.h>

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
            continue;
        }
        
        break;
        
    } while (val != 1 || cantidad <= 0);
    
    // Guardar el componente
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
        printf("%s\t%.2f\n", componentes[i], cantidades[i]);
    }
}

// opcion3
