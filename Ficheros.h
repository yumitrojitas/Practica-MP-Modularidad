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
    int id_sala;
    char nomb_sala[31];
    char tipo[10];
    char descrip[151];
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

// Declaración de funciones

/*
  Función:      cargar_jugadores
  Descripcion:  Lee el fichero .txt donde esta guardada la
                información sobre los jugadores y la carga
                en el array dinamico que guardará a los jugadores
  Parámetros:   ruta_fichero - cadena de caracteres (entrada)
                num_jugadores - puntero a entero con el total de jugadores (entrada/salida)
*/
Jugador** cargar_jugadores(char* ruta_fichero, int* num_jugadores);

/*
  Función:      guardar_jugadores
  Descripcion:  Escribe la información del array dinámico de
                jugadores en el fichero .txt correspondiente
  Parámetros:   ruta_fichero - cadena de caracteres (entrada)
                lista_jugadores - array dinámico de jugadores (entrada)
                num_jugadores - entero con el total de jugadores (entrada)
*/
void guardar_jugadores(char* ruta_fichero, Jugador** lista_jugadores, int num_jugadores);

/*
  Función:      destruir_lista_jugadores
  Descripcion:  Libera la memoria dinámica reservada para el array
                de jugadores y sus estructuras
  Parámetros:   lista_jugadores - array dinámico de jugadores (entrada)
                total_jugadores - entero con el total de jugadores (entrada)
*/
void destruir_lista_jugadores(Jugador** lista_jugadores, int total_jugadores);


/*
  Función:      cargar_partidas
  Descripcion:  Lee el fichero .txt donde esta guardada la
                información sobre las partidas y la carga
                en el array dinámico que guardará las partidas
  Parámetros:   ruta_fichero - cadena de caracteres (entrada)
                total_partidas - puntero al total de partidas (entrada/salida)
*/
Partida** cargar_partidas(char* ruta_fichero, int* total_partidas);

/*
  Función:      guardar_partidas
  Descripcion:  Escribe la información del array dinámico de
                partidas en el fichero .txt correspondiente
  Parámetros:   ruta_fichero - cadena de caracteres (entrada)
                lista_partidas - array dinámico de partidas (entrada)
                total_partidas - entero con el total de partidas (entrada)
*/
void guardar_partidas(char* ruta_fichero, Partida** lista_partidas, int total_partidas);

/*
  Función:      destruir_partidas
  Descripcion:  Libera la memoria dinámica reservada para el array
                de partidas y sus estructuras internas
  Parámetros:   lista_partidas - array dinámico de partidas (entrada)
                total_partidas - entero con el total de partidas (entrada)
*/
void destruir_partidas(Partida** lista_partidas, int total_partidas);


/*
  Función:      cargar_salas
  Descripcion:  Lee el fichero .txt donde esta guardada la
                información sobre las salas y la carga
                en un array dinámico que contendrá las salas
  Parámetros:   ruta_fichero - cadena de caracteres (entrada)
                total_salas - puntero al total de salas (entrada/salida)
*/
Sala** cargar_salas(char* ruta_fichero, int* total_salas);

/*
  Función:      destruir_lista_salas
  Descripcion:  Libera la memoria dinámica reservada para el array
                de salas
  Parámetros:   lista_salas - array dinámico de salas (entrada)
                total_salas - entero con el total de salas (entrada)
*/
void destruir_lista_salas(Sala** lista_salas, int total_salas);


/*
  Función:      cargar_conexiones
  Descripcion:  Lee el fichero .txt donde esta guardada la
                información sobre las conexiones y la carga
                en un array dinámico
  Parámetros:   ruta_fichero - cadena de caracteres (entrada)
                total_conexiones - puntero al total de conexiones (entrada/salida)
*/
Conexion** cargar_conexiones(char* ruta_fichero, int* total_conexiones);

/*
  Función:      destruir_lista_conexiones
  Descripcion:  Libera la memoria dinámica reservada para el array
                de conexiones
  Parámetros:   lista_conexiones - array dinámico de conexiones (entrada)
                total_conexiones - entero con el total de conexiones (entrada)
*/
void destruir_lista_conexiones(Conexion** lista_conexiones, int total_conexiones);


/*
  Función:      cargar_objetos
  Descripcion:  Lee el fichero .txt donde esta guardada la
                información sobre los objetos y la carga
                en un array dinámico
  Parámetros:   ruta_fichero - cadena de caracteres (entrada)
                total_objetos - puntero al total de objetos (entrada/salida)
*/
Objeto** cargar_objetos(char* ruta_fichero, int* total_objetos);

/*
  Función:      destruir_lista_objetos
  Descripcion:  Libera la memoria dinámica reservada para el array
                de objetos
  Parámetros:   lista_objetos - array dinámico de objetos (entrada)
                total_objetos - entero con el total de objetos (entrada)
*/
void destruir_lista_objetos(Objeto** lista_objetos, int total_objetos);


/*
  Función:      cargar_puzles
  Descripcion:  Lee el fichero .txt donde esta guardada la
                información sobre los puzles y la carga
                en un array dinámico
  Parámetros:   ruta_fichero - cadena de caracteres (entrada)
                total_puzles - puntero al total de puzles (entrada/salida)
*/
Puzle** cargar_puzles(char* ruta_fichero, int* total_puzles);

/*
  Función:      destruir_lista_puzles
  Descripcion:  Libera la memoria dinámica reservada para el array
                de puzles
  Parámetros:   lista_puzles - array dinámico de puzles (entrada)
                total_puzles - entero con el total de puzles (entrada)
*/
void destruir_lista_puzles(Puzle** lista_puzles, int total_puzles);
#endif // FICHEROS_H
