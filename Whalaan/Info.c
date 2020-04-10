/**
 * @file: Info.c
 * @brief: implementación de las funciones necesarias para obtener
 * información del grafo.
 */

#include "Rii.h"

/**
 * @fun: InfoGrafo(Grafo G, enum InfoGrafo x).
 * @param G: una estructura Grafo.
 * @param x: un dato de tipo InfoGrafo.
 * @return: si el grafo no es nulo, devuelve información del grafo en función
 * del parámetro x ingresado.
 */
u32 InfoGrafo(Grafo G, enum InfoGrafo x) {
    if (G == NULL) return 0;
    return G->infoGrafo[x];
}

/**
 * @fun: NumeroDeVertices(Grafo G).
 * @param G: una estructura Grafo.
 * @return: devuelve el número de vértices del grafo si no es nulo; 0 en caso
 * contrario.
 */
u32 NumeroDeVertices(Grafo G) {
    return InfoGrafo(G, Vertices);
}

/**
 * @fun: NumeroDeLados(Grafo G).
 * @param G: una estructura Grafo.
 * @return: devuelve el número de lados del grafo si no es nulo; 0 en caso
 * contrario.
 */
u32 NumeroDeLados(Grafo G) {
    return InfoGrafo(G, Lados);
}

/**
 * @fun: NumeroDeColores(Grafo G).
 * @param G: una estructura Grafo.
 * @return: devuelve el número de colores del grafo si no es nulo; 0 en caso
 * contrario.
 */
u32 NumeroDeColores(Grafo G) {
    return InfoGrafo(G, Colores);
}

/**
 * @fun: XDelVertice(enum InfoVertice x, Grafo G, u32 i).
 * @param x: un dato de tipo InfoVertice.
 * @param G: una estructura grafo.
 * @param i: un orden de un vértice.
 * @return: devuelve información de un vertice con orden i en el grafo G si
 * no es nulo; 0, en caso contrario.
 */
u32 XDelVertice(enum InfoVertice x, Grafo G, u32 i) {
    if (G == NULL) return 0;
    // Obtenemos el índice del vértice de acuerdo al orden.
    u32 indice_i = G->array_orden_vertices[i];
    // obtemos el número de vértices de G.
    u32 num_vertices = G->infoGrafo[Vertices];
    // Se define un máximo color que podria o no tener un vertice,
    // en el caso de que si podria, seria inmanejable su utilización. 
    u32 max_color = (2^32) - 1;
    if (indice_i >= num_vertices) {
            return max_color;
    }
    return G->array_vertices[indice_i].infoVertice[x];
}

/**
 * @fun: NombreDelVertice(Grafo G, u32 i).
 * @param G: una estructura Grafo.
 * @param i: orden de un vértice.
 * @return: devuelve el nombre del vértice en el orden i guardado en ese
 * momento en G si no es nulo; 0 en caso contrario.
 */
u32 NombreDelVertice(Grafo G, u32 i) {
    return XDelVertice(Nombre, G, i);
}

/**
 * @fun: ColorDelVertice(Grafo G, u32 i).
 * @param G: una estructura Grafo.
 * @param i: orden de un vértice.
 * @return: devuelve el color del vértice en el orden i guardado en ese
 * momento en G si no es nulo; 0 en caso contrario.
 */
u32 ColorDelVertice(Grafo G, u32 i) {
    return XDelVertice(Color, G, i);
}

/**
 * @fun: GradoDelVertice(Grafo G, u32 i).
 * @param G: una estructura Grafo.
 * @param i: orden de un vértice.
 * @return: devuelve el grado del vértice en el orden i guardado en ese
 * momento en G si no es nulo; 0 en caso contrario.
 */
u32 GradoDelVertice(Grafo G, u32 i) {
    return XDelVertice(Grado, G, i);
}

/**
 * @fun: XJotaesimoVecino(enum InfoVertice x, Grafo G, u32 i, u32 j).
 * @param x: un dato de tipo InfoVertice.
 * @param G: una estructura Grafo.
 * @param i: orden de un vértice.
 * @param j: número de vecino de un vértice.
 * @return: devuelve información (x) del vécino numero j del vértice en el
 * orden i  guardado en ese momento en G si no nulo; 0 en caso contrario o que
 * j sea mayor al grado del vértice en el orden i.
 */
u32 XJotaesimoVecino(enum InfoVertice x, Grafo G, u32 i, u32 j) {
    if (G == NULL) return 0;
    // Obtenemos el índice del vértice de acuerdo al orden.
    u32 indice_i = G->array_orden_vertices[i];
    // obtemos el número de vértices de G.
    u32 num_vertices = G->infoGrafo[Vertices];
    // Se define un máximo color que podria o no tener un vertice,
    // en el caso de que si podria, seria inmanejable su utilización. 
    u32 max_color = (2^32) - 1;
    // Comprobación si el indice_i cae fuera del array_orden_vertices, y si j  
    // cae fuera del arreglo de vecinos_v. en caso de que si no es posible que 
    // el vertice en orden i tenga un vecino j...
    if (((indice_i >= num_vertices) || (j >= GradoDelVertice(G, i))) 
                                                    && (x == Grado)) {
        // Devolvemos cero.
        return max_color;
        // En caso contrario...
    } else {
        VerticeSt vertice_i, vertice_j;
        // Obtenemos el vértice que se corresponde con el indice_i.
        vertice_i = G->array_vertices[indice_i];
        // Obtenemos el índice del vecino número j del vértice anterior.
        u32 ind_j = vertice_i.vecinos_v[j];
        // Obtenemos el vértice que se corresponde con el índice anterior, y
        // por ende con el vertice vecino j.
        vertice_j = G->array_vertices[ind_j];
        // Finalmente obtenemos el color del vértice anterior.
        return vertice_j.infoVertice[x];
    }
}

/**
 * @fun: ColorJotaesimoVecino(Grafo G, u32 i, u32 j)
 * @param G: una estructura Grafo.
 * @param i: orden de un vértice.
 * @param j: número de vecino de un vértice.
 * @return: devuelve el color del vécino numero j del vértice en el orden i
 * guardado en ese momento en G si no nulo; 0 en caso contrario o que j sea
 * mayor al grado del vértice en el orden i.
 */
u32 ColorJotaesimoVecino(Grafo G, u32 i, u32 j) {
        return XJotaesimoVecino(Color, G, i, j);
}

/**
 * @fun: NombreJotaesimoVecino(Grafo G, u32 i, u32 j)
 * @param G: una estructura Grafo.
 * @param i: un orden de un vértice.
 * @param j: un número de vecino de un vértice.
 * @return: devuelve el nombre del vécino numero j del vértice en el orden i
 * guardado en ese momento en G si no nulo; 0 en caso contrario o que j sea
 * mayor al grado del vértice en el orden i.
 */
u32 NombreJotaesimoVecino(Grafo G, u32 i, u32 j) {
    return XJotaesimoVecino(Nombre, G, i, j);
}
