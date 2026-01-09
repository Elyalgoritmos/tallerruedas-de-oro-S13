#ifndef CLIENTES_H
#define CLIENTES_H

typedef struct {
    int id;
    char nombre[64];
    int edad;
} Cliente;

void registrarCliente();
void mostrarClientes();
int cargarClientePorId(int id, Cliente *c);
int existeCliente(int id);

#endif
