#include <stdlib.h>
#include <string.h>
#include "Hash.h"
#include "Lista.h"
#include <stdio.h>

#define TAM_INICIAL 17

// Struct de los nodos del hash
typedef struct nodo_hash {
	char *clave;
	void *valor;
} nodo_hash_t;

struct hash {
	lista_t** tabla;
	size_t cantidad;
	size_t tamanio;
	hash_destruir_dato_t destruir_dato;
};

struct hash_iter{
	const hash_t *hash; 
	size_t pos_vect;
	lista_iter_t *iter_lista;
};

/******************************************************************************
 *                          Funciones Auxiliares     
 *****************************************************************************/

// Crea un nuevo nodo del hash
nodo_hash_t* nodo_hash_crear(char *clave, void *dato) {
	nodo_hash_t* nodo_hash = malloc(sizeof(nodo_hash_t));
	if (!nodo_hash) return NULL;
	nodo_hash->clave = clave;
	nodo_hash->valor = dato;
	return nodo_hash;
}

// Devuelve el factor de carga del hash
float factor_de_carga(hash_t *hash) {
	return ((float) (hash->cantidad / hash->tamanio));
}

// Función de hashing
unsigned long int fhash(const char* clave, size_t tam) {
	unsigned long int hash = 0;
	unsigned long int i = 0;
		
	while (clave[i] != '\0') { 
    // Hash de tipo "Rotating/XOR" (intercambia posiciones entre bits y 
	// verifica los que se repiten con el tam.)
		hash = (hash << 2) ^ (hash >> 14) ^ (unsigned long int) clave[i];
		i++;
	}
	return hash%tam;
}

// Devuelve el nodo si la clave existe en la lista hash->tabla[pos_vect], NULL 
// si no.
nodo_hash_t* nodo_en_lista(const hash_t *hash, const char *clave, 
                           size_t *pos_vect) {
	lista_iter_t* iter = lista_iter_crear(hash->tabla[*pos_vect]);
	nodo_hash_t* nodo;
	bool pertenece = false;
	while (!lista_iter_al_final(iter)) {
		nodo = lista_iter_ver_actual(iter);
		// Si la clave del nodo es igual a la pasada por parámetro termino 
        // el ciclo
		if (strcmp(nodo->clave, clave) == 0){ 
			pertenece = true;
			break;
		}
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	if (pertenece) return nodo;
	return NULL;
}

// Reemplaza el dato de una clave del hash por otro dato pasado
// como parámetro. En caso de no encontrar la clave, devuelve false.
bool hash_reemplazar(hash_t *hash, const char *clave, void *dato) {
	size_t pos_vect = fhash(clave, hash->tamanio);
	nodo_hash_t* nodo = nodo_en_lista(hash, clave, &pos_vect);
	if (!nodo) return false;
	if (hash->destruir_dato) hash->destruir_dato(nodo->valor);
	nodo->valor = dato;
	return true;
}

// Redimensiona la tabla Hash
bool hash_redimensionar(hash_t* hash) {
	// Elijo un nuevo tamaño igual a 15 veces el tamaño anterior
	size_t nuevo_tamanio = hash->tamanio * 10;
	lista_t** nueva_tabla = calloc(nuevo_tamanio, sizeof(lista_t*));
	if (!nueva_tabla) return false;
	
	// Creo una nueva lista por cada posición de la nueva tabla
	for (unsigned int i = 0; i < nuevo_tamanio; i++) {
		nueva_tabla[i] = lista_crear();
	}
	// Saco los nodos del hash anterior, los vuelvo a hashear y los inserto
	// en el nuevo hash
	for (unsigned int i = 0; i < hash->tamanio; i++){
		while (!lista_esta_vacia(hash->tabla[i])){
			nodo_hash_t* nodo = lista_borrar_primero(hash->tabla[i]);
			size_t pos_vect = fhash(nodo->clave, nuevo_tamanio);
			lista_insertar_primero(nueva_tabla[pos_vect], nodo);
		}
		// Destruyo las listas del hash anterior
		lista_destruir(hash->tabla[i], NULL);
	}
	
	free(hash->tabla);
	hash->tabla = nueva_tabla;
	hash->tamanio = nuevo_tamanio;
	return true;
}

/******************************************************************************
 *                          Funciones del Hash       
 *****************************************************************************/

// Crea el hash
hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
	hash_t* hash = malloc(sizeof(hash_t));
	if (!hash) return NULL;
	
	// Genero una tabla inicializada con ceros
	lista_t** tabla = calloc(TAM_INICIAL, sizeof(lista_t*));
	if (!tabla) {
		free(hash);
		return NULL;
	}
	
	// Genero una lista por cada posición de la tabla
	for (unsigned int i = 0; i < TAM_INICIAL; i++){
		tabla[i] = lista_crear();
	}
	hash->tabla = tabla;
	hash->destruir_dato = destruir_dato;
	hash->cantidad = 0;
	hash->tamanio = TAM_INICIAL;
	return hash;
}

