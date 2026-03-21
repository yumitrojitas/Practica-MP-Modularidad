#include "Ficheros.h"

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
        int bytes_leidos = 0; // Para rastrear nuestra posición en la cadena

        // Primero leemos los campos fijos.
        // %n guarda en bytes_leidos cuántos caracteres se han procesado hasta ahí.
        if (sscanf(linea, "%d-%20[^-]-%10[^-]-%8[^-]%n", &id, nombre, user, pass, &bytes_leidos) == 4) {

            array_jugadores = realloc(array_jugadores, (*num_jugadores + 1) * sizeof(Jugador*));

            // Asignación de memoria básica
            Jugador* nuevo_jugador = malloc(sizeof(Jugador));
            nuevo_jugador->Id_jugador = id;
            strcpy(nuevo_jugador->Nomb_jugador, nombre);
            strcpy(nuevo_jugador->Jugador, user);
            strcpy(nuevo_jugador->Contrasenna, pass);
            nuevo_jugador->Inventario = NULL;
            nuevo_jugador->Num_objetos = 0;

            // Avanzamos el puntero a la posición donde se quedó sscanf
            char* ptr_restante = linea + bytes_leidos;
            char id_obj[5];
            int avance = 0;

            // Bucle para leer el inventario dinámico: lee "-OB01", "-OB02", etc.
            while (sscanf(ptr_restante, "-%4[^-]%n", id_obj, &avance) == 1) {
                // Redimensionamos el array de cadenas (inventario)
                nuevo_jugador->Inventario = realloc(nuevo_jugador->Inventario, (nuevo_jugador->Num_objetos + 1) * sizeof(char*));

                // Reservamos memoria para la cadena en sí y la copiamos
                nuevo_jugador->Inventario[nuevo_jugador->Num_objetos] = malloc(5 * sizeof(char));
                strcpy(nuevo_jugador->Inventario[nuevo_jugador->Num_objetos], id_obj);

                nuevo_jugador->Num_objetos++;

                // Avanzamos el puntero por la cadena para la siguiente iteración
                ptr_restante += avance;
            }

            // Guardamos el jugador procesado en nuestro array principal
            array_jugadores[*num_jugadores] = nuevo_jugador;
            (*num_jugadores)++;
        }
    }

    fclose(f);
    return array_jugadores;
}

void guardar_jugadores(char* ruta_fichero, Jugador** lista_jugadores, int num_jugadores) {
    // Si la lista está vacía, no hacemos nada para no borrar el archivo por error
    if (lista_jugadores == NULL || num_jugadores == 0) {
        return;
    }

    // Abrimos el archivo en modo "w" (write) para sobreescribirlo
    FILE* f = fopen(ruta_fichero, "w");
    if (f == NULL) {
        printf("Error: No se pudo abrir %s para guardar los datos.\n", ruta_fichero);
        return;
    }

    // Recorremos todos los jugadores de nuestro array
    for (int i = 0; i < num_jugadores; i++) {
        Jugador* j = lista_jugadores[i];

        // Escribimos los campos fijos.
        // Usamos %02d para asegurarnos de que el ID siempre tenga 2 dígitos (ej: 01, 02)
        fprintf(f, "%02d-%s-%s-%s", j->Id_jugador, j->Nomb_jugador, j->Jugador, j->Contrasenna);

        // Recorremos el inventario del jugador y lo añadimos a la línea
        for (int k = 0; k < j->Num_objetos; k++) {
            fprintf(f, "-%s", j->Inventario[k]);
        }

        // Metemos el salto de línea al final para separar de cara al siguiente jugador
        fprintf(f, "\n");
    }

    fclose(f);
    printf("Datos de los jugadores guardados correctamente.\n");
}

