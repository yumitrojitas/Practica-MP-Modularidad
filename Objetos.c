//Este módulo contiene la lógica para gestionar los objetos del juego, su localización en salas o en el inventario, y su visualización

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Objetos.h" 

//Función: inicializarObjeto
//Descripción: Inicializa todos los campos de un objeto a valores por defecto.
void inicializarObjeto(tObjeto *obj) {
    if (obj != NULL) {
        memset(obj->idObj, '\0', TAM_ID_OBJ);
        memset(obj->nombObj, '\0', TAM_NOMB_OBJ);
        memset(obj->describ, '\0', TAM_DESCRIB_OBJ);
        memset(obj->localiz, '\0', TAM_LOCALIZ);
    }
}

//Función: inicializarListaObjetos
//Descripcion: Inicializa la lista completa de objetos del juego.
void inicializarListaObjetos(tListaObjetos *lista) {
    if (lista != NULL) {
        lista->numObjetos = 0;
        for (int i = 0; i < MAX_OBJETOS; i++) {
            inicializarObjeto(&(lista->objetos[i]));
        }
    }
}

//Función: buscarObjeto
//Descripción: Busca un objeto por su identificador en la lista global.
//Retorna el indice del array si se encuentra, -1 si no.
int buscarObjeto(tListaObjetos lista, char idObj[]) {
    for (int i = 0; i < lista.numObjetos; i++) {
        if (strcmp(lista.objetos[i].idObj, idObj) == 0) {
            return i;
        }
    }
    return -1;
}

//Funcion: buscarObjetoEnSala
//Descripción: Busca el primer objeto ubicado en una sala específica 
//Retorna el indice del array si se encuentra, -1 si no. 
int buscarObjetoEnSala(tListaObjetos lista, char idSala[]) {
    for (int i = 0; i < lista.numObjetos; i++) {
        if (strcmp(lista.objetos[i].localiz, idSala) == 0) {
            return i;
        }
    }
    return -1;
}

//Función: estaEnInventario
//Descripción: Comprueba si un objeto pertenece al inventario del jugador.
//Retorna 1 si esta en el inventario, 0 si no. 
int estaEnInventario(tObjeto *obj) {
    if (obj != NULL && strcmp(obj->localiz, "Inventario") == 0) {
        return 1;
    }
    return 0;
}

//Función: moverObjetoASala
//Descripción: Cambia la ubicación de un objeto a una sala determinada.
void moverObjetoASala(tObjeto *obj, char idSala[]) {
    if (obj != NULL && idSala != NULL) {
        strncpy(obj->localiz, idSala, TAM_LOCALIZ - 1);
        obj->localiz[TAM_LOCALIZ - 1] = '\0'; // Asegura el fin de cadena
    }
}

//Función: moverObjetoAInventario
//Descripción: Cambia la ubicación de un objeto al inventario del jugador.
void moverObjetoAInventario(tObjeto *obj) {
    if (obj != NULL) {
        strcpy(obj->localiz, "Inventario");
    }
}

//Función: mostrarObjeto
//Descripción: Muestra la información descriptiva de un objeto por consola.
void mostrarObjeto(tObjeto *obj) {
    if (obj != NULL) {
        printf("%s: %s\n",  obj->nombObj, obj->describ);
    }
}

//Función: listarObjetosEnSala
//Descripción: Lista todos los objetos presentes en una sala.
void listarObjetosEnSala(tListaObjetos lista, char idSala[]) {
    int encontrado = 0;
    for (int i = 0; i < lista.numObjetos; i++) {
        if (strcmp(lista.objetos[i].localiz, idSala) == 0) {
            mostrarObjeto(&(lista.objetos[i]));
            encontrado = 1;
        }
    }
    if (!encontrado) {
        printf("No hay objetos en esta sala.\n");
    }
}

//Función: listarInventario
//Descripción: Muestra el contenido del inventario del jugador.
void listarInventario(tListaObjetos lista) {
    int contador = 0;
    printf("\n--- INVENTARIO ---\n");
    for (int i = 0; i < lista.numObjetos; i++) {
        if (estaEnInventario(&(lista.objetos[i]))) {
            mostrarObjeto(&(lista.objetos[i]));
            contador++;
        }
    }
    if (contador == 0) {
        printf("El inventario esta vacio.\n");
    }
    printf("------------------\n");
}

//Función: validarObjeto
//Descripción: Comprueba si los campos de un objeto no esten vacíos y que la localización sea válida.
//Retorna 1 si es válido, 0 si no. 
int validarObjeto(tObjeto *obj) {
    if (obj == NULL) return 0;
    if (strlen(obj->idObj) == 0 || strlen(obj->nombObj) == 0) {
        return 0;
    }
    
    if (strlen(obj->localiz) == 0) {
        return 0;
    }
    
    return 1;
}