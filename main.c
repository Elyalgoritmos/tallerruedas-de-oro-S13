#include <stdio.h>
#include <stdlib.h>
#include "vehiculos.h"
#include "clientes.h"
#include "ventas.h"
#include "utils.h"

int main() {
    int opcion;
    inicializarArchivos(); // crea archivos si no existen

    do {
        printf("\n--- MENU SGIC ---\n");
        printf("1. Registrar Vehiculo\n");
        printf("2. Mostrar Inventario\n");
        printf("3. Buscar Vehiculos\n");
        printf("4. Registrar Cliente\n");
        printf("5. Mostrar Clientes\n");
        printf("6. Registrar Venta\n");
        printf("0. Salir\n");
        printf("Opcion: ");
        if (!leerEntero(&opcion)) { opcion = -1; }

        switch (opcion) {
            case 1:
                registrarVehiculo();
                break;

            case 2:
                mostrarInventario();
                break;

            case 3: {
                char marca[32], tipo[32];
                float presupuesto;
                printf("Marca (solo letras): ");
                leerCadena(marca, sizeof(marca));
                if (!validarSoloLetras(marca)) {
                    printf("Marca invalida. Debe contener solo letras.\n");
                    break;
                }
                printf("Tipo (camioneta/auto/etc.): ");
                leerCadena(tipo, sizeof(tipo));
                printf("Presupuesto: ");
                if (!leerFlotante(&presupuesto)) {
                    printf("Presupuesto invalido.\n");
                    break;
                }
                buscarVehiculos(marca, tipo, presupuesto);
                break;
            }

            case 4:
                registrarCliente();
                break;

            case 5:
                mostrarClientes();
                break;

            case 6: {
                int idCliente;
                char placaVehiculo[16];
                printf("ID Cliente: ");
                if (!leerEntero(&idCliente)) {
                    printf("ID Cliente invalido.\n");
                    break;
                }
                printf("Placa del Vehiculo (letras y numeros): ");
                leerCadena(placaVehiculo, sizeof(placaVehiculo));
                if (!validarAlfaNum(placaVehiculo)) {
                    printf("Placa invalida. Debe contener solo letras y numeros.\n");
                    break;
                }
                registrarVenta(idCliente, placaVehiculo);
                break;
            }

            case 0:
                printf("Saliendo del sistema...\n");
                break;

            default:
                printf("Opcion invalida. Intente nuevamente.\n");
                break;
        }
    } while (opcion != 0);

    return 0;
}
