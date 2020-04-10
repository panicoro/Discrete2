/**
 * @file: Construye.c
 * @brief: Implementación funciones necesarias para construir/copiar/destruir la
 * estructura del grafo desde una estructura datos_t con la información
 * tomada desde standard input.
 */

#include "Construye.h"

/**
 * @fun: ConstruccionDelGrafo().
 * @return: Devuelve una estructura Grafo con la información de un grafo
 * ingresado por standard input en el formato DIMACS especificado.
 */
Grafo ConstruccionDelGrafo() {
    // Variable en la que cargar los datos leidos con la funcion CargaDatos().
    datos_t carga_inicial = NULL;
    // Se obtiene una estructura con los datos leidos usando la funcion
    // CargaDatos().
    carga_inicial = CargaDatos();
    // Si hay un error en la carga de los datos, se devuelve NULL y no se
    // construye nada.
    if (carga_inicial == NULL) return NULL;
    // Si carga_inicial leyo correctamente, empezamos a construir el grafo...
    // Variable en la que cargar el grafo a crear.
    Grafo grafo_nuevo;
    // Se aloca memoria para los elementos del grafo a construir.
    grafo_nuevo = calloc(1, sizeof(GrafoSt));
    // Si falla lo anterior entonces devolvemos NULL.
    if (grafo_nuevo == NULL) return NULL;

    // Se establece el numero de vértices y el número de lados.
    grafo_nuevo->infoGrafo[Vertices] = carga_inicial->cant_vertices;
    grafo_nuevo->infoGrafo[Lados] = carga_inicial->cant_lados;

    // Pedimos memoria para el arreglo de vértices...
    // Este es un arreglo de estructuras vértices por lo que contendrá la
    // información de cada vértice.
    grafo_nuevo->array_vertices = calloc(carga_inicial->cant_vertices,
                                         sizeof(VerticeSt));
    // Si falla devolvemos NULL.
    if (grafo_nuevo->array_vertices == NULL) return NULL;

    // Pedimos memoria para el arreglo de orden de los vértices...
    // El orden de los vértices en este grafo estará dado por este arreglo,
    // en donde guardaremos los índices que los vértices tienen en el arreglo
    // de vértices; así en la posición 0 de esta arreglo estara el primer
    // vértice del orden, en la 1 el segundo y así sucesivamente.
    // Para modificar el orden se modifica este arreglo mientras que el arreglo
    // de vértices permanece invariable.
    grafo_nuevo->array_orden_vertices = calloc(carga_inicial->cant_vertices,
                                               sizeof(u32));
    // Si falla devolvemos NULL.
    if (grafo_nuevo->array_orden_vertices == NULL) return NULL;

    // Ahora tenemos que agregar los vértices del grafo...
    // Creamos una tabla Hash.
    hash_t *hash = hash_crear(free);

    // Varible que cuenta la cantidad de vertices ingresados al grafo nuevo
    // hasta el momento. Si se alocó memoria entonces estará estára
    // inicializada en 0.
    u32 *vertices_ingresados = calloc(1, sizeof(u32));
    // Si falla lo anterior devolvemos NULL.
    if (vertices_ingresados == NULL) return NULL;

    // Variable error para controlar si hay errores en los pasos que siguen.
    bool error;

    // Recorremos el arreglo de lados de la carga_incial.
    for (u32 i = 0; i < (grafo_nuevo->infoGrafo[Lados]) * 2; i += 2) {
        // Para cada par de lados...
        for (u32 j = 0; j < 2; j++) {
            // Llamamos a la función AgregarVertice() pasándole
            // como vértice a agregar uno de cada lado por vez; como se ve
            // carga_inicial->arreglo_lados[i+j] se corresponderá con
            // carga_inicial->arreglo_lados[i] primero y luego con
            // carga_inicial->arreglo_lados[i+1] variando el i de dos en dos.
            error = AgregarVertice(hash, grafo_nuevo, vertices_ingresados,
                                   carga_inicial->arreglo_lados[i+j]);
            // Si se reportó un error entonces...
            if (error) {
                // Destruimos el grafo creado hasta el momento y liberamos
                // memoria.
                DestruccionDelGrafo(grafo_nuevo);
                grafo_nuevo = NULL;
                // goto FREE nos lleva a las últimas sentencias para liberar el
                // resto de las estructuras de memoria.
                goto FREE;
            }
        }
    }

    // Si se terminó el ciclo for anterior, entonces contabilizamos que los
    // vertices ingresados en el la estructura sean la cantidad declarada
    // por el formato de entrada. Si faltaron vertices por ingresar entonces
    // liberamos memoria de las estructuras usadas y devolvemos NULL.
    if (FaltanVertices(grafo_nuevo->infoGrafo[Vertices], *vertices_ingresados)) {
        printf("Cantidad de vértices leidos no es la declarada\n");
        DestruccionDelGrafo(grafo_nuevo);
        grafo_nuevo = NULL;
        goto FREE;
    }

    // Luego si llegamos hasta acá para cada vértice tenemos que agregar sus
    // vecinos.
    // Usamos la funcion AgregarVecinos, que aloja memoria para el arreglo de
    // vecinos para cada vértice según su grado, y despues usa la carga_inicial
    // y la tabla hash generada para buscar los índices correspondientes y
    // almacenarlos cruzadamente entre vecinos.
    error = AgregarVecinos(grafo_nuevo, carga_inicial, hash);

    // Si ocurrio un error en la función anterior, entonces...
    if (error) {
        // Destruimos lo construido y liberamos las estructuras de mermoria y
        // devolvemos NULL;
        DestruccionDelGrafo(grafo_nuevo);
        grafo_nuevo = NULL;
        goto FREE;
    }

    // LLamamos a la función InicializarGrafo para darle un orden a los
    // vértices y luego correr Greedy para que tenga un coloreo. Tambien se
    // establece el valor de la delta grande...
    error = InicializarGrafo(grafo_nuevo);

    // Si ocurrio un error en la función anterior, entonces...
    if (error) {
        // Destruimos lo construido y liberamos las estructuras de mermoria y
        // devolvemos NULL;
        DestruccionDelGrafo(grafo_nuevo);
        grafo_nuevo = NULL;
    }
    // Liberamos el resto de las estructuras...
    FREE:
        free(vertices_ingresados);
        hash_destruir(hash);
        DestruccionCargaDatos(carga_inicial);
        return grafo_nuevo;
}

