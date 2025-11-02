#ifndef PROCESOS_H 
#define PROCESOS_H

#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#include<curses.h>

//Estructura de la lista PCB
struct PCB
{
   int ID;
   int PC;
   int AX;
   int BX;
   int CX;
   int DX;
   char IR[100];
   char status[100];
   char fileName[100];
   char lineaL[100];
   FILE *programa;

    struct PCB* sig; 
};

//Inicio las listas vacias

struct PCB* listaEjecucion = NULL;
struct PCB* listaListos = NULL;
struct PCB* listaTerminados = NULL;

int sigID = 1;

//se crea como una nueva lista

struct PCB* creaPCB(const char *nombreArchivo)
{
    struct PCB* nuevoP = (struct PCB*)malloc(sizeof(struct PCB));

    if (nuevoP==NULL)
    {
        printw("No se pudo acceder memoria\n");
        exit(1);
    }

    //se inicia PCB
    
    nuevoP->ID=sigID++;
    nuevoP->PC=0;
    nuevoP->AX=0;
    nuevoP->BX=0;
    nuevoP->CX=0;
    nuevoP->DX=0;
    strcpy(nuevoP->IR,"-");
    strcpy(nuevoP->status,"Correcto");
    strcpy(nuevoP->fileName,nombreArchivo);
    strcpy(nuevoP->lineaL,"");//vacia
    nuevoP->programa=NULL;

    nuevoP->sig=NULL;

    return nuevoP;


}


//agrega un nodo (el PCB) al final de la lista
void agregarPCB(struct PCB** lista, struct PCB* proceso)
{
   
    if (*lista == NULL)
    {
        *lista  = proceso;
    }
    else {
        //si hay mas archivos
        struct PCB* temp = *lista;
        while (temp->sig != NULL)
        {   
            temp = temp->sig;
        }
        temp->sig = proceso;
    }
}

//quitar un nodo (proceso)
struct PCB* removerPrimero(struct PCB** lista)
{
    // Si la lista está vacía, no hay nada que remover
    if(*lista==NULL){
        return NULL;
    }
    // Apunta al primer nodo de la lista
    struct PCB* temp = *lista;
    // Actualiza la cabeza de la lista al siguiente nodo
    *lista = (*lista)->sig;
    // Desconecta el nodo removido de la lista
    temp->sig = NULL;
    // Retorna el nodo removido
    return temp;
}

//iniciar un nuevo proceso
struct PCB* inicializarProceso(char* nombreArchivo){
    struct PCB* nuevoP = creaPCB(nombreArchivo);
    
    if (nuevoP == NULL)
    {
        return NULL;
    }

    //si encontro un archivo lo abre
    nuevoP->programa = fopen(nombreArchivo,"r");
    if(nuevoP->programa == NULL){
        printw("Error: No se abrio el archivo\n");
        free(nuevoP);
        return NULL;
    }
    return nuevoP;
    
}

//Liberar memoria
void liberarPCB(struct PCB* proceso){
    if (proceso != NULL)
    {
       if (proceso->programa != NULL)
       {
            fclose(proceso->programa);

       }
       free(proceso);
       
    }

}

//libera toda la lista

void liberarListaPCB(struct PCB** lista){
    struct PCB* temp;
    while (*lista != NULL)
    {
        temp = *lista;
        *lista = (*lista)->sig;
        liberarPCB(temp);
    }
    
}

struct PCB* killID(struct PCB** lista, int ID){


    if(*lista==NULL){
        return NULL;
    }

//si en la lista x es el primero que buscamos
    if((*lista)->ID==ID){
        struct PCB* temp = *lista;
        strcpy(temp->status,"KILLED");
        (*lista)=(*lista)->sig;
        temp->sig=NULL;
        return temp;

    }
//si no es el primero

    struct PCB* prev = *lista;// primer nodo //el nodo que va a conectar 
    struct PCB* temp = (*lista)->sig;//segundo nodo // el nodo que se va a eliminar

    while (temp!=NULL){
        if(temp->ID==ID){
            strcpy(temp->status,"KILLED");
            prev->sig=temp->sig;
            temp->sig=NULL;
            return temp; //o NULL?
        }

        prev=temp;
        temp=temp->sig;

    }
    return NULL;//no encontro nada
}

#endif