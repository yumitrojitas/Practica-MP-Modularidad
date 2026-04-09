#ifndef ACCIONES_H
#define ACCIONES_H

// Declaración de funciones

/*
  Función:      hacer_login
  Descripcion:  Comprueba las credenciales de un usuario buscando
                coincidencias de nombre de usuario y contraseña en el sistema.
  Parámetros:   lista_jugadores - array dinámico de jugadores (entrada)
                num_jugadores - entero con el total de jugadores (entrada)
                user - cadena de caracteres con el nombre del usuario (entrada)
                pass - cadena de caracteres con la contraseña (entrada)
*/
Jugador* hacer_login(Jugador** lista_jugadores, int num_jugadores, char* user, char* pass);

/*
  Función:      id_mayor
  Descripcion:  Recorre la lista de jugadores para
                encontrar el identificador ID más grande.
  Parámetros:   lista_jugadores - array dinámico de jugadores (entrada)
                num_jugadores - entero con el total de jugadores (entrada)
*/
int id_mayor(Jugador** lista_jugadores, int num_jugadores);

/*
  Función:      guardar_partida
  Descripcion:  Guarda o actualiza el estado de la partida de un jugador,
                guardando la sala actual y el estado de los objetos, conexiones
                y puzles de la sesión.
  Parámetros:   jugador_actual - puntero al jugador actual (entrada)
                id_sala_actual - entero con la ID de la sala en la que se encuentra (entrada)
                lista_partidas - array dinámico de partidas del sistema (entrada)
                total_partidas - puntero al total de partidas registradas (entrada/salida)
                lista_objetos_activos - array dinámico de objetos en la sesión (entrada)
                total_obj - entero con el total de objetos en sesión (entrada)
                lista_conexiones_activas - array dinámico de conexiones en la sesión (entrada)
                total_con - entero con el total de conexiones en sesión (entrada)
                lista_puzles_activos - array dinámico de puzles en la sesión (entrada)
                total_puz - entero con el total de puzles en sesión (entrada)
*/
Partida** guardar_partida(Jugador* jugador_actual, int id_sala_actual, Partida** lista_partidas, int* total_partidas, Objeto** lista_objetos_activos, int total_obj, Conexion** lista_conexiones_activas, int total_con, Puzle** lista_puzles_activos, int total_puz);

#endif // ACCIONES_H
