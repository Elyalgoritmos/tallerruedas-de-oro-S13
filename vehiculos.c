#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h> // strcasecmp
#include "vehiculos.h"
#include "utils.h"

static const char *ARCH_VEH = "vehiculos.txt";

int validarSoloLetras(const char *cadena) {
    if (!cadena || !cadena[0]) return 0;
    for (int i = 0; cadena[i] != '\0'; i++) {
        if (!isalpha((unsigned char)cadena[i])) return 0;
    }
    return 1;
}

int validarAlfaNum(const char *cadena) {
    if (!cadena || !cadena[0]) return 0;
    for (int i = 0; cadena[i] != '\0'; i++) {
        if (!isalnum((unsigned char)cadena[i])) return 0;
    }
    return 1;
}

static void guardarVehiculo(const Vehiculo *v) {
    FILE *f = fopen(ARCH_VEH, "a");
    if (!f) { printf("Error: no se pudo abrir %s\n", ARCH_VEH); return; }
    fprintf(f, "%s,%s,%s,%s,%s,%.2f,%d\n",
            v->placa, v->marca, v->modelo, v->tipo, v->estado, v->precio, v->disponible);
    fclose(f);
}

void registrarVehiculo() {
    Vehiculo v;

    // Placa: letras y números
    do {
        printf("Placa (letras y numeros, sin espacios): ");
        leerCadena(v.placa, sizeof(v.placa));
        if (!validarAlfaNum(v.placa)) {
            printf("Placa invalida. Debe contener solo letras y numeros.\n");
        }
    } while (!validarAlfaNum(v.placa));

    if (existeVehiculo(v.placa)) {
        printf("Error: la placa ya existe.\n");
        return;
    }

    // Marca: solo letras
    do {
        printf("Marca (solo letras): ");
        leerCadena(v.marca, sizeof(v.marca));
        if (!validarSoloLetras(v.marca)) {
            printf("Marca invalida. Solo se permiten letras.\n");
        }
    } while (!validarSoloLetras(v.marca));

    // Modelo: solo letras
    do {
        printf("Modelo (solo letras): ");
        leerCadena(v.modelo, sizeof(v.modelo));
        if (!validarSoloLetras(v.modelo)) {
            printf("Modelo invalido. Solo se permiten letras.\n");
        }
    } while (!validarSoloLetras(v.modelo));

    // Tipo: libre
    printf("Tipo (camioneta/auto/etc.): ");
    leerCadena(v.tipo, sizeof(v.tipo));

    // Estado: nuevo/usado
    do {
        printf("Estado (nuevo/usado): ");
        leerCadena(v.estado, sizeof(v.estado));
        if (strcasecmp(v.estado, "nuevo") != 0 && strcasecmp(v.estado, "usado") != 0) {
            printf("Estado invalido. Debe ser 'nuevo' o 'usado'.\n");
        }
    } while (strcasecmp(v.estado, "nuevo") != 0 && strcasecmp(v.estado, "usado") != 0);

    // Precio
    printf("Precio: ");
    if (!leerFlotante(&v.precio)) {
        printf("Precio invalido.\n");
        return;
    }

    v.disponible = 1;
    guardarVehiculo(&v);
    printf("Vehiculo registrado.\n");
}

void mostrarInventario() {
    FILE *f = fopen(ARCH_VEH, "r");
    if (!f) { printf("No se pudo abrir %s\n", ARCH_VEH); return; }
    char linea[256];
    printf("\n--- Inventario ---\n");
    while (fgets(linea, sizeof(linea), f)) {
        Vehiculo v;
        if (sscanf(linea, "%15[^,],%31[^,],%31[^,],%31[^,],%15[^,],%f,%d",
                   v.placa, v.marca, v.modelo, v.tipo, v.estado, &v.precio, &v.disponible) == 7) {
            printf("Placa:%s | %s %s | Tipo:%s | %s | Precio:%.2f | %s\n",
                   v.placa, v.marca, v.modelo, v.tipo, v.estado, v.precio,
                   v.disponible ? "Disponible" : "Vendido");
        }
    }
    fclose(f);
}

void buscarVehiculos(const char *marca, const char *tipo, float presupuesto) {
    FILE *f = fopen(ARCH_VEH, "r");
    if (!f) { printf("No se pudo abrir %s\n", ARCH_VEH); return; }
    char linea[256];
    int encontrados = 0;
    printf("\n--- Resultados de busqueda ---\n");
    while (fgets(linea, sizeof(linea), f)) {
        Vehiculo v;
        if (sscanf(linea, "%15[^,],%31[^,],%31[^,],%31[^,],%15[^,],%f,%d",
                   v.placa, v.marca, v.modelo, v.tipo, v.estado, &v.precio, &v.disponible) == 7) {
            if (v.disponible &&
                strcasecmp(v.marca, marca) == 0 &&
                strcasecmp(v.tipo, tipo) == 0 &&
                v.precio <= presupuesto) {
                printf("Placa:%s | %s %s | Tipo:%s | %s | Precio:%.2f\n",
                       v.placa, v.marca, v.modelo, v.tipo, v.estado, v.precio);
                encontrados++;
            }
        }
    }
    if (!encontrados) printf("No se encontraron vehiculos con esos criterios.\n");
    fclose(f);
}

int cargarVehiculoPorPlaca(const char *placa, Vehiculo *v) {
    FILE *f = fopen(ARCH_VEH, "r");
    if (!f) return 0;
    char linea[256];
    while (fgets(linea, sizeof(linea), f)) {
        Vehiculo temp;
        if (sscanf(linea, "%15[^,],%31[^,],%31[^,],%31[^,],%15[^,],%f,%d",
                   temp.placa, temp.marca, temp.modelo, temp.tipo, temp.estado,
                   &temp.precio, &temp.disponible) == 7) {
            if (strcmp(temp.placa, placa) == 0) {
                *v = temp;
                fclose(f);
                return 1;
            }
        }
    }
    fclose(f);
    return 0;
}

int marcarVehiculoVendido(const char *placa) {
    FILE *f = fopen(ARCH_VEH, "r");
    if (!f) return 0;

    FILE *tmp = fopen("vehiculos_tmp.txt", "w");
    if (!tmp) { fclose(f); return 0; }

    char linea[256];
    int actualizado = 0;
    while (fgets(linea, sizeof(linea), f)) {
        Vehiculo v;
        if (sscanf(linea, "%15[^,],%31[^,],%31[^,],%31[^,],%15[^,],%f,%d",
                   v.placa, v.marca, v.modelo, v.tipo, v.estado, &v.precio, &v.disponible) == 7) {
            if (strcmp(v.placa, placa) == 0) { v.disponible = 0; actualizado = 1; }
            fprintf(tmp, "%s,%s,%s,%s,%s,%.2f,%d\n",
                    v.placa, v.marca, v.modelo, v.tipo, v.estado, v.precio, v.disponible);
        }
    }
    fclose(f);
    fclose(tmp);

    remove(ARCH_VEH);
    rename("vehiculos_tmp.txt", ARCH_VEH);
    return actualizado;
}

int existeVehiculo(const char *placa) {
    Vehiculo v;
    return cargarVehiculoPorPlaca(placa, &v);
}
