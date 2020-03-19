/**
 * @file: Ordenacion.c
 * @brief: implmentación de las funciones para cambiar el orden de los
 * vértices del grafo.
 */

#include "Lista.h"
#include "U32ToString.h"
#include "Ordenacion.h"
#include "Rii.h"

/**
 * @fun: DescendenteGrado(const void * a, const void * b).
 * @param a: puntero a void.
 * @param b: puntero a void.
 * @return: devuelve 0 si los campos datoVertice de referenciados por a y b son
 * iguales; -1 si a > b, y 1 si b > a.
 */
int DescendenteDato(const void * a, const void * b) {
    u32 primero = ((*(Aux_Array_t *)a)->datoVertice);
    u32 segundo = ((*(Aux_Array_t *)b)->datoVertice);
    if (primero > segundo) {
        return -1;
    } else if (primero < segundo) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * @fun: AscendenteNombre(const void * a, const void * b)
 * @param a: puntero a void.
 * @param b: puntero a void.
 * @return: devuelve 0 si los campos datoVertice de referenciados por a y b son
 * iguales; 1 si a > b, y -1 si b > a.
 */
int AscendenteDato(const void * a, const void * b) {
    u32 primero = ((*(Aux_Array_t*)a)->datoVertice);
    u32 segundo = ((*(Aux_Array_t*)b)->datoVertice);
    if (primero > segundo) {
        return 1;
    } else if (primero < segundo) {
        return -1;
    } else {
        return 0;
    }
}

// Definimos un tipo para crear un arreglo de funciones de comparacion de
// qsort().
typedef int (*FuncionesComparacion)(const void *, const void *);
// Creamos el arreglo y lo inicializamos...
const FuncionesComparacion funCompares[2] = {AscendenteDato, DescendenteDato};

/**
 * @fun: GenerarNuevoOrden(Grafo G, Aux_Array_t  *array).
 * @param G: una estructura grafo inicializada.
 * @param array: un arreglo de estructuras Aux_Array.
 * @brief: un procedimiento que copia en array_orden_vértices del
 * grafo un nuevo orden dado por array y libera la memoria ocupada por cada
 * estructura luego de la copia.
 */
void GenerarNuevoOrden(Grafo G, Aux_Array_t  *array) {
    for (u32 i = 0; i < G->infoGrafo[Vertices]; i++) {
        G->array_orden_vertices[i] = array[i]->indice;
        free(array[i]);
    }
}

/**
 * @fun: GuardarIndice_DatoVertice(Grafo grafo, Aux_Array_t *array, enum
        InfoVertice x).
 * @param grafo: una estructura Grafo.
 * @param array: un arreglo de estructuras Aux_Array.
 * @param x: la información de un vértice.
 * @brief: es una función que almacena la información del vértice
 * solicitada junto a su posición en array_vertices de la estructura grafo en
 * un arreglo de estructuras Aux_Array. La posicion se almacena en el campo
 * índice de la estructura y el dato en el campo datoVertice.
 * @return: retorna 1 si hubo errores; 0 en caso contrario.
 */
char GuardarIndice_DatoVertice(Grafo grafo, Aux_Array_t *array, enum
        InfoVertice x) {
    for (u32 i = 0; i < grafo->infoGrafo[Vertices]; i++) {
        array[i] = malloc(sizeof(Aux_Array));
        if (array[i] == NULL) return 1;
        array[i]->indice = i;
        array[i]->datoVertice = grafo->array_vertices[i].infoVertice[x];
    }
    return 0;
}

/**
 * @fun: OrdenarSegunX(enum InfoVertice x, Grafo G);
 * @param info: la información de un vértice.
 * @param modo: el modo en que se va a ordenar.
 * @param G: una estructura Grafo correctamente creada.
 * @brief: es una función que modifica el orden que el grafo según el
 * parámetro x y modo ingresado.
 * @return: retorna 1 si hubo errores; 0 en caso contrario.
 */
char OrdenarSegunX(enum InfoVertice info, enum ModoOrden modo, Grafo G) {
    //si el grafo G es vacio termina.
    if (G == NULL) return 1;
    // Se aloja memoria para el arreglo de estructuras Aux_Array.
    Aux_Array_t *array = malloc(G->infoGrafo[Vertices]*sizeof(Aux_Array_t));
    // Verificamos que se haya alojado memoria correctamente.
    if (array == NULL) return 1;
    // Usamos la función GuardarIndice_DatoVertice() para almacenar en el
    // arreglo la información de los vertices que necesitemos y la posición en
    // que estos se encuentren.
    char error = GuardarIndice_DatoVertice(G, array, info);
    // Si la función devolvió un error entonces devolvemos 1
    if (error) return 1;
    // Seleccionamos la función de comparacion para qsort() basándonos en el
    // parámetro info y modo de la siguiente forma: se toma modulo 2 del
    // parámetro info de forma que siempre de 0 o 1; luego el modo que puede
    // ser Normal (=0) o Invertido (=1).
    // Tenemos las siguientes casos:
    // ----------- Info          Modo           funCompare -----------
    //             Nombre(=0)    Normal(=0)     0 -> AscendenteDato
    //             Grado(=1)     Normal(=0)     1 -> DescendenteDato
    //             Color(=0)     Normal(=0)     0 -> AscendenteDato
    //             Color(=0)     Invertido(=1)  1 -> DescendenteDato
    // Por lo tanto elegimos la función de comparación esta forma:
    bool numCompare = (info%2) + modo;
    qsort(array, G->infoGrafo[Vertices], sizeof(Aux_Array_t),
          funCompares[numCompare]);
    // Usamos la funcion GenerarNuevoOrden() para dar el nuevo orden al grafo.
    GenerarNuevoOrden(G,array);
    // Liberamos la memoria del arreglo.
    free(array);
    // Sino se reportaron errores entonces devolvemos 0.
    return 0;
}

/**
 * @fun: OrdenNatural(Grafo G).
 * @param G: una estructura Grafo.
 * @brief: es una función que modifica el orden de los vértices, de tal
 * manera que estén ordenados en forma ascendente por sus nombres.
 * @return: retorna 1 si hubo errores; 0 en caso contrario.
 */
char OrdenNatural(Grafo G) {
    return OrdenarSegunX(Nombre, Normal, G);
}

/**
 * @fun: OrdenWelshPowell(Grafo G).
 * @param G: una estructura Grafo.
 * @brief: es una función que modifica el orden de los vértices, de tal
 * manera que estén ordenados en forma descendente por sus grados.
 * @return: retorna 1 si hubo errores; 0 en caso contrario.
 */
char OrdenWelshPowell(Grafo G) {
    return OrdenarSegunX(Grado, Normal, G);
}

/**
 * @fun: RMBCnormal(Grafo G).
 * @param G: una estructura Grafo.
 * @brief: si el grafo tiene 1, 2,...,r colores esta función, reordena
 * colocando primero los vertices de color 1, luego los vértices de color 2,
 * etc, hasta los de color r.
 * @return: retorna 1 si hubo errores; 0 en caso contrario.
 */
char RMBCnormal(Grafo G) {
    return OrdenarSegunX(Color, Normal, G);
}

/**
 * @fun: RMBCrevierte(Grafo G).
 * @param G: una estructura Grafo.
 * @brief: Si G esta coloreado con r colores y VC1 son los vértices
 * coloreados con 1, VC2 los coloreados con 2, etc, entonces esta función ordena
 * los vértices poniendo primero los vértices de VCr−1 , luego los de VCr−2,
 * luego los de VCr−3, etc, hasta VC1.
 * @return: retorna 1 si hubo errores; 0 en caso contrario.
 */
char RMBCrevierte(Grafo G) {
    return OrdenarSegunX(Color, Invertido, G);
}

/**
 * @fun: GuardarBloqueColores(Grafo G, u32 cantidadColores,
 *                                    Aux_Array_t *array)
 * @param G: una estructura grafo correctamente construida.
 * @param cantidadColores: la cantidad de colores del grafo.
 * @param array: un arreglo de estrucutras Aux_Array.
 * @brief: es una función que almacena un color y la cantidad de vértices de
 * ese color. El color se almacena en el campo índice de la estructura y la
 * cantidad de vértices de ese color en el campo datoVertice.
 * @return: devuelte 1 si ocurrió algun error; 0 en caso contrario.
 */
char GuardarBloqueColores(Grafo G, u32 cantidadColores, Aux_Array_t *array) {
    // Cargamos el arreglo de estructuras Aux_Array de la siguiente forma...
    for (u32 i = 0; i < cantidadColores; ++i) {
        array[i] = malloc(sizeof(Aux_Array));
        // Verificamos si se asignó memoria.
        if (array[i] == NULL) return 1;
        // Como dato se guarda la cantidad de vertices con color i que tiene
        // el grafo...
        array[i]->datoVertice = G->array_cantidad_colores[i];
        // En el índice asociamos el color i a esta cantidad.
        array[i]->indice = i;
    }
    // Sino hubo errores devolvemos cero.
    return 0;
}

/**
 * @fun: LiberarBloqueColores(u32 cantColores, Aux_Array_t *bloques).
 * @param cantColores: cantidad de colores del grafo.
 * @param bloques: un arreglo de estructuras Aux_Array.
 * @brief: libera la memoria asignada a bloques.
 */
void LiberarBloqueColores(u32 cantColores, Aux_Array_t *bloques) {
    for(u32 i =0; i < cantColores; i++) {
        free(bloques[i]);
    }
    free(bloques);
}

/**
 * @fun: RMBCchicogrande(Grafo G).
 * @param G: una estructura Grafo.
 * @brief: Si G esta coloreado con r colores y VC1 son los vértices coloreados
 * con 1, VC2 los coloreados con 2, etc, entonces esta función ordena los
 * vértices poniendo primero los vértices de VCj1 , luego los de VCj2 ,
 * etc, donde j1, j2,..., jr son tales que |VCj1| ≤ |VCj2| ≤ ... ≤ |VCjr| ,
 * @return: Retorna 0 si no hubo errores, 1 si hubo algún problema.
 */
char RMBCchicogrande(Grafo G) {
    // Variable que guarda el número de colores del grafo.
    u32 numColores = G->infoGrafo[Colores];
    // Se aloja memoria para un arreglo de estructuras Aux_Array del tamaño
    // de los colores del grafo en ese momento.
    Aux_Array_t *bloquesColores = malloc(numColores*sizeof(Aux_Array_t));
    // Verificamos que se haya asignado memoria.
    if (bloquesColores == NULL) return 1;

    // Llamamos a la función GuardarBloqueColores para llenar el
    // arreglo con los colores y sus cantidades...
    char error = GuardarBloqueColores(G,numColores,bloquesColores);
    // Si hubo un error, devolvemos 1.
    if (error) return 1;

    // Ordenamos el arreglo de estructuras de menor a mayor por cantidad de
    // colores.
    qsort(bloquesColores, numColores, sizeof(Aux_Array_t), AscendenteDato);

    // Creamos un arreglo para guardar los límites superiores de los índices de
    // los bloques de colores. Los índices del arreglo representarán los
    // diferentes colores; así en el índice 0 tendremos el límite superior
    // del color 0 y así...
    u32 *array_indices = calloc(numColores,sizeof(u32));
    // Verificamos que se haya asignado memoria.
    if (array_indices == NULL) return 1;

    // Establecemos el límite superior del primer bloque de color, como la
    // cantidad de colores del mismo menos 1.
    array_indices[bloquesColores[0]->indice] = bloquesColores[0]->datoVertice
                                               - 1;
    // Establecemos la variable verticesAcumulados como la cantidad de colores
    // del primer bloque. A medida que iteremos sobre los diferentes bloques
    // iremos sumándole la cantidad de colores del bloque actual para luego
    // restarle 1 y obtener el límite superior del bloque en cuestión.
    u32 verticesAcumulados = bloquesColores[0]->datoVertice;
    // Iteramos sobre los bloques de colores...
    for (u32 k = 1; k < numColores; ++k){
        // Actualizamos la cantidad de vértices acumulados
        verticesAcumulados += bloquesColores[k]->datoVertice;
        // Establecemos el limite superior de ese bloque como la cantidad de
        // vértices acumualdos menos 1.
        array_indices[bloquesColores[k]->indice] = verticesAcumulados - 1;
    }
    // Destruimos el arreglo de bloques de colores.
    LiberarBloqueColores(numColores, bloquesColores);
    // Variable para guardar el color de un vértice.
    u32 color_i;
    // Ahora iteramos sobre todos los vértices para establecer el nuevo orden...
    u32 numVertices = G->infoGrafo[Vertices];
    for (u32 i = 0; i < numVertices; ++i){
        // Guardamos el color del vértice
        color_i = G->array_vertices[i].infoVertice[Color];
        // Accedemos al límite superior para ese color en el arreglo de
        // índices y nos posicionamos en ese límite en el arreglo de orden y
        // establecemos el índice i en ese lugar.
        G->array_orden_vertices[array_indices[color_i]] = i;
        // Luego disminuimos el limite superior para ese color en el arreglo
        // de índices.
        array_indices[color_i]--;
    }
    // Liberamos memoria
    free(array_indices);
    return 0;
}

/**
 * @fun: char switchVertices(Grafo G, u32 i, u32 j);
 * @param G: una estructura Grafo.
 * @param i: una posición de un vértice del orden de G.
 * @param j: una posición de un vértice del orden de G.
 * @brief: Si las posiciones están en el intervalo permitido, entonces
 * intercambia las posiciones de los vértices en los lugares i y j del orden
 * interno de G que tenga en ese momento.
 * @return: Retorna 1 si las posiciones no están en el intervalo permitido; 0
 * en caso contrario.
 */
char SwitchVertices(Grafo G, u32 i, u32 j) {
    // Variable para mantener el número de vértices.
    u32 numVertices = G->infoGrafo[Vertices];
    // Verificamos que i y j sean menores al número de vértices.
    if (i >= numVertices || j >= numVertices ) return 1;
    // Variables para almacenar el índice de la posición i.
    u32 indice_i;
    // Guardamos el índice de la posición i temporalmente.
    indice_i = G->array_orden_vertices[i];
    // A la posición i le asignamos el índice de la posición j.
    G->array_orden_vertices[i] = G->array_orden_vertices[j];
    // A la posición j le asignamos el índice la posición i antes del cambio.
    G->array_orden_vertices[j] = indice_i;
    // Sino hubo errores devolvemos 0.
    return 0;
}

/**
 * @fun: char SwitchColores(Grafo G, u32 i, u32 j).
 * @param G: una estructura Grafo.
 * @param i: un color de un vértice de G.
 * @param j: un color de un vértice de G.
 * @brief: Si los colores están en el intervalo permitido, entonces
 * intercambia los colores de los vertices, es decir, los vértices de color i
 * pasan a ser j y viceversa.
 * @return: Retorna 1 si las posiciones no están en el intervalo permitido; 0
 * en caso contrario.
 */
char SwitchColores(Grafo G, u32 i, u32 j) {
    // Variable para mantener el número de colores.
    u32 numColores = NumeroDeColores(G);
    // Verificamos que i y j sean menores al número de colores.
    if (i >= numColores || j >= numColores ) return 1;
    // Declaracion de variables a usar...
    u32 color_k, cant_color_i, cant_color_j, contador_vertices;
    // Obtenemos vertices de color i y j respectivamente...
    cant_color_i = G->array_cantidad_colores[i];
    cant_color_j = G->array_cantidad_colores[j];
    // Calculamos el total de vértices a intercambiar para saber cuando hemos
    // terminado el cambio de colores...
    contador_vertices = cant_color_i + cant_color_j;
    // Iteramos sobre todos los vertices...
    u32 numVertices = NumeroDeVertices(G);
    for (u32 k= 0; k < numVertices; k++) {
        // Obteniendo el color del vértice...
        color_k = G->array_vertices[k].infoVertice[Color];
        // Si el vertice tenia color i...
        if (color_k == i) {
            // Lo establecemos con color j.
            G->array_vertices[k].infoVertice[Color] = j;
            // Disminuimos el total de vértices que faltan intercambiar...
            contador_vertices--;
        }
        // Idem para el caso en que el color es j
        else if (color_k == j) {
            G->array_vertices[k].infoVertice[Color] = i;
            contador_vertices--;
        }
        // Sino es ni color i ni j seguimos...
        else {
            continue;
        }
        // Si el contador de vértices llego a cero entonces terminamos el
        // intercambio...
        if (contador_vertices == 0) break;

    }
    // Por último, actualizamos la cantidad de colores en el grafo.
    G->array_cantidad_colores[i] = cant_color_j;
    G->array_cantidad_colores[j] = cant_color_i;
    return 0;
}