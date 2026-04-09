#include "Ficheros.h"

/*
  Función:      cargar_jugadores
  Descripcion:  Lee el fichero .txt donde esta guardada la
                información sobre los jugadores y la carga
                en el array dinamico que guardará a los jugadores
  Devuelve:     Array dinámico con los jugadores cargados.
*/
Jugador** cargar_jugadores(char* ruta_fichero, int* num_jugadores) {
    FILE* f = fopen(ruta_fichero, "r");
    if (f == NULL) {
        printf("Error: No se pudo abrir %s\n", ruta_fichero);
        return NULL;
    }

    Jugador** array_jugadores = NULL;
    *num_jugadores = 0;

    char linea[256];

    while (fgets(linea, sizeof(linea), f) != NULL) {
        linea[strcspn(linea, "\r\n")] = '\0';

        int id;
        char nombre[21], user[11], pass[9];
        int bytes_leidos = 0;

        if (sscanf(linea, "%d-%20[^-]-%10[^-]-%8[^-]%n", &id, nombre, user, pass, &bytes_leidos) == 4) {

            array_jugadores = realloc(array_jugadores, (*num_jugadores + 1) * sizeof(Jugador*));

            Jugador* nuevo_jugador = malloc(sizeof(Jugador));
            nuevo_jugador->Id_jugador = id;
            strcpy(nuevo_jugador->Nomb_jugador, nombre);
            strcpy(nuevo_jugador->Jugador, user);
            strcpy(nuevo_jugador->Contrasenna, pass);
            nuevo_jugador->Inventario = NULL;
            nuevo_jugador->Num_objetos = 0;

            char* ptr_restante = linea + bytes_leidos;
            char id_obj[5];
            int avance = 0;

            while (sscanf(ptr_restante, "-%4[^-]%n", id_obj, &avance) == 1) {
                nuevo_jugador->Inventario = realloc(nuevo_jugador->Inventario, (nuevo_jugador->Num_objetos + 1) * sizeof(char*));

                nuevo_jugador->Inventario[nuevo_jugador->Num_objetos] = malloc(5 * sizeof(char));
                strcpy(nuevo_jugador->Inventario[nuevo_jugador->Num_objetos], id_obj);

                nuevo_jugador->Num_objetos++;

                ptr_restante += avance;
            }

            array_jugadores[*num_jugadores] = nuevo_jugador;
            (*num_jugadores)++;
        }
    }

    fclose(f);
    return array_jugadores;
}

/*
  Función:      guardar_jugadores
  Descripcion:  Escribe la información del array dinámico de
                jugadores en el fichero .txt correspondiente
*/
void guardar_jugadores(char* ruta_fichero, Jugador** lista_jugadores, int num_jugadores) {
    if (lista_jugadores == NULL || num_jugadores == 0) {
        return;
    }

    FILE* f = fopen(ruta_fichero, "w");
    if (f == NULL) {
        printf("Error: No se pudo abrir %s para guardar los datos.\n", ruta_fichero);
        return;
    }

    for (int i = 0; i < num_jugadores; i++) {
        Jugador* j = lista_jugadores[i];

        fprintf(f, "%02d-%s-%s-%s", j->Id_jugador, j->Nomb_jugador, j->Jugador, j->Contrasenna);

        for (int k = 0; k < j->Num_objetos; k++) {
            fprintf(f, "-%s", j->Inventario[k]);
        }

        fprintf(f, "\n");
    }

    fclose(f);
    printf("Datos de los jugadores guardados correctamente.\n");
}

