/**
 * @file: main.c
 * @brief: implementación de las funciones de testeo.
 * @author: Verónica Leaño, Pablo Nicolás Rosa.
 */

//#include "Info.h"
#include "Ordenacion.h"
#include "Rii.h"
#include "U32ToString.h"
#include "TestMain.h"
#include <time.h>


/*void imprimir(Grafo G ){
    for (u32 i = 0; i < G->infoGrafo[Vertices];i++){
        u32 x = G->array_vertices[G->array_orden_vertices[i]]
                .infoVertice[Nombre];
        printf("\nEl vertice de la posicion %u es %u\n ",i,x);
    }  
    //GradodelVertice(G,i); 
}*/

void imprimir_color(Grafo G ){
    for (u32 i = 0; i < G->infoGrafo[Vertices];i++){
        u32 x = G->array_vertices[G->array_orden_vertices[i]]
                .infoVertice[Nombre];
        printf("El vertice de la posicion %u es %u\n ",i,x);
        u32 y = G->array_vertices[G->array_orden_vertices[i]].infoVertice[Color];
        printf("El color de ete vertice es: %u\n", y);
    }  
    //GradodelVertice(G,i); 
}

void imprimir_2(Grafo G ){
    for (u32 i = 0; i < G->infoGrafo[Vertices];i++){
        u32 x = G->array_vertices[G->array_orden_vertices[i]]
                .infoVertice[Color];
        u32 y = G->array_vertices[G->array_orden_vertices[i]]
                .infoVertice[Nombre];
        printf("La posicion %u pertenece al vertice %u su color es es %u\n ",i,y,x);
    }  
    //GradodelVertice(G,i); 
}

int main(int argc, char *argv[]){
    /*u32 chi,  // Menor cantidad de colores de todas las ejecuciónes de Greedy.
        cantColores, // Contador de colores, puede variar si utilizo numeros aleotorios.
        num_colores, // Cantidad de colores que tiene el grafo original.
        //num_vertices, // Cantidad de vertices que tiene un grafo.
        num_iteraciones,* // Cantidad de iteraciones ingresada por stdin.
        u32 semilla, num_iteraciones; // Inicializaciion de los números
                                     // aleotorios, ingresada por stdin.
        char error;

    // Comprueba la cantidad de argumentos pasados por stdin.
    if (argc < 3)  {
       printf("Debes ingresar mas parametros...\n");
       return -1;
    }

    // El primer argumento es la el nùmero de iteraciones
    num_iteraciones = (u32)atoi(argv[1]);
    // El segundo la semilla para el rand()
    semilla = (u32)atoi(argv[2]);
    
    printf("\t------- ------- TESTEO DE PROYECTO -------  -------\t\n");
    printf("\t------- -------    DISCRETA II     -------  -------\t\n");
    printf("\t------- -------       2019         -------  -------\t\n");

    printf("♨ Construyendo el grafo...\n");
    
    Grafo grafo;     // Original se prueba con greedy
   
    grafo = ConstruccionDelGrafo();
    if (grafo == NULL) {
        printf("✖ No se puedo construir el grafo\n");
        DestruccionDelGrafo(grafo);
        return 0;
    } else {
        printf("✔ Grafo construido exitosamente\n");
    }

    // Informacion de grafo.
    printf("\n-------- Información del grafo introducido ------------------\n");
    test_InfoGrafo(grafo);
    printf("---------------------------------------------------------------\n");

    printf("\n--------------  Testeo de coloreo  --------------------------\n");
    //-------------- Testeo de bipartito ------------------------------
    char esBipartito = test_Bipartito(grafo);
    if (esBipartito) {
        printf("Ya fue Whalaan...\n");
        return 0;
    }

    // Testo de funciones de ordenación
    printf("\n------- Testeo de funciones de ordenación  ------------------\n");
    test_Ordenaciones(grafo);
    printf("---------------------------------------------------------------\n");


    //------------ Test un orden y un greedy sobre el grafo original ----------
    error = test_UnOrdenUnGreedy(grafo);
    if (error) {
        printf("✖ Error en correr un orden y luego un greedy\n");
        return 0;
    }
    printf("\n*************************************************************\n");

    //------------ Test 200 Orden Welsh-Powell --------------------------------
    error = test_200WelshPowell(grafo, semilla);
    if (error) {
        printf("✖ Error en correr 200 órdenes Welsh-Powell\n");
        return 0;
    }
    printf("\n*************************************************************\n");

    //------------ Test 200 Orden Welsh-Powell --------------------------------
    error = test_500SwitchVertices(grafo, semilla);
    if (error) {
        printf("✖ Error en correr 500 SwitchVertices\n");
        return 0;
    }
    printf("***************************************************************\n");

    //------------ Test Reordenes de bloques de colores ------------------------
    error = test_RMBCs(grafo, num_iteraciones, semilla);
    if (error) {
        printf("✖ Error en correr %u reordenes aleatorios\n", num_iteraciones);
        return 0;
    }
    DestruccionDelGrafo(grafo);
	return 0;    
}
