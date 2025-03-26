#ifndef PRODUCTS_H
#define PRODUCTS_H

#include "models.h"
#include "db_handler.h"

// Función para imprimir productos en catálogo
void print_products();

// Función para agregar nuevas familias de producto
void set_product_family();

// Función para agregar nuevos productos
void set_product();

// Función para eliminar un producto mediante su ID
void delete_product();

#endif /* PRODUCTS_H */