/**
 * @fun: AgregarVertice(hash_t *HashTable, Grafo grafo, u32 *VerticesIngresados,
                        u32 vertice).
 * @param HashTable: una tabla hash creada.
 * @param grafo: una estructura grafo cuyos vértices no han sido cargados.
 * @param VerticesIngresados: cantidad de vértices ingresados hasta el momento.
 * @param vertice: nombre del vértice a agregar.
 * @return: Devuelve false si el vértice se agregó y contabilizó su grado
 * correctamente; true, en caso de error.
 */
bool AgregarVertice(hash_t *HashTable, Grafo grafo, u32 *VerticesIngresados,
                    u32 vertice) {
    // Puntero al dato a almacenar.
    u32 *dato;
    // Punteros para generar la clave de la tabla hash.
    char *clave, *cadena;
    // Variable para controlar que el vértice se guardó en la tabla hash.
    bool fueGuardado;
    // Puntero que señala el índice de un vértice ya incoporado en la
    // estructura grafo.
    u32 *Ind_array;

    // Primero generamos la clave para la tabla hash, convirtiendo el nombre
    // vértice en un string...
    cadena = calloc(1, 11*sizeof(char));
    // Si no se asignó memoria devolvemos true para indicar error.
    if (cadena == NULL) return true;
    // Con la función itoa propia convertimos el nombre del vértice en string.
    clave = itoa_propia(vertice, cadena, 10);

    //Luego tenemos que ver si el vertice se encuentra en la tabla hash,
    // viendo si con esa clave ya hay un dato asociado o no...
    if (!hash_pertenece(HashTable, clave)) {
        // Si no está en la tabla Hash entonces tenemos que agregar el dato.
        // Usamos el número de vértices ingresados hasta el momento como
        // dato y lo agregamos...
        dato = malloc(sizeof(u32));
        // Si no se aloco memoria para guardar el dato, es un error...
        if (dato == NULL) return true;
        *dato = *VerticesIngresados;
        // Si el dato que queremos agregar es igual a la cantidad de vértices
        // entonces esto es un error, pues en la tabla asociados el nombre
        // del vértice con el índice en el arreglo de vecinos; luego este
        // indice a lo sumo va a valer num. de vertices - 1.
        if (*dato == grafo->infoGrafo[Vertices]) {
            printf("Cantidad de vértices leidos no es la declarada\n");
            return true;
        }
        // Entonces lo guardamos en la misma.
        fueGuardado = hash_guardar(HashTable, clave, dato);
        // Si no podemos ingresarla obtenemos un error.
        if (!fueGuardado) {
            free(cadena);
            return true;
            // Si pudimos ingresarla...
        } else {
            // Cargamos en la posicion (*dato) el nombre (u32) del vertice en el
            // arreglo de vértices del grafo.
            grafo->array_vertices[*dato].infoVertice[Nombre] = vertice;
            // Aumentamos el grado de ese vértice.
            grafo->array_vertices[*dato].infoVertice[Grado]++;
            // Aumentamos la cantidad de vértices ingresados hasta el momento.
            *VerticesIngresados += 1;
            // Liberamos la cadena y retornamos false.
            free(cadena);
            return false;
        }
        // Si hay un dato asociado con esa clave, entonces...
    } else {
        // obtenemos el dato asociado a dicha clave, que es el índice de un
        // vértice ya agregado.
        Ind_array = hash_obtener(HashTable, clave);
        // Aumentamos el grado de dicho vértice.
        grafo->array_vertices[*Ind_array].infoVertice[Grado]++;
        // Liberamos la cadena y retornamos false.
        free(cadena);
        return false;
    }
}

