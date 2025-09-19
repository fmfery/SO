#ifndef OPERACIONES_H
#define OPERACIONES_H
#include <stdio.h>

int sumar(int a, int b);
int restar(int a, int b);
int multiplicar(int a, int b);
int dividir(int a, int b);
int incrementar(int a);
int decrementar(int a);

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

int incrementar(int a) {
    return a + 1;
}
int decrementar(int a) {
    return a - 1;
}


#endif