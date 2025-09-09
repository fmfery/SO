#ifndef INTERPRETE_H 
#define INTERPRETE_H
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//---Variables globales----
int PC=1;
int Ax=0;
int Bx=0;
int Cx=0;
int Dx=0;
int ID=0;

/*
    Comprueba que la cadena sean solo dígitos
*/
int Numeros(char *numero){
    if(numero[0] == '\0') return 0;   
    for (int i = 0; numero[i] != '\0'; i++){
        if (!isdigit(numero[i])){
            return 0;  
        }
    }
    return 1; 
}

/* 
    Comprobar si es correcto el registro de las letras
*/
int registroValido(char *letrasM){
   return(strcmp(letrasM,"Ax")==0 || strcmp(letrasM,"Bx")==0 || 
          strcmp(letrasM,"Cx")==0 || strcmp(letrasM,"Dx")==0);    
}

/*
    Procesa e imprime la instrucción
*/
int leer_instruccion(FILE *fp, const char *archivo, int ID) {
    char linea[256];
    char instruccion[5];
    char letrasM[3];
    char numeros[10];
    char IR[50];
    char status[40] = "Correcto";

    if (fgets(linea, sizeof(linea), fp) == NULL) {
        return 1;
    }

    PC++;
    linea[strcspn(linea, "\n")] = '\0';
    if (linea[0] == '\0') return 0;

    int token;
    token = sscanf(linea, "%s %[^,],%s", instruccion, letrasM, numeros);

    if (token < 2) {
        token = sscanf(linea, "%s %s", instruccion, letrasM);
        numeros[0] = '\0';
    }
    if (token < 1) {
        strcpy(status, "ERROR: Falta instrucción");
    }

    // Construir IR (ej: MOV Ax,5 o INC Bx)
    if (numeros[0] != '\0')
        sprintf(IR, "%s %s,%s", instruccion, letrasM, numeros);
    else
        sprintf(IR, "%s %s", instruccion, letrasM);

    // Procesar instrucciones
    if (strcmp(instruccion, "MOV") == 0 || strcmp(instruccion, "ADD") == 0 ||
        strcmp(instruccion, "SUB") == 0 || strcmp(instruccion, "MULT") == 0 ||
        strcmp(instruccion, "DIV") == 0) {

        if (token < 3) {
            strcpy(status, "ERROR: Falta valor");
        } else if (!registroValido(letrasM)) {
            strcpy(status, "ERROR: Registro inválido");
        } else if (!Numeros(numeros)) {
            strcpy(status, "ERROR: No es número");
        } else {
            int valor = atoi(numeros);
            if (strcmp(instruccion, "MOV") == 0) {
                if (strcmp(letrasM, "Ax") == 0) Ax = valor;
                else if (strcmp(letrasM, "Bx") == 0) Bx = valor;
                else if (strcmp(letrasM, "Cx") == 0) Cx = valor;
                else if (strcmp(letrasM, "Dx") == 0) Dx = valor;
            } else if (strcmp(instruccion, "ADD") == 0) {
                if (strcmp(letrasM, "Ax") == 0) Ax += valor;
                else if (strcmp(letrasM, "Bx") == 0) Bx += valor;
                else if (strcmp(letrasM, "Cx") == 0) Cx += valor;
                else if (strcmp(letrasM, "Dx") == 0) Dx += valor;
            } else if (strcmp(instruccion, "SUB") == 0) {
                if (strcmp(letrasM, "Ax") == 0) Ax -= valor;
                else if (strcmp(letrasM, "Bx") == 0) Bx -= valor;
                else if (strcmp(letrasM, "Cx") == 0) Cx -= valor;
                else if (strcmp(letrasM, "Dx") == 0) Dx -= valor;
            } else if (strcmp(instruccion, "MULT") == 0) {
                if (strcmp(letrasM, "Ax") == 0) Ax *= valor;
                else if (strcmp(letrasM, "Bx") == 0) Bx *= valor;
                else if (strcmp(letrasM, "Cx") == 0) Cx *= valor;
                else if (strcmp(letrasM, "Dx") == 0) Dx *= valor;
            } else if (strcmp(instruccion, "DIV") == 0) {
                if (valor == 0) {
                    strcpy(status, "ERROR: División por cero");
                } else {
                    if (strcmp(letrasM, "Ax") == 0) Ax /= valor;
                    else if (strcmp(letrasM, "Bx") == 0) Bx /= valor;
                    else if (strcmp(letrasM, "Cx") == 0) Cx /= valor;
                    else if (strcmp(letrasM, "Dx") == 0) Dx /= valor;
                }
            }
        }
    } else if (strcmp(instruccion, "INC") == 0 || strcmp(instruccion, "DEC") == 0) {
        if (token < 2) {
            strcpy(status, "ERROR: Falta registro");
        } else if (!registroValido(letrasM)) {
            strcpy(status, "ERROR: Registro inválido");
        } else if (token == 3) {
            strcpy(status, "ERROR: Sintaxis");
        } else {
            if (strcmp(instruccion, "INC") == 0) {
                if (strcmp(letrasM, "Ax") == 0) Ax++;
                else if (strcmp(letrasM, "Bx") == 0) Bx++;
                else if (strcmp(letrasM, "Cx") == 0) Cx++;
                else if (strcmp(letrasM, "Dx") == 0) Dx++;
            } else if (strcmp(instruccion, "DEC") == 0) {
                if (strcmp(letrasM, "Ax") == 0) Ax--;
                else if (strcmp(letrasM, "Bx") == 0) Bx--;
                else if (strcmp(letrasM, "Cx") == 0) Cx--;
                else if (strcmp(letrasM, "Dx") == 0) Dx--;
            }
        }
    }

    // Imprimir fila alineada
    printf("%-3d %-3d %-3d %-3d %-3d %-3d %-10s %-12s %-25s\n",
           ID, PC, Ax, Bx, Cx, Dx, archivo, IR, status);

    return 0;
}

/*
    Verifica extensión .asm
*/
int VerificarE(const char *nombre_archivo) {
    char copia[255];
    char *token_acceso;

    strcpy(copia, nombre_archivo);
    token_acceso = strtok(copia, ".");
    while (token_acceso != NULL) {
        char *ultimo = token_acceso;
        token_acceso = strtok(NULL, ".");
        if (token_acceso == NULL) {
            if (strcmp(ultimo, "asm") == 0) {
                return 1;   
            } else {
                printf("Error: extensión no válida\n");
                return 0;
            }
        }
    }
    return 0; 
}

/*
    Ejecutar archivo .asm
*/
void ejecutar_archivo(const char *archivo) {
    int ID = 0;

    if (!VerificarE(archivo)) return;

    FILE *fp = fopen(archivo, "r");
    if (fp == NULL) {
        printf("No se pudo abrir el archivo: %s\n", archivo);
        return;
    }

    // Encabezado
    printf("ID  PC  Ax  Bx  Cx  Dx   Proceso    IR           Status\n");
    printf("---------------------------------------------------------------\n");

    while (!feof(fp)) {
        leer_instruccion(fp, archivo, ID);
    }

    fclose(fp);
}

#endif

