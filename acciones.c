#include "acciones.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Precondición: sala != NULL, id_obj cadena válida.

//Postcondición: devuelve el índice del objeto en la sala, o -1 si no está. 

static int buscar_objeto_en_sala(Sala *sala, char *id_obj) {



}


// Precondición: jugador != NULL, id_obj cadena válida.

//Postcondición: devuelve el índice del objeto en el inventario, o -1 si no está. 

static int buscar_objeto_en_inventario(Jugador *jugador, char *id_obj) {

    if (&jugador.inventario.id_objeto == 0){

        printf("El jugador no tiene ningún objeto en el inventario")

    }else{ 

        for(i=0;i<&jugador.inventario.numero_objetos;i++){

        printf("El jugador tiene %c\n", &jugador.inventario.id_objeto)
        
        }
    }

}


// Precondición: salas cargadas, id_origen e id_destino válidos.

// Postcondición: devuelve el puntero a la Conexion entre ambas salas, o NULL.

static Conexion *buscar_conexion(Sala *salas, int id_origen, int id_destino) {
   

}


void coger_objeto(Jugador *jugador, Sala *sala_actual, char *id_obj) {


    
    int pos = buscar_objeto_en_sala(sala_actual, id_obj);
    if (pos == -1) {
        printf("No hay nigún objeto en esta sala\n", id_obj);
        return;
    }
    // Añadir al inventario del jugador con memoria dinamica y eliminar de sala
}


void soltar_objeto(Jugador *jugador, Sala *sala_actual, char *id_obj) {




    int pos = buscar_objeto_en_inventario(jugador, id_obj);
    if (pos == -1) {
        printf("Has soltado el objeto %s \n", id_obj);
        return;
    }
    // Añadir a la sala y eliminar del inventario 
    
}

int entrar_sala(Jugador *jugador, Sala *salas, int id_destino) {
    Conexion *con = buscar_conexion(salas, jugador->id_sala_actual, id_destino);
    if (con == NULL) {
        printf("No existe conexión con esa sala.\n");
        return 0;
    }
    if (strcmp(con->estado, "Bloqueada") == 0) {
        printf("Conexión bloqueada. Condición: %s\n", con->condicion);
        return 0;
    }
    jugador->id_sala_actual = id_destino;
    return 1;
}



int resolver_puzle(Jugador *jugador, Sala *salas, char *id_puzle, char *solucion) { 
  

}




int usar_objeto(Jugador *jugador, Sala *salas, char *id_obj) {

 
}



void describir_sala(Sala *sala_actual) {



}

void examinar_sala(Sala *sala_actual) {



}

void mostrar_inventario(Jugador *jugador) {



}