#ifndef U32_TO_STRING_H
#define U32_TO_STRING_H
#include "Rii.h"

/**
* @fun: cadena_reversa(char *inicio, char *final)
* @param inicio: cadena a revertir.
* @param final: cadena revertida
* @brief: es un procedimiento que revierte una cadena.
*/
void cadena_reversa(char * inicio, char * final);



/**
* @fun: itoa_propia(unsigned int valor, char* cadena, int base).
* @param valor: un entero sin signo.
* @param cadena: un arreglo a char.
* @param base: un entero entre 2 y 32 que será la base para la conversión.
* @return convierte un valor entero sin signo en una cadena terminada en
* nulo usando la base especificada y almacena el resultado en la matriz dada
* por el parámetro cadena.
*/
char * itoa_propia(u32 valor, char *cadena, int base);

#endif // U32_TO_STRING_H
