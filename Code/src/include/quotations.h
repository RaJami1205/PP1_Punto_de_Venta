#ifndef QUOTATIONS_H
#define QUOTATIONS_H

#include "models.h"
#include "db_handler.h"

#define MAX_INPUT 100
#define MAX_LINES 100

static Quotation current_quotation;

// TO-DO
int get_last_quo_id();

// TO-DO
bool product_exists(const char *input, char *product_name);

// TO-DO
void quote_product(bool is_filtered);

// TO-DO
void add_product_to_quotation();

// TO-DO
void rm_product_from_quotation();

// TO-DO
void filter_products();

// TO-DO
void save_quotation();

// TO-DO
void print_quotation();

// TO-DO
void print_quotation_menu();

// TO-DO
void new_quotation();

#endif /* QUOTATIONS_H */