void destruir_lista_jugadores(Jugador** lista_jugadores, int total_jugadores) {
    // 1. Comprobamos que la lista principal exista
    if (lista_jugadores != NULL) {

        // 2. Recorremos todos los jugadores de la lista
        for (int i = 0; i < total_jugadores; i++) {

            if (lista_jugadores[i] != NULL) {
                // 3. Si el jugador tiene objetos en el inventario, los liberamos primero
                if (lista_jugadores[i]->Inventario != NULL) {
                    for (int j = 0; j < lista_jugadores[i]->Num_objetos; j++) {
                        free(lista_jugadores[i]->Inventario[j]); // Libera la palabra (ej: "OB01")
                    }
                    free(lista_jugadores[i]->Inventario); // Libera el array de punteros del inventario
                }

                // 4. Liberamos la estructura del jugador en sí
                free(lista_jugadores[i]);
            }
        }

        // 5. Finalmente, liberamos el gran array contenedor principal
        free(lista_jugadores);
    }
}


Partida** cargar_partidas(char* ruta_fichero, int* total_partidas) {
    FILE* f = fopen(ruta_fichero, "r");
    if (f == NULL) {
        printf("Aviso: No se encontro %s. Se creara uno nuevo al guardar.\n", ruta_fichero);
        *total_partidas = 0;
        return NULL;
    }

    Partida** array_partidas = NULL;
    *total_partidas = 0;

    // Este puntero nos dirá a qué partida le estamos metiendo datos ahora mismo
    Partida* p_actual = NULL;
    char linea[256];

    while (fgets(linea, sizeof(linea), f) != NULL) {
        // Limpiamos los saltos de línea al final
        linea[strcspn(linea, "\r\n")] = '\0';

        // Ignoramos líneas vacías
        if (strlen(linea) == 0) continue;

        // 1. Detectamos si empieza un nuevo jugador
        if (strncmp(linea, "JUGADOR:", 8) == 0) {
            array_partidas = realloc(array_partidas, (*total_partidas + 1) * sizeof(Partida*));
            p_actual = malloc(sizeof(Partida));

            // Inicializamos el nuevo bloque
            sscanf(linea, "JUGADOR: %d", &p_actual->id_jugador);
            p_actual->id_sala_actual = 0; // Valor por defecto

            p_actual->objetos_modificados = NULL;
            p_actual->num_objetos = 0;
            p_actual->conexiones_modificadas = NULL;
            p_actual->num_conexiones = 0;
            p_actual->puzles_modificados = NULL;
            p_actual->num_puzles = 0;

            array_partidas[*total_partidas] = p_actual;
            (*total_partidas)++;
        }
        // 2. Si es SALA, actualizamos la sala del p_actual
        else if (strncmp(linea, "SALA:", 5) == 0 && p_actual != NULL) {
            sscanf(linea, "SALA: %d", &p_actual->id_sala_actual);
        }
        // 3. Si es OBJETO, añadimos a la lista dinámica de objetos
        else if (strncmp(linea, "OBJETO:", 7) == 0 && p_actual != NULL) {
            p_actual->objetos_modificados = realloc(p_actual->objetos_modificados,
                                            (p_actual->num_objetos + 1) * sizeof(EstadoObjeto));

            // Formato: OBJETO: OB01-Inventario o OBJETO: OB02-03
            sscanf(linea, "OBJETO: %4[^-]-%14s",
                   p_actual->objetos_modificados[p_actual->num_objetos].id_obj,
                   p_actual->objetos_modificados[p_actual->num_objetos].localizacion);
            p_actual->num_objetos++;
        }
        // 4. Si es CONEXIÓN (comprobamos "CONEXI" para evitar bugs con la 'Ó')
        else if (strncmp(linea, "CONEXI", 6) == 0 && p_actual != NULL) {
            p_actual->conexiones_modificadas = realloc(p_actual->conexiones_modificadas, (p_actual->num_conexiones + 1) * sizeof(EstadoConexion));

            // Formato: CONEXIÓN: 04-Activa o CONEXIÓN: C01-Activa
            // Buscamos a partir de los dos puntos + espacio
            char* inicio_datos = strstr(linea, ": ");
            if (inicio_datos != NULL) {
                sscanf(inicio_datos + 2, "%3[^-]-%10s", p_actual->conexiones_modificadas[p_actual->num_conexiones].id_conexion, p_actual->conexiones_modificadas[p_actual->num_conexiones].estado);
                p_actual->num_conexiones++;
            }
        }
        // 5. Si es PUZLE
        else if (strncmp(linea, "PUZLE:", 6) == 0 && p_actual != NULL) {
            p_actual->puzles_modificados = realloc(p_actual->puzles_modificados,
                                           (p_actual->num_puzles + 1) * sizeof(EstadoPuzle));

            // Formato: PUZLE: P01-Resuelto
            sscanf(linea, "PUZLE: %3[^-]-%10s",
                   p_actual->puzles_modificados[p_actual->num_puzles].id_puzle,
                   p_actual->puzles_modificados[p_actual->num_puzles].estado);
            p_actual->num_puzles++;
        }
    }

    fclose(f);
    return array_partidas;
}