// Guarda un elemento en el hash, si la clave ya se encuentra en la
// estructura, la reemplaza. De no poder guardarlo devuelve false.
// Pre: La estructura hash fue inicializada
// Post: Se almacenó el par (clave, dato)
bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
	// Redimensiono el hash en caso de que el factor de carga sea >= 70%
	if (factor_de_carga(hash) >= 0.7) {
		if (!hash_redimensionar(hash)) return false;
	}
	// Obtengo la posición del vector donde guardar el nodo
	size_t pos_vect = fhash(clave, hash->tamanio);
	// Creo una copia de la clave en caso de que la modifiquen desde afuera
	char *clave_copia = strcpy(malloc(strlen(clave) + 1), clave);
	// Reemplazo en caso de que la clave pertenezca al hash
	if (hash_reemplazar(hash, clave_copia, dato)) {
		free(clave_copia);
		return true;
	}
	// Genero un nuevo nodo del hash
	nodo_hash_t* nodo = nodo_hash_crear(clave_copia, dato);
	if (!nodo) return false;
	
	// Inserto el nodo en la lista correspondiente
	lista_insertar_primero(hash->tabla[pos_vect], nodo);
	hash->cantidad++;
	return true;
}

// Borra un elemento del hash y devuelve el dato asociado.  Devuelve
// NULL si el dato no estaba.
// Pre: La estructura hash fue inicializada
// Post: El elemento fue borrado de la estructura y se lo devolvió,
// en el caso de que estuviera guardado.
void *hash_borrar(hash_t *hash, const char *clave) {
	size_t pos_vect = fhash(clave, hash->tamanio);
	void* dato = NULL;
	// Creo un iterador y recorro la lista que se encuentra en pos_vect
	// de la tabla hash
	lista_iter_t* iter = lista_iter_crear(hash->tabla[pos_vect]);
	while (!lista_iter_al_final(iter)) {
		nodo_hash_t* nodo_actual = lista_iter_ver_actual(iter);
		// Si existe la clave, borro el nodo de la lista y devuelvo el dato
		if (strcmp(nodo_actual->clave, clave) == 0) {
			lista_borrar(hash->tabla[pos_vect], iter);
			dato = nodo_actual->valor;
			free(nodo_actual->clave);
			free(nodo_actual);
			hash->cantidad--;
			break;
		}
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	return dato;
}

// Obtiene el valor de un elemento del hash, si la clave no se encuentra
// devuelve NULL.
// Pre: La estructura hash fue inicializada
void *hash_obtener(const hash_t *hash, const char *clave) {
	size_t pos_vect = fhash(clave, hash->tamanio);
	void* dato = NULL;
	nodo_hash_t* nodo = nodo_en_lista(hash, clave, &pos_vect);
	if (!nodo) return NULL;
	dato = nodo->valor;
	return dato;
}

// Determina si clave pertenece o no al hash.
// Pre: La estructura hash fue inicializada
bool hash_pertenece(const hash_t *hash, const char *clave) {
	size_t pos_vect = fhash(clave, hash->tamanio);
	nodo_hash_t* nodo = nodo_en_lista(hash, clave, &pos_vect);
	return nodo != NULL;
}

// Devuelve la cantidad de elementos del hash.
// Pre: La estructura hash fue inicializada
size_t hash_cantidad(const hash_t *hash) {
	return hash->cantidad;
}

// Destruye la estructura liberando la memoria pedida y llamando a la función
// destruir para cada par (clave, dato).
// Pre: La estructura hash fue inicializada
// Post: La estructura hash fue destruida
void hash_destruir(hash_t *hash) {
	size_t largo = hash->tamanio;
	for (unsigned int i = 0; i < largo; i++) {
		while (!lista_esta_vacia(hash->tabla[i])){
			nodo_hash_t* nodo = lista_borrar_primero(hash->tabla[i]);
			if (hash->destruir_dato != NULL)
				hash->destruir_dato(nodo->valor);
			free(nodo->clave);
			free(nodo);
		}
		lista_destruir(hash->tabla[i], NULL);
	}
	free(hash->tabla);
	free(hash);
}

/******************************************************************************
 *                    Funciones del Iterador del Hash 
 *****************************************************************************/

//Devuelve true o false segun si quedan o no listas para iterar. En caso
// de si quedar, modifica el valor de la posicion pasada por parametro.*/
bool posicion_en_tabla(const hash_t* hash, size_t* pos_en_tabla){
	size_t i = *pos_en_tabla;
	while (i != hash->tamanio){
		if (!lista_esta_vacia(hash->tabla[i])){
			*pos_en_tabla = i;
			return true;
		}
		i++;
	}
	return false;
}

/* Crea un iterador para el hash pasado. Si este es nulo devuelve NULL,
 * si no, chequea cual es la primera lista no vacia dentro de la tabla
 * de hash. En el caso de no haber, asigna el iterador de lista a NULL.
 * Si no, asigna a la lista correspondiente y devuelve el iterador del hash.*/
hash_iter_t *hash_iter_crear(const hash_t *hash){
	if (!hash) return NULL;
	hash_iter_t* hash_iter = malloc(sizeof(hash_iter_t));
	if (!hash_iter) return NULL;
	hash_iter->hash = hash;
	size_t pos_tabla = 0;
	if (!posicion_en_tabla(hash, &pos_tabla)) // No hay listas no vacias en la tabla de hash.
		hash_iter->iter_lista = NULL;
	else 
		hash_iter->iter_lista = lista_iter_crear(hash->tabla[pos_tabla]);
	hash_iter->pos_vect = pos_tabla;
	return hash_iter;
}

// Avanza el iterador de hash. Si el hash esta al final, se pasa un iterador 
// nulo, o la tabla de hash esta vacia, devuelve 'false'. Si no, avanza el 
// iterador de lista. Si estuviese al final, destruye este y crea uno nuevo, 
// para la proxima lista no vacia. Si no hay listas no vacias, devuelve false. 
// En otro caso, true.
bool hash_iter_avanzar(hash_iter_t *iter){
	if (!iter || !iter->iter_lista) return false;
	if (!hash_iter_al_final(iter)){
		lista_iter_avanzar(iter->iter_lista);
		if (!lista_iter_al_final(iter->iter_lista))
			return true;
		iter->pos_vect++; // Paso a la siguiente posicion en la tabla de hash.
		if (posicion_en_tabla(iter->hash, &iter->pos_vect)){
			lista_iter_destruir(iter->iter_lista);
			iter->iter_lista = lista_iter_crear(iter->hash->
                                                tabla[iter->pos_vect]);
			return true;
		}
	}
	return false; // No quedan listas por iterar, estaria al final de la tabla 
                  // de hash.
}

// Devuelve la clave del nodo actual. En caso de estar al final del iterador
// de hash, devuelve NULL.*/
const char *hash_iter_ver_actual(const hash_iter_t *iter){
	if (hash_iter_al_final(iter)) return NULL;
	nodo_hash_t* actual = lista_iter_ver_actual(iter->iter_lista);
	return actual->clave; // Sabiendo que el nodo 'actual' nunca va a ser NULL.
}

// Devuelve true o false segun si esta al final o no. Si el iterador pasado
// fuera NULL, devuelve true. Si no, verifica que no hayan mas listas no vacias
bool hash_iter_al_final(const hash_iter_t *iter){
	if (!iter) return true;
	size_t aux_pos_vect = iter->pos_vect;
	return !posicion_en_tabla(iter->hash, &aux_pos_vect);
}

// Destruye el iterador de hash (siendo este uno valido). En el caso que 
// el iterador fuera sobre un hash vacio, libera solo el iterador. Si no,
// tambien destruye el iterador de lista.*/
void hash_iter_destruir(hash_iter_t* iter){
	if (iter != NULL){
		if (iter->iter_lista != NULL)
			lista_iter_destruir(iter->iter_lista);
		free(iter);
	}
}
