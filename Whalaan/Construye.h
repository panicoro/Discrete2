/**
 * @file: Construye.h
 * @brief: fichero de las funciones auxiliares usadas en usadas en Construye.c
 */

#ifndef CONSTRUYE_H
#define CONSTRUYE_H

#include "Lista.h"
#include "Hash.h"
#include "Ordenacion.h"
#include "U32ToString.h"
#include "Rii.h"

/**
 * @fun: CargarVecinos(hash_t* hashTable, Grafo grafo, u32 grados[],
 *                      u32 vertice1, u32 vertice2).
 * @param hashTable: una tabla hash cargada con (clave, dato) donde clave es
 * el nombre del vértice y dato es índice del vértice en el arreglo de la
 * Estructura grafo.
 * @param grafo: una estructura Grafo ya inicializada con los datos del
 * grafo y los vértices del mismo junto a su grado y sus arreglos de vecinos.
 * @param grados: un arreglo de los grados de los vértices de la estructura
 * grafo. La posición i del arreglo indica el grado del vértice en la posición i
 * del arreglo de vértices de grafo.
 * @param vertice1: nombre de un vértice.
 * @param vertice2: nombre de un vértice.
 * @brief: es una función que agrega en cada vértice el índice (del
 * arreglo de vértices del grafo) de uno de sus vecinos en su arreglo de
 * vecinos.
 * @return: false si no hubo errores, true en caso contrario.
 */
bool CargarVecinos(hash_t* hashTable, Grafo grafo, u32 grados[], u32 vertice1,
                   u32 vertice2);

/**
 * @fun: InicializarArregloVecinos(Grafo grafo, u32 array_grados[]).
 * @param grafo: una estructura Grafo con sus vértices y los grados de cada
 * uno cargados en ella.
 * @param array_grados: un arreglo del tamaño de los vértices existentes en
 * el grafo.
 * @brief: la función registra el grado del vértice "i" en el array de vértices
 * en lA posición "i" de array_grados y aloca memoria para el arreglo vecinos_v
 * de cada vértice del grafo.
 * @return: true si hubo algún error, false si no lo hubo.
 */
bool InicializarArregloVecinos(Grafo grafo, u32 array_grados[]);

/**
 * @fun: CalcularDeltaGrande(Grafo grafo)
 * @param grafo: un grafo con todos sus vecinos cargados.
 * @brief: calcula la delta grande del grafo, es decir, el grado máximo de
 * un vértice en el grafo, y guarda esta informacion en el grafo.
 */
void CalcularDeltaGrande(Grafo grafo);

/**
 * @fun: InicializarGrafo(Grafo grafo).
 * @param grafo: estructura grafo sin un orden y sin nungún coloreo.
 * @brief: inicializa la estructura grafo con el orden de vértices
 * [0..(numero_vertices-1)]. Luego con el orden dado se corre Greedy.
 * @return: devuelve true si el número de colores es cero; false en caso
 * contrario.
 */
bool InicializarGrafo(Grafo grafo);


/**
 * @fun: bool AgregaVecinos(Grafo G, datos_t carga, hash_t *hash).
 * @param G: estructura Grafo con los vértices cargados.
 * @param carga: estructura datos_t.
 * @param hash: estructura hash_t (tabla hash).
 * @return: devuelve false si los vecinos fueron agregador correctamente; true
 *          si ocurrió un error.
 */
bool AgregarVecinos(Grafo G, datos_t carga, hash_t *hash);

/**
 * @fun: AgregarVertice(hash_t *HashTable, Grafo grafo, u32 *VerticesIngresados,
                        u32 vertice).
 * @param HashTable: una tabla hash creada.
 * @param grafo: una estructura grafo cuyos vértices no han sido cargados.
 * @param VerticesIngresados: cantidad de vértices ingresados hasta el momento.
 * @param vertice: nombre del vértice a agregar.
 * @return: Devuelve false si el vértice se agregó o contabilizó su grado
 * correctamente; true, en caso de error.
 */
bool AgregarVertice(hash_t *HashTable, Grafo grafo, u32 *VerticesIngresados,
                        u32 vertice);

/**
 * @fun: FaltanVertices(u32 verticesTotales, u32 verticesIngresados).
 * @param verticesTotales: cantidad de vértices que tiene el grafo.
 * @param verticesIngresados: cantidad de vértices que se han cargado hasta
 * el momento en el grafo.
 * @return: true si no se han ingresado todos los vértices; false, en caso
 * contrario.
 */
bool FaltanVertices(u32 verticesTotales, u32 verticesIngresados);

/**
 * @fun: HayVecinosRepetidos(Grafo grafo).
 * @param grafo: una estructura grafo con todos sus campos completos.
 * @return true si algun vértice tiene un vecino repetido en sus arreglo de
 * vecinos, false en caso contrario.
 */
bool HayVecinosRepetidos(Grafo grafo);

/**
 * @fun: contarRepetidos(u32 array[], u32 size).
 * @param array: arreglo de u32.
 * @param size: tamaño del arreglo de u32.
 * @return: la cantidad (u32) de datos repetidos en el arreglo.
 */
u32 ContarRepetidos(u32 array[], u32 size);

/**
 * @fun: ascendente(const void * a, const void * b).
 * @param a: puntero a void.
 * @param b: puntero a void.
 * @return: devuelve 0 referenciados por a y b son iguales; 1 si a es mayor a
 * b, y -1 si b > a.
 */
int ascendente(const void *a, const void *b);

#endif // CONSTRUYE_H
