#include "./include/invoices.h"

void invoices_menu() {

    MYSQL *conn = connect_to_db();
    sales_point = get_sales_point(conn);
    close_db_connection(conn);

    printf("\nFacturar Cotización\n");

    print_all_quotations(false);

    set_current_quot(); 
    quotation_to_invoice();

    // Obtener la fecha actual del sistema
    get_current_date(current_invoice->date);

    // Obtener el nombre del cliente
    printf("\n\nIngrese el nombre del cliente: ");
    scanf(" %[^\n]", current_invoice->customer_name);  // Captura con espacios

    print_invoice();
}

int get_last_inv_id() {
    
    MYSQL *conn = connect_to_db();
    if (!conn) {
        fprintf(stderr, "Error al conectar con la base de datos.\n");
        return -1;
    }
    
    MYSQL_RES *result = get_last_invoice_id(conn);
    if (!result) {
        close_db_connection(conn);
        return -1;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int last_id = (row && row[0]) ? atoi(row[0]) : 0;
    
    mysql_free_result(result);
    close_db_connection(conn);
    
    return last_id;
}

void quotation_to_invoice() {
    Quotation *quot = get_current_quotation();
    
    if (quot == NULL) {
        printf("Error: No hay cotización activa.\n");
        return;
    }

    // Crear nueva factura
    current_invoice = (Invoice *)malloc(sizeof(Invoice));
    if (current_invoice == NULL) {
        printf("Error: No se pudo asignar memoria para la factura.\n");
        return;
    }

    // Copiar datos de la cotización
    current_invoice->id = get_last_inv_id() + 1;
    current_invoice->quotation_reference_id = quot->id;
    current_invoice->sub_total = quot->sub_total;
    current_invoice->total_taxes = quot->total_taxes;
    current_invoice->total = quot->total;
    current_invoice->num_lines = quot->num_lines;

    // Asignar memoria para las líneas de factura
    current_invoice->lines = (Invoice_Line *)malloc(sizeof(Invoice_Line) * quot->num_lines);
    if (current_invoice->lines == NULL) {
        printf("Error: No se pudo asignar memoria para las líneas de factura.\n");
        free(current_invoice);
        return;
    }

    // Convertir cada línea de cotización en una línea de factura
    for (int i = 0; i < quot->num_lines; i++) {
        Quotation_Line *ql = &quot->lines[i];
        Invoice_Line *il = &current_invoice->lines[i];

        il->line_id = ql->line_id;
        il->invoice_id = current_invoice->id;
        strcpy(il->product_name, ql->product_name);
        il->quantity = ql->quantity;
        il->price = ql->price; 
        il->line_sub_total = ql->line_sub_total;
        il->line_total_taxes = ql->line_total_taxes;
    }
}

void print_invoice() {
    printf("\nFactura  : N°%-18d %-40s %-30s\n", current_invoice->id, "", sales_point.name);
    printf("Fecha    : %-20s %-40s %-30s\n", current_invoice->date, "", sales_point.legal_entity_id);
    printf("Cliente  : %-50s %-10s %-30s\n", current_invoice->customer_name, "", sales_point.phone_number);
    
    printf("┌─────────┬──────────────────────────────┬─────────┬─────────────┬───────────────┬───────────┐\n");
    printf("│ # Línea │ Producto                     │ Cant    │ Precio      │ Subtotal      │ IVA       │\n");
    printf("├─────────┼──────────────────────────────┼─────────┼─────────────┼───────────────┼───────────┤\n");

    for (int i = 0; i < current_invoice->num_lines; i++) {
        printf("│ %-7d │ %-28s │ %-7d │ %-11.2f │ %-13.2f │ %-9.2f │\n",
                i + 1,  // Usamos i+1 para numerar las líneas
                current_invoice->lines[i].product_name,
                current_invoice->lines[i].quantity,
                current_invoice->lines[i].price,
                current_invoice->lines[i].line_sub_total,
                current_invoice->lines[i].line_total_taxes);
    }
    printf("└─────────┴──────────────────────────────┴─────────┴─────────────┴───────────────┴───────────┘\n");
    printf("\n                                                                   Subtotal :      %-9.2f  \n", current_invoice->sub_total);
    printf("                                                                   IVA Total:      %-9.2f  \n", current_invoice->total_taxes);
    printf("                                                                   Total    :      %-9.2f  \n", current_invoice->total);
}

void adjust_quantities()
{
}

void get_current_date(char *date_str) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date_str, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}