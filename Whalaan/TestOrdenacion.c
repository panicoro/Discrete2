//
// Testeo de funciones de ordenación.
//

#include "TestOrdenacion.h"
#include <time.h>

char mayor(u32 a, u32 b) {
    return a >= b;
}

char menor(u32 a, u32 b) {
    return a <= b;
}

typedef char (*funcionesComparacion)(u32, u32);
funcionesComparacion compares[2] = {menor, mayor};


funcionesOrden ordenes[4] = {OrdenNatural, OrdenWelshPowell, RMBCnormal,
                             RMBCrevierte};

/**
 * @fun: char test_OrdenSegunX(funcionesOrden function, enum InfoVertice x,
 *       Grafo grafo)
 * @param function: una funcion de ordenación a testear.
 * @param x: parametro en base al que ordena la función a testear.
 * @param grafo: un grafo correctamente construido.
 * @return
 */
char test_OrdenSegunX(funcionesOrden function, enum InfoVertice x, enum
        ModoOrden o, Grafo grafo) {
    // Llamamos a la funcion a testear.
    char error = function(grafo);
    if (error) {
        printf("✖ Falla por error de llamada específica de la función\n");
        return 1;
    }
    else {
        u32 vertice_actual, vertice_sucesor;
        u32 dato_actual, dato_sucesor;
        for (u32 i =0; i < (grafo->infoGrafo[Vertices]-1); ++i){
            vertice_actual= grafo->array_orden_vertices[i];
            vertice_sucesor = grafo->array_orden_vertices[i+1];
            dato_actual = grafo->array_vertices[vertice_actual].infoVertice[x];
            dato_sucesor = grafo->array_vertices[vertice_sucesor]
                    .infoVertice[x];
            //printf("%u ", dato_actual);
            error = compares[((x%2)+o)](dato_actual,dato_sucesor);
            if (!error) {
                printf("✖ Falla por no haber ordenado según se esperaba\n");
                return 1;
            }

        }
    }
    return 0;
}

void test_OrdenNatural(Grafo grafo) {
    char error = test_OrdenSegunX(ordenes[Nombre], Nombre, Normal, grafo);
    if (error) {
        printf("✖ Error - Orden Natural\n");
    }
    else {
        printf("✔ OK - Orden Natural\n");
    }
}

void test_WelshPowell(Grafo grafo) {
    char error = test_OrdenSegunX(ordenes[Grado], Grado, Normal, grafo);
    char check_delta = (grafo->infoGrafo[DeltaGrande] !=
                        grafo->array_vertices[grafo->array_orden_vertices[0]]
                                .infoVertice[Grado]);
    if (error || check_delta) {
        printf("✖ Error - Orden Welsh-Powell\n");
    }
    else {
        printf("✔ OK - Orden Welsh-Powell\n");
    }
}

void test_RMBCNormal(Grafo grafo) {
    char error = test_OrdenSegunX(ordenes[2], Color, Normal, grafo);
    if (error) {
        printf("✖ Error - RMBCNormal\n");
    }
    else {
        printf("✔ OK - RMBCNormal\n");
    }
}

void test_RMBCRevierte(Grafo grafo) {
    char error = test_OrdenSegunX(ordenes[3], Color, Invertido, grafo);
    if (error) {
        printf("✖ Error - RMBCRevierte\n");
    }
    else {
        printf("✔ OK - RMBCRevierte\n");
    }
}

char check_BloqueColores(Grafo G, u32 numColores, Aux_Array_t* bloques) {
    u32 vertice_i, color_j, contador_color_j, indice_i;
    vertice_i = contador_color_j = 0;
    for (u32 j = 0; j < numColores; j++) {
        contador_color_j += bloques[j]->datoVertice;
        color_j = bloques[j]->indice;
        for (u32 k = vertice_i; k < contador_color_j; k++) {
            indice_i = G->array_orden_vertices[k];
            if (G->array_vertices[indice_i].infoVertice[Color] != color_j) {
                return 1;

            }

        }
        vertice_i += contador_color_j;
    }
    return 0;
}

