/**
 * @file: Coloreo.c
 * @brief: implmentación de las funciones para colorear el grafo.
 */

#include "Ordenacion.h"
#include "Rii.h"
#include "Lista.h"

/**
 * @fun: Bipartito(Grafo G).
 * @param G: un grafo correctamente construido.
 * @return: devuelve 1 si G es bipartito, 0 si no.
 */

int Bipartito(Grafo G) {
    // Número de vértices coloreados.
    u32 num_vertices_coloreados,
    // Puntero para señalar a un vértice sin colorear.
        *vert_sin_color = NULL, 
    // Puntero para señalar a los vecinos de un vértice.
        *vecino,
    // Puntero para el vértice que esta primero en la cola.
        *indice_primero;
    // Lista que será usada con semántica de cola.
        lista_t *lista;
    // Puntero a un vértice.
        VerticeSt *vertice;
    // Variable de control para el agregado de elementos en la cola.
    bool insertado;
    // Al comenzar tenemos 0 vertices coloreados.
    num_vertices_coloreados = 0;
    // Si G es NULL, no podemos aplicar el algoritmo.
    char error = false;
    if (G == NULL) return 0;
   // max se utiliza para decir q no tiene color un vertice.
    u32 max = G->infoGrafo[Vertices];
    if (G->infoGrafo[Colores] != 0) {
        for (u32 i = 0; i < G->infoGrafo[Vertices]; i++) {
            G->array_vertices[i].infoVertice[Color] = max;
        }
        // Establecemos que no hay colores en el grafo.
        G->infoGrafo[Colores] = 0;
    }
    // Creamos una nueva lista.
    lista = lista_crear();
    // Mientras no terminamos de colorear todos los vértices,
    // permanecemos en el ciclo.
    while (num_vertices_coloreados < G->infoGrafo[Vertices]) {
        // Se busca vértice no coloreado, recorriendo el arreglo de vértices.
        for (u32 i = 0; i < G->infoGrafo[Vertices]; i++) {
            // Si el vértice no está coloreado, entonces 
            //guardamos el indice de dicho vertice.
            if (G->array_vertices[i].infoVertice[Color] == max) {
                //Alocamos memoria el índice del vértice sin color.
                vert_sin_color = calloc(1, sizeof(u32));
                *vert_sin_color = i;
                break;
            }
        }
        // Le ponemos color 0 a dicho vértice.
        G->array_vertices[*vert_sin_color].infoVertice[Color] = 0;
        // Aumentamos la cantidad de vértices coloreados.
        num_vertices_coloreados++;
        // Agrego el índice del vértice a una cola para realizar el BFS.
        insertado = lista_insertar_ultimo(lista, vert_sin_color);
        // Si no se insertó a la cola es un error.
        if (insertado == false) {
            break;
        }
        // Mientras haya elementos en la cola...
        while (!lista_esta_vacia(lista)) {
            // Tomo el primer vértice de la cola y lo elimino de la misma.
            indice_primero = lista_borrar_primero(lista);
            // Señalo al vértice que saque de la cola.
            vertice = &(G->array_vertices[*indice_primero]);
            free(indice_primero);
            // Recorro los vecinos del vértice.
            for (u32 i = 0; i < vertice->infoVertice[Grado]; i++) { 
                // Alocamos memoria para el índice del vértice.
                vecino = calloc(1, sizeof(u32));
                *vecino = vertice->vecinos_v[i];     
                //Si el vecino del vértice no tiene color,lo agregamos a la cola
                if (G->array_vertices[*vecino].infoVertice[Color] == max) {
                    insertado = lista_insertar_ultimo(lista, vecino);
                    // Incrementamos la cantidad de vértices coloreados.
                    num_vertices_coloreados++;
                    // Le ponemos un color que este entre 0 y 1,
                    // según vertice->infoVertice[Color]
                    G->array_vertices[*vecino].infoVertice[Color] = 
                                                1 - vertice->infoVertice[Color];
                }
                else {
                    free(vecino);
                }
            }
            // Actualizamos la cantidad de colores.
            if (G->infoGrafo[Colores] < vertice->infoVertice[Color]) {
                G->infoGrafo[Colores] = vertice->infoVertice[Color];
            }
        }
    }
    // Si ya terminamos de colorear destruimos la lista.
   	lista_destruir(lista, free);
    // Verificamos si hay dos vértices que sean vecinos y tengan el mismo color.
    for (u32 i = 0; i < G->infoGrafo[Vertices]; i++) {
        vertice = &(G->array_vertices[i]);
        for (u32 j = 0; j < vertice->infoVertice[Grado]; j++) {
            // No es bipartito si algún vecino tiene el mismo color que vertice.
            u32 vertice_vecino =
                    G->array_vertices[vertice->vecinos_v[j]].infoVertice[Color];
            if (vertice->infoVertice[Color] == vertice_vecino) {
                error = OrdenWelshPowell(G);
                if (error) return 0;
                Greedy(G);
				        return 0;
            }
        }
    }
    // En caso contrario es bipartito.
    return 1;
}

