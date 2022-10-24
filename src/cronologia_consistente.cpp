/*
  Módulo de implementación de `cronologia_consistente'.
  Laboratorio de Programación 3.
  InCo-FIng-UDELAR
*/
#include <stdio.h>
#include "../include/grafo.h"
#include "../include/lista.h"
#include "../include/utils.h"
#include "../include/cronologia_consistente.h"
#include <math.h>

Grafo construir_grafo(nat n, ListaDatos tipo1, ListaDatos tipo2) {
    //DECLARACIÓN DE ESTRUCTUAS AUXILIARES
    Grafo grafo = crear_grafo(2 * n, true);
    ListaDatos convivencias = copia_lista_datos(tipo2);
    ListaDatos muertes = copia_lista_datos(tipo1);

    //AÑADE VERTICES PARA EL NACIMIENTO Y MUERTE DE CADA PERSONA
    for (nat i = 1; i <= (2 * n); i++) {
        agregar_vertice(i, grafo);
        if (i % 2 == 0) {
            agregar_arista(i - 1, i, grafo);
        }
    }

    //AÑADE ARISTAS DE MUERTES
    while (!es_vacia_lista_datos(muertes)) {
        nat persona1 = id1(primer_dato(muertes));
        nat persona2 = id2(primer_dato(muertes));
        remover_dato(primer_dato(muertes), muertes);
        agregar_arista((2 * persona1), (2 * persona2) - 1, grafo);
    }

    //AÑADE ARISTAS DE CONVIVENCIA
    while (!es_vacia_lista_datos(convivencias)) {
        nat persona1 = id1(primer_dato(convivencias));
        nat persona2 = id2(primer_dato(convivencias));
        remover_dato(primer_dato(convivencias), convivencias);
        agregar_arista((2 * persona1) - 1, (2 * persona2), grafo);
        agregar_arista((2 * persona2) - 1, (2 * persona1), grafo);
    }

    //DESTRUCCIÓN DE ESTRUCTUAS AUXILIARES
    destruir_lista_datos(muertes);
    destruir_lista_datos(convivencias);

    return grafo;
}

Cronologia cronologia_consistente(nat n, ListaDatos tipo1, ListaDatos tipo2) {
    // DECLARACION DE ESTRUCTURAS AUXILIARES
    Grafo grafo = construir_grafo(n, tipo1, tipo2);
    nat * grado = new nat[(2 * n) + 1];
    Lista verticesGrado0 = crear_lista();
    Cronologia cr = new evento[2 * n];

    // INICIALIZA ARREGLO grado[] Y LISTA verticesGrado0
    for (nat i = 1; i <= (2 * n); i++) {
        grado[i] = in_grado(i, grafo);
        if (grado[i] == 0)
            insertar_al_inicio(i, verticesGrado0);
    }

    // ALGORITMO
    nat i = 0;
    while (!es_vacia_lista(verticesGrado0)) {
        comienzo(verticesGrado0);
        nat vertice = actual(verticesGrado0);

        // AÑADE VERTICE A LA CRONOLOGIA
        cr[i].id = ceil((vertice + 1) / 2);
        if (vertice % 2 == 1)
            cr[i].tipo = nace;
        else
            cr[i].tipo = muere;

        // ACTUALIZA GRADO DE VERTICES ADYACENTES Y LISTA verticesGrado0
        Lista ady = adyacentes(vertice, grafo);
        while (!es_vacia_lista(ady)) {
            grado[primero(ady)]--;
            if (grado[primero(ady)] == 0) {
                insertar_al_final(primero(ady), verticesGrado0);
            }
            remover_al_inicio(ady);
        }
        remover_al_inicio(verticesGrado0); // ELIMINA VERTICE DE LISTA verticesGrado0
        i = i + 1;
    }

    // DESTRUCCION DE ESTRUCTURAS AUXILIARES
    delete[] grado;
    destruir_grafo(grafo);
    destruir_lista(verticesGrado0);

    // DEVUELVE CRONOLOGIA SI LA HAY SINO DEVUELVE NULL
    if (i < (2 * n) - 1) {
        delete[] cr;
        return NULL;
    } else {
        return cr;
    }
}