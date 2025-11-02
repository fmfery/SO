#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>

#include "interprete.h"
#include "listaProcesos.h"
#include "kbhit.h"

#define MAX_PROGRAMAS 5
#define QUANTUM 4

int contador_Q=0;

int pausa=0;


 
   int AX_global=0;
   int BX_global=0;
   int CX_global=0;
   int DX_global=0;

   void CargarContexto(struct PCB* proceso){
        AX_global=proceso->AX;
        BX_global=proceso->BX;
        CX_global=proceso->CX;
        DX_global=proceso->DX;

   }

    void GuardarContexto(struct PCB* proceso){
        proceso->AX = AX_global;
        proceso->BX = BX_global;
        proceso->CX = CX_global;
        proceso->DX = DX_global;
    }


int main() {
  
  char lectura[100];//toda la linea de comando
  
  /*
  Inicio las listas PCB
  */

  listaEjecucion=NULL;
  listaListos=NULL;
  listaTerminados=NULL;

  sigID=1;

       
        initscr();
        cbreak();
        noecho();
        keypad(stdscr, TRUE); // Habilitar teclas especiales (BACKSPACE)


        int alto, ancho;
        //el numero de dimensiones de la pantalla
        getmaxyx(stdscr,alto,ancho);
//               ^ la terminal
        WINDOW* IDventanaCMD = newwin(alto,ancho/3, 0, 0);
        box(IDventanaCMD, 0, 0);
        keypad(IDventanaCMD, TRUE); // Habilitar teclas especiales en esta ventana, es decir, cosas como KEY_BACKSPACE
         mvwprintw(IDventanaCMD, 0, 2, "CMD");
         mvwprintw(IDventanaCMD,2, 2, "=== Bienvenido al prompt ===");
         mvwprintw(IDventanaCMD,3, 2, "Comandos disponibles:");
         mvwprintw(IDventanaCMD,4, 2, "  ejecutar <archivo1.asm> [archivo2.asm]...");
         mvwprintw(IDventanaCMD,5, 2, "  salir");
         mvwprintw(IDventanaCMD,6, 2,"(Maximo 5 archivos)");


        //newwin(TamañoYVentana, TamañoXVentana, PosicionY, PosicionX)
         WINDOW* IDventanaEjecucion = newwin(alto/3,(ancho * 2)/3,0,ancho/3);
        box(IDventanaEjecucion, 0, 0);
        mvwprintw(IDventanaEjecucion,0,2,"EJECUCION");
        mvwprintw(IDventanaEjecucion,2,2,"Proceso actual:");
        mvwprintw(IDventanaEjecucion,3,2,"...");

        //newwin(TamañoYVentana, TamañoXVentana, PosicionY, PosicionX)
         WINDOW *IDventanaListos= newwin(alto/3, (ancho * 2) / 3, alto/3, ancho/ 3); 
        box(IDventanaListos, 0, 0);
        mvwprintw(IDventanaListos, 0, 2, "LISTOS");
        mvwprintw(IDventanaListos, 2, 2, "ID  PC  AX  BX  CX  DX  Proceso      Status                 IR");
        mvwprintw(IDventanaListos, 3, 2, "----------------------------------------------------------------");

        //newwin(TamañoYVentana, TamañoXVentana, PosicionY, PosicionX)
        WINDOW *IDventanaTerminados= newwin(alto/3, (ancho * 2) / 3, 2*alto/3, ancho/ 3);
        box(IDventanaTerminados, 0, 0);
        mvwprintw(IDventanaTerminados, 0, 2, "TERMINADOS");
        mvwprintw(IDventanaTerminados, 2, 2, "ID  PC  AX  BX  CX  DX  Proceso      Status              IR");
        mvwprintw(IDventanaTerminados, 3, 2, "------------------------------------------------------------");



     refresh();
      wrefresh(IDventanaCMD);
      wrefresh(IDventanaEjecucion);
      wrefresh(IDventanaListos);
      wrefresh(IDventanaTerminados);

      int filaCmd = 8;//posicion del comando
      int posComando = 0; // Posición actual en el comando
    
      
      // Mostrar el prompt inicial
      mvwprintw(IDventanaCMD, filaCmd, 2, "> ");
      wrefresh(IDventanaCMD);



  while (1) {
      
    

  if(!pausa){// ->0 si NO esta en pausa
      if(listaEjecucion!=NULL){
        struct PCB* procesoActual = listaEjecucion; //apunta al primer proceso

        if (contador_Q==0)
        {
            CargarContexto(procesoActual);
        }
        
       
        int resultado = ejecutarInstruccionPCB(procesoActual); //cuando pasa a esa funcion y termina regresa un 1

        contador_Q++;

        // Actualizar ventana de ejecución, esta en el interprete.h
        actualizarVentanaEjecucion(IDventanaEjecucion, procesoActual);
        usleep(1000000);

        actualizarVentanaEjecucion(IDventanaEjecucion,procesoActual);
        
        if (resultado == 1) { //termino el programa bien

           GuardarContexto(procesoActual);  //guarda el contexto antes de mover a terminado

                struct PCB* terminado = removerPrimero(&listaEjecucion); 
                agregarPCB(&listaTerminados, terminado);
                
                
                actualizarVentanaEjecucion(IDventanaEjecucion, NULL);
                actualizarVentanaTerminados(IDventanaTerminados);
              
                // Tomar siguiente proceso de listos si existe
                if (listaListos != NULL) {
                    struct PCB* siguiente = removerPrimero(&listaListos);
                    agregarPCB(&listaEjecucion, siguiente);
                    actualizarVentanaListos(IDventanaListos);
                }
                contador_Q=0;//se reinicia cada que acaba un proceso
            }

                          //   4
        else if(contador_Q>=QUANTUM){
            GuardarContexto(procesoActual);

            struct PCB* procesoActualTemporal = removerPrimero(&listaEjecucion);
            
           // GuardarContexto(procesoActualTemporal);

            actualizarVentanaEjecucion(IDventanaEjecucion,NULL);
            agregarPCB(&listaListos,procesoActualTemporal);

            if(listaListos!=NULL && listaListos!= procesoActualTemporal){
                struct PCB* siguiente = removerPrimero(&listaListos);

                usleep(1000000);

                agregarPCB(&listaEjecucion,siguiente);
                actualizarVentanaEjecucion(IDventanaEjecucion,siguiente);
                usleep(1000000);
                actualizarVentanaListos(IDventanaListos);
    
            }

            else{
                actualizarVentanaEjecucion(IDventanaEjecucion,NULL);
                usleep(1000000);
                actualizarVentanaListos(IDventanaListos);
                usleep(1000000);
            }
           contador_Q=0;

        }



    }

    
      else if(listaListos!=NULL){
            // Si no hay proceso en ejecución pero sí en listos, tomar el primero, esa comprobación se hace en el if anterior -> if(contadorEjecucion >= TIEMPO_EJECUCION && listaEjecucion!=NULL)
            struct PCB* siguiente = removerPrimero(&listaListos); // Variable temporal a la cual se le asigna el primer proceso de la lista de listos
            agregarPCB(&listaEjecucion, siguiente); // Mover el proceso a la lista de ejecución
            actualizarVentanaListos(IDventanaListos);
            actualizarVentanaEjecucion(IDventanaEjecucion, siguiente);
          contador_Q=0;  
            
    }
}
   

    // Verificar entrada de usuario con kbhit
      if (kbhit()) {
            // Obtener la tecla presionada
            int tecla = getch();
            
            // enter
            
            if (tecla == '\n') {
                pausa=0;//continua con la ejecucion < || >
                lectura[posComando] = '\0'; 
                //          0
                
                char *comando = strtok(lectura, " "); 
                //si presiono enter sin tener nada
                if (comando == NULL) {
                    filaCmd++;
                    posComando = 0;
                    lectura[0] = '\0';
                    continue;
                } 
                
                if (strcasecmp(comando, "salir") == 0) {
                    mvwprintw(IDventanaCMD, filaCmd + 1, 2, "Saliendo del programa...");
                   
                    wrefresh(IDventanaCMD);
                    break;
                } 

                else if (strcmp(comando, "ejecutar") == 0) {
                    int cont = 0;
                    char *archivos[MAX_PROGRAMAS];
                    
                    // Obtener todos los archivos de la línea de comando
                    
                    char *token = strtok(NULL, " ");
                    while (token != NULL && cont < MAX_PROGRAMAS) {
                        archivos[cont++] = token;

                        token = strtok(NULL, " ");
                    }

                    
                    // Verificar límite de archivos
                    if (token != NULL) {
                        mvwprintw(IDventanaCMD, filaCmd + 1, 2, "Error: Maximo %d archivos", MAX_PROGRAMAS);
                        wrefresh(IDventanaCMD);
                        //reinicia todo
                        filaCmd += 2;
                        posComando = 0;
                        lectura[0] = '\0';
                        continue;
                    }
                               
                    // Verificar que se proporcionaron archivos
                    if (cont == 0) {
                        mvwprintw(IDventanaCMD, filaCmd + 1, 2, "Error: Falta archivo a ejecutar");
                        wrefresh(IDventanaCMD);
                        filaCmd += 2;
                        posComando = 0;
                        lectura[0] = '\0';
                        continue;
                    }

                    // Procesar cada archivo
                    // Primer paso: Esto se ejecuta antes de todo, agrega los procesos a la lista de listos             

                    for (int i = 0; i < cont; i++) {
                        //antes de crear el proceso verifica que existen
                        FILE* verificaArchivo = fopen(archivos[i], "r");
                        if(verificaArchivo==NULL){
                            mvwprintw(IDventanaCMD,filaCmd +1,2,"ERROR: no se pudo abrir el archivo %s o no existe",archivos[i]);
                            wrefresh(IDventanaCMD);
                            filaCmd++;

                        }
                        else{
                            //el archivo si existe 
                            fclose(verificaArchivo);
                        
                        
                        // Crear nuevo proceso PCB
                        struct PCB* nuevoProceso = inicializarProceso(archivos[i]);
                        if (nuevoProceso == NULL) {
                            mvwprintw(IDventanaCMD, filaCmd + 1, 2, "Error: No se pudo crear proceso para %s", archivos[i]);
                            wrefresh(IDventanaCMD);
                            filaCmd+3;

                        } else {
                            // Inicializar PC correctamente
                            nuevoProceso->PC = 0;
                            // Agregar a lista de listos
                            agregarPCB(&listaListos, nuevoProceso);

                        }
                      }
                       
                    }
                    
                    actualizarVentanaListos(IDventanaListos);
                    wrefresh(IDventanaCMD);

                    filaCmd += 2;
                }
                else if(strcmp(comando,"kill")==0){
                    char* id = strtok(NULL," ");

                    if(id==NULL){
                        mvwprintw(IDventanaCMD,filaCmd+1,2,"Error: falta ID");
                        
                        wrefresh(IDventanaCMD);
                        filaCmd+=3;

                    }

                    else if(!Numeros(id)){
                        mvwprintw(IDventanaCMD,filaCmd+1,2,"Error: eso no es un numero");
                        wrefresh(IDventanaCMD);
                        filaCmd+=3;
                    }
                    else{
                        struct PCB* proceso = NULL;

                        int idNum = atoi(id);

                       proceso = killID(&listaEjecucion,idNum);//si encontro algo != NULL

                       if(proceso==NULL){
                        proceso= killID(&listaListos,idNum);
                       }

                       if (proceso!=NULL)
                       {
                        agregarPCB(&listaTerminados,proceso);
                        actualizarVentanaEjecucion(IDventanaEjecucion,listaEjecucion);
                        
                        actualizarVentanaListos(IDventanaListos);
                        actualizarVentanaTerminados(IDventanaTerminados);

                        contador_Q = 0;

                        mvwprintw(IDventanaCMD,filaCmd+2,2,"Se elimino el proceso con ID: %d",idNum);
                        filaCmd += 3;
                        
                       }
                       //si aun despues de todo es NULL
                       else{
                        mvwprintw(IDventanaCMD,filaCmd+2,2,"Error: Id no fue encontrado");

                       }
                       

                        wrefresh(IDventanaListos);
                        wrefresh(IDventanaCMD);
                        wrefresh(IDventanaEjecucion);
                       

                    }
                   

                }
                 

                // Comando no reconocido
                else {
                    mvwprintw(IDventanaCMD, filaCmd +1, 2, "Error: Comando no reconocido: %s", comando);
                    wrefresh(IDventanaCMD);
                    filaCmd += 3;
                }
                
                // Reiniciar posición del comando
                posComando = 0;
                lectura[0] = '\0';
                
                // Si llegamos al final de la ventana, limpiar y reiniciar
                if (filaCmd > alto - 4) {
                    wclear(IDventanaCMD);
                    box(IDventanaCMD, 0, 0);
                    mvwprintw(IDventanaCMD, 0, 2, "CMD");
                    mvwprintw(IDventanaCMD, 2, 2, "=== Bienvenido al prompt ===");
                    mvwprintw(IDventanaCMD, 3, 2, "Comandos disponibles:");
                    mvwprintw(IDventanaCMD, 4, 2, "  ejecutar <archivo1.asm> [archivo2.asm]...");
                    mvwprintw(IDventanaCMD, 5, 2, "  salir");
                    mvwprintw(IDventanaCMD, 6, 2, "(Maximo 5 archivos)");
                    filaCmd = 8;
                    wrefresh(IDventanaCMD);
                }
                
                // Mostrar nuevo prompt
                mvwprintw(IDventanaCMD, filaCmd, 2, "> ");
                wrefresh(IDventanaCMD);
            }
            // Si se presiona Backspace y hay caracteres - Tecla 127 = DEL
            else if ((tecla == KEY_BACKSPACE || tecla == 127) && posComando > 0) {
                //pausa el programa
                pausa = 1;
                
                posComando--; // retrocede
                lectura[posComando] = '\0'; 
                
                // Obtener coordenadas del cursor
                int y, x;
                getyx(IDventanaCMD, y, x); //getyx obtiene la posicion actual del cursor (ventana, y, x)
                
                // Mover el cursor a la izquierda y borrar
                wmove(IDventanaCMD, y, x - 1); //wmove mueve el cursor a una posicion (ventana, y, x)
                mvwprintw(IDventanaCMD, y, x - 1, " ");
                wmove(IDventanaCMD, y, x - 1);
                wrefresh(IDventanaCMD);
            }
            // Si es un caracter imprimible - Tecla entre 32 y 126 = caracteres ASCII desde espacio a ~
            // posComando < 99 para evitar overflow, el maximo es 100, dejando espacio para el '\0', por eso es 99 + '\0'
            else if (tecla >= 32 && tecla <= 126 && posComando < 99) {
                pausa = 1;
                // Almacenar la tecla en el comando
                lectura[posComando] = tecla; // Guardar el caracter
                lectura[posComando + 1] = '\0'; // Mantener el final de cadena
                posComando++;
                
                // Mostrar el caracter
                waddch(IDventanaCMD, tecla); //waddch agrega un caracter a la ventana
                wrefresh(IDventanaCMD);
            }
        } 
        
        
        //usleep(1000000); // 100ms
    
  }
  
    // Limpiar sistema PCB
    liberarListaPCB(&listaEjecucion);
    liberarListaPCB(&listaListos);
    liberarListaPCB(&listaTerminados);
    
    // Limpiar ventanas
    delwin(IDventanaCMD);
    delwin(IDventanaEjecucion);
    delwin(IDventanaListos);
    delwin(IDventanaTerminados);
           

    
  endwin();
  return 0;
}