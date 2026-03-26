
//Descripcion: Implementacion del modulo de gestion de puzles del juego.
//Proporciona operaciones para inicializar, buscar, mostrar
//y comprobar la solucion de los puzles del Escape Room.
 
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "Puzles.h"
 
//Funciones auxuliares
 

// Funcion auxiliar: strToLower
// Descripcion: Convierte una cadena a minusculas en un buffer destino. No modifica la cadena original.
static void strToLower(char dest[], const char origen[], int tam) {
    int i;
    for (i = 0; i < tam - 1 && origen[i] != '\0'; i++) {
        dest[i] = (char) tolower((unsigned char) origen[i]);
    }
    dest[i] = '\0';
}
 
//Implementacion de funciones


//Funcion: inicualizarPuzle
//Descripcion: Inicializa todos los campos de un puzle a valores por defecto.
//Los campos de cadena quedan como cadena vacia y resuelto = PUZLE_PENDIENTE.
void inicializarPuzle(tPuzle *puzle) {
    puzle->idPuzle[0] = '\0';
    puzle->nombPuz[0] = '\0';
    puzle->idSala [0] = '\0';
    puzle->tipo   [0] = '\0';
    puzle->descrip[0] = '\0';
    puzle->sol    [0] = '\0';
    puzle->resuelto   = PUZLE_PENDIENTE;
}
 

//Funcion: inicializarListaPuzles
//Descripcion: Inicializa la lista de puzles: pone a cero el contador e inicializa cada puzle del array a valores por defecto.
void inicializarListaPuzles(tListaPuzles *lista) {
    int i;
    lista->numPuzles = 0;
    for (i = 0; i < MAX_PUZLES; i++) {
        inicializarPuzle(&lista->puzles[i]);
    }
}
 

//Funcion    : buscarPuzle
//Descripcion: Busca un puzle por su identificador en la lista. Retorna el indice del puzle si se encuentra, -1 si no.
//La busqueda distingue mayusculas de minusculas tal como estan almacenados los identificadores en el fichero.
int buscarPuzle(tListaPuzles lista, char idPuzle[]) {
    int i;
    int indice = -1;
 
    for (i = 0; i < lista.numPuzles && indice == -1; i++) {
        if (strcmp(lista.puzles[i].idPuzle, idPuzle) == 0) {
            indice = i;
        }
    }
    return indice;
}
 
/*
 * Funcion    : buscarPuzleEnSala
 * Descripcion: Devuelve el indice del PRIMER puzle asociado a la sala
 *              indicada que se encuentre PENDIENTE de resolver.
 *              Util para verificar si una sala tiene un puzle activo
 *              antes de permitir el acceso a una conexion bloqueada.
 * Retorno    : Indice del puzle si se encuentra; -1 si no existe ninguno
 *              pendiente para esa sala.
 */
int buscarPuzleEnSala(tListaPuzles lista, char idSala[]) {
    int i;
    int indice = -1;
 
    for (i = 0; i < lista.numPuzles && indice == -1; i++) {
        if (strcmp(lista.puzles[i].idSala, idSala) == 0 &&
            lista.puzles[i].resuelto == PUZLE_PENDIENTE) {
            indice = i;
        }
    }
    return indice;
}
 
//Funcion    : estaResuelto
//Descripcion: Consulta el estado de resolucion de un puzle. Retorna 1 si esta resuelto.
int estaResuelto(tPuzle *puzle) {
    return puzle->resuelto == PUZLE_RESUELTO;
}
 

//Funcion    : mostrarPuzle
//Descripcion: Muestra por pantalla el número y enunciado del puzle, y si está resuelto o no.
void mostrarPuzle(tPuzle *puzle) {
    printf("\n+-----------------------------------------+\n");
    printf("  Puzle Número %s \n", puzle->idPuzle);
    printf("+-----------------------------------------+\n");
    printf("  %s\n", puzle->descrip);
 
    if (puzle->resuelto == PUZLE_RESUELTO) {
        printf("  Estado : [RESUELTO]\n");
    } else {
        printf("  Estado : [PENDIENTE]\n");
    }
}
 

//Funcion    : comprobarSolucion
//Descripcion: Compara la respuesta del jugador con la solucion almacenada en el puzle sin distinguir mayusculas de minusculas.
//Si la respuesta es correcta, marca el puzle como resuelto y retorna 1. 
int comprobarSolucion(tPuzle *puzle, char respuesta[]) {
    char respLower[TAM_SOL_PUZ];
    char solLower [TAM_SOL_PUZ];
    int  correcto;
 
    strToLower(respLower, respuesta,     TAM_SOL_PUZ);
    strToLower(solLower,  puzle->sol,    TAM_SOL_PUZ);
 
    correcto = (strcmp(respLower, solLower) == 0);
 
    if (correcto) {
        puzle->resuelto = PUZLE_RESUELTO;
    }
 
    return correcto;
}

//Funcion: validarPuzle
//Descripcion: comprueba que ningun apartado del registro está vacío. Retorna 1 si es válido y 0 en caso contrario.
int validarPuzle(tPuzle *puzle) {
    int valido = 1;
 
    if (puzle->idPuzle[0] == '\0') {
        fprintf(stderr, "  [PUZLE] Error: identificador vacio.\n");
        valido = 0;
    }
    if (puzle->nombPuz[0] == '\0') {
        fprintf(stderr, "  [PUZLE %s] Error: nombre vacio.\n", puzle->idPuzle);
        valido = 0;
    }
    if (puzle->idSala[0] == '\0') {
        fprintf(stderr, "  [PUZLE %s] Error: idSala vacio.\n", puzle->idPuzle);
        valido = 0;
    }
    if (strcmp(puzle->tipo, "Codigo") != 0 &&
        strcmp(puzle->tipo, "Palabra") != 0) {
        fprintf(stderr, "  [PUZLE %s] Error: tipo '%s' no valido "
                        "(debe ser 'Codigo' o 'Palabra').\n",
                puzle->idPuzle, puzle->tipo);
        valido = 0;
    }
    if (puzle->descrip[0] == '\0') {
        fprintf(stderr, "  [PUZLE %s] Error: descripcion vacia.\n",
                puzle->idPuzle);
        valido = 0;
    }
    if (puzle->sol[0] == '\0') {
        fprintf(stderr, "  [PUZLE %s] Error: solucion vacia.\n",
                puzle->idPuzle);
        valido = 0;
    }
 
    return valido;
}
 

//Funcion    : mostrarListaPuzles
//Descripcion: Muestra por pantalla todos los puzles de la lista con su estado actual (RESUELTO / PENDIENTE).
void mostrarListaPuzles(tListaPuzles lista) {
    int i;
 
    printf("\n===== LISTA DE PUZLES (%d) =====\n", lista.numPuzles);
 
    if (lista.numPuzles == 0) {
        printf("  No hay puzles registrados.\n");
    } else {
        for (i = 0; i < lista.numPuzles; i++) {
            printf("\n[%d] ID: %-4s | Nombre: %-16s | Sala: %-3s | Tipo: %-8s | Estado: %s\n",
                   i + 1,
                   lista.puzles[i].idPuzle,
                   lista.puzles[i].nombPuz,
                   lista.puzles[i].idSala,
                   lista.puzles[i].tipo,
                   lista.puzles[i].resuelto == PUZLE_RESUELTO
                       ? "RESUELTO"
                       : "PENDIENTE");
            printf("    Descripcion: %s\n", lista.puzles[i].descrip);
        }
    }
    printf("================================\n\n");
}