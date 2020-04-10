//
// Created by pablonr on 05/04/19.
//

#ifndef MATDISCRETA2019_TESTMAIN_H
#define MATDISCRETA2019_TESTMAIN_H

#include "Rii.h"

void test_InfoGrafo(Grafo grafo);

void test_Ordenaciones(Grafo grafo);

char test_Bipartito(Grafo grafo);

char test_UnOrdenUnGreedy(Grafo grafo);

char test_200WelshPowell(Grafo grafo, u32 semilla);

char test_500SwitchVertices(Grafo grafo, u32 semilla);

char test_RMBCs(Grafo grafo, u32 num_iteraciones, u32 semilla);

#endif //MATDISCRETA2019_TESTMAIN_H