/*
  Función:      destruir_lista_jugadores
  Descripcion:  Libera la memoria dinámica reservada para el array
                de jugadores y sus estructuras internas
*/
void destruir_lista_jugadores(Jugador** lista_jugadores, int total_jugadores) {
    if (lista_jugadores != NULL) {

        for (int i = 0; i < total_jugadores; i++) {

            if (lista_jugadores[i] != NULL) {
                if (lista_jugadores[i]->Inventario != NULL) {
                    for (int j = 0; j < lista_jugadores[i]->Num_objetos; j++) {
                        free(lista_jugadores[i]->Inventario[j]);
                    }
                    free(lista_jugadores[i]->Inventario);
                }

                free(lista_jugadores[i]);
            }
        }

        free(lista_jugadores);
    }
}


/*
  Función:      cargar_partidas
  Descripcion:  Lee el fichero .txt donde esta guardada la
                información sobre las partidas y la carga
                en el array dinámico correspondiente
  Devuelve:     Array dinámico con las partidas cargadas.
*/
Partida** cargar_partidas(char* ruta_fichero, int* total_partidas) {
    FILE* f = fopen(ruta_fichero, "r");
    if (f == NULL) {
        printf("Aviso: No se encontro %s. Se creara uno nuevo al guardar.\n", ruta_fichero);
        *total_partidas = 0;
        return NULL;
    }

    Partida** array_partidas = NULL;
    *total_partidas = 0;

    Partida* p_actual = NULL;
    char linea[256];

    while (fgets(linea, sizeof(linea), f) != NULL) {
        linea[strcspn(linea, "\r\n")] = '\0';

        if (strlen(linea) == 0) continue;

        if (strncmp(linea, "JUGADOR:", 8) == 0) {
            array_partidas = realloc(array_partidas, (*total_partidas + 1) * sizeof(Partida*));
            p_actual = malloc(sizeof(Partida));

            sscanf(linea, "JUGADOR: %d", &p_actual->id_jugador);
            p_actual->id_sala_actual = 0;

            p_actual->objetos_modificados = NULL;
            p_actual->num_objetos = 0;
            p_actual->conexiones_modificadas = NULL;
            p_actual->num_conexiones = 0;
            p_actual->puzles_modificados = NULL;
            p_actual->num_puzles = 0;

            array_partidas[*total_partidas] = p_actual;
            (*total_partidas)++;
        }
        else if (strncmp(linea, "SALA:", 5) == 0 && p_actual != NULL) {
            sscanf(linea, "SALA: %d", &p_actual->id_sala_actual);
        }
        else if (strncmp(linea, "OBJETO:", 7) == 0 && p_actual != NULL) {
            p_actual->objetos_modificados = realloc(p_actual->objetos_modificados,
                                            (p_actual->num_objetos + 1) * sizeof(EstadoObjeto));

            sscanf(linea, "OBJETO: %4[^-]-%14s",
                   p_actual->objetos_modificados[p_actual->num_objetos].id_obj,
                   p_actual->objetos_modificados[p_actual->num_objetos].localizacion);
            p_actual->num_objetos++;
        }
        else if (strncmp(linea, "CONEXI", 6) == 0 && p_actual != NULL) {
            p_actual->conexiones_modificadas = realloc(p_actual->conexiones_modificadas, (p_actual->num_conexiones + 1) * sizeof(EstadoConexion));

            char* inicio_datos = strstr(linea, ": ");
            if (inicio_datos != NULL) {
                sscanf(inicio_datos + 2, "%3[^-]-%10s", p_actual->conexiones_modificadas[p_actual->num_conexiones].id_conexion, p_actual->conexiones_modificadas[p_actual->num_conexiones].estado);
                p_actual->num_conexiones++;
            }
        }
        else if (strncmp(linea, "PUZLE:", 6) == 0 && p_actual != NULL) {
            p_actual->puzles_modificados = realloc(p_actual->puzles_modificados,
                                           (p_actual->num_puzles + 1) * sizeof(EstadoPuzle));

            sscanf(linea, "PUZLE: %3[^-]-%10s",
                   p_actual->puzles_modificados[p_actual->num_puzles].id_puzle,
                   p_actual->puzles_modificados[p_actual->num_puzles].estado);
            p_actual->num_puzles++;
        }
    }

    fclose(f);
    return array_partidas;
}

