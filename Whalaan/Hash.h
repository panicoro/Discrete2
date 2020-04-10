#ifndef HASH_H
#define HASH_H

#include <stdbool.h>
#include <stddef.h>

/******************************************************************************
 *                   Definición de los tipos de Datos
 *****************************************************************************/ 

struct hash;
struct hash_iter;

typedef struct hash hash_t;
typedef struct hash_iter hash_iter_t;


/******************************************************************************
 *                      Primitivas de Tabla Hash
 *****************************************************************************/

// tipo de función para destruir dato
typedef void (*hash_destruir_dato_t)(void *);

//Crea el hash
hash_t *hash_crear(hash_destruir_dato_t destruir_dato);


//Guarda un elemento en el hash, si la clave ya se encuentra en la
// estructura, la reemplaza. De no poder guardarlo devuelve false.
// Pre: La estructura hash fue inicializada
// Post: Se almacenó el par (clave, dato)
bool hash_guardar(hash_t *hash, const char *clave, void *dato);


// Borra un elemento del hash y devuelve el dato asociado.  Devuelve
// NULL si el dato no estaba.
// Pre: La estructura hash fue inicializada
// Post: El elemento fue borrado de la estructura y se lo devolvió,
// en el caso de que estuviera guardado.
void *hash_borrar(hash_t *hash, const char *clave);


// Obtiene el valor de un elemento del hash, si la clave no se encuentra
// devuelve NULL.
// Pre: La estructura hash fue inicializada
void *hash_obtener(const hash_t *hash, const char *clave);


// Determina si clave pertenece o no al hash.
// Pre: La estructura hash fue inicializada
bool hash_pertenece(const hash_t *hash, const char *clave);


// Devuelve la cantidad de elementos del hash.
// Pre: La estructura hash fue inicializada
size_t hash_cantidad(const hash_t *hash);

// Destruye la estructura liberando la memoria pedida y llamando a la función
// destruir para cada par (clave, dato).
// Pre: La estructura hash fue inicializada
// Post: La estructura hash fue destruida
void hash_destruir(hash_t *hash);



/******************************************************************************
 *                      Funciones del Iterador del Hash 
 *****************************************************************************/

// Crea iterador
hash_iter_t *hash_iter_crear(const hash_t *hash);

// Avanza iterador
bool hash_iter_avanzar(hash_iter_t *iter);

// Devuelve clave actual, esa clave no se puede modificar ni liberar.
const char *hash_iter_ver_actual(const hash_iter_t *iter);

// Comprueba si terminó la iteración
bool hash_iter_al_final(const hash_iter_t *iter);

// Destruye iterador
void hash_iter_destruir(hash_iter_t* iter);

#endif // HASH_H