void guardar_partidas(char* ruta_fichero, Partida** lista_partidas, int total_partidas) {
    if (lista_partidas == NULL || total_partidas == 0) {
        return; // Nada que guardar
    }

    // Abrimos en modo escritura ("w") para sobreescribir el fichero con los datos nuevos
    FILE* f = fopen(ruta_fichero, "w");
    if (f == NULL) {
        printf("Error: No se pudo abrir %s para guardar la partida.\n", ruta_fichero);
        return;
    }

    for (int i = 0; i < total_partidas; i++) {
        Partida* p = lista_partidas[i];

        // 1. Guardamos el Jugador y la Sala actual
        fprintf(f, "JUGADOR: %02d\n", p->id_jugador);
        fprintf(f, "SALA: %02d\n", p->id_sala_actual);

        // 2. Guardamos la lista de objetos modificados [cite: 99]
        for (int j = 0; j < p->num_objetos; j++) {
            fprintf(f, "OBJETO: %s-%s\n",
                    p->objetos_modificados[j].id_obj,
                    p->objetos_modificados[j].localizacion);
        }

        // 3. Guardamos la lista de conexiones desbloqueadas [cite: 100]
        for (int j = 0; j < p->num_conexiones; j++) {
            fprintf(f, "CONEXIÓN: %s-%s\n",
                    p->conexiones_modificadas[j].id_conexion,
                    p->conexiones_modificadas[j].estado);
        }

        // 4. Guardamos la lista de puzles resueltos [cite: 103]
        for (int j = 0; j < p->num_puzles; j++) {
            fprintf(f, "PUZLE: %s-%s\n",
                    p->puzles_modificados[j].id_puzle,
                    p->puzles_modificados[j].estado);
        }
    }

    fclose(f);
    printf("Progreso de todas las partidas guardado correctamente en %s.\n", ruta_fichero);
}

void destruir_partidas(Partida** lista_partidas, int total_partidas) {
    if (lista_partidas != NULL) {
        // Recorremos cada partida guardada
        for (int i = 0; i < total_partidas; i++) {
            if (lista_partidas[i] != NULL) {
                // 1. Liberamos las sub-listas dinámicas si existen
                if (lista_partidas[i]->objetos_modificados != NULL) {
                    free(lista_partidas[i]->objetos_modificados);
                }
                if (lista_partidas[i]->conexiones_modificadas != NULL) {
                    free(lista_partidas[i]->conexiones_modificadas);
                }
                if (lista_partidas[i]->puzles_modificados != NULL) {
                    free(lista_partidas[i]->puzles_modificados);
                }

                // 2. Liberamos la estructura principal de la partida
                free(lista_partidas[i]);
            }
        }
        // 3. Finalmente, liberamos el array contenedor principal
        free(lista_partidas);
    }
}


