#ifndef PRODUCTS_H
#define PRODUCTS_H

#include "models.h"
#include "db_handler.h"

// Función para imprimir productos en catálogo
void print_products();

// Función para preguntar al usuario método de filtrado de productos y printearlos
void print_filtered_products();

// Función para agregar nuevas familias de producto
void set_product_family();

// Función para agregar nuevos productos
void set_product();

// Función para eliminar un producto mediante su ID
void delete_product();

// Función para aumentar cantidad en stock de los productos
void loadProductStock();

#endif /* PRODUCTS_H */