#include <stdio.h>
#include <string.h>
#include "interprete2.h"

int main() {
    char comando[50];       // para guardar el comando principal 
    char argumento[100];    // para guardar un posible argumento 
    
    printf("=== Bienvenido al prompt ===\n");
    printf("Comandos disponibles:\n");
    printf("  ejecutar <archivo.asm>\n");
    printf("  salir\n\n");

    while (1) {
        printf("$ ");
        scanf("%49s", comando);

        if (strcmp(comando, "salir") == 0) {
            printf("Saliendo del programa...\n");
            break;
        } 
        else if (strcmp(comando, "ejecutar") == 0) {
            printf("Archivo a ejecutar > ");
            scanf("%99s", argumento);  // leemos el nombre del archivo
//-----------Llamamos a la funcion para que haga las operaciones
            ejecutar_archivo(argumento);
            if (argumento[0]=='\0' || strcmp(argumento,".asm")==0)       //Comparo por si es .asm, falta nombre de archivo
            {
                printf("Error: Falta el nombre del archivo\n");
            }
            
        } 
        else {
            printf("Error: Comando no reconocido: %s\n", comando);
        }
    }

    return 0;
}
