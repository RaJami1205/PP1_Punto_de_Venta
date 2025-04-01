#ifndef STATISTICS_H
#define STATISTICS_H

#include <stdio.h>
#include <string.h>
#include "db_handler.h"

// Función para imprimir la cantidad de cotizaciones pendientes
void print_pending_quotations();

// Función para imprimir la cantidad de cotizaciones facturadas
void print_quotations_with_invoices();

// Función para imprimir el promedio de compra total
void print_total_purchase_average();

// Función para imprimir los 5 productos más vendidos
void print_top_5_selling_products();

// Función para imprimir los productos más vendidos por familia
void print_top_product_per_family();

// Función para imprimir los montos vendidos por familia
void print_total_sales_per_family();

#endif /* STATISTICS_H */