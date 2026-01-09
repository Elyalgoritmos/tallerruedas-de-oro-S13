#include <stdio.h>
#include <string.h>
#include <time.h>
#include "ventas.h"
#include "vehiculos.h"
#include "clientes.h"

static const char *ARCH_VENT = "ventas.txt";

static void obtenerFechaISO(char *buf, size_t n) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    snprintf(buf, n, "%04d-%02d-%02d %02d:%02d:%02d",
             tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
             tm->tm_hour, tm->tm_min, tm->tm_sec);
}

void registrarVenta(int idCliente, const char *placaVehiculo) {
    Cliente c;
    Vehiculo v;

    if (!cargarClientePorId(idCliente, &c)) {
        printf("Error: cliente no existe.\n");
        return;
    }

    if (!cargarVehiculoPorPlaca(placaVehiculo, &v)) {
        printf("Error: vehiculo no existe.\n");
        return;
    }

    if (!v.disponible) {
        printf("Error: vehiculo ya fue vendido.\n");
        return;
    }

    FILE *f = fopen(ARCH_VENT, "a");
    if (!f) {
        printf("Error: no se pudo abrir archivo de ventas.\n");
        return;
    }

    int idVenta = 1;
    {
        FILE *fr = fopen(ARCH_VENT, "r");
        if (fr) {
            char linea[256];
            while (fgets(linea, sizeof(linea), fr)) idVenta++;
            fclose(fr);
        }
    }

    char fecha[32];
    obtenerFechaISO(fecha, sizeof(fecha));
    fprintf(f, "%d,%d,%s,%.2f,%s\n", idVenta, idCliente, v.placa, v.precio, fecha);
    fclose(f);

    if (marcarVehiculoVendido(placaVehiculo)) {
        printf("Venta registrada.\n");
        printf("ID Venta: %d | Cliente: %s | Vehiculo: %s %s | Precio: %.2f\n",
               idVenta, c.nombre, v.marca, v.modelo, v.precio);
    } else {
        printf("Error: no se pudo actualizar estado del vehiculo.\n");
    }
}
