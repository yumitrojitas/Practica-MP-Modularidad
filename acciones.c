#include "Acciones.h"

/*
  Función:      hacer_login
  Descripcion:  Comprueba las credenciales de un usuario buscando
                coincidencias de nombre de usuario y contraseña en el sistema.
  Devuelve:     Puntero al jugador si es correcto.
*/
Jugador* hacer_login(Jugador** lista_jugadores, int num_jugadores, char* user, char* pass) {
    if (lista_jugadores == NULL) return NULL;

    for (int i = 0; i < num_jugadores; i++) {
        if (strcmp(lista_jugadores[i]->Jugador, user) == 0 && strcmp(lista_jugadores[i]->Contrasenna, pass) == 0) {
            return lista_jugadores[i];
        }
    }

    return NULL;
}

/*
  Función:      id_mayor
  Descripcion:  Recorre la lista de jugadores para encontrar y devolver
                el identificador numérico (ID) más alto registrado.
  Devuelve:     Entero con el ID más alto.
*/
int id_mayor(Jugador** lista_jugadores, int num_jugadores) {
    if (lista_jugadores == NULL) return 0;

    int max = 0;

    for (int i = 0; i < num_jugadores; i++)
    {
        if(lista_jugadores[i]->Id_jugador > max)
            max = lista_jugadores[i]->Id_jugador;
    }

    return max;
}

/*
  Función:      guardar_partida
  Descripcion:  Guarda o actualiza el estado actual de la partida de un jugador,
                almacenando la sala actual y el estado de los objetos, conexiones
                y puzles de la sesión.
  Devuelve:     Array dinámico de partidas actualizado.
*/
Partida** guardar_partida(Jugador* jugador_actual, int id_sala_actual, Partida** lista_partidas, int* total_partidas, Objeto** lista_objetos_activos, int total_obj, Conexion** lista_conexiones_activas, int total_con, Puzle** lista_puzles_activos, int total_puz) {
    Partida* p_actual = NULL;

    for (int i = 0; i < *total_partidas; i++) {
        if (lista_partidas[i]->id_jugador == jugador_actual->id_jugador) {
            p_actual = lista_partidas[i];
            break;
        }
    }

    if (p_actual == NULL) {
        lista_partidas = realloc(lista_partidas, (*total_partidas + 1) * sizeof(Partida*));
        p_actual = malloc(sizeof(Partida));

        p_actual->id_jugador = jugador_actual->id_jugador;

        lista_partidas[*total_partidas] = p_actual;
        (*total_partidas)++;
    } else {
        if (p_actual->objetos_modificados != NULL) free(p_actual->objetos_modificados);
        if (p_actual->conexiones_modificadas != NULL) free(p_actual->conexiones_modificadas);
        if (p_actual->puzles_modificados != NULL) free(p_actual->puzles_modificados);
    }

    p_actual->id_sala_actual = id_sala_actual;

    p_actual->objetos_modificados = NULL;
    p_actual->num_objetos = 0;

    for (int i = 0; i < total_obj; i++) {
        p_actual->objetos_modificados = realloc(p_actual->objetos_modificados,
                                                (p_actual->num_objetos + 1) * sizeof(EstadoObjeto));

        strcpy(p_actual->objetos_modificados[p_actual->num_objetos].id_obj, lista_objetos_activos[i]->id_obj);
        strcpy(p_actual->objetos_modificados[p_actual->num_objetos].localizacion, lista_objetos_activos[i]->localiz);

        p_actual->num_objetos++;
    }

    p_actual->conexiones_modificadas = NULL;
    p_actual->num_conexiones = 0;

    for (int i = 0; i < total_con; i++) {
        if (strcmp(lista_conexiones_activas[i]->estado, "Activa") == 0) {
            p_actual->conexiones_modificadas = realloc(p_actual->conexiones_modificadas,
                                                       (p_actual->num_conexiones + 1) * sizeof(EstadoConexion));

            strcpy(p_actual->conexiones_modificadas[p_actual->num_conexiones].id_conexion, lista_conexiones_activas[i]->id_conexion);
            strcpy(p_actual->conexiones_modificadas[p_actual->num_conexiones].estado, lista_conexiones_activas[i]->estado);

            p_actual->num_conexiones++;
        }
    }

    p_actual->puzles_modificados = NULL;
    p_actual->num_puzles = 0;

    for (int i = 0; i < total_puz; i++) {
        if (strcmp(lista_puzles_activos[i]->estado_actual, "Resuelto") == 0) {
            p_actual->puzles_modificados = realloc(p_actual->puzles_modificados, (p_actual->num_puzles + 1) * sizeof(EstadoPuzle));

            strcpy(p_actual->puzles_modificados[p_actual->num_puzles].id_puzle, lista_puzles_activos[i]->id_puzle);
            strcpy(p_actual->puzles_modificados[p_actual->num_puzles].estado, lista_puzles_activos[i]->estado_actual);

            p_actual->num_puzles++;
        }
    }

    return lista_partidas;
}