/**
 * @fun: FaltanVertices(u32 verticesTotales, u32 verticesIngresados).
 * @param verticesTotales: cantidad de vértices que tiene el grafo.
 * @param verticesIngresados: cantidad de vértices que se han cargado hasta
 * el momento en el grafo.
 * @return: true si no se han ingresado todos los vértices; false, en caso
 * contrario.
 */
bool FaltanVertices(u32 verticesTotales, u32 verticesIngresados) {
    return verticesTotales != verticesIngresados;
}

/**
 * @fun: bool AgregaVecinos(Grafo G, datos_t carga, hash_t *hash).
 * @param G: estructura Grafo con los vértices cargados.
 * @param carga: estructura datos_t.
 * @param hash: estructura hash_t (tabla hash).
 * @return: devuelve false si los vecinos fueron agregador correctamente; true
 *          si ocurrió un error.
 */
bool AgregarVecinos(Grafo G, datos_t carga, hash_t *hash) {
    // Arreglo para almacenar el (grado-1) de cada vertice para
    // saber como almacenarlos en el arreglo de vecinos de cada uno.
    u32 *grados_vertices;
    // Inicializamos el arreglo de grado de los vértices.
    grados_vertices = calloc(G->infoGrafo[Vertices], sizeof(u32));
    // Verificamos si se alocó memoria correctamente.
    if (grados_vertices == NULL) return true;

    // Llamamos a la función InicializarArregloVecinos para alocar memoria para
    // arreglos de vecinos de cada vértice y registar su (grado-1) en
    // grado_vertices.
    bool error = InicializarArregloVecinos(G, grados_vertices);
    // Si ocurrió un error devolvemos true.
    if (error) return true;

    // Variables para los nombres de los vertices a cargar.
    u32 vertice_1, vertice_2;

    // Iteramos sobre los lados en carga...
    for (u32 i = 0;i < (G->infoGrafo[Lados]) * 2; i+=2) {
        // Obtenemos los nombres de los vertices que forman un lado.
        vertice_1 = carga->arreglo_lados[i];
        vertice_2 = carga->arreglo_lados[i + 1];
        // Usamos la función CargarVecinos() para agregar que los vértices se
        // agreguen mutuamente como vecinos.
        error = CargarVecinos(hash, G, grados_vertices, vertice_1, vertice_2);
        if (error) return true;
    }
    // Chequear que en los vertices no haya vecinos repetidos...
    // Para esto usamos la funcion HayVecinosRepetidos(); si los hay
    // devolvemos true ya que ocurrio un error.
    if (HayVecinosRepetidos(G)) {
        free(grados_vertices);
        return true;
    }
    // Si llegamos hasta acá liberamos memoria y devolvemos false.
    free(grados_vertices);
    return false;
}

/**
 * @fun: InicializarArregloVecinos(Grafo grafo, u32 array_grados[]).
 * @param grafo: una estructura Grafo con sus vértices y los grados de cada
 * uno cargados en ella.
 * @param array_grados: un arreglo del tamaño de los vértices existentes en
 * el grafo.
 * @brief: la función registra el grado-1 del vértice  "i" en el array de
 * vértices en lA posición "i" de array_grados y aloca memoria para el arreglo
 * vecinos_v de cada vértice del grafo.
 * @return: true si hubo algún error, false si no lo hubo.
 */
