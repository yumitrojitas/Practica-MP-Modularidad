#ifndef FICHEROS_H
#define FICHEROS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {    //ESTRUCTURA JUGADOR

    int Id_jugador, Num_objetos;
    char Nomb_jugador[21], Jugador[11], Contrasenna[9];
    char** Inventario;
}Jugador;

typedef struct {    //ESTRUCTURA ESTADO OBJETO

    char id_obj[5];
    char localizacion[15]; // Puede ser una sala ("03") o "Inventario"
} EstadoObjeto;

typedef struct {    //ESTRUCTURA ESTADO CONEXION
    char id_conexion[4];
    char estado[11]; // "Activa" o "Bloqueada"
} EstadoConexion;

typedef struct {    //ESTRUCTURA ESTADO PUZLE
    char id_puzle[4];
    char estado[11]; // "Resuelto" o "Pendiente"
} EstadoPuzle;

typedef struct {    //ESTRUCTURA PARTIDA
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

typedef struct {    //ESTRUCUTRA SALA
    int id_sala;             // 2 dígitos
    char nomb_sala[31];      // 30 caracteres máximo + '\0'
    char tipo[10];           // INICIAL, NORMAL o SALIDA + '\0'
    char descrip[151];       // 150 caracteres máximo + '\0'
} Sala;

typedef struct {    //ESTRUCTURA CONEXION
    char id_conexion[4];     // 3 caracteres máximo + '\0'
    int id_origen;           // 2 dígitos
    char nomb_origen[31];    // Nombre de la sala origen (según el ejemplo del guion)
    int id_destino;          // 2 dígitos
    char nomb_destino[31];   // Nombre de la sala destino (según el ejemplo del guion)
    char estado[11];         // "Activa" o "Bloqueada" + '\0'
    char cond[5];            // 0, Id_obj (4 chars) o Id_puzle (3 chars) + '\0'
} Conexion;

typedef struct {    //ESTRUCTURA OBJETO
    char id_obj[5];      // 4 caracteres + '\0'
    char nomb_obj[16];   // 15 caracteres máximo + '\0'
    char descrip[51];    // 50 caracteres máximo + '\0'
    char localiz[15];    // Id_sala o "Inventario" + '\0'
} Objeto;

typedef struct {    //ESTRUCTURA PUZLE
    char id_puzle[4];    // 3 caracteres + '\0'
    char nomb_puz[16];   // 15 caracteres máximo + '\0'
    int id_sala;         // 2 dígitos
    char tipo[15];       // "Código" o "Palabra" + '\0' (damos margen por la tilde)
    char descrip[151];   // 150 caracteres máximo + '\0'
    char sol[51];        // 50 caracteres máximo + '\0'
} Puzle;

Jugador** cargar_jugadores(char* ruta_fichero, int* num_jugadores);

void guardar_jugadores(char* ruta_fichero, Jugador** lista_jugadores, int num_jugadores);

void destruir_lista_jugadores(Jugador** lista_jugadores, int total_jugadores);


Partida** cargar_partidas(char* ruta_fichero, int* total_partidas);

void guardar_partidas(char* ruta_fichero, Partida** lista_partidas, int total_partidas);

void destruir_partidas(Partida** lista_partidas, int total_partidas);


Sala** cargar_salas(char* ruta_fichero, int* total_salas);

void destruir_lista_salas(Sala** lista_salas, int total_salas);


Conexion** cargar_conexiones(char* ruta_fichero, int* total_conexiones);

void destruir_lista_conexiones(Conexion** lista_conexiones, int total_conexiones);


Objeto** cargar_objetos(char* ruta_fichero, int* total_objetos);

void destruir_lista_objetos(Objeto** lista_objetos, int total_objetos);


Puzle** cargar_puzles(char* ruta_fichero, int* total_puzles);

void destruir_lista_puzles(Puzle** lista_puzles, int total_puzles);
#endif // FICHEROS_H
