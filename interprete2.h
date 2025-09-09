#ifndef INTERPRETE_H 
#define INTERPRETE_H
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "operaciones.h"

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
   return(strcasecmp(letrasM,"Ax")==0 || strcasecmp(letrasM,"Bx")==0 || 
          strcasecmp(letrasM,"Cx")==0 || strcasecmp(letrasM,"Dx")==0);    
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
    strcpy(IR, linea);

    int token;
    token = sscanf(linea, "%s %[^,],%s", instruccion, letrasM, numeros);

    if (token < 2) {
        token = sscanf(linea, "%s %s", instruccion, letrasM);
        numeros[0] = '\0';
    }
    if (token < 1) {
        strcpy(status, "ERROR: Falta instrucción");
    }

    // strcasecmp para comparar sin importar mayusculas y minusculas
    if (strcasecmp(instruccion, "MOV") == 0 || strcasecmp(instruccion, "ADD") == 0 ||
        strcasecmp(instruccion, "SUB") == 0 || strcasecmp(instruccion, "MUL") == 0 ||
        strcasecmp(instruccion, "DIV") == 0) {

        if (token < 3) {
            strcpy(status, "ERROR: Falta valor");
        } else if (!registroValido(letrasM)) {
            strcpy(status, "ERROR: Registro inválido");
        } else if (!Numeros(numeros)) {
            strcpy(status, "ERROR: No es número");
        } else {
            int valor = atoi(numeros);
            if (strcasecmp(instruccion, "MOV") == 0) {
                if (strcasecmp(letrasM, "Ax") == 0) Ax = valor;
                else if (strcasecmp(letrasM, "Bx") == 0) Bx = valor;
                else if (strcasecmp(letrasM, "Cx") == 0) Cx = valor;
                else if (strcasecmp(letrasM, "Dx") == 0) Dx = valor;
            } else if (strcasecmp(instruccion, "ADD") == 0) {
                if (strcasecmp(letrasM, "Ax") == 0) Ax = sumar(Ax, valor);
                else if (strcasecmp(letrasM, "Bx") == 0) Bx = sumar(Bx, valor);
                else if (strcasecmp(letrasM, "Cx") == 0) Cx = sumar(Cx, valor);
                else if (strcasecmp(letrasM, "Dx") == 0) Dx = sumar(Dx, valor);
            } else if (strcasecmp(instruccion, "SUB") == 0) {
                if (strcasecmp(letrasM, "Ax") == 0) Ax = restar(Ax, valor);
                else if (strcasecmp(letrasM, "Bx") == 0) Bx = restar(Bx, valor);
                else if (strcasecmp(letrasM, "Cx") == 0) Cx = restar(Cx, valor);
                else if (strcasecmp(letrasM, "Dx") == 0) Dx = restar(Dx, valor);
            } else if (strcasecmp(instruccion, "MUL") == 0) {
                if (strcasecmp(letrasM, "Ax") == 0) Ax = multiplicar(Ax, valor);
                else if (strcasecmp(letrasM, "Bx") == 0) Bx = multiplicar(Bx, valor);
                else if (strcasecmp(letrasM, "Cx") == 0) Cx = multiplicar(Cx, valor);
                else if (strcasecmp(letrasM, "Dx") == 0) Dx = multiplicar(Dx, valor);
            } else if (strcasecmp(instruccion, "DIV") == 0) {
                if (valor == 0) {
                    strcpy(status, "ERROR: División por cero");
                } else {
                    if (strcasecmp(letrasM, "Ax") == 0) Ax = dividir(Ax, valor);
                    else if (strcasecmp(letrasM, "Bx") == 0) Bx = dividir(Bx, valor);
                    else if (strcasecmp(letrasM, "Cx") == 0) Cx = dividir(Cx, valor);
                    else if (strcasecmp(letrasM, "Dx") == 0) Dx = dividir(Dx, valor);
                }
            }
        }
    } else if (strcasecmp(instruccion, "INC") == 0 || strcasecmp(instruccion, "DEC") == 0) {
        if (token < 2) {
            strcpy(status, "ERROR: Falta registro");
        } else if (!registroValido(letrasM)) {
            strcpy(status, "ERROR: Registro inválido");
        } else if (token == 3) {
            strcpy(status, "ERROR: Sintaxis");
        } else {
            if (strcasecmp(instruccion, "INC") == 0) {
                if (strcasecmp(letrasM, "Ax") == 0) Ax = incrementar(Ax);
                else if (strcasecmp(letrasM, "Bx") == 0) Bx = incrementar(Bx);
                else if (strcasecmp(letrasM, "Cx") == 0) Cx = incrementar(Cx);
                else if (strcasecmp(letrasM, "Dx") == 0) Dx = incrementar(Dx);
            } else if (strcasecmp(instruccion, "DEC") == 0) {
                if (strcasecmp(letrasM, "Ax") == 0) Ax = decrementar(Ax);
                else if (strcasecmp(letrasM, "Bx") == 0) Bx = decrementar(Bx);
                else if (strcasecmp(letrasM, "Cx") == 0) Cx = decrementar(Cx);
                else if (strcasecmp(letrasM, "Dx") == 0) Dx = decrementar(Dx);
            }
        }
    }

    // Imprimir fila alineada
    printf("%-3d %-3d %-3d %-3d %-3d %-3d %-10s %-12s %-30s\n",
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
            if (strcasecmp(ultimo, "asm") == 0) {
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
    ID++;
    PC = 1;
    Ax = Bx = Cx = Dx = 0;

    fclose(fp);
}

#endif
