#ifndef JUGADOR_H
#define JUGADOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int Id_jugador, Num_objetos;
    char Nomb_jugador[21], Jugador[11], Contrasenna[9];
    char** Inventario;
}Jugador;

Jugador* crear_jugador(char* nombre, char* user, char* pass, Jugador** lista_jugadores, int* num_jugadores);

void destruir_jugador(Jugador* j);

Jugador** cargar_jugadores(char* ruta_fichero, int* num_jugadores);

Jugador* hacer_login(Jugador** lista_jugadores, int num_jugadores, char* user, char* pass);

int id_mayor(Jugador** lista_jugadores, int num_jugadores);

void guardar_jugadores(char* ruta_fichero, Jugador** lista_jugadores, int num_jugadores);

#endif // JUGADOR_H