bool InicializarArregloVecinos(Grafo grafo, u32 array_grados[]) {
    // Variable para almacenar el grado de cada vértice.
    u32 grado;
    // Recorriendo el arreglo de vértices del grado...
    for (u32 i = 0; i < grafo->infoGrafo[Vertices]; ++i){
        // Tomo el grado de un vértice.
        grado = grafo->array_vertices[i].infoVertice[Grado];
        // Cargo el arreglo de grados con el grado-1 del vértice en la misma
        // misma posicion "i" que el arreglo de vértices.
        array_grados[i] = grado-1;
        // Alocamos memoria para para los vecinos de cada vértice.
        grafo->array_vertices[i].vecinos_v = calloc(grado, sizeof(u32));
        // Verificamos que se haya asignado memoria.
        if (grafo->array_vertices[i].vecinos_v == NULL) return true;
    }
    return false;
}

/**
 * @fun: CargarVecinos(hash_t* hashTable, Grafo grafo, u32 grados[],
 *                      u32 vertice1, u32 vertice2).
 * @param hashTable: una tabla hash cargada con (clave, dato) donde clave es
 * el nombre del vértice y dato es índice del vértice en el arreglo de la
 * Estructura grafo.
 * @param grafo: una estructura Grafo ya inicializada con los datos del
 * grafo y los vértices del mismo junto a su grado y sus arreglos de vecinos.
 * @param grados: un arreglo de los grados de los vértices de la estructura
 * grafo. La posición i del arreglo indica el grado-1 del vértice en la
 * posición i del arreglo de vértices de grafo.
 * @param vertice1: nombre de un vértice.
 * @param vertice2: nombre de un vértice.
 * @brief: es una función que agrega en cada vértice el índice (del
 * arreglo de vértices del grafo) de uno de sus vecinos en su arreglo de
 * vecinos.
 * @return: false si no hubo errores, true en caso contrario.
 */
bool CargarVecinos(hash_t* hashTable, Grafo grafo, u32 grados[], u32 vertice1,
                   u32 vertice2) {
    // Variables para obtener las claves de string que se usarán
    // en el Hash.
    char *clave_1, *clave_2, *cadena_1, *cadena_2;
    cadena_1 = malloc(11*sizeof(char));
    cadena_2 = malloc(11*sizeof(char));
    if (cadena_1 == NULL || cadena_2 == NULL) return true;
    // Punteros para obtener los índices de los vértices en el arreglo
    // de vértices.
    u32 *Ind_array1, *Ind_array2;
    // Convertimos los nombres de los vertices en claves de tipo string.
    clave_1 = itoa_propia(vertice1, cadena_1, 10);
    clave_2 = itoa_propia(vertice2, cadena_2, 10);

    //Obtenemos los índices de la tabla hash.
    Ind_array1 = hash_obtener(hashTable, clave_1);
    Ind_array2 = hash_obtener(hashTable, clave_2);
    // Cargamos los indices en el arreglo de vecinos de cada vértice
    // entrecruzándose. El lugar a donde los colocamos viene dado por el
    // arreglo de grados lo que hace que los coloquemos de atrás para
    // adelante.
    grafo->array_vertices[*Ind_array1].vecinos_v[grados[*Ind_array1]] =
            *Ind_array2;
    grafo->array_vertices[*Ind_array2].vecinos_v[grados[*Ind_array2]] =
            *Ind_array1;
    // Disminuimos los indices para los proximos vecinos.
    grados[*Ind_array1]--;
    grados[*Ind_array2]--;
    // Liberamos la cadenas
    free(cadena_1);
    free(cadena_2);
    return false;
}

/**
 * @fun: HayVecinosRepetidos(Grafo grafo).
 * @param grafo: una estructura grafo con todos sus campos completos.
 * @return true si algun vértice tiene un vecino repetido en sus arreglo de
 * vecinos, false en caso contrario.
 */
