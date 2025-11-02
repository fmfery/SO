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

int Q=0;

 extern int AX_global, BX_global, CX_global, DX_global;

int Numeros(char *numero){
    if(numero[0] == '\0') return 0;   
    for (int i = 0; numero[i] != '\0'; i++){
        if (!isdigit(numero[i])){
            return 0;  
        }
    }
    return 1; 
}


int registroValido(char *letrasM){
   return(strcasecmp(letrasM,"Ax")==0 || strcasecmp(letrasM,"Bx")==0 || 
          strcasecmp(letrasM,"Cx")==0 || strcasecmp(letrasM,"Dx")==0);    
}

int ejecutarInstruccionPCB(struct PCB* proceso) {
    char linea[256];
    char instruccion[5];
    char letrasM[10];   
    char numeros[10];   
    char IR[50];
    char status[40] = "Correcto";
    
//               archivo
    if (proceso->programa==NULL) {
        strcpy(proceso->status,"Error: no se pudo abrir");
        return 1;
    }


    if (fgets(linea, sizeof(linea), proceso->programa) == NULL) {
        strcpy(proceso->IR, "END");
        
        if (strncmp(proceso->status, "ERROR", 5) != 0) {
            strcpy(proceso->status, "BIEN");//ok
        }
        return 1; 
    }

    proceso->PC++;
    
    linea[strcspn(linea, "\n")] = '\0'; 
    
    if (linea[0] == '\0') return 0;

    strcpy(proceso->IR, linea);
    strcpy(proceso->lineaL,linea);//la instruccion que acaba de leer

    
    char *token = strtok(linea, " ");


    if (token) {
        strcpy(instruccion, token);
    } else {
        strcpy(proceso->status, "ERROR: Falta instrucción");
        return 1;
    }

    token = strtok(NULL, " ,");
    if (token) {
        strcpy(letrasM, token);
    } else {
        letrasM[0] = '\0';
    }

    token = strtok(NULL, " ");
    if (token) {
        strcpy(numeros, token);
    } else {
        numeros[0] = '\0';
    }

    if (strcasecmp(instruccion, "END") == 0) {

        if (strncmp(proceso->status, "ERROR", 5) != 0) {
            strcpy(proceso->status, "BIEN");
        }
        return 1;
    }


    if (strcasecmp(instruccion, "MOV") == 0 || strcasecmp(instruccion, "ADD") == 0 ||
        strcasecmp(instruccion, "SUB") == 0 || strcasecmp(instruccion, "MUL") == 0 ||
        strcasecmp(instruccion, "DIV") == 0) {

           

        if (letrasM[0] == '\0' || numeros[0] == '\0') {
            strcpy(proceso->status, "ERROR: Faltan operandos");
            return 1;
            
        } else if (strlen(letrasM) != 2 || !registroValido(letrasM)) {
            strcpy(proceso->status, "ERROR: Registro inválido");
            return 1;
            
        } else if (!Numeros(numeros)) {
            strcpy(proceso->status, "ERROR: No es número");
            return 1;
            
        } else {
            int valor = atoi(numeros);
            if (strcasecmp(instruccion, "MOV") == 0) {
                if (strcasecmp(letrasM, "Ax") == 0) AX_global = valor;
                else if (strcasecmp(letrasM, "Bx") == 0) BX_global = valor;
                else if (strcasecmp(letrasM, "Cx") == 0) CX_global = valor;
                else if (strcasecmp(letrasM, "Dx") == 0) DX_global = valor;
            } else if (strcasecmp(instruccion, "ADD") == 0) {
                if (strcasecmp(letrasM, "Ax") == 0) AX_global = sumar(AX_global, valor);
                else if (strcasecmp(letrasM, "Bx") == 0) BX_global = sumar(BX_global, valor);
                else if (strcasecmp(letrasM, "Cx") == 0) CX_global = sumar(CX_global, valor);
                else if (strcasecmp(letrasM, "Dx") == 0) DX_global = sumar(DX_global, valor);
            } else if (strcasecmp(instruccion, "SUB") == 0) {
                if (strcasecmp(letrasM, "Ax") == 0) AX_global = restar(AX_global, valor);
                else if (strcasecmp(letrasM, "Bx") == 0) BX_global = restar(BX_global, valor);
                else if (strcasecmp(letrasM, "Cx") == 0) CX_global = restar(CX_global, valor);
                else if (strcasecmp(letrasM, "Dx") == 0) DX_global = restar(DX_global, valor);
            } else if (strcasecmp(instruccion, "MUL") == 0) {
                if (strcasecmp(letrasM, "Ax") == 0) AX_global = multiplicar(AX_global, valor);
                else if (strcasecmp(letrasM, "Bx") == 0) BX_global = multiplicar(BX_global, valor);
                else if (strcasecmp(letrasM, "Cx") == 0) CX_global = multiplicar(CX_global, valor);
                else if (strcasecmp(letrasM, "Dx") == 0) DX_global = multiplicar(DX_global, valor);
            } else if (strcasecmp(instruccion, "DIV") == 0) {
                if (valor == 0) {
                    strcpy(proceso->status, "ERROR: División por cero");
                    return 1;
                   
                } else {
                    if (strcasecmp(letrasM, "Ax") == 0) AX_global = dividir(AX_global, valor);
                    else if (strcasecmp(letrasM, "Bx") == 0) BX_global = dividir(BX_global, valor);
                    else if (strcasecmp(letrasM, "Cx") == 0) CX_global = dividir(CX_global, valor);
                    else if (strcasecmp(letrasM, "Dx") == 0) DX_global = dividir(DX_global, valor);
                }
            }
        }

       
    }
    else if (strcasecmp(instruccion, "INC") == 0 || strcasecmp(instruccion, "DEC") == 0) {
        if (letrasM[0] == '\0') {
            strcpy(proceso->status, "ERROR: Falta registro");
            return 1;

        } else if (strlen(letrasM) != 2 || !registroValido(letrasM)) {
            strcpy(proceso->status, "ERROR: Registro inválido"); 
            return 1;

        } else if (numeros[0] != '\0') {
            strcpy(proceso->status, "ERROR: Sintaxis");
            return 1;
            
        } else {
            if (strcasecmp(instruccion, "INC") == 0) {
                if (strcasecmp(letrasM, "Ax") == 0) AX_global = incrementar(AX_global);
                else if (strcasecmp(letrasM, "Bx") == 0) BX_global = incrementar(BX_global);
                else if (strcasecmp(letrasM, "Cx") == 0) CX_global = incrementar(CX_global);
                else if (strcasecmp(letrasM, "Dx") == 0) DX_global = incrementar(DX_global);
            } else if (strcasecmp(instruccion, "DEC") == 0) {
                if (strcasecmp(letrasM, "Ax") == 0) AX_global = decrementar(AX_global);
                else if (strcasecmp(letrasM, "Bx") == 0) BX_global = decrementar(BX_global);
                else if (strcasecmp(letrasM, "Cx") == 0) CX_global = decrementar(CX_global);
                else if (strcasecmp(letrasM, "Dx") == 0) DX_global = decrementar(DX_global);
            }
        }
    }
    else {
        strcpy(proceso->status, "ERROR: Instrucción desconocida");
        return 1;
       
    }


    
}


