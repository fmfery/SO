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
    

    if (proceso->programa==NULL) {
        strcpy(proceso->status,"Error: no se pudo abrir");
        return;
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
    strcpy(proceso->lineaL,linea);

    char *token = strtok(linea, " ");

/////
    if (token) {
        strcpy(instruccion, token);
    } else {
        strcpy(proceso->status, "ERROR: Falta instrucción");
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
            
        } else if (strlen(letrasM) != 2 || !registroValido(letrasM)) {
            strcpy(proceso->status, "ERROR: Registro inválido");
            
        } else if (!Numeros(numeros)) {
            strcpy(proceso->status, "ERROR: No es número");
            
        } else {
            int valor = atoi(numeros);
            if (strcasecmp(instruccion, "MOV") == 0) {
                if (strcasecmp(letrasM, "Ax") == 0) proceso-> BX = valor;
                else if (strcasecmp(letrasM, "Bx") == 0) proceso-> BX = valor;
                else if (strcasecmp(letrasM, "Cx") == 0) proceso-> CX = valor;
                else if (strcasecmp(letrasM, "Dx") == 0) proceso-> DX = valor;
            } else if (strcasecmp(instruccion, "ADD") == 0) {
                if (strcasecmp(letrasM, "Ax") == 0) proceso->AX = sumar(proceso-> AX, valor);
                else if (strcasecmp(letrasM, "Bx") == 0) proceso-> BX = sumar(proceso-> BX, valor);
                else if (strcasecmp(letrasM, "Cx") == 0) proceso-> CX = sumar(proceso-> CX, valor);
                else if (strcasecmp(letrasM, "Dx") == 0) proceso-> DX = sumar(proceso-> DX, valor);
            } else if (strcasecmp(instruccion, "SUB") == 0) {
                if (strcasecmp(letrasM, "Ax") == 0) proceso-> AX = restar(proceso-> AX, valor);
                else if (strcasecmp(letrasM, "Bx") == 0) proceso-> BX = restar(proceso-> BX, valor);
                else if (strcasecmp(letrasM, "Cx") == 0) proceso-> CX = restar(proceso-> CX, valor);
                else if (strcasecmp(letrasM, "Dx") == 0) proceso-> DX = restar(proceso-> DX, valor);
            } else if (strcasecmp(instruccion, "MUL") == 0) {
                if (strcasecmp(letrasM, "Ax") == 0) proceso-> AX = multiplicar(proceso-> AX, valor);
                else if (strcasecmp(letrasM, "Bx") == 0) proceso-> BX = multiplicar(proceso-> BX, valor);
                else if (strcasecmp(letrasM, "Cx") == 0) proceso-> CX = multiplicar(proceso-> CX, valor);
                else if (strcasecmp(letrasM, "Dx") == 0) proceso-> DX = multiplicar(proceso-> DX, valor);
            } else if (strcasecmp(instruccion, "DIV") == 0) {
                if (valor == 0) {
                    strcpy(proceso->status, "ERROR: División por cero");
                   
                } else {
                    if (strcasecmp(letrasM, "Ax") == 0) proceso-> AX = dividir(proceso-> AX, valor);
                    else if (strcasecmp(letrasM, "Bx") == 0) proceso-> BX = dividir(proceso-> BX, valor);
                    else if (strcasecmp(letrasM, "Cx") == 0) proceso-> CX = dividir(proceso-> CX, valor);
                    else if (strcasecmp(letrasM, "Dx") == 0) proceso-> DX = dividir(proceso-> DX, valor);
                }
            }
        }
    }
    else if (strcasecmp(instruccion, "INC") == 0 || strcasecmp(instruccion, "DEC") == 0) {
        if (letrasM[0] == '\0') {
            strcpy(proceso->status, "ERROR: Falta registro");
        } else if (strlen(letrasM) != 2 || !registroValido(letrasM)) {
            strcpy(proceso->status, "ERROR: Registro inválido");      
        } else if (numeros[0] != '\0') {
            strcpy(proceso->status, "ERROR: Sintaxis");
            
        } else {
            if (strcasecmp(instruccion, "INC") == 0) {
                if (strcasecmp(letrasM, "Ax") == 0) proceso-> AX = incrementar(proceso-> AX);
                else if (strcasecmp(letrasM, "Bx") == 0) proceso-> BX = incrementar(proceso-> BX);
                else if (strcasecmp(letrasM, "Cx") == 0) proceso-> CX = incrementar(proceso-> CX);
                else if (strcasecmp(letrasM, "Dx") == 0) proceso-> DX = incrementar(proceso-> DX);
            } else if (strcasecmp(instruccion, "DEC") == 0) {
                if (strcasecmp(letrasM, "Ax") == 0) proceso-> AX = decrementar(proceso-> AX);
                else if (strcasecmp(letrasM, "Bx") == 0) proceso-> BX = decrementar(proceso-> BX);
                else if (strcasecmp(letrasM, "Cx") == 0) proceso-> CX = decrementar(proceso-> CX);
                else if (strcasecmp(letrasM, "Dx") == 0) proceso-> DX = decrementar(proceso-> DX);
            }
        }
    }
    else {
        strcpy(proceso->status, "ERROR: Instrucción desconocida");
       
    }

}

