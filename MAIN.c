#include <stdio.h>
#include <string.h>

#include "Jugador.h"
#include "Menu.h"

int main()
{
    //Declaracion de variables
    Jugador** lista_jugadores = NULL;
    int total_jugadores = 0;
    Jugador* jugador_actual = NULL;

    char usuario[30], contrasenna[30], nombreJugador[30];

    int eleccion;

    //Inicio de la Partida
    lista_jugadores = cargar_jugadores("Jugadores.txt", &total_jugadores);

    printf("Usuario: ");
    if(fgets(usuario, sizeof(usuario), stdin) != NULL)
    {
        usuario[strcspn(usuario, "\n")] = '\0';
    }
    printf("Contraseña: ");
    if(fgets(contrasenna, sizeof(contrasenna), stdin) != NULL)
    {
        contrasenna[strcspn(contrasenna, "\n")] = '\0';
    }
    printf("\n");

    jugador_actual = hacer_login(lista_jugadores, total_jugadores, usuario, contrasenna);

    if(jugador_actual == NULL)
    {
        printf("Jugador no existe\n");
        printf("Nombre: ");
        if(fgets(nombreJugador, sizeof(nombreJugador), stdin) != NULL)
        {
            nombreJugador[strcspn(nombreJugador, "\n")] = '\0';
        }

        jugador_actual = crear_jugador(nombreJugador, usuario, contrasenna, lista_jugadores, &total_jugadores);

        guardar_jugadores("Jugadores.txt", lista_jugadores, total_jugadores);
        printf("Jugador nuevo creado\n");
    }

    printf("===============================================\n");
    printf("        BIENVENIDO A ESI ESCAPE                \n");
    printf("===============================================\n");

    do{
        printf("MENU:\n1. Nueva partida\n2. Cargar partida\n3. Salir\n\nElgir opcion: ");
        do{
            scanf("%i", &eleccion);
        }while(eleccion < 1 || eleccion > 3);

        switch(eleccion)
        {
        case 1:
            bucle_partida(jugador_actual, 0);
            break;
        case 2:
            bucle_partida(jugador_actual, 1);
            break;
        default:
            break;
        }
    }while(eleccion != 3);

    return 0;
}
