## 1. Estructura del Programa
El programa debe estar modularizado en distintos archivos para facilitar su mantenimiento y claridad:

main.c: Punto de entrada del programa, maneja la interfaz de usuario y la ejecución de funciones clave.

productos.h / productos.c: Manejo de los datos de los productos, incluyendo CRUD.

calculos.h / calculos.c: Cálculo del tiempo total de producción y recursos necesarios.

verificacion.h / verificacion.c: Determinación de viabilidad con los recursos disponibles.

utilidades.h / utilidades.c: Algoritmos de ordenamiento y búsqueda personalizados.

## 2. Estrategia para CRUD sin Estructuras
Dado que no podemos usar estructuras, aprovecharemos arreglos paralelos:

Un arreglo char nombres[5][30] para almacenar los nombres.

Un arreglo int tiempos[5] para los tiempos de fabricación.

Un arreglo int recursos[5] para los recursos requeridos.

Un arreglo int cantidades[5] para la demanda.

### Para CRUD:

Ingreso: Se almacena el producto en el índice correspondiente de los arreglos.

Edición: Se busca el índice del producto y se actualizan los valores.

Eliminación: Se reemplaza el producto con valores nulos o se compacta el arreglo.

## 3. Algoritmos de Búsqueda y Ordenamiento
Dado que no podemos usar funciones predefinidas:

Búsqueda:

Lineal para modificar o eliminar productos (adecuada por ser pocos elementos).

Binaria si decidimos ordenar los productos por nombre o demanda.

## Ordenamiento:

Bubble Sort mejorado para facilitar la implementación sin estructuras.

Selection Sort si buscamos eficiencia con poca cantidad de datos.

### 4. Enfoque para Viabilidad de Producción
Calcular el tiempo total sumando tiempos[i] * cantidades[i].

Calcular los recursos totales sumando recursos[i] * cantidades[i].

Comparar los valores con los límites disponibles y determinar si se puede cumplir la demanda.

### 5. Consejos para Mantenibilidad y Eficiencia
Modularización con archivos .h y .c para una estructura clara.

Uso de funciones independientes para cada operación en lugar de código redundante.

Arreglos paralelos bien documentados para facilitar la comprensión.

Comentarios detallados en las funciones clave para mejorar la legibilidad.

Evitar iteraciones innecesarias en búsquedas y ordenamientos para optimizar el tiempo de ejecución.