/*
  Función:      guardar_partidas
  Descripcion:  Escribe la información del array dinámico de
                partidas en el fichero .txt correspondiente
*/
void guardar_partidas(char* ruta_fichero, Partida** lista_partidas, int total_partidas) {
    if (lista_partidas == NULL || total_partidas == 0) {
        return;
    }

    FILE* f = fopen(ruta_fichero, "w");
    if (f == NULL) {
        printf("Error: No se pudo abrir %s para guardar la partida.\n", ruta_fichero);
        return;
    }

    for (int i = 0; i < total_partidas; i++) {
        Partida* p = lista_partidas[i];

        fprintf(f, "JUGADOR: %02d\n", p->id_jugador);
        fprintf(f, "SALA: %02d\n", p->id_sala_actual);

        for (int j = 0; j < p->num_objetos; j++) {
            fprintf(f, "OBJETO: %s-%s\n",
                    p->objetos_modificados[j].id_obj,
                    p->objetos_modificados[j].localizacion);
        }

        for (int j = 0; j < p->num_conexiones; j++) {
            fprintf(f, "CONEXIÓN: %s-%s\n",
                    p->conexiones_modificadas[j].id_conexion,
                    p->conexiones_modificadas[j].estado);
        }

        for (int j = 0; j < p->num_puzles; j++) {
            fprintf(f, "PUZLE: %s-%s\n",
                    p->puzles_modificados[j].id_puzle,
                    p->puzles_modificados[j].estado);
        }
    }

    fclose(f);
    printf("Progreso de todas las partidas guardado correctamente en %s.\n", ruta_fichero);
}

/*
  Función:      destruir_partidas
  Descripcion:  Libera la memoria dinámica reservada para el array
                de partidas y sus estructuras internas
*/
void destruir_partidas(Partida** lista_partidas, int total_partidas) {
    if (lista_partidas != NULL) {
        for (int i = 0; i < total_partidas; i++) {
            if (lista_partidas[i] != NULL) {
                if (lista_partidas[i]->objetos_modificados != NULL) {
                    free(lista_partidas[i]->objetos_modificados);
                }
                if (lista_partidas[i]->conexiones_modificadas != NULL) {
                    free(lista_partidas[i]->conexiones_modificadas);
                }
                if (lista_partidas[i]->puzles_modificados != NULL) {
                    free(lista_partidas[i]->puzles_modificados);
                }

                free(lista_partidas[i]);
            }
        }
        free(lista_partidas);
    }
}

/*
  Función:      cargar_salas
  Descripcion:  Lee el fichero .txt donde esta guardada la
                información sobre las salas y la carga
                en un array dinámico
  Devuelve:     Array dinámico con las salas cargadas.
*/
Sala** cargar_salas(char* ruta_fichero, int* total_salas) {
    FILE* f = fopen(ruta_fichero, "r");
    if (f == NULL) {
        printf("Error: No se pudo abrir el fichero %s\n", ruta_fichero);
        *total_salas = 0;
        return NULL;
    }

    Sala** array_salas = NULL;
    *total_salas = 0;

    char linea[256];

    while (fgets(linea, sizeof(linea), f) != NULL) {
        linea[strcspn(linea, "\r\n")] = '\0';

        if (strlen(linea) == 0) continue;

        int id;
        char nombre[31], tipo[10], descripcion[151];

        if (sscanf(linea, "%d-%30[^-]-%9[^-]-%150[^\n]", &id, nombre, tipo, descripcion) == 4) {

            array_salas = realloc(array_salas, (*total_salas + 1) * sizeof(Sala*));

            Sala* nueva_sala = malloc(sizeof(Sala));

            nueva_sala->id_sala = id;
            strcpy(nueva_sala->nomb_sala, nombre);
            strcpy(nueva_sala->tipo, tipo);
            strcpy(nueva_sala->descrip, descripcion);

            array_salas[*total_salas] = nueva_sala;
            (*total_salas)++;
        } else {
            printf("Aviso: Formato incorrecto en la línea: %s\n", linea);
        }
    }

    fclose(f);
    return array_salas;
}

