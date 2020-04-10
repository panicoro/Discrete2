//
// Created by pablonr on 30/03/19.
//

#ifndef TESTORDENACION_H
#define TESTORDENACION_H

#include "Ordenacion.h"

typedef char (*funcionesOrden)(Grafo);

void test_OrdenNatural(Grafo Grafo);

void test_WelshPowell(Grafo grafo);

void test_RMBCNormal(Grafo grafo);

void test_RMBCRevierte(Grafo grafo);

void test_RMBCchicogrande(Grafo grafo);

void test_SwitchVertices(Grafo grafo);

void test_SwitchColores(Grafo grafo);

#endif // TESTORDENACION_H