bool HayVecinosRepetidos(Grafo grafo) {
    // Variable para contar repeteciones.
    u32 repeticiones;
    // Para todos los vértices del grafo usando la funcion ContarRepetidos
    // vemos si en algun caso hay repeticiones en cuyo caso devolvemos true;
    // sino hay repeticiones entonces devolvemos false.
    u32 numVertices = grafo->infoGrafo[Vertices];
    for (u32 i = 0; i < numVertices; ++i) {
        repeticiones = ContarRepetidos(grafo->array_vertices[i].vecinos_v,
                                       grafo->array_vertices[i]
                                               .infoVertice[Grado]);
        if (repeticiones != 0) {
            return true;
        }
    }
    return false;
}

/**
 * @fun: contarRepetidos(u32 array[], u32 size).
 * @param array: arreglo de u32.
 * @param size: tamaño del arreglo de u32.
 * @brief: cuenta la cantidad de datos repetidos en un arreglo.
 * @return: la cantidad (u32) de datos repetidos en el arreglo.
 */
u32 ContarRepetidos(u32 array[], u32 size) {
    // Variable para contar repetidos.
    u32 n_repetidos = 0;
    // Ordenamos el arreglo en orden ascendente.
    qsort(array, size, sizeof(u32), ascendente);
    for (u32 i = 0; i < (size-1); ++i) {
        // Si hay dos consecutivos iguales aumentados el contador.
        if (array[i] == array[i+1]) {
            n_repetidos++;
        }
    }
    // Devolvemos el contador.
    return n_repetidos;
}

/**
 * @fun: ascendente(const void * a, const void * b).
 * @param a: puntero a void.
 * @param b: puntero a void.
 * @return: devuelve 0 referenciados por a y b son iguales; 1 si a es mayor a
 * b, y -1 si b > a.
 */
int ascendente(const void *a, const void *b) {
    u32 primero = *(u32*)a;
    u32 segundo = *(u32*)b;
    if (primero > segundo) {
        return 1;
    } else if (primero < segundo) {
        return -1;
    } else {
        return 0;
    }
}

/**
 * @fun: CalcularDeltaGrande(Grafo grafo)
 * @param grafo: un grafo con todos sus vecinos cargados y un orden dado.
 * @brief: calcula la delta grande del grafo, es decir, el grado máximo de
 * un vértice en el grafo, y guarda esta informacion en el grafo.
 */
void CalcularDeltaGrande(Grafo grafo) {
    u32 delta = 0;
    for(u32 i = 0; i < grafo->infoGrafo[Vertices]; i++) {
        u32 grado = GradoDelVertice(grafo, i);
        if (grado > delta) {
            delta = grado;
        }
    }
    grafo->infoGrafo[DeltaGrande] = delta;
}

/**
 * @fun: InicializarGrafo(Grafo grafo).
 * @param grafo: estructura grafo sin un orden y sin nungún coloreo.
 * @brief: inicializa la estructura grafo con el orden de vértices
 * [0..(numero_vertices-1)]. Luego con el orden dado se corre Greedy.
 * @return: devuelve true si el número de colores es cero; false en caso
 * contrario.
 */
bool InicializarGrafo(Grafo grafo) {
    for (u32 i = 0; i < grafo->infoGrafo[Vertices]; ++i) {
        grafo->array_orden_vertices[i] = i;
    }
    // Calculamos la delta grande del grafo...
    CalcularDeltaGrande(grafo);
    // Creamos el arreglo de colores usados del grafo.
    // Le damos esta cantidad ya que sabemos que Greedy colorea con a lo suma
    // delta granda + 1 colores. Este arreglo nos será util para el reorden
    // chicogrande de bloque de colores.
    grafo->array_cantidad_colores = calloc(grafo->infoGrafo[DeltaGrande]+1,
                                         sizeof(u32));
    if (grafo->array_cantidad_colores == NULL) return true;
    // Ahora llamamos a greedy para darle un coloreo.
    u32 colores = Greedy(grafo);
    return colores == 0 ? true: false;
}

/**
 * @fun: DestruccionDelGrafo(Grafo G).
 * @param G: estructura Grafo.
 * @brief: Un procedimiento que toma un estructura Grafo y libera la memoria
 *          utilizada por la misma.
 */
void DestruccionDelGrafo(Grafo G) {
    if (G != NULL) {
        for (u32 i = 0; i < G->infoGrafo[Vertices]; ++i) {
            if (G->array_vertices[i].vecinos_v != NULL) {
                free(G->array_vertices[i].vecinos_v);
                G->array_vertices[i].vecinos_v = NULL;
            }
        }
        free(G->array_vertices);
        G->array_vertices = NULL;
        free(G->array_orden_vertices);
        G->array_orden_vertices = NULL;
        free(G->array_cantidad_colores);
        G->array_cantidad_colores = NULL;
        free(G);
    }
}