void test_RMBCchicogrande(Grafo G) {
    u32 numColores = G->infoGrafo[Colores];
    Aux_Array_t *bloqueColores = malloc(numColores*sizeof(Aux_Array));
    GuardarBloqueColores(G, numColores, bloqueColores);
    qsort(bloqueColores, numColores, sizeof(Aux_Array_t), AscendenteDato);
    char error = RMBCchicogrande(G);
    if (error) {
        printf("✖ Falla por error de llamada específica de la función\n");
    }
    error = check_BloqueColores(G, numColores, bloqueColores);
    if (error) {
        printf("✖ Error - RMBCchicogrande\n");
    } else {
        printf("✔ OK - RMBCchicogrande\n");
    }
    LiberarBloqueColores(numColores,bloqueColores);
}

// Devuelve uno si no son iguales
char CompararVertices(VerticeSt vert_1, VerticeSt vert_2) {
    for (u32 i = Nombre; i <= Color; i++) {
        if (vert_1.infoVertice[i] != vert_2.infoVertice[i]) return 1;
    }
    for (u32 i = 0; i < vert_1.infoVertice[Grado]; ++i) {
        if (vert_1.vecinos_v[i] != vert_2.vecinos_v[i]) return 1;
    }
    return 0;
}

void test_SwitchVertices(Grafo G) {
    srand((u32) time(NULL));
    u32 pos_i, pos_j, numVertices, counter;
    VerticeSt vertice_i, vertice_j, new_vertice_i, new_vertice_j;
    char error1, error2;
    numVertices = G->infoGrafo[Vertices];
    counter = 0;
    for (u32 k = 0; k < 50; ++k) {
        pos_i = rand() % numVertices;
        pos_j = rand() % numVertices;
        vertice_i = G->array_vertices[G->array_orden_vertices[pos_i]];
        vertice_j = G->array_vertices[G->array_orden_vertices[pos_j]];
        error1 = SwitchVertices(G, pos_i, pos_j);
        if (error1) {
            printf("✖ Falla por error de llamada específica de la función "
                   "en la iteración %u\n", k+1);
        }
        new_vertice_i = G->array_vertices[G->array_orden_vertices[pos_i]];
        new_vertice_j = G->array_vertices[G->array_orden_vertices[pos_j]];
        // Despues del intercambio chequeo que el vertice que está en la posición
        // i sea el vértice j, y viceversa.
        error1 = CompararVertices(vertice_j, new_vertice_i);
        error2 = CompararVertices(vertice_i, new_vertice_j);
        if (error1 || error2) {
            printf("✖ Error - SwitchVertices en la iteración %u\n", k+1);
            break;
        } else {
            counter++;
        }
    }
    if (counter == 50) {
        printf("✔ OK - 50 Iteraciones de SwitchVertices\n");
    }
}

char CompararColores(Grafo G, u32 *array, u32 color1, u32 color2,
                     u32 size) {
    for (u32 i = 0; i < size; ++i) {
        if (array[i] == color1) {
            if (G->array_vertices[i].infoVertice[Color] != color2) return 1;
        } else if (array[i] == color2) {
            if (G->array_vertices[i].infoVertice[Color] != color1) return 1;
        } else {
            continue;
        }
    }
    return 0;
}

void GuardarColores(Grafo G, u32 *array, u32 size) {
    for (u32 i = 0; i < size; ++i) {
        array[i] = G->array_vertices[i].infoVertice[Color];
    }
}

void test_SwitchColores(Grafo G) {
    u32 numVertices = G->infoGrafo[Vertices];
    u32 numColores = G->infoGrafo[Colores];
    u32 *colores = calloc(numVertices, sizeof(u32));
    u32 color_i, color_j;
    char error;
    srand((u32) time(NULL));
    u32 counter = 0;
    for (u32 k = 0; k < 50; k++) {
        GuardarColores(G,colores,numVertices);
        color_i = rand() % numColores;
        color_j = rand() % numColores;
        error = SwitchColores(G, color_i, color_j);
        if (error) {
            printf("✖ Falla por error de llamada específica de la función "
                   "en la iteración %u\n", k+1);
            break;
        }
        error = CompararColores(G, colores, color_i, color_j, numVertices);
        if (error) {
            printf("✖ Error - Switch Colores en la iteración %u\n", k+1);
            break;
        } else {
            counter++;
        }
    }
    free(colores);
    if (counter == 50) {
        printf("✔ OK - 50 Iteraciones de SwitchColores\n");
    }
}
