/**
 * @file: Ordenación.h
 * @brief: Fichero con las funciones auxiliares de Ordenacion.c
 */

#ifndef Ordenacion_H
#define Ordenacion_H

#include "Rii.h"

// Tipo para diferenciar entre las funciones que debe tomar el qsort() en los
// diferentes algoritmos de orden.
enum ModoOrden {Normal, Invertido};

/*
 Estructura para el arreglo auxiliar que consta de dos campos indice y
 datoVertice. E general, el campo índice se usa para asociar cada vértice con
 el lugar que tiene en el arreglo de vértices del grafo, mientras que el campo
 datoVetirce lo usamos para guardar el dato que nos interese de cada vértice.
 */
typedef struct _Aux_Array_ {
    u32 indice;
    u32 datoVertice;
} Aux_Array;

// Definimos el tipo puntero a la estructura anterior.
typedef Aux_Array *Aux_Array_t;

/**
 * @fun: DescendenteGrado(const void * a, const void * b).
 * @param a: puntero a void.
 * @param b: puntero a void.
 * @return: devuelve 0 si los campos datoVertice de referenciados por a y b son
 * iguales; -1 si a > b, y -1 si b > a.
 */
int DescendenteDato(const void * a, const void * b);

/**
 * @fun: AscendenteNombre(const void * a, const void * b)
 * @param a: puntero a void.
 * @param b: puntero a void.
 * @return: devuelve 0 si los campos datoVertice de referenciados por a y b son
 * iguales; 1 si a > b, y -1 si b > a.
 */
int AscendenteDato(const void * a, const void * b);

/**
 * @fun: GenerarNuevoOrden(Grafo G, Aux_Array_t  *array).
 * @param G: una estructura grafo inicializada.
 * @param array: un arreglo de estructuras Aux_Array.
 * @brief: un procedimiento que copia en array_orden_vértices del
 * grafo un nuevo orden dado por array y libera la memoria ocupada por cada
 * estructura luego de la copia.
 */
void GenerarNuevoOrden(Grafo G, Aux_Array_t  *array);

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
        InfoVertice x);

/**
 * @fun: OrdenarSegunX(enum InfoVertice x, Grafo G);
 * @param x: la información de un vértice.
 * @param modo: el modo en que se va a ordenar.
 * @param G: una estructura Grafo correctamente creada.
 * @brief: es una función que modifica el orden que el grafo según el
 * parámetro x ingresado.
 * @return: retorna 1 si hubo errores; 0 en caso contrario.
 *
 */
char OrdenarSegunX(enum InfoVertice info, enum ModoOrden modo, Grafo G);

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
char GuardarBloqueColores(Grafo G, u32 cantidadColores, Aux_Array_t *array);

/**
 * @fun: LiberarBloqueColores(u32 cantColores, Aux_Array_t *bloques).
 * @param cantColores: cantidad de colores del grafo.
 * @param bloques: un arreglo de estructuras Aux_Array.
 * @brief: libera la memoria asignada a bloques.
 */
void LiberarBloqueColores(u32 cantColores, Aux_Array_t *bloques);

#endif