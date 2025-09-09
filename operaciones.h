#include "operaciones.h"
#include <stdio.h>
#ifndef OPERACIONES_H
#define OPERACIONES_H

int sumar(int a, int b);
int restar(int a, int b);
int multiplicar(int a, int b);
int dividir(int a, int b);

int sumar(int a, int b) {
    return a + b;
}

int restar(int a, int b) {
    return a - b;
}

int multiplicar(int a, int b) {
    return a * b;
}

int dividir(int a, int b) {
    if (b == 0) {
        return 0;
    }
    return a / b;
}

#endif