/**
 * @fun: Greedy(Grafo G).
 * @param G: un grafo correctamente construido.
 * @return: devuelve el número de colores que se obtiene al correr Greedy en
 * algún orden.
 */
u32 Greedy(Grafo G) {
    // Arreglo para indicar los colores usados. Con calloc lo inicializamos
    // en false para todas sus posiciones. La posicion i del arreglo
    // representará el color i; si fue usado entonces tendra un true y si no
    // lo fue, un false.
    // Por el teorema sabemos que Greedy en cualquier orden colorea con a lo
    // sumo delta grande + 1 colores. Asi que nunca podrán usarse mas de
    // estos colores.
    bool *colores_usados = calloc(G->infoGrafo[DeltaGrande]+1, sizeof(bool));
    // Seteamos todos los colores de los vértices con UINT_MAX, el
    // máximo valor para una variable u32, lo cual indicará la ausencia de
    // color. Usamos esto porque el 0 puede ser un color y porque asumimos
    // que para que un grafo tenga el color UINT_MAX este sería demasiado
    // grande e inmanejable.
    u32 numVertices = NumeroDeVertices(G);
    for (u32 i = 0; i < numVertices; i++) {
        G->array_vertices[i].infoVertice[Color] = UINT_MAX;
    }
    // Seteamos la cantidad de colores usados en el grafo para volver a
    // contabilizar en este nuevo coloreo.
    memset(G->array_cantidad_colores,0,(G->infoGrafo[DeltaGrande]+1)*sizeof
            (u32));

    // Coloreamos el primer vértice en el orden dado con 0.
    G->array_vertices[G->array_orden_vertices[0]].infoVertice[Color] = 0;
    // Aumentamos la cantidad de colores del grafo a 1 y en el arreglo de
    // colores usados.
    G->infoGrafo[Colores] = 1;
    G->array_cantidad_colores[0]++;
    // Ahora iteramos sobre todos los vértices restantes...
    // Para cada vértice numero i en el orden de ese momento iteramos sobre los
    // vecinos j del  mismo...
    u32 grado_i, color_i, indice_i, color_vecino_j, max_color_vecino;
    for (u32 i = 1; i < numVertices; i++) {
        // Variable para contabilizar el máximo color de un vecino.
        max_color_vecino = 0;
        // Obtenemos el grado del vértice numero i en el orden dado.
        grado_i = GradoDelVertice(G, i);
        // Para cada vecino del vertice...
        for (u32 j = 0; j < grado_i; j++) {
            // Obtenemos el color del vecino numero j del vertice número i.
            color_vecino_j = ColorJotaesimoVecino(G, i, j);
            // Si fue coloreado entonces...
            if (color_vecino_j < UINT_MAX) {
                // Marcamos en el arreglo de colores usados el color del
                // vecino j como usado.
                colores_usados[color_vecino_j] = 1;
                // Verificamos si es el maximoco color de un vecino.
                if (color_vecino_j >= max_color_vecino) {
                    max_color_vecino = color_vecino_j;
                }
            }
        }
        // Ahora vemos el color que tendrá el vértice i..
        color_i = 0;
        // Iteramos sobre el arreglo de colores usados. Si el color en la
        // posiicion i fue usado seguimos buscando en la posicion inmediata.
        // Si encontramos un color que no fue usado salimos del ciclo.
        while (colores_usados[color_i] == 1) {
            color_i++;
        }
        // Al llegar aquí tenemos el color del vértice i, así que se lo
        // asignamos y además contabilizamos este color en el arreglo de
        // cantidad de colores del grafo...
        indice_i = G->array_orden_vertices[i];
        G->array_vertices[indice_i].infoVertice[Color] = color_i;
        G->array_cantidad_colores[color_i]++;
        // Si el color que acaba de tomar el vértice i + 1 (pues al ser el
        // cero un color debe sumarse 1 para contar la cantidad e colores) es
        // mayor a la cantidad de colores que tenía el grafo entonces
        // aumentamos la cantidad de colores del grafo.
        if ((color_i+1) > G->infoGrafo[Colores]) {
            G->infoGrafo[Colores] = (color_i+1);
        }
        // Luego seteamos el arreglo de colores usados para el próximo
        // vértice; seteamos solo las posiciones que fueron usadas...
        memset(colores_usados,0,max_color_vecino+1);
    }
    // Finalmente liberamos memoria...
    free(colores_usados);
    // Y devolvemos el número de colores que se obtuvo...
    return NumeroDeColores(G);
}
