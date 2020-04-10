#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stddef.h>

/* ****************************************************************************
 *                   Definición de los tipos de datos
 * ***************************************************************************/

/* La lista está planteada como una lista de punteros genéricos. */

typedef struct lista lista_t;

typedef struct lista_iter lista_iter_t;


/* ****************************************************************************
 *                  Primitivas básicas de la lista
 * ***************************************************************************/

// Crea una lista.
// Post: devuelve una nueva lista vacía.
lista_t *lista_crear(void);

// Pre: la lista fue creada.
// Post: Devuelve verdadero o falso, según si la lista tiene o no elementos.
bool lista_esta_vacia(const lista_t *lista);

// Inserta un nuevo elemento en la primera posición de la lista.
// Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se insertó un nuevo elemento al principio de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Inserta un nuevo elemento al final de la lista. Devuelve falso en
// caso de error. Funciona en tiempo constante.
// Pre: la lista fue creada.
// Post: se insertó un nuevo elemento al final de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Elimina el primer elemento de la lista. Si está vacía devuelve NULL, caso
// contrario devuelve el valor del elemento eliminado.
// Pre: la lista fue creada.
// Post: se eliminó el primer elemento de la lista.
void *lista_borrar_primero(lista_t *lista);

// Devuelve el primer elemento de la lista.
// Pre: la lista fue creada.
void *lista_ver_primero(const lista_t *lista);

// Devuelve la cantidad de elementos de la lista. Funciona en tiempo constante.
// Pre: la lista fue creada.
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *));


/* ****************************************************************************
 *                     Primitivas de Iteración
 *
 * Un iterador es un puntero que es utilizado por un algoritmo para recorrer
 * los elementos almacenados en un contenedor.
 *****************************************************************************/

// Recibe una lista como parámetro y crea un iterador de ella.
// Pre: la lista fue creada.
// Post: devuelve un iterador de lista.
lista_iter_t *lista_iter_crear(const lista_t *lista);

// Avanza una posición en la lista.
// Pre: el iterador fue creado.
// Post: se avanzó una posición en el iterador. Devuelve false si la
// posición era la última.
bool lista_iter_avanzar(lista_iter_t *iter);

// Devuelve un puntero al valor de la posición actual del iterador.
// Devuelve NULL si la lista está vacía.
// Pre: el iterador fue creado.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Devuelve true si el iterador se encuentra al final de la lista o si
// está vacía; false si no lo está.
// Pre: el iterador fue creado.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador.
// Pre: el iterador fue creado.
// Post: se destruyó el iterador.
void lista_iter_destruir(lista_iter_t *iter);


/* ****************************************************************************
 *              Primitivas de listas junto con Iterador
 * ***************************************************************************/ 

// Inserta un nuevo elemento a la lista en la posición donde se encuentre
// el iterador. Devuelve true si pudo realizarlo, false en caso contrario.
// Pre: la lista y el iterador fueron creados.
// Post: se insertó un nuevo elemento a la lista.
bool lista_insertar(lista_t *lista, lista_iter_t *iter, void *dato);

// Elimina el elemento de la lista al que apunte el iterador.
// Pre: la lista y el iterador fueron creados.
// Post: se eliminó el elemento de la lista.
void *lista_borrar(lista_t *lista, lista_iter_t *iter);

 
/* ******************************************************************
 *                 PRIMITIVAS DE ITERADOR INTERNO
 * *****************************************************************/ 

// La función de callback "visitar" recibe el dato y un puntero extra,
// y devuelve true si se debe seguir iterando, false en caso contrario.
// Pre: la lista fue creada.
void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra), void *extra);

 
/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

// Realiza pruebas sobre la implementación del alumno.
// void pruebas_lista_alumno(void);

#endif // LISTA_H
