//
// Created by pablonr on 05/04/19.
//

#include "TestMain.h"
#include "TestOrdenacion.h"
#include <time.h>

char test_ColoreoPropio(Grafo grafo) {
    u32 color_vertice_i, grado_vertice_i, color_jotaesimo_vecino;
    for (u32 i = 0; i < grafo->infoGrafo[Vertices]; ++i ) {
        color_vertice_i = ColorDelVertice(grafo, i);
        grado_vertice_i = GradoDelVertice(grafo, i);
        for (u32 j = 0; j < grado_vertice_i; j++) {
            color_jotaesimo_vecino = ColorJotaesimoVecino(grafo, i, j);
            if (color_vertice_i == color_jotaesimo_vecino) {
                printf("Este no es un Coloreo Propio\n");
                return 1;
            }
        }
    }
    return 0;
}

void test_InfoGrafo(Grafo grafo) {
    u32 numVertices = NumeroDeVertices(grafo);
    printf("El número de vértices del grafo es: %u\n", numVertices);
    u32 numLados = NumeroDeLados(grafo);
    printf("El número de lados del grafo es: %u\n", numLados);
    u32 numColores = NumeroDeColores(grafo);
    printf("El número de Colores del grafo es: %u\n", numColores);
}

void test_Ordenaciones(Grafo grafo) {
    test_OrdenNatural(grafo);
    test_WelshPowell(grafo);
    test_RMBCNormal(grafo);
    test_RMBCRevierte(grafo);
    test_RMBCchicogrande(grafo);
    test_SwitchVertices(grafo);
    test_SwitchColores(grafo);
}

char test_Bipartito(Grafo grafo) {
    Grafo grafo_copia;
    int esBipartito;
    grafo_copia = CopiarGrafo(grafo);
    // Comprueba si es bipartito ,luego imprime algo.
    esBipartito = Bipartito(grafo_copia);
    if (esBipartito) {
        // Bipartito no necesita orden,por eso se ejecuta 1ro.
        printf("Grafo Bipartito\n\n");
        // Se Destruye por q la comprobacion fue true.
        DestruccionDelGrafo(grafo_copia);
        return 1;
    } else {
        printf("Grafo No Bipartito...\n\n");
        return 0;
    }
}

funcionesOrden funcOrdenes[5] = {OrdenNatural, OrdenWelshPowell, RMBCnormal,
                             RMBCrevierte, RMBCchicogrande};
char *nombreOrden[5] = {"Orden Natural", "Orden Welsh Powell", "RMBCnormal",
                          "RMBCrevierte", "RMBCchicogrande"};

char test_UnOrdenUnGreedy(Grafo grafo) {
    u32 cantColores;
    char error;
    Grafo grafo_copia;
    printf("*** Test 1 Orden - 1 Greedy sobre el grafo orginal\n");
    for (u32 i = 0; i < 5; i++) {
        grafo_copia = CopiarGrafo(grafo);
        error = funcOrdenes[i](grafo_copia);
        if (error) {
            printf("✖ Error al llamar a la función %s\n", nombreOrden[i]);
            return 1;
        }
        cantColores = Greedy(grafo_copia);
        error = test_ColoreoPropio(grafo);
        if (error) return 1;
        printf("\t✔ Usando %s, el coloreo obtenido fue = %u\n",
               nombreOrden[i], cantColores);
        DestruccionDelGrafo(grafo_copia);
    }
    return 0;
}

char test_200WelshPowell(Grafo grafo, u32 semilla) {
    char error;
    u32 mejor_coloreo = UINT_MAX - 1;
    u32 colores, indice;
    Grafo grafo_copia = CopiarGrafo(grafo);
    srand((u32)semilla);
    printf("*** Test 200 Órdenes Welsh Powell para obtener el mejor coloreo\n");
    for (u32 i = 0; i < 200; ++i) {
        indice = (u32)(2 + rand() % 3);
        error = OrdenWelshPowell(grafo);
        if (error) return 1;
        colores = Greedy(grafo);
        if (colores == 0) return 1;
        if (colores < mejor_coloreo) {
            mejor_coloreo = colores;
        }
        error = funcOrdenes[indice](grafo);
        if (error) return 1;
    }
    printf("\t ✔ El mejor coloreo con 200 Welsh Powell es %d\n", mejor_coloreo);
    DestruccionDelGrafo(grafo_copia);
    return 0;
}