Sala** cargar_salas(char* ruta_fichero, int* total_salas) {
    // Abrimos el fichero en modo lectura
    FILE* f = fopen(ruta_fichero, "r");
    if (f == NULL) {
        printf("Error: No se pudo abrir el fichero %s\n", ruta_fichero);
        *total_salas = 0;
        return NULL;
    }

    Sala** array_salas = NULL;
    *total_salas = 0;

    // Un búfer grande, ya que la descripción sola puede tener 150 caracteres
    char linea[256];

    while (fgets(linea, sizeof(linea), f) != NULL) {
        // Limpiamos los saltos de línea (\r o \n) que puedan venir al final
        linea[strcspn(linea, "\r\n")] = '\0';

        // Evitamos procesar líneas vacías
        if (strlen(linea) == 0) continue;

        int id;
        char nombre[31], tipo[10], descripcion[151];

        // Leemos los 4 campos. El último campo lee todo hasta el final de la cadena
        if (sscanf(linea, "%d-%30[^-]-%9[^-]-%150[^\n]", &id, nombre, tipo, descripcion) == 4) {

            // Agrandamos el array principal para que quepa un puntero más
            array_salas = realloc(array_salas, (*total_salas + 1) * sizeof(Sala*));

            // Reservamos memoria para la estructura de la sala en sí
            Sala* nueva_sala = malloc(sizeof(Sala));

            // Asignamos los datos leídos a la estructura
            nueva_sala->id_sala = id;
            strcpy(nueva_sala->nomb_sala, nombre);
            strcpy(nueva_sala->tipo, tipo);
            strcpy(nueva_sala->descrip, descripcion);

            // Guardamos la sala en el array y aumentamos el contador
            array_salas[*total_salas] = nueva_sala;
            (*total_salas)++;
        } else {
            printf("Aviso: Formato incorrecto en la línea: %s\n", linea);
        }
    }

    fclose(f);
    return array_salas;
}

void destruir_lista_salas(Sala** lista_salas, int total_salas) {
    if (lista_salas != NULL) {
        // Recorremos el array liberando cada estructura de Sala individual
        for (int i = 0; i < total_salas; i++) {
            if (lista_salas[i] != NULL) {
                free(lista_salas[i]);
            }
        }
        // Finalmente, liberamos el array de punteros
        free(lista_salas);
    }
}


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
        // Limpiamos los saltos de línea (\r o \n)
        linea[strcspn(linea, "\r\n")] = '\0';

        if (strlen(linea) == 0) continue;

        char id_con[4], n_orig[31], n_dest[31], est[11], condicion[5];
        int id_orig, id_dest;

        // Leemos los 7 campos basados en el ejemplo: C01-01-Conserjería-02-Escaleras-Bloqueada-OB01
        if (sscanf(linea, "%3[^-]-%d-%30[^-]-%d-%30[^-]-%10[^-]-%4[^\n]",
                   id_con, &id_orig, n_orig, &id_dest, n_dest, est, condicion) == 7) {

            // Agrandamos el array principal
            array_conexiones = realloc(array_conexiones, (*total_conexiones + 1) * sizeof(Conexion*));

            // Reservamos memoria para la conexión
            Conexion* nueva_conexion = malloc(sizeof(Conexion));

            // Asignamos los datos
            strcpy(nueva_conexion->id_conexion, id_con);
            nueva_conexion->id_origen = id_orig;
            strcpy(nueva_conexion->nomb_origen, n_orig);
            nueva_conexion->id_destino = id_dest;
            strcpy(nueva_conexion->nomb_destino, n_dest);
            strcpy(nueva_conexion->estado, est);
            strcpy(nueva_conexion->cond, condicion);

            // Guardamos en el array y aumentamos contador
            array_conexiones[*total_conexiones] = nueva_conexion;
            (*total_conexiones)++;
        } else {
            printf("Aviso: Formato incorrecto en la línea de conexiones: %s\n", linea);
        }
    }

    fclose(f);
    return array_conexiones;
}

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


Objeto** cargar_objetos(char* ruta_fichero, int* total_objetos) {
    FILE* f = fopen(ruta_fichero, "r");
    if (f == NULL) {
        printf("Error: No se pudo abrir el fichero %s\n", ruta_fichero);
        *total_objetos = 0;
        return NULL;
    }

    Objeto** array_objetos = NULL;
    *total_objetos = 0;

    // Búfer suficiente para los campos del objeto
    char linea[256];

    while (fgets(linea, sizeof(linea), f) != NULL) {
        // Limpiamos los saltos de línea (\r o \n)
        linea[strcspn(linea, "\r\n")] = '\0';

        if (strlen(linea) == 0) continue;

        char id[5], nombre[16], descripcion[51], localizacion[15];

        // Leemos los 4 campos. El último lee todo hasta el final de la cadena
        // Formato esperado: OB01-Tarjeta Laboratorio-Tarjeta...-03 [cite: 82]
        if (sscanf(linea, "%4[^-]-%15[^-]-%50[^-]-%14[^\n]",
                   id, nombre, descripcion, localizacion) == 4) {

            // Agrandamos el array principal
            array_objetos = realloc(array_objetos, (*total_objetos + 1) * sizeof(Objeto*));

            // Reservamos memoria para la estructura del objeto
            Objeto* nuevo_objeto = malloc(sizeof(Objeto));

            // Copiamos los datos leídos
            strcpy(nuevo_objeto->id_obj, id);
            strcpy(nuevo_objeto->nomb_obj, nombre);
            strcpy(nuevo_objeto->descrip, descripcion);
            strcpy(nuevo_objeto->localiz, localizacion);

            // Guardamos en el array y aumentamos el contador
            array_objetos[*total_objetos] = nuevo_objeto;
            (*total_objetos)++;
        } else {
            printf("Aviso: Formato incorrecto en la línea de objetos: %s\n", linea);
        }
    }

    fclose(f);
    return array_objetos;
}

void destruir_lista_objetos(Objeto** lista_objetos, int total_objetos) {
    if (lista_objetos != NULL) {
        for (int i = 0; i < total_objetos; i++) {
            if (lista_objetos[i] != NULL) {
                // Como no hay listas dinámicas dentro del struct Objeto, un solo free basta
                free(lista_objetos[i]);
            }
        }
        // Finalmente, liberamos el array contenedor
        free(lista_objetos);
    }
}


Puzle** cargar_puzles(char* ruta_fichero, int* total_puzles) {
    FILE* f = fopen(ruta_fichero, "r");
    if (f == NULL) {
        printf("Error: No se pudo abrir el fichero %s\n", ruta_fichero);
        *total_puzles = 0;
        return NULL;
    }

    Puzle** array_puzles = NULL;
    *total_puzles = 0;

    // Búfer amplio ya que la descripción puede ser larga (150) + solución (50)
    char linea[300];

    while (fgets(linea, sizeof(linea), f) != NULL) {
        // Limpiamos los saltos de línea (\r o \n)
        linea[strcspn(linea, "\r\n")] = '\0';

        if (strlen(linea) == 0) continue;

        char id[4], nombre[16], tipo_puz[15], descripcion[151], solucion[51];
        int sala_id;

        // Leemos los 6 campos. El último lee todo hasta el final de la línea.
        if (sscanf(linea, "%3[^-]-%15[^-]-%d-%14[^-]-%150[^-]-%50[^\n]",
                   id, nombre, &sala_id, tipo_puz, descripcion, solucion) == 6) {

            // Agrandamos el array principal
            array_puzles = realloc(array_puzles, (*total_puzles + 1) * sizeof(Puzle*));

            // Reservamos memoria para el puzle
            Puzle* nuevo_puzle = malloc(sizeof(Puzle));

            // Copiamos los datos leídos a la estructura
            strcpy(nuevo_puzle->id_puzle, id);
            strcpy(nuevo_puzle->nomb_puz, nombre);
            nuevo_puzle->id_sala = sala_id;
            strcpy(nuevo_puzle->tipo, tipo_puz);
            strcpy(nuevo_puzle->descrip, descripcion);
            strcpy(nuevo_puzle->sol, solucion);

            // Guardamos el puzle en el array y aumentamos el contador
            array_puzles[*total_puzles] = nuevo_puzle;
            (*total_puzles)++;
        } else {
            printf("Aviso: Formato incorrecto en la línea de puzles: %s\n", linea);
        }
    }

    fclose(f);
    return array_puzles;
}

void destruir_lista_puzles(Puzle** lista_puzles, int total_puzles) {
    if (lista_puzles != NULL) {
        for (int i = 0; i < total_puzles; i++) {
            if (lista_puzles[i] != NULL) {
                // Liberación directa, ya que no hay arrays dinámicos internos
                free(lista_puzles[i]);
            }
        }
        // Finalmente, liberamos el contenedor general
        free(lista_puzles);
    }
}
