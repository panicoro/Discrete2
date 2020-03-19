/**
 * @file U32ToString.c
 * @brief Implementación de la función itoa_propia().
 */

#include "Rii.h"

/**
 * @fun: cadena_reversa(char *inicio, char *final)
 * @param inicio: cadena a revertir.
 * @param final: cadena revertida
 * @brief: es un procedimiento que revierte una cadena.
 */
void cadena_reversa(char *inicio, char *final) {
    char aux;
    while(final > inicio)
        aux=*final, *final--=*inicio, *inicio++=aux;
}

/**
 * @fun: itoa_propia(unsigned int valor, char* cadena, int base).
 * @param valor: un entero sin signo.
 * @param cadena: un puntero a char con memoria alocada.
 * @param base: un entero entre 2 y 32 que será la base para la conversión.
 * @return convierte un valor entero sin signo en una cadena terminada en
 * nulo usando la base especificada y almacena el resultado en la matriz dada
 * por el parámetro cadena.
 */
char* itoa_propia(unsigned int valor, char* cadena, int base) {
    static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char *w_cadena = cadena;
    unsigned int cociente, resto;
   
	// Validar la base
    if (base < 2 || base > 35) {
        *w_cadena='\0';
        return NULL;
    }
    // Hacemos la conversion, pero el número queda al reves.
    do {
        cociente = valor / base;
		resto = valor % base; 
        *w_cadena++ = num[resto];
    } while ((valor = cociente));
   
    // Terminamos las cadena.
    *w_cadena = '\0';
    // Ahora la revertimos.
    cadena_reversa(cadena, w_cadena-1);
    // Devolvemos la cadena
    return cadena;
}
