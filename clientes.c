#include <stdio.h>
#include <string.h>
#include "clientes.h"
#include "utils.h"

static const char *ARCH_CLI = "clientes.txt";

void registrarCliente() {
    Cliente c;
    printf("ID: ");
    if (!leerEntero(&c.id)) { printf("ID invalido.\n"); return; }
    if (existeCliente(c.id)) { printf("Error: ID de cliente ya existe.\n"); return; }

    printf("Nombre: ");
    leerCadena(c.nombre, sizeof(c.nombre));
    printf("Edad: ");
    if (!leerEntero(&c.edad)) { printf("Edad invalida.\n"); return; }

    FILE *f = fopen(ARCH_CLI, "a");
    if (!f) { printf("Error: no se pudo abrir %s\n", ARCH_CLI); return; }
    fprintf(f, "%d,%s,%d\n", c.id, c.nombre, c.edad);
    fclose(f);
    printf("Cliente registrado.\n");
}

void mostrarClientes() {
    FILE *f = fopen(ARCH_CLI, "r");
    if (!f) { printf("No se pudo abrir %s\n", ARCH_CLI); return; }
    char linea[128];
    printf("\n--- Clientes ---\n");
    while (fgets(linea, sizeof(linea), f)) {
        Cliente c;
        if (sscanf(linea, "%d,%63[^,],%d", &c.id, c.nombre, &c.edad) == 3) {
            printf("ID:%d | %s | Edad:%d\n", c.id, c.nombre, c.edad);
        }
    }
    fclose(f);
}

int cargarClientePorId(int id, Cliente *c) {
    FILE *f = fopen(ARCH_CLI, "r");
    if (!f) return 0;
    char linea[128];
    while (fgets(linea, sizeof(linea), f)) {
        Cliente temp;
        if (sscanf(linea, "%d,%63[^,],%d", &temp.id, temp.nombre, &temp.edad) == 3) {
            if (temp.id == id) { *c = temp; fclose(f); return 1; }
        }
    }
    fclose(f);
    return 0;
}

int existeCliente(int id) {
    Cliente c;
    return cargarClientePorId(id, &c);
}
