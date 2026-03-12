#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Jugador.h"

// Sub-estructuras para guardar el estado modificado
typedef struct {
    char id_obj[5];
    char localizacion[15]; // Puede ser una sala ("03") o "Inventario"
} EstadoObjeto;

typedef struct {
    char id_conexion[4];
    char estado[11]; // "Activa" o "Bloqueada"
} EstadoConexion;

typedef struct {
    char id_puzle[4];
    char estado[11]; // "Resuelto" o "Pendiente"
} EstadoPuzle;

// Estructura principal de un bloque de guardado
typedef struct {
    int id_jugador;
    int id_sala_actual;

    // Arrays dinámicos para las listas variables
    EstadoObjeto* objetos_modificados;
    int num_objetos;

    EstadoConexion* conexiones_modificadas;
    int num_conexiones;

    EstadoPuzle* puzles_modificados;
    int num_puzles;
} Partida;

void bucle_partida(Jugador* jugador, int partida_cargada);

Partida** cargar_partidas(char* ruta_fichero, int* total_partidas);

Partida* buscar_partida_actual(Partida** lista_partidas, Jugador* jugador, int num_partidas);

void guardar_partidas(char* ruta_fichero, Partida** lista_partidas, int total_partidas);

void destruir_partidas(Partida** lista_partidas, int total_partidas);

#endif // MENU_H
