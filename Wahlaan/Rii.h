/**
 * @file: Rii.h
 * @brief: Fichero que contiene todas las funciones pedidas por el proyecto.
 */

#ifndef LEANO_ROSA_RII_H
#define LEANO_ROSA_RII_H

//VERONICA LEAÑO  verito.cba933@gmail.com
//PABLO NICOLÁS ROSA panicoro770@gmail.com

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

// Redefinimos el tipo unsigned int como u32.
typedef unsigned int u32;

// Tipo de dato para referirnos a la información del grafo.
enum InfoGrafo { Vertices, Lados, Colores, DeltaGrande};

// Tipo de dato para referirnos a la información del vértice.
enum InfoVertice {Nombre, Grado, Color};

// Estructura que implementa cada vértice del grafo.
typedef struct _VerticeSt_ {
    // Arreglo que guarda la información del grafo de la siguiente forma:
    // infoVertice[0]: Nombre del vértice.
    // infoVertice[1]: Grado del vértice.
    // infoVertice[2]: Color del vértice.
    u32 infoVertice[3];
    // Arreglo de vecinos del vértice.
    u32 *vecinos_v;
} VerticeSt;

// Estructura que implementa el grafo.
typedef struct _GrafoSt_ {
    // Arreglo que guarda la información del grafo de la siguiente forma:
    // infoGrafo[0]: Número de Vertices
    // infoGrafo[1]: Número de Lados
    // infoGrafo[2]: Número de Colores
    // infoGrafo[3]: DeltaGrande del grafo.
    u32 infoGrafo[4];
    // Arreglo de estructuras vértices
    VerticeSt *array_vertices;
    // Arreglo que da orden a los vértices
    u32 *array_orden_vertices;
    // Arreglo que cuenta la cantidad de colores usados.
    u32 *array_cantidad_colores;
} GrafoSt;

typedef GrafoSt *Grafo;

// Estructura que implementa la lectura del grafo desde los archivos.
struct _datos_t {
    // Campos para guardar la cantidad de vértices y lados.
    u32 cant_vertices;
    u32 cant_lados;
    // Arreglo para cargar los lados.
    u32 *arreglo_lados;
};

typedef struct _datos_t * datos_t;

/*****************************************************************************
 *                 FUNCIONES DE CONSTRUCCIÓN/DESTRUCCIÓN
 *                             DEL GRAFO
 ****************************************************************************/

//Realiza la carga de datos del archivo dimacs.
datos_t CargaDatos();

//Destruye la estructura de datos y libera la memoria alocada.
void DestruccionCargaDatos(datos_t datos);

//Construye el GrafoSt y aloca memoria.
Grafo ConstruccionDelGrafo();

//Destruye G y libera la memoria alocada.
void DestruccionDelGrafo(Grafo G);

//Copia todos los datos guardados en G.
Grafo CopiarGrafo(Grafo G);

/*****************************************************************************
 *                        FUNCIONES DE COLOREO
 ****************************************************************************/

//Devuelve el numero de colores que se obtiene.
u32 Greedy(Grafo G);

// Devuelve 1 si G es Bipartito(coloreado solo con 2 colores) en caso contrario 
// devuelve 0 si no es Bipartito.
int Bipartito(Grafo G);


/*****************************************************************************
*              FUNCIONES PARA EXTRAER INFORMACIÓN DEL GRAFO
 ****************************************************************************/

// Devuelve el número de vértices de G
u32 NumeroDeVertices(Grafo G);

// Devuelve el número de lados de G.
u32 NumeroDeLados(Grafo G);

// Devuelve la cantidad de colores usados en el coloreo que
// tiene en ese momento G
u32 NumeroDeColores(Grafo G);


/*****************************************************************************
 *      FUNCIONES PARA EXTRAER INFORMACIÓN DE LOS VÉRTICES DEL GRAFO
 ****************************************************************************/

// Devuelve el nombre real del vértice número i en el orden guardado en
// ese momento en G.
u32 NombreDelVertice(Grafo G , u32 i);

// Devuelve el color con el que está coloreado el vértice número i en el orden
// guardado en ese momento en G.
u32 ColorDelVertice(Grafo G, u32 i);

// Devuelve el grado del vértice número i en el orden guardado en ese momento
// en G.
u32 GradoDelVertice(Grafo G, u32 i);

// Devuelve el color del vécino numero j
// del vértice número i en el orden guardado en ese momento en G
u32 ColorJotaesimoVecino(Grafo G, u32 i, u32 j);

//Devuelve el nombre del vécino numero j 
// del vértice número i en el orden guardado en ese momento en G
u32 NombreJotaesimoVecino(Grafo G, u32 i, u32 j);


/*****************************************************************************
 *                      FUNCIONES DE ORDENACIÓN
 ****************************************************************************/

 //Ordena los vértices en orden creciente de sus “nombres” reales
 //retorna 0 si todo anduvo bien, 1 si hubo algún problema.
char OrdenNatural(Grafo G);

//Ordena los vertices de W de acuerdo con el orden Welsh-Powell, es decir, 
//con los grados en orden no creciente. 
//Retorna 0 si todo anduvo bien, 1 si hubo algún problema.
char OrdenWelshPowell(Grafo G);

//Verifica que i, j <número de vértices. Si no es cierto, retorna 1.
//Si ambos estan en el intervalo permitido,intercambia las posiciones de los 
//vertices en los lugares i y j  del orden interno de G y retorna 0.
char SwitchVertices(Grafo G, u32 i, u32 j);

//Si G esta coloreado con r colores y V C 1 son los vertices coloreados con 1,
// V C 2 los coloreados con 2, etc, entonces esta función ordena los vertices 
//poniendo primero los vertices de V C 1 , luego los de V C 2 , etc, 
//hasta V C r−1 .
char RMBCnormal(Grafo G);

//Si G esta coloreado con r colores y V C 1 son los vertices coloreados con 1,
// V C 2 los coloreados con 2, etc, entonces esta función ordena los vertices 
//poniendo primero los vertices de V C r−1 , luego los de V C r−2 ,
// luego los de V C r−3 , etc, hasta V C 1 .
char RMBCrevierte(Grafo G);

// Si G esta coloreado con r colores y V C 1 son los vertices coloreados con 1,
// V C 2 los coloreados con 2, etc, entonces esta función ordena los vertices
// poniendo primero los vertices de V C j 1 , luego los de V C j 2 , etc, 
// donde j 1 , j 2 , ..., j r son tales que |V C j 1 | ≤ |V C j 2 | ≤ ... ≤ 
// |V C j r| , Retorna 0 si todo anduvo bien, 1 si hubo algún problema.
char RMBCchicogrande(Grafo G);

// Verifica que i, j <número de colores que tiene G en ese momento.
// Si no es cierto, retorna 1. Si ambos estan en el intervalo permitido, 
//entonces intercambia los colores i, j. Retorna 0 si todo se hizo bien.
char SwitchColores(Grafo G, u32 i, u32 j);


#endif // LEANO_ROSA_Rii_h