/**
 * @fun: Grafo CopiarGrafo(Grafo G)
 * @param G: un grafo correctamente construido.
 * @return: devuelve un nuevo Grafo igual a G; si hubo errores NULL.
 */
Grafo CopiarGrafo(Grafo G) {
    Grafo grafo_copia = NULL;
    // Se aloca memoria para los elementos del grafo a copiar.
    grafo_copia = calloc(1, sizeof(GrafoSt));
    // Se verifica si se asignó memoria.
    if (grafo_copia == NULL) return NULL;

    // Se copia el nro. de vertices, lados, colores y delta grande de G.
    grafo_copia->infoGrafo[Vertices] = G->infoGrafo[Vertices];
    grafo_copia->infoGrafo[Lados] = G->infoGrafo[Lados];
    grafo_copia->infoGrafo[Colores] = G->infoGrafo[Colores];
    grafo_copia->infoGrafo[DeltaGrande] = G->infoGrafo[DeltaGrande];
    
    // Reservo memoria para el arreglo de estructura de vértices.
    u32 numVertices = G->infoGrafo[Vertices];
    grafo_copia->array_vertices = calloc(numVertices, sizeof(VerticeSt));
    // Verifico la asignación de memoria.
    if (grafo_copia->array_vertices == NULL) return NULL;

    // Reservo memoria para el array_orden_vertices en grafo_copia.
    grafo_copia->array_orden_vertices = calloc(numVertices, sizeof(u32));

    // Se verifica que se haya asignado correctamente.
    if (grafo_copia->array_orden_vertices == NULL) return NULL;

    // Copio la información de cada vertice en G al grafo_copia y el orden en
    // el arreglo de orden de la copia.
    for (u32 i=0; i < numVertices; i++) {
        // Obtenemos e grado de cada vértice.
        u32 cant_vecinos = G->array_vertices[i].infoVertice[Grado];
        // Copiamos el Nombre de cada vertice en G.
        grafo_copia->array_vertices[i].infoVertice[Nombre] = 
                                       G->array_vertices[i].infoVertice[Nombre];
        // Copiamos el Grado de cada vertice en G.
        grafo_copia->array_vertices[i].infoVertice[Grado] = 
                                        G->array_vertices[i].infoVertice[Grado];
        // Copiamos el color de cada vertice en G.
        grafo_copia->array_vertices[i].infoVertice[Color] = 
                                        G->array_vertices[i].infoVertice[Color];
        // Reservo memoria para los vecinos de cada vertice en G.
        grafo_copia->array_vertices[i].vecinos_v = 
                                              calloc(cant_vecinos, sizeof(u32));
        // Verificamos la asignación de memoria.
        if (grafo_copia->array_vertices[i].vecinos_v == NULL) return NULL;
        // Copiamos el orden por último.
        grafo_copia->array_orden_vertices[i] = G->array_orden_vertices[i];

    }

    // Copiamos los vecinos de cada vertice en G, al grafo_copia.
    for (u32 i=0;i < numVertices; i++) {
        u32 cant_vecinos = G->array_vertices[i].infoVertice[Grado];
        //Copiamos los vecinos de cada vertice en G.
        for (u32 j=0;j < cant_vecinos; j++) {
            grafo_copia->array_vertices[i].vecinos_v[j] =
                    G->array_vertices[i].vecinos_v[j];
        }
    }

    // Reservo memoria para el arreglo de cantidad de colores.
    grafo_copia->array_cantidad_colores = calloc(G->infoGrafo[DeltaGrande]+1,
                                                 sizeof(u32));
    // Se verifica que se haya asignado correctamente.
    if (grafo_copia->array_cantidad_colores == NULL) return NULL;

    // delta es la delta grande + 1 que tiene el grafo.
    u32 delta = grafo_copia->infoGrafo[DeltaGrande]+1;
    // Copiamos la cantidad de colores de G en grafo copia.
    for (u32 i = 0; i < delta; i++) {
        grafo_copia->array_cantidad_colores[i] = G->array_cantidad_colores[i];
    }
    // Finalmente devolvemos el grafo copiado.
    return grafo_copia;
}