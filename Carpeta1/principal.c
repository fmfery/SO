#include <stdio.h>
#include <curses.h>
#include <unistd.h>
#include <string.h>
#include "interprete2.h"

int main() {
    char comando[50]; // para guardar el comando principal 
    char argumento[100];  // para guardar un posible argumento 
  // === Iniciar ventana ncurses ===
  WINDOW *window = NULL;
  window = initscr();   // inicializar pantalla

  // Encabezado
  mvprintw(20, 0, "=== Bienvenido al prompt ===");
  mvprintw(21, 0, "Comandos disponibles:");
  mvprintw(22, 0, "  ejecutar <archivo.asm>");
  mvprintw(23, 0, "  salir");
  refresh();

  while (1) {
    mvprintw(17, 0, "$ "); // prompt en fila fija
    clrtoeol(); // limpiar el resto de la línea
    move(17, 2); // mover el cursor después del prompt
    refresh();

    argumento[0] = '\0'; // limpiar el argumento
    comando[0] = '\0'; // limpiar el comando
    scanw("%49s %99s", comando, argumento);

    if (strcmp(comando, "salir") == 0) {
      mvprintw(15, 0, "Saliendo del prompt...");
        clrtoeol();
        refresh();
        sleep(1);
        break;
    } 
    else if (strcmp(comando, "ejecutar") == 0) {
      if (argumento[0] == '\0') {
        mvprintw(15, 0, "Error: Falta el nombre del archivo");
        clrtoeol();
        refresh();
      } else {
        clrtoeol();
        refresh();
        mvprintw(15, 0, "Ejecutando archivo: %s", argumento);
        ejecutar_archivo(argumento);
      }
    } 
    else {
      mvprintw(15, 0, "Error: Comando no reconocido: %s", comando);
      clrtoeol();
      refresh();
    }
  }

  endwin();
  return 0;
}
