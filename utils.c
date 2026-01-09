#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

int leerEntero(int *out) {
    char buf[64];
    if (!fgets(buf, sizeof(buf), stdin)) return 0;
    char *end = NULL;
    long val = strtol(buf, &end, 10);
    if (end == buf) return 0;
    *out = (int)val;
    return 1;
}

int leerFlotante(float *out) {
    char buf[64];
    if (!fgets(buf, sizeof(buf), stdin)) return 0;
    char *end = NULL;
    float val = strtof(buf, &end);
    if (end == buf) return 0;
    *out = val;
    return 1;
}

void leerCadena(char *buf, size_t n) {
    if (fgets(buf, n, stdin)) {
        size_t len = strlen(buf);
        if (len && buf[len - 1] == '\n') buf[len - 1] = '\0';
    } else {
        if (n) buf[0] = '\0';
    }
}

void inicializarArchivos() {
    const char *archs[] = {"vehiculos.txt", "clientes.txt", "ventas.txt"};
    for (int i = 0; i < 3; i++) {
        FILE *f = fopen(archs[i], "a");
        if (f) fclose(f);
    }
}