char test_500SwitchVertices(Grafo grafo, u32 semilla) {
    char error;
    u32 mejor_coloreo = UINT_MAX - 1;
    u32 colores, vertice_i, vertice_j, num_vertices;
    num_vertices = grafo->infoGrafo[Vertices];
    Grafo grafo_copia = CopiarGrafo(grafo);
    printf("*** Test 500 Switch Vertices para obtener el mejor coloreo\n");
    srand(semilla);
    for (u32 i = 0; i < 500; ++i) {
        vertice_i = rand() % num_vertices;
        vertice_j = rand() % num_vertices;
        error = SwitchVertices(grafo, vertice_i, vertice_j);
        if (error) return 1;
        colores = Greedy(grafo);
        if (colores == 0) return 1;
        if (colores < mejor_coloreo) {
            mejor_coloreo = colores;
        }
    }
    printf("\t ✔ El mejor coloreo con 500 SwitchVertices aleatorios es %d\n",
           mejor_coloreo);
    DestruccionDelGrafo(grafo_copia);
    return 0;
}

char test_RMBCs(Grafo grafo, u32 num_iteraciones, u32 semilla) {
    char error;
    Grafo grafo_copia;
    u32 choise, num_colores, mejorColoreo, coloreoActual, coloreoAnterior;
    printf("*** Test usando RMBCs aleatoriamente - RMBCNormal se usa con "
           "SwitchColores\n");
    grafo_copia = CopiarGrafo(grafo);
    srand(semilla);
    mejorColoreo = UINT_MAX - 1;
    coloreoAnterior = UINT_MAX - 1;
    for (u32 j = 0; j < num_iteraciones; j++) {
        choise = (u32) (2 + rand() % 3);
        num_colores = NumeroDeColores(grafo_copia);
        // Teorema y advertencia del profe:
        // Durante cualquiera de los RMBCs o SwitchColores seguido de algún
        // RMBC, la cantidad de colores aumenta respecto de la cantidad de
        // colores que tenia antes de ese reordenamiento.
        // En clase demostramos que esto no puede pasar.
        // * Teorema en clase *:

        // Seleccionamos las opciones para colorear segun el número aleatorio
        if (choise == 2) {
            u32 color_i = rand() % num_colores;
            u32 color_j = rand() % num_colores;
            error = SwitchColores(grafo_copia, color_i, color_j);
            if (error) {
                printf("\t ✖ Error al llamar a la función SwitchVertices en la "
                       "iteracion %u\n", j + 1);
                return 1;
            }
            error = funcOrdenes[choise](grafo_copia);
            if (error) {
                printf("\t ✖ Error al llamar a la función %s en la "
                       "iteracion %u\n", nombreOrden[choise], j + 1);
                return 1;
            }
        } else {
            error = funcOrdenes[choise](grafo_copia);
            if (error) {
                printf("\t ✖ Error al llamar a la función %s en la "
                       "iteracion %u\n", nombreOrden[choise], j + 1);
                return 1;
            }
        }
        // Luego de ordenar segun haya tocado, se elige el coloreo actual...
        coloreoActual = Greedy(grafo_copia);
        // Si el coloreo actual es mayor que el anterior...
        if (coloreoActual > coloreoAnterior) {
            // Esto es un error...
            printf("\t ✖ Error se obtiene un coloreo mayor al anterior en "
                   "una de las permutaciones de colores\n");
            return 1;
        }
        // Si no hubo errores actualizamos el mejor coloreo si corresponde.
        if (coloreoActual < mejorColoreo) {
            mejorColoreo = coloreoActual;
        }
        // Actualizamos el coloreo anterior para la próximo iteración.
        coloreoAnterior = coloreoActual;
    }
    // Imprimimos el mejor coloreo al finalizar...
    printf("\t ✔ El mejor coloreo con %u iteraciones usando RMBCs es %u\n",
           num_iteraciones, mejorColoreo);
    DestruccionDelGrafo(grafo_copia);
    return 0;
}