/*
    Funcion para actualizar la ventana de los procesos listos
    Muestra todos los procesos que estan esperando a ejecutarse 
*/
    void actualizaVentanaListos(WINDOW* ventana){
        //imprime las verticales
        //del 3 al maximo de filas menos uno que es el borde
        for (int i = 3; i < getmaxy(ventana)-1; i++)
        {//borra lo que hay dentro pero deja los bordes
            mvwprintw(ventana,i,2,"%*s",getmaxx(ventana)-4,"");
        }

        mvwprintw(ventana,3,2,"ID  PC  Ax  Bx  Cx  Dx   Proceso    IR           Status\n");
        mvwprintw(ventana,3,2,"---------------------------------------------------------\n");
        
        //creo la lista de listos
       struct PCB* temp = listaListos;
        int fila = 5;//y

        while (temp != NULL && fila<getmaxy(ventana)-1)
        {                        //x
            mvwprintw(ventana,fila,2,"%-3d %-3d %-3d %-3d %-3d %-3d %-12s %-9s %s",
                temp->ID,
                temp->PC,
                temp->AX,
                temp->BX,
                temp->CX,
                temp->DX,
                temp->fileName,
                "LISTOS",
                temp->IR[0]=='\0'?"--":temp->IR);
                

                temp->sig;

                fila++;

         }
        wrefresh(ventana);
    }

/*
    Funcion para limpiar la linea de procesos.
    Muestra el proceso actual que esta ejecutando
*/

    void actualizaVentanaEjecucion(WINDOW* ventana, struct PCB* proceso){
        for (int i = 2; i < getmaxy(ventana)-4; i++)
        {
            mvwprintw(ventana,i,2,"%*s",getmaxx(ventana)-1,"");
        }
        if (proceso!=NULL)
        {
           mvwprintw(ventana,2,2,"Proceso actual:");
           mvwprintw(ventana,3,2,"ID  PC  AX  BX  CX  DX  Proceso      Status    IR");
           mvwprintw(ventana,4,2,"----------------------------------------------------");
           mvwprintw(ventana,5,2,"%-3d %-3d %-3d %-3d %-3d %-3d %-12s %-9s %s",
                    proceso->ID,
                    proceso->PC,
                    proceso->AX,
                    proceso->BX,
                    proceso->CX,
                    proceso->DX,
                    proceso->fileName,                       //en que se quedo, puede ser ERROR o BIEN, si no es eso, esntonces, se esta ejecutando
                   strncmp(proceso->status, "ERROR", 5) == 0 ? proceso->status : "EJECUTANDO",//se esta cambiando constantemente en el programa
                   proceso->IR=='\0'?"--":proceso->IR
                    

            );

        }
        else
        {
            mvwprintw(ventana,2,2,"No hay proceso a ejecutar");
        }
        
        wrefresh(ventana);

    }

/*
    Funcion para actualizar los procesos terminados.
    Los procesos que ya acabaron de ejecutarse (si estan bien o no)
*/

    void actualizarVentanaTerminados(WINDOW* ventana){

        for (int i = 2; i < getmaxy(ventana)-1; i++)
        {
            mvwprintw(ventana,i,2,"%*s",getmaxx(ventana)-4,"");

        }
        
        mvwprintw(ventana,2,2,"ID  PC  Ax  Bx  Cx  Dx   Proceso     Status         IR\n");
        mvwprintw(ventana,3,2,"---------------------------------------------------------\n");
        
        //creo una lista, es decir aqui creo una variable temporal 
        struct PCB* temp = listaTerminados;
        int fila = 4;

        while (temp!=NULL && fila < getmaxy(ventana)-1)
        {
            mvwprintw(ventana,fila,2,"%-3d %-3d %-3d %-3d %-3d %-3d %-12s %-9s %s",
                    temp->ID,
                    temp->PC,
                    temp->AX,
                    temp->BX,
                    temp->CX,
                    temp->DX,
                    temp->fileName,
                    temp->status,
                    temp->IR == '\0'?"--":temp->IR

            );
            fila++;
            temp=temp->sig;
        }

        wrefresh(ventana);   

    }



#endif