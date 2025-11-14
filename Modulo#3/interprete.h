#ifndef INTERPRETE_H
#define INTERPRETE_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

#include "listaProcesos.h"
#include "operaciones.h"

/* ====================== FUNCIONES AUXILIARES ====================== */

int Numeros(char *numero) {
    if (numero[0] == '\0') return 0;   
    for (int i = 0; numero[i] != '\0'; i++) {
        if (!isdigit(numero[i])) return 0;  
    }
    return 1; 
}

int registroValido(char *letrasM) {
   return (strcasecmp(letrasM, "Ax") == 0 || strcasecmp(letrasM, "Bx") == 0 || 
           strcasecmp(letrasM, "Cx") == 0 || strcasecmp(letrasM, "Dx") == 0);    
}

/* ====================== EJECUCIÓN DE INSTRUCCIONES ====================== */

int ejecutarInstruccionPCB(struct PCB* proceso) {
    char linea[256];
    char instruccion[5];
    char letrasM[10];   
    char numeros[10];   
    char IR[50];

    if (proceso->programa == NULL) {
        strcpy(proceso->status, "ERROR: no se pudo abrir");
        return 1;
    }

    if (fgets(linea, sizeof(linea), proceso->programa) == NULL) {
        strcpy(proceso->IR, "END");
        if (strncmp(proceso->status, "ERROR", 5) != 0)
            strcpy(proceso->status, "BIEN");
        return 1; 
    }

    proceso->PC++;
    linea[strcspn(linea, "\n")] = '\0'; 
    if (linea[0] == '\0') return 0;

    strcpy(proceso->IR, linea);
    strcpy(proceso->lineaL, linea);

    char *token = strtok(linea, " ");
    if (token) strcpy(instruccion, token);
    else strcpy(proceso->status, "ERROR: Falta instrucción");

    token = strtok(NULL, " ,");
    if (token) strcpy(letrasM, token);
    else letrasM[0] = '\0';

    token = strtok(NULL, " ");
    if (token) strcpy(numeros, token);
    else numeros[0] = '\0';

    /* --- Instrucción END --- */
    if (strcasecmp(instruccion, "END") == 0) {
        if (strncmp(proceso->status, "ERROR", 5) != 0)
            strcpy(proceso->status, "BIEN");
        return 1;
    }

    /* --- Instrucciones aritméticas y de movimiento --- */
    if (strcasecmp(instruccion, "MOV") == 0 || strcasecmp(instruccion, "ADD") == 0 ||
        strcasecmp(instruccion, "SUB") == 0 || strcasecmp(instruccion, "MUL") == 0 ||
        strcasecmp(instruccion, "DIV") == 0) {

        if (letrasM[0] == '\0' || numeros[0] == '\0')
            strcpy(proceso->status, "ERROR: Faltan operandos");
        else if (strlen(letrasM) != 2 || !registroValido(letrasM))
            strcpy(proceso->status, "ERROR: Registro inválido");
        else if (!Numeros(numeros))
            strcpy(proceso->status, "ERROR: No es número");
        else {
            int valor = atoi(numeros);
            if (strcasecmp(instruccion, "MOV") == 0) {
                if (strcasecmp(letrasM, "Ax") == 0) proceso->AX = valor;
                else if (strcasecmp(letrasM, "Bx") == 0) proceso->BX = valor;
                else if (strcasecmp(letrasM, "Cx") == 0) proceso->CX = valor;
                else if (strcasecmp(letrasM, "Dx") == 0) proceso->DX = valor;
            } else if (strcasecmp(instruccion, "ADD") == 0) {
                if (strcasecmp(letrasM, "Ax") == 0) proceso->AX = sumar(proceso->AX, valor);
                else if (strcasecmp(letrasM, "Bx") == 0) proceso->BX = sumar(proceso->BX, valor);
                else if (strcasecmp(letrasM, "Cx") == 0) proceso->CX = sumar(proceso->CX, valor);
                else if (strcasecmp(letrasM, "Dx") == 0) proceso->DX = sumar(proceso->DX, valor);
            } else if (strcasecmp(instruccion, "SUB") == 0) {
                if (strcasecmp(letrasM, "Ax") == 0) proceso->AX = restar(proceso->AX, valor);
                else if (strcasecmp(letrasM, "Bx") == 0) proceso->BX = restar(proceso->BX, valor);
                else if (strcasecmp(letrasM, "Cx") == 0) proceso->CX = restar(proceso->CX, valor);
                else if (strcasecmp(letrasM, "Dx") == 0) proceso->DX = restar(proceso->DX, valor);
            } else if (strcasecmp(instruccion, "MUL") == 0) {
                if (strcasecmp(letrasM, "Ax") == 0) proceso->AX = multiplicar(proceso->AX, valor);
                else if (strcasecmp(letrasM, "Bx") == 0) proceso->BX = multiplicar(proceso->BX, valor);
                else if (strcasecmp(letrasM, "Cx") == 0) proceso->CX = multiplicar(proceso->CX, valor);
                else if (strcasecmp(letrasM, "Dx") == 0) proceso->DX = multiplicar(proceso->DX, valor);
            } else if (strcasecmp(instruccion, "DIV") == 0) {
                if (valor == 0)
                    strcpy(proceso->status, "ERROR: División por cero");
                else {
                    if (strcasecmp(letrasM, "Ax") == 0) proceso->AX = dividir(proceso->AX, valor);
                    else if (strcasecmp(letrasM, "Bx") == 0) proceso->BX = dividir(proceso->BX, valor);
                    else if (strcasecmp(letrasM, "Cx") == 0) proceso->CX = dividir(proceso->CX, valor);
                    else if (strcasecmp(letrasM, "Dx") == 0) proceso->DX = dividir(proceso->DX, valor);
                }
            }
        }
    }

    /* --- Instrucciones INC / DEC --- */
    else if (strcasecmp(instruccion, "INC") == 0 || strcasecmp(instruccion, "DEC") == 0) {
        if (letrasM[0] == '\0')
            strcpy(proceso->status, "ERROR: Falta registro");
        else if (strlen(letrasM) != 2 || !registroValido(letrasM))
            strcpy(proceso->status, "ERROR: Registro inválido");
        else if (numeros[0] != '\0')
            strcpy(proceso->status, "ERROR: Sintaxis");
        else {
            if (strcasecmp(instruccion, "INC") == 0) {
                if (strcasecmp(letrasM, "Ax") == 0) proceso->AX = incrementar(proceso->AX);
                else if (strcasecmp(letrasM, "Bx") == 0) proceso->BX = incrementar(proceso->BX);
                else if (strcasecmp(letrasM, "Cx") == 0) proceso->CX = incrementar(proceso->CX);
                else if (strcasecmp(letrasM, "Dx") == 0) proceso->DX = incrementar(proceso->DX);
            } else if (strcasecmp(instruccion, "DEC") == 0) {
                if (strcasecmp(letrasM, "Ax") == 0) proceso->AX = decrementar(proceso->AX);
                else if (strcasecmp(letrasM, "Bx") == 0) proceso->BX = decrementar(proceso->BX);
                else if (strcasecmp(letrasM, "Cx") == 0) proceso->CX = decrementar(proceso->CX);
                else if (strcasecmp(letrasM, "Dx") == 0) proceso->DX = decrementar(proceso->DX);
            }
        }
    }

    /* --- Instrucción desconocida --- */
    else {
        strcpy(proceso->status, "ERROR: Instrucción desconocida");
    }

    return 0;
}

/* ====================== FUNCIONES DE VENTANAS ====================== */

void actualizarVentanaNuevos(WINDOW* ventana) {
    for (int i = 2; i < getmaxy(ventana) - 1; i++)
        mvwprintw(ventana, i, 2, "%*s", getmaxx(ventana) - 4, "");

    mvwprintw(ventana, 2, 2, "Procesos NUEVOS:");
    mvwprintw(ventana, 3, 2, "----------------------------------------------------");

    struct PCB* temp = listaNuevos;
    int fila = 4;
    while (temp != NULL && fila < getmaxy(ventana) - 1) {
        mvwprintw(ventana, fila, 2, "ID:%-3d  %-12s", temp->ID, temp->fileName);
        temp = temp->sig;
        fila++;
    }

    wrefresh(ventana);
}

void actualizarVentanaListos(WINDOW* ventana) {
    for (int i = 2; i < getmaxy(ventana) - 1; i++)
        mvwprintw(ventana, i, 2, "%*s", getmaxx(ventana) - 4, "");

    mvwprintw(ventana, 2, 2, "ID  PC P  AX  BX  CX  DX  Proceso      Status    IR");
    mvwprintw(ventana, 3, 2, "----------------------------------------------------");

    struct PCB* temp = listaListos;
    int fila = 4;
    while (temp != NULL && fila < getmaxy(ventana) - 1) {
        mvwprintw(ventana, fila, 2, "%-3d %-3d %-3d %-3d %-3d %-3d %-3d %-12s %-9s %s",
            temp->ID, temp->PC, temp->prioridad, temp->AX, temp->BX, temp->CX, temp->DX,
            temp->fileName, "LISTO",
            temp->IR[0] == '\0' ? "---" : temp->IR);
        temp = temp->sig;
        fila++;
    }

    wrefresh(ventana);
}

void actualizarVentanaEjecucion(WINDOW* ventana, struct PCB* proceso) {
    for (int i = 2; i < getmaxy(ventana) - 1; i++)
        mvwprintw(ventana, i, 2, "%*s", getmaxx(ventana) - 4, "");

    if (proceso != NULL) {
        mvwprintw(ventana, 2, 2, "Proceso actual:");
        mvwprintw(ventana, 3, 2, "ID  PC P AX  BX  CX  DX  Proceso      Status    IR");
        mvwprintw(ventana, 4, 2, "----------------------------------------------------");
        mvwprintw(ventana, 5, 2, "%-3d %-3d %-3d %-3d %-3d %-3d %-3d %-12s %-9s %s",
            proceso->ID, proceso->PC, proceso->prioridad,proceso->AX, proceso->BX, proceso->CX, proceso->DX,
            proceso->fileName,
            strncmp(proceso->status, "ERROR", 5) == 0 ? proceso->status : "EJECUTANDO",
            proceso->IR[0] == '\0' ? "---" : proceso->IR);
    } else {
        mvwprintw(ventana, 2, 2, "No hay procesos en ejecución");
    }

    wrefresh(ventana);
}

void actualizarVentanaTerminados(WINDOW* ventana) {
    for (int i = 2; i < getmaxy(ventana) - 1; i++)
        mvwprintw(ventana, i, 2, "%*s", getmaxx(ventana) - 4, "");

    mvwprintw(ventana, 2, 2, "ID  PC P  AX  BX  CX  DX  Proceso      Status    IR");
    mvwprintw(ventana, 3, 2, "----------------------------------------------------");

    struct PCB* temp = listaTerminados;
    int fila = 4;
    while (temp != NULL && fila < getmaxy(ventana) - 1) {
        mvwprintw(ventana, fila, 2, "%-3d %-3d %-3d %-3d %-3d %-3d %-3d %-12s %-9s %s",
            temp->ID, temp->PC, temp->prioridad,temp->AX, temp->BX, temp->CX, temp->DX,
            temp->fileName, temp->status,
            temp->IR[0] == '\0' ? "---" : temp->IR);
        temp = temp->sig;
        fila++;
    }

    wrefresh(ventana);
}

#endif