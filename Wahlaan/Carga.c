/**
 * @file: Carga.c
 * @brief: Implementación de funciones necesarias para cargar los datos del
 *        grafo desde standard input.
 */

#include "Rii.h"

/**
 * @fun: VerticesIguales(u32 vertice_1, u32 vertice_2)
 * @param vertice_1: primer vértice a comparar.
 * @param vertice_2: segundo vértice a comparar.
 * @return devuele true si los dos vértices son iguales; false, en caso
 * contrario.
 */
bool VerticesIguales(u32 vertice_1, u32 vertice_2) {
    return vertice_1 == vertice_2;
}

/**
 * @fun: datos_t CargaDatos()
 * @brief: toma los datos ingresados por standard input, reconociendo el formato
 *        DIMACS propuesto.
 * @return: si no hubo errores (ya sea por fallas en alocar memoria o por
 * formato de entrada no válido), devuelve un puntero a una estructura con
 * la información (cantidad de lados, cantidad de vértices y lados)
 * recabada del grafo. Si hubo errores entonces devuelve NULL.
 */
datos_t CargaDatos() {
    // Alojamos memoria para la estructura que carga los datos
    datos_t carga = calloc(1,sizeof(struct _datos_t));
    // Si no se pudo alocar memoria entonces devolvemos NULL
    if (carga == NULL) return NULL;
    // Variable para almecenar una línea que se lee.
    // Como no tenemos un límite para leer una línea, usamos BUFSIZ.
    char linea[BUFSIZ];
    // Variables de comprobación.
    bool comp1, comp2;
    // Variables que representan un lado.
    u32 vertice_1, vertice_2;
    // Establecemos que el primer carácter de la línea sea un caracter 'c'
    // para poder entrar al ciclo que busca líneas que sean comentarios.
    // linea[0] = 'c';
    do {
        // Leemos lineas hasta que no sean mas comentarios
        // La función fgets() lee (BUFSIZ-1) caracteres desde stdin y lo
        // escribe en el array linea. Ningún carácter adicional es leído
        // después del carácter de nueva línea (el cual es retenido) o
        // después de un final de fichero (EOF). Un carácter nulo ('\0') es
        // escrito inmediatamente después del último carácter leído en el array.
        // Si un final de fichero (EOF) es encontrado y ningún carácter ha sido
        // leído en el array, entonces el contenido del array permanece
        // invariable y un puntero nulo es retornado. Si ocurre un error de
        // lectura durante el proceso, el contenido del array es indeterminado
        // y un puntero nulo es retornado.
        if (fgets(linea, BUFSIZ , stdin) == NULL) {
            // Si ocurrió un error liberamos la estructura y retornamos
            // NULL.
            free(carga);
            return NULL;
        }
    } while (linea[0] == 'c');
    // Si llegamos hasta aca hemos saltado todos los comentarios y no ha
    // ocurrido ningún error.
    // Punteros para señalar el inicio y el resto de la linea que leemos.
    char *inicio_cadena, *resto_cadena;
    // Comparamos los primeros 7 caracteres de la línea siguiente a los
    // comentarios con la función strncmp(), que devuelve un número entero
    // mayor, igual, o menor que cero, si lo que hay en linea es mayor,
    // igual o menor que "p edge ", respectivamente.
    int comparacion = strncmp(linea, "p edge ", 7);
    if (comparacion) {
        // Se libera la memoria reservada para carga en caso de error; es decir
        // no se encontró "p edge " después de los comentarios.
        printf("Error en la primera linea sin comentario\n");
        free(carga);
        return NULL;
    // Si da cero, entonces seguimos...
    } else {
        // Dividimos el string linea con la función strtok() que utiliza como
        // delimitador "P edge ".
        inicio_cadena = strtok(linea, "p edge ");
        // Convertimos la parte inicial de *inicio_cadena en un valor int largo
        // sin signo con la funcion strtoul() y le damos base 10. Entonces
        // obtenemos el 1er. numero que es la cantidad de vertices del grafo.
        // Lo guardamos en nuestra estructura como el número de vértices.
        carga->cant_vertices = (u32)strtoul(inicio_cadena, &resto_cadena, 10);
        // Notar que &resto_cadena apunta a un puntero que guardará el caracter
        // que sigue inmediatamente al char que representa el número de vértices
        // en inicio_cadena. Este caracter debe ser el nulo ('\0') ya que fue
        // puesto por strtok() luego de delimitar con "p edge ".
        // Si no se puede formar un entero desde inicio_cadena,
        // "strtoul" hace que resto_cadena apunte al primer carácter de
        // inicio_cadena y devuelve 0.
        // Por lo tanto, verificamos que la conversión haya sido válida y que
        // haya un caracter nulo después del número leído. En caso de error,
        // devolveremos NULL.
        if ((carga->cant_vertices == 0) || ((int)resto_cadena[0] != 0)) {
            printf("Error en primera linea sin comentario\n");
            free(carga);
            return NULL;
        }
        // Particiono el resto de la línea con el delimitador " ".
        inicio_cadena = strtok(NULL," ");
        // Obtengo el 2do. numero que es la cantidad de lados del grafo
        // utilizando strtoul() nuevamente.
        carga->cant_lados = (u32)strtoul(inicio_cadena, &resto_cadena, 10);
        // Verificamos que la conversión haya sido válida y que al ultimo haya
        // un salto de línea ('\n') o retorno de carro. En caso de error, 
        // devolveremos NULL.
        if ((carga->cant_lados == 0) || (((int)resto_cadena[0] != 10 &&
                                         ((int)resto_cadena[0] != 13)))) {
            printf("Error en primer linea sin comentario\n");
            free(carga);
            return NULL;
        } 
    }
    // Ahora que conocemos la cantidad de lados alojamos memoria en el arreglo
    // de la estructura para ir leyendo los lados.
    carga->arreglo_lados = calloc(carga->cant_lados * 2, sizeof(u32));
    // Sino se pudo alojar memoria devolvemos NULL.
    if (carga->arreglo_lados == NULL) {
        free(carga);
        return NULL;
    }
    u32 j = 0;
    for (u32 i = 0; i < (carga->cant_lados); i++) {
        // Si la línea comienza con algo diferente a "e " en este punto, o si no
        // se ha leído ningún caracter, termina, libera memoria y devuelve NULL.
        // Tambien contemplamos el caso en que no haya más lineas para leer;
        // en ese caso fgets leera un EOF y devolverá NULL.
        if ((fgets(linea, BUFSIZ, stdin) == NULL) || (strncmp(linea, "e ", 2)
                                                     != 0)) {
            printf("Error de lectura en el lado %d\n", i+1);
            free(carga->arreglo_lados);
            free(carga);
            return NULL;
        }
        // Particionamos los lados para ir leyendo la información.
        // Dividimos el string linea con la funcion strtok() ahora utilizando
        // como delimitador "e ".
        inicio_cadena = strtok(linea, "e ");
        // Lectura del extremo inicial de un lado.
        carga->arreglo_lados[j] = (u32)strtoul(inicio_cadena, &resto_cadena,
                                               10);
        // Se realizan las siguientes comprobaciones...
        // Que la conversión haya sido válida, es decir, que no haya devuelto 0,
        // pero considerando además que 0 puede ser un vértice.
        comp1 = ((inicio_cadena[0] != '0') && (carga->arreglo_lados[j] == 0));
        // Que haya un caracter nulo después del número leÍdo.
        comp2 = (int)resto_cadena[0]!= 0;
        // Si ocurre alguna de las dos, tenemos un error...
        if (comp1 || comp2) {
            printf("Error de lectura en la lado %d\n", i+1);
            free(carga->arreglo_lados);
            free(carga);
            return NULL;
        }
        // Lectura del extremo final de un lado.
        inicio_cadena = strtok(NULL, " ");
        carga->arreglo_lados[j+1] = (u32)strtoul(inicio_cadena, &resto_cadena,
                                                 10);
        // Se realizan las siguientes comprobaciones...
        // Que la conversión haya sido válida, es decir, que no haya devuelto 0,
        // pero considerando además que 0 puede ser un vértice.
        comp1 = ((inicio_cadena[0] != '0') && (carga->arreglo_lados[j+1] == 0));
        // Que haya un salto de línea o retorno de carro después del nro. leido.
        comp2 = ((int)resto_cadena[0] != 10 && (int)resto_cadena[0] != 13);
        // Si ocurre algunas de las dos, tenemos un error...
        if (comp2 || comp1) {
            printf("Error de lectura en el lado %d\n", i+1);
            free(carga->arreglo_lados);
            free(carga);
            return NULL;
        }
        vertice_1 = carga->arreglo_lados[j];
        vertice_2 = carga->arreglo_lados[j+1];
        // Verificamos que los vértices no sean iguales. Si lo son, entonces
        // es error y devolvemos NULL.
        if (VerticesIguales(vertice_1, vertice_2)) {
            printf("Error de lectura en el lado %d\n", i+1);
            free(carga->arreglo_lados);
            free(carga);
            return NULL;
        }
        // Al llegar aquí se hizo una lectura de un lado, por lo que
        // continuamos con los próximos dos...
        j=j+2;
    }
    // Devolvemos la estructura con los datos cargados...
    return carga;
}

/**
 * @fun: void DestruccionCargaDatos(datos_t datos)
 * @brief: Toma una estructura datos_t y libera la memoria utilizada por ella.
 */
void DestruccionCargaDatos(datos_t datos){
    if (datos != NULL) {
        free(datos->arreglo_lados);
        free(datos);
    }
}
