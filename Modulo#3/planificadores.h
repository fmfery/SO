#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H


#include "interprete.h"
#include "listaProcesos.h"

#define MAX_PROGRAMAS 5
#define QUANTUM 4
#define MAX_LISTOS 3



extern int pausa;
extern int contador_Q;

extern int AX_global, BX_global, CX_global, DX_global;

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

/*
    PLANIFICADOR A LARGO PLAZO
    Controla el paso de procesos de NUEVOS a LISTOS
    Se ejecuta cada ciclo, verifica si hay espacio en LISTOS
    Límite máximo en LISTOS: definido por MAX_LISTOS
*/
void PlanificadorLargoPlazo(WINDOW* ventanaNuevos, WINDOW* ventanaListos) {
    // No hacer nada si el sistema está en pausa
    if (pausa) return;
    
    int cantidadListos = contarProcesos(listaListos);
    
    // Si hay espacio en LISTOS y hay procesos nuevos esperando
    if (cantidadListos < MAX_LISTOS && listaNuevos != NULL) {
        // Mover un proceso de NUEVOS a LISTOS
        struct PCB* procesoNuevo = removerPrimero(&listaNuevos);
        agregarPCB(&listaListos, procesoNuevo);
        
        // Actualizar ventanas con delay para visualizar la transición
        actualizarVentanaNuevos(ventanaNuevos);
        actualizarVentanaListos(ventanaListos);
        usleep(700000); // 0.7 segundos de delay
    }
}

/*
    PLANIFICADOR A CORTO PLAZO

*/
void PlanificadorCortoPlazo(WINDOW* ventanaEjecucion, WINDOW* ventanaListos, 
                            WINDOW* ventanaTerminados, WINDOW* ventanaNuevos) {
    
    // No hacer nada si el sistema está en pausa
    if (pausa) return;
    
    // Si hay un proceso en ejecución
    if (listaEjecucion != NULL) {
        struct PCB* procesoActual = listaEjecucion;
        
        // Cargar contexto solo al inicio
        if (contador_Q == 0) {
            CargarContexto(procesoActual);
        }
        
        // Ejecutar una instrucción
        int resultado = ejecutarInstruccionPCB(procesoActual);
        contador_Q++;
        
        // Actualizar ventana de ejecución
        actualizarVentanaEjecucion(ventanaEjecucion, procesoActual);
        usleep(1000000); // 1 segundo
        
        // Si el proceso terminó (bien o con error)
        if (resultado == 1) {
            GuardarContexto(procesoActual);
            
            // Mover proceso a terminados
            struct PCB* terminado = removerPrimero(&listaEjecucion);
            agregarPCB(&listaTerminados, terminado);
            
            // Actualizar ventanas
            actualizarVentanaEjecucion(ventanaEjecucion, NULL);
            actualizarVentanaTerminados(ventanaTerminados);
            
            // Reiniciar quantum
            contador_Q = 0;
        }
        // Si se agotó el quantum (cambio de contexto)
        else if (contador_Q >= QUANTUM) {
            GuardarContexto(procesoActual);
            
            // Mover proceso actual de EJECUCIÓN a LISTOS
            struct PCB* procesoTemporal = removerPrimero(&listaEjecucion);
            actualizarVentanaEjecucion(ventanaEjecucion, NULL);
            agregarPCB(&listaListos, procesoTemporal);
            
            // Si hay más procesos en listos
            if (listaListos != NULL && listaListos != procesoTemporal) {
                usleep(1000000);
                actualizarVentanaListos(ventanaListos);
                usleep(1000000);
            } else {
                usleep(1000000);
                actualizarVentanaListos(ventanaListos);
                usleep(1000000);
            }
            
            // Reiniciar quantum
            contador_Q = 0;
        }
    }
    // Si NO hay proceso en ejecución pero SÍ hay en listos
    else if (listaListos != NULL) {
        struct PCB* siguiente = removerPrimero(&listaListos);
        agregarPCB(&listaEjecucion, siguiente);
        
        // Actualizar ventanas
        actualizarVentanaListos(ventanaListos);
        actualizarVentanaEjecucion(ventanaEjecucion, listaEjecucion);
        
        contador_Q = 0;
    }
}

#endif