/*
  Función:      destruir_lista_salas
  Descripcion:  Libera la memoria dinámica reservada para el array
                de salas
*/
void destruir_lista_salas(Sala** lista_salas, int total_salas) {
    if (lista_salas != NULL) {
        for (int i = 0; i < total_salas; i++) {
            if (lista_salas[i] != NULL) {
                free(lista_salas[i]);
            }
        }
        free(lista_salas);
    }
}


/*
  Función:      cargar_conexiones
  Descripcion:  Lee el fichero .txt donde esta guardada la
                información sobre las conexiones y la carga
                en un array dinámico
  Devuelve:     Array dinámico con las conexiones cargadas.
*/
Conexion** cargar_conexiones(char* ruta_fichero, int* total_conexiones) {
    FILE* f = fopen(ruta_fichero, "r");
    if (f == NULL) {
        printf("Error: No se pudo abrir el fichero %s\n", ruta_fichero);
        *total_conexiones = 0;
        return NULL;
    }

    Conexion** array_conexiones = NULL;
    *total_conexiones = 0;

    char linea[256];

    while (fgets(linea, sizeof(linea), f) != NULL) {
        linea[strcspn(linea, "\r\n")] = '\0';

        if (strlen(linea) == 0) continue;

        char id_con[4], n_orig[31], n_dest[31], est[11], condicion[5];
        int id_orig, id_dest;

        if (sscanf(linea, "%3[^-]-%d-%30[^-]-%d-%30[^-]-%10[^-]-%4[^\n]",
                   id_con, &id_orig, n_orig, &id_dest, n_dest, est, condicion) == 7) {

            array_conexiones = realloc(array_conexiones, (*total_conexiones + 1) * sizeof(Conexion*));

            Conexion* nueva_conexion = malloc(sizeof(Conexion));

            strcpy(nueva_conexion->id_conexion, id_con);
            nueva_conexion->id_origen = id_orig;
            strcpy(nueva_conexion->nomb_origen, n_orig);
            nueva_conexion->id_destino = id_dest;
            strcpy(nueva_conexion->nomb_destino, n_dest);
            strcpy(nueva_conexion->estado, est);
            strcpy(nueva_conexion->cond, condicion);

            array_conexiones[*total_conexiones] = nueva_conexion;
            (*total_conexiones)++;
        } else {
            printf("Aviso: Formato incorrecto en la línea de conexiones: %s\n", linea);
        }
    }

    fclose(f);
    return array_conexiones;
}

/*
  Función:      destruir_lista_conexiones
  Descripcion:  Libera la memoria dinámica reservada para el array
                de conexiones
*/
void destruir_lista_conexiones(Conexion** lista_conexiones, int total_conexiones) {
    if (lista_conexiones != NULL) {
        for (int i = 0; i < total_conexiones; i++) {
            if (lista_conexiones[i] != NULL) {
                free(lista_conexiones[i]);
            }
        }
        free(lista_conexiones);
    }
}


/*
  Función:      cargar_objetos
  Descripcion:  Lee el fichero .txt donde esta guardada la
                información sobre los objetos y la carga
                en un array dinámico
  Devuelve:     Array dinámico con los objetos cargados.
*/
Objeto** cargar_objetos(char* ruta_fichero, int* total_objetos) {
    FILE* f = fopen(ruta_fichero, "r");
    if (f == NULL) {
        printf("Error: No se pudo abrir el fichero %s\n", ruta_fichero);
        *total_objetos = 0;
        return NULL;
    }

    Objeto** array_objetos = NULL;
    *total_objetos = 0;

    char linea[256];

    while (fgets(linea, sizeof(linea), f) != NULL) {
        linea[strcspn(linea, "\r\n")] = '\0';

        if (strlen(linea) == 0) continue;

        char id[5], nombre[16], descripcion[51], localizacion[15];

        if (sscanf(linea, "%4[^-]-%15[^-]-%50[^-]-%14[^\n]",
                   id, nombre, descripcion, localizacion) == 4) {

            array_objetos = realloc(array_objetos, (*total_objetos + 1) * sizeof(Objeto*));

            Objeto* nuevo_objeto = malloc(sizeof(Objeto));

            strcpy(nuevo_objeto->id_obj, id);
            strcpy(nuevo_objeto->nomb_obj, nombre);
            strcpy(nuevo_objeto->descrip, descripcion);
            strcpy(nuevo_objeto->localiz, localizacion);

            array_objetos[*total_objetos] = nuevo_objeto;
            (*total_objetos)++;
        } else {
            printf("Aviso: Formato incorrecto en la línea de objetos: %s\n", linea);
        }
    }

    fclose(f);
    return array_objetos;
}

/*
  Función:      destruir_lista_objetos
  Descripcion:  Libera la memoria dinámica reservada para el array
                de objetos
*/
void destruir_lista_objetos(Objeto** lista_objetos, int total_objetos) {
    if (lista_objetos != NULL) {
        for (int i = 0; i < total_objetos; i++) {
            if (lista_objetos[i] != NULL) {
                free(lista_objetos[i]);
            }
        }
        free(lista_objetos);
    }
}


/*
  Función:      cargar_puzles
  Descripcion:  Lee el fichero .txt donde esta guardada la
                información sobre los puzles y la carga
                en un array dinámico
  Devuelve:     Array dinámico con los puzles cargados.
*/
Puzle** cargar_puzles(char* ruta_fichero, int* total_puzles) {
    FILE* f = fopen(ruta_fichero, "r");
    if (f == NULL) {
        printf("Error: No se pudo abrir el fichero %s\n", ruta_fichero);
        *total_puzles = 0;
        return NULL;
    }

    Puzle** array_puzles = NULL;
    *total_puzles = 0;

    char linea[300];

    while (fgets(linea, sizeof(linea), f) != NULL) {
        linea[strcspn(linea, "\r\n")] = '\0';

        if (strlen(linea) == 0) continue;

        char id[4], nombre[16], tipo_puz[15], descripcion[151], solucion[51];
        int sala_id;

        if (sscanf(linea, "%3[^-]-%15[^-]-%d-%14[^-]-%150[^-]-%50[^\n]",
                   id, nombre, &sala_id, tipo_puz, descripcion, solucion) == 6) {

            array_puzles = realloc(array_puzles, (*total_puzles + 1) * sizeof(Puzle*));

            Puzle* nuevo_puzle = malloc(sizeof(Puzle));

            strcpy(nuevo_puzle->id_puzle, id);
            strcpy(nuevo_puzle->nomb_puz, nombre);
            nuevo_puzle->id_sala = sala_id;
            strcpy(nuevo_puzle->tipo, tipo_puz);
            strcpy(nuevo_puzle->descrip, descripcion);
            strcpy(nuevo_puzle->sol, solucion);

            array_puzles[*total_puzles] = nuevo_puzle;
            (*total_puzles)++;
        } else {
            printf("Aviso: Formato incorrecto en la línea de puzles: %s\n", linea);
        }
    }

    fclose(f);
    return array_puzles;
}

/*
  Función:      destruir_lista_puzles
  Descripcion:  Libera la memoria dinámica reservada para el array
                de puzles
*/
void destruir_lista_puzles(Puzle** lista_puzles, int total_puzles) {
        if (lista_puzles != NULL) {
        for (int i = 0; i < total_puzles; i++) {
            if (lista_puzles[i] != NULL) {
                free(lista_puzles[i]);
            }
        }
        free(lista_puzles);
    }
}
