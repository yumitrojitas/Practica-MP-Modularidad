#include "Menu.h"

void bucle_partida(Jugador* jugador, int partida_cargada){
    Partida** lista_partidas = NULL;
    int num_partidas = 0;

    Partida* partida_actual = NULL;
    int opcion = 0;

    lista_partidas = cargar_partidas("Partida.txt", &num_partidas);
    partida_actual = buscar_partida_actual(lista_partidas, jugador, num_partidas);

    // 1. PREPARACIÓN DE LA PARTIDA
    if (partida_cargada == 1) {
        printf("\nCargando la situación almacenada en Partida.txt...\n");


    } else {
        printf("\nIniciando una aventura desde cero...\n");

        if (partida_actual == NULL) {
            lista_partidas = realloc(lista_partidas, (num_partidas + 1) * sizeof(Partida*));
            partida_actual = malloc(sizeof(Partida));

            partida_actual->id_jugador = jugador->Id_jugador;
            partida_actual->id_sala_actual = 0;

            partida_actual->objetos_modificados = NULL;
            partida_actual->num_objetos = 0;
            partida_actual->conexiones_modificadas = NULL;
            partida_actual->num_conexiones = 0;
            partida_actual->puzles_modificados = NULL;
            partida_actual->num_puzles = 0;

            lista_partidas[num_partidas] = partida_actual;
            num_partidas++;
        }
        else {
            partida_actual->id_sala_actual = 0;
            if (partida_actual->objetos_modificados != NULL) {
                free(partida_actual->objetos_modificados);
            }
            if (partida_actual->conexiones_modificadas != NULL) {
                free(partida_actual->conexiones_modificadas);
            }
            if (partida_actual->puzles_modificados != NULL) {
                free(partida_actual->puzles_modificados);
            }
        }
    }

    do {
        // En el futuro, aquí sacarás el nombre de la sala actual dinámicamente
        printf("               Sala: (Nombre de la sala actual)\n");
        printf("-----------------------------------------------\n");
        printf(" 1. Describir sala\n 2. Examinar (objetos y salidas)\n 3. Entrar en otra sala\n 4. Coger objeto\n 5. Soltar ojeto\n 6. Inventario\n 7. Usar objeto\n 8. Resolver puzle / introducir codigo\n 9. Guardar partida\n 10. Volver\n\nElegir opcion: ");

        scanf("%i", &opcion);

        // 3. LÓGICA DE LAS ACCIONES
        switch(opcion) {
            case 1:
                printf("\nDescribiendo la sala...\n");
                break;
            // ... resto de casos ...
            case 9:
                printf("\nGuardando partida en Partida.txt...\n");
                // Llamada a función de guardar
                break;
            case 10:
                // La opción 10 permitirá al jugador regresar al menú principal[cite: 155].
                printf("\nAbandonando la sala... Volviendo al menú principal.\n");
                break;
            default:
                printf("\nAcción no válida.\n");
        }

    } while (opcion != 10);
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

Partida* buscar_partida_actual(Partida** lista_partidas, Jugador* jugador, int num_partidas) {
    if (lista_partidas == NULL) return NULL;

    for (int i = 0; i < num_partidas; i++) {
        if (lista_partidas[i]->id_jugador == jugador->Id_jugador) {
            return lista_partidas[i];
        }
    }

    return NULL;
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
