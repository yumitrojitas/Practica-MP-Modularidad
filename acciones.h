#ifndef __ACCIONES_H__
#define __ACCIONES_H__
#include "jugador.h"
#include "salas.h"

/* Precondición: jugador inicializado; sala actual válida con objeto id_obj.

   Postcondición: el objeto pasa del array de la sala al inventario del jugador. */

void coger_objeto(Jugador *jugador, Sala *sala_actual, char *id_obj);


/* Precondición: jugador con id_obj en su inventario; sala_actual válida.

   Postcondición: el objeto pasa del inventario a la sala actual. */

void soltar_objeto(Jugador *jugador, Sala *sala_actual, char *id_obj);


/* Precondición: jugador y salas inicializadas; id_destino es una sala conectada.

   Postcondición: si la conexión está activa, actualiza la sala actual del jugador.
   Devuelve 1 si el movimiento tuvo éxito, 0 si la conexión está bloqueada. */

int entrar_sala(Jugador *jugador, Sala *salas, int id_destino);


/* Precondición: jugador en sala con un puzle pendiente de id_puzle.

   Postcondición: si la solución es correcta, marca el puzle como resuelto
   y desbloquea la conexión asociada. Devuelve 1 si es correcto, 0 si no. */

int resolver_puzle(Jugador *jugador, Sala *salas, char *id_puzle, char *solucion);


/* Precondición: jugador con id_obj en inventario; conexión adyacente bloqueada.

   Postcondición: si el objeto es la condición de desbloqueo, cambia el estado
   de la conexión a Activa. Devuelve 1 si se aplicó, 0 si no. */

int usar_objeto(Jugador *jugador, Sala *salas, char *id_obj);


/* Precondición: sala_actual válida.

   Postcondición: muestra por pantalla la descripción de la sala
   e indica si es la sala SALIDA. */

void describir_sala(Sala *sala_actual);


/* Precondición: sala_actual válida con sus conexiones cargadas.

   Postcondición: lista por pantalla los objetos de la sala
   y las salidas indicando si están bloqueadas o no. */

void examinar_sala(Sala *sala_actual);


/* Precondición: jugador inicializado con inventario cargado.

   Postcondición: muestra por pantalla los objetos del inventario
   con sus descripciones. */
   
void mostrar_inventario(Jugador *jugador);

#endif