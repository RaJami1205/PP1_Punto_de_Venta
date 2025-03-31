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

void invoices_menu();

void close_invoice();

void save_invoice();

int get_last_inv_id();

void quotation_to_invoice();

void print_invoice();

void adjust_quantities(Quotation *quot);

void get_current_date(char *date_str);

#endif /* INVOICES_H */