/*
    Funcion para actualizar la ventana de los procesos listos
    Muestra todos los procesos que estan esperando a ejecutarse 
*/
void actualizarVentanaListos(WINDOW* ventana){
    // Limpiar contenido anterior (excepto el marco)
    for (int i = 2; i < getmaxy(ventana) - 1; i++) {
        mvwprintw(ventana, i, 2, "%*s", getmaxx(ventana) - 4, "");
    }
    
    // Mostrar encabezado
    mvwprintw(ventana, 2, 2, "ID  PC  AX  BX  CX  DX  Proceso      Status           IR");
    mvwprintw(ventana, 3, 2, "--------------------------------------------------------------");
    
    // Mostrar cada PCB en la lista de listos
    struct PCB* temp = listaListos;
    int fila = 4;
    
    while (temp != NULL && fila < getmaxy(ventana) - 1) {
        mvwprintw(ventana, fila, 2, "%-3d %-3d %-3d %-3d %-3d %-3d %-12s %-9s %s",
                 temp->ID,
                 temp->PC,
                 temp->AX,
                 temp->BX,
                 temp->CX,
                 temp->DX,
                 temp->fileName,
                 "LISTO",
                 temp->IR[0] == '\0' ? "---" : temp->IR);
        temp = temp->sig;
        fila++;
    }
    
    wrefresh(ventana);
}

/*
    Funcion para limpiar la linea de procesos.
    Muestra el proceso actual que esta ejecutando
*/
void actualizarVentanaEjecucion(WINDOW* ventana, struct PCB* proceso){
    // Limpiar contenido anterior (excepto el marco)
    for (int i = 2; i < getmaxy(ventana) - 1; i++) {
        mvwprintw(ventana, i, 2, "%*s", getmaxx(ventana) - 4, "");
    }
    
    if (proceso != NULL) {
        mvwprintw(ventana, 2, 2, "Proceso actual:");
        mvwprintw(ventana, 3, 2, "ID  PC  AX  BX  CX  DX  Proceso      Status    IR");
        mvwprintw(ventana, 4, 2, "----------------------------------------------------");
        mvwprintw(ventana, 5, 2, "%-3d %-3d %-3d %-3d %-3d %-3d %-12s %-9s %s",
                 proceso->ID,
                 proceso->PC,
                 AX_global,
                 BX_global,
                 CX_global,
                 DX_global,
                 proceso->fileName,
                 strncmp(proceso->status, "ERROR", 5) == 0 ? proceso->status : "EJECUTANDO",
                 proceso->IR[0] == '\0' ? "---" : proceso->IR);
    } else {
        mvwprintw(ventana, 2, 2, "No hay procesos en ejecucion");
    }
    
    wrefresh(ventana);
}

/*
    Funcion para actualizar los procesos terminados.
    Los procesos que ya acabaron de ejecutarse (si estan bien o no)
*/
void actualizarVentanaTerminados(WINDOW* ventana){
    // Limpiar contenido anterior
    for (int i = 2; i < getmaxy(ventana) - 1; i++) {
        mvwprintw(ventana, i, 2, "%*s", getmaxx(ventana) - 4, "");
    }
    
    mvwprintw(ventana, 2, 2, "ID  PC  AX  BX  CX  DX  Proceso      Status                    IR");
    mvwprintw(ventana, 3, 2, "-------------------------------------------------------------------");
    
    // Mostrar cada PCB en la lista de terminados
    struct PCB* temp = listaTerminados;
    int fila = 4;
    
    while (temp != NULL && fila < getmaxy(ventana) - 1) {
        mvwprintw(ventana, fila, 2, "%-3d %-3d %-3d %-3d %-3d %-3d %-12s %-9s %s",
                 temp->ID,
                 temp->PC,
                 temp->AX,
                 temp->BX,
                 temp->CX,
                 temp->DX,
                 temp->fileName,
                 temp->status,
                 temp->IR[0] == '\0' ? "---" : temp->IR);
        temp = temp->sig;
        fila++;
    }
    
    wrefresh(ventana);
}



#endif