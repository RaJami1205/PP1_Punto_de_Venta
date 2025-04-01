#ifndef INVOICES_H
#define INVOICES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "models.h"
#include "db_handler.h"
#include "products.h"
#include "quotations.h"

static Invoice *current_invoice;
static Sales_Point sales_point;

// Función para consultar facturas
bool check_invoices();

// Buscar factura mediante su id
bool search_invoice(int invoice_id);

// Busca las líneas de una facturacion mediante el id
void search_invoice_lines(int invoice_id);

// Menú para facturación
void invoices_menu();

// Proceso para cerrar factura
void close_invoice();

// Función para almacenar la factura en la bd
void save_invoice();

// Función para obtener el último id de facturas
int get_last_inv_id();

// Función para convertir cotizaciones a facturas
void quotation_to_invoice();

// Función para imprimir factura
void print_invoice();

// Función para ajustar cantidades
void adjust_quantities(Quotation *quot);

// Función para obtener fecha actual formateada
void get_current_date(char *date_str);

#endif /* INVOICES_H */