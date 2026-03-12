#include "jugador.h"

Jugador* crear_jugador(char* nombre, char* user, char* pass, Jugador** lista_jugadores, int* num_jugadores) {
    Jugador* nuevo_jugador = (Jugador*)malloc(sizeof(Jugador));
    if (nuevo_jugador == NULL) {
        printf("Error: No se pudo reservar memoria para el jugador.\n");
        return NULL;
    }

    nuevo_jugador->Id_jugador = id_mayor(lista_jugadores, *num_jugadores) + 1;

    strncpy(nuevo_jugador->Nomb_jugador, nombre, 20);
    nuevo_jugador->Nomb_jugador[20] = '\0';

    strncpy(nuevo_jugador->Jugador, user, 10);
    nuevo_jugador->Jugador[10] = '\0';

    strncpy(nuevo_jugador->Contrasenna, pass, 8);
    nuevo_jugador->Contrasenna[8] = '\0';

    // Inicializamos el inventario vacío
    nuevo_jugador->Inventario = NULL;
    nuevo_jugador->Num_objetos = 0;

    lista_jugadores = realloc(lista_jugadores, (*num_jugadores + 1) * sizeof(Jugador*));

    lista_jugadores[*num_jugadores] = nuevo_jugador;
    (*num_jugadores)++;
    return nuevo_jugador;
}

void destruir_jugador(Jugador* j) {
    if (j != NULL) {
        // 1. Liberar el inventario si tiene objetos
        if (j->Inventario != NULL) {
            for (int i = 0; i < j->Num_objetos; i++) {
                free(j->Inventario[i]); // Libera la cadena de cada ID de objeto
            }
            free(j->Inventario); // Libera el array de punteros
        }

        // 2. Liberar la estructura principal
        free(j);
    }
}

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

Jugador* hacer_login(Jugador** lista_jugadores, int num_jugadores, char* user, char* pass) {
    if (lista_jugadores == NULL) return NULL;

    // Recorremos el array buscando coincidencias
    for (int i = 0; i < num_jugadores; i++) {
        if (strcmp(lista_jugadores[i]->Jugador, user) == 0 && strcmp(lista_jugadores[i]->Contrasenna, pass) == 0) {
            // Usuario y contraseña coinciden [cite: 112, 113]
            return lista_jugadores[i];
        }
    }

    // Si termina el bucle y no lo encuentra
    return NULL;
}

int id_mayor(Jugador** lista_jugadores, int num_jugadores){
    if (lista_jugadores == NULL) return 0;

    int max = 0;

    for (int i = 0; i < num_jugadores; i++)
    {
        if(lista_jugadores[i]->Id_jugador > max)
            max = lista_jugadores[i]->Id_jugador;
    }

    return max;
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
