#ifndef VEHICULOS_H
#define VEHICULOS_H

typedef struct {
    char placa[16];       // identificador único (letras y números)
    char marca[32];       // solo letras
    char modelo[32];      // solo letras
    char tipo[32];        // camioneta, auto, etc.
    char estado[16];      // nuevo o usado
    float precio;
    int disponible;       // 1 disponible, 0 vendido
} Vehiculo;

void registrarVehiculo();
void mostrarInventario();
void buscarVehiculos(const char *marca, const char *tipo, float presupuesto);
int cargarVehiculoPorPlaca(const char *placa, Vehiculo *v);
int marcarVehiculoVendido(const char *placa);
int existeVehiculo(const char *placa);

// Validaciones reutilizables
int validarSoloLetras(const char *cadena);
int validarAlfaNum(const char *cadena);

#endif
