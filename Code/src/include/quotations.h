#ifndef QUOTATIONS_H
#define QUOTATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "models.h"
#include "db_handler.h"
#include "products.h"
#include "menus.h"

#define MAX_INPUT 100
#define MAX_LINES 100

static Quotation current_quotation;
static bool edit_quotation = false;

// Función para obtener el puntero de la cotización actual
Quotation *get_current_quotation();

// Función para obtener el último id de cotización
int get_last_quo_id();

// Función para verificar la existencia de un producto
bool product_exists(const char *input, Product *product);

// Función para manejar las opciones para cotizar producto
void quote_product(bool is_filtered);

// Función para encontrar linea conforme a nombre de producto
Quotation_Line *find_product_line(const char *product_name);

// Función para agregar linea a la cotización
void add_product_to_quotation();

// Función para remover una línea de la cotización
void rm_product_from_quotation(int line_to_remove);

// Función para imprimir detalles de cotización
void seek_quotation();

// Función para consultar si desea salir sin guardar
void ask_save_quotation();

// Función para guardar la cotización en la bd
void save_quotation();

// Función para imprimir las cotizaciones
void print_all_quotations(bool show_all_quots);

// Función para imprimir la cotización
void print_quotation();

// Función para imprimir el menú de cotización
void print_quotation_menu();

// Función para manejar la creación de nueva cotización
void new_quotation();

// Buscar cotización mediante su id
void search_quotation(int quotation_id);

// Busca las líneas de una cotización mediante el id de la cotización
void search_quotation_lines(int quotation_id);

// Establece la cotización actual que está leyendo el sistema
void set_current_quot();

// Función para editar cotización existente
void modify_quotation();

#endif /* QUOTATIONS_H */