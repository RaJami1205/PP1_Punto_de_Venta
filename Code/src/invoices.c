#include "./include/invoices.h"

bool check_invoices() {

    MYSQL *conn = connect_to_db();
    if (!conn) {
        printf("\n❌ Error al conectar con la base de datos.\n");
        return false;
    }

    MYSQL_RES *result = get_invoices(conn);
    if (!result) {
        printf("\n❌ Error al obtener las facturas.\n");
        close_db_connection(conn);
        return false;
    }

    if (mysql_num_rows(result) == 0) {
        printf("\n❌ No hay facturas disponibles.\n");
        mysql_free_result(result);
        close_db_connection(conn);
        return false;
    }

    MYSQL_ROW row;

    printf("\nListado de Facturas\n");
    printf("┌────────┬──────────────────────┬────────────────┬────────────────┬──────────────────┐\n");
    printf("│ # Fact │ Fecha Y Hora         │ Subtotal       │ IVA            │ Total            │\n");
    printf("├────────┼──────────────────────┼────────────────┼────────────────┼──────────────────┤\n");

    while ((row = mysql_fetch_row(result))) {
        printf("│ %-6s │ %-20s │ %-14s │ %-14s │ %-16s │\n",
                row[0], row[2], row[4], row[5], row[6]);
    }
    printf("└────────┴──────────────────────┴────────────────┴────────────────┴──────────────────┘\n");

    mysql_free_result(result);
    
    int invoice_id;
    char input[20];

    printf("Seleccione una factura mediante su ID para ver los detalles\n= ");
    fgets(input, sizeof(input), stdin);

    if (sscanf(input, "%d", &invoice_id) != 1 || invoice_id <= 0) {
        printf("\n❌ ID de factura inválido.\n");
        close_db_connection(conn);
        return false;
    }

    if (search_invoice(invoice_id)) {
        search_invoice_lines(invoice_id);
        print_invoice();
        close_db_connection(conn);
        return true;
    }

    close_db_connection(conn);
    return false;
}


bool search_invoice(int invoice_id) {
    MYSQL *conn = connect_to_db();
    if (!conn) {
        printf("\n❌ Error al conectar con la base de datos.\n");
        return false;
    }

    // Obtener cotización
    MYSQL_RES *invoice_result = get_invoice_by_id(conn, invoice_id);
    if (!invoice_result) {
        printf("\n❌ No se encontró la factura con ID %d.\n", invoice_id);
        close_db_connection(conn);
        return false;
    }

    // Asignar datos de la cotización a current_quotation
    MYSQL_ROW row = mysql_fetch_row(invoice_result);
    if (!row) {
        printf("\n❌ No se encontraron datos para la factura.\n");
        mysql_free_result(invoice_result);
        close_db_connection(conn);
        return false;
    }

    current_invoice = (Invoice *)malloc(sizeof(Invoice));
    if (current_invoice == NULL) {
        printf("❌ Error: No se pudo asignar memoria para la factura.\n");
        return false;
    }

    current_invoice->id = atoi(row[0]);
    current_invoice->quotation_reference_id = atoi(row[1]);
    strncpy(current_invoice->date, row[2], sizeof(current_invoice->date) - 1);
    strncpy(current_invoice->customer_name, row[3], sizeof(current_invoice->customer_name) - 1);
    current_invoice->sub_total = atof(row[4]);
    current_invoice->total_taxes = atof(row[5]);
    current_invoice->total = atof(row[6]);
    current_invoice->num_lines = 0;
    current_invoice->lines = NULL;

    mysql_free_result(invoice_result);
    close_db_connection(conn);
    return true;
}

void search_invoice_lines(int invoice_id) {
    MYSQL *conn = connect_to_db();
    MYSQL_ROW row;
    
    // Obtener líneas de factura
    MYSQL_RES *lines_result = get_invoice_lines(conn, invoice_id);
    if (!lines_result) {
        printf("\n❌ Error al obtener las líneas de la factura.\n");
        close_db_connection(conn);
        return;
    }

    int num_lines = mysql_num_rows(lines_result);
    if (num_lines == 0) {
        printf("\n❌ No hay líneas de factura para la factura ID %d.\n", invoice_id);
        mysql_free_result(lines_result);
        close_db_connection(conn);
        return;
    }

    // Si se encuentran líneas, asignarlas a la estructura actual
    current_invoice->lines = malloc(num_lines * sizeof(Invoice_Line));
    if (!current_invoice->lines) {
        printf("\n❌ Error al asignar memoria para las líneas de factura.\n");
        mysql_free_result(lines_result);
        close_db_connection(conn);
        return;
    }

    int i = 0;
    while ((row = mysql_fetch_row(lines_result))) {
        current_invoice->lines[i].line_id = atoi(row[0]);
        current_invoice->lines[i].invoice_id = atoi(row[1]);
        strncpy(current_invoice->lines[i].product_name, row[2], sizeof(current_invoice->lines[i].product_name) - 1);
        current_invoice->lines[i].quantity = atoi(row[3]);
        current_invoice->lines[i].price = atof(row[4]);
        current_invoice->lines[i].line_sub_total = atof(row[5]);
        current_invoice->lines[i].line_total_taxes = atof(row[6]);
        i++;
    }
    current_invoice->num_lines = num_lines;

    mysql_free_result(lines_result);
    close_db_connection(conn);
}


bool set_current_invoice() {
    int invoice_id;
    char input[20];

    while (1) {

        printf("Seleccione una factura mediante su ID\n= ");
        fgets(input, sizeof(input), stdin);

        if (sscanf(input, "%d", &invoice_id) == 1 && invoice_id > 0) {
            break;
        } else {
            printf("\n\n❌ ID inválido. Intente de nuevo.\n\n");
        }
    }

    if (search_invoice(invoice_id)) {
        search_invoice_lines(invoice_id);
        return true;
    }
    return false;
}

void invoices_menu() {
    printf("\nFacturar Cotización\n");

    if(!print_all_quotations(false)) {
        return;
    }

    int option;
    char input[20];  // Buffer para la entrada del usuario

    while (1) {
        printf("\nOpciones:\n");
        printf("[1] Seleccionar Cotización\n");
        printf("[2] Ver Detalles de Cotización\n");
        printf("[3] Salir\n");

        printf("\nSeleccione una opción\n= ");
        fgets(input, sizeof(input), stdin);

        if (sscanf(input, "%d", &option) == 1 && option >= 1 && option <= 3) {
            break;  // Si la opción es válida, salir del bucle
        } else {
            printf("\n❌ Opción inválida. Por favor ingrese una opción entre 1 y 3.\n");
        }
    }

    switch (option) {
        case 1:
            close_invoice();
            break;
        case 2:
            seek_quotation();
            invoices_menu();
        case 3:
            print_general_submenu();
            break;
        default:
            printf("\n\n❌ Opción Inválida, Intente de nuevo.\n\n");
            invoices_menu();
            break;
    }
}

void close_invoice() {
    MYSQL *conn = connect_to_db();
    sales_point = get_sales_point(conn);
    close_db_connection(conn);

    set_current_quot(); 

    quotation_to_invoice();

    get_current_date(current_invoice->date);

    printf("\n\nIngrese el nombre del cliente para la factura: ");
    fgets(current_invoice->customer_name, sizeof(current_invoice->customer_name), stdin);
    current_invoice->customer_name[strcspn(current_invoice->customer_name, "\n")] = '\0';  // Limpiar el salto de línea

    print_invoice();


    char option;
    char input[3];

    while (1) {
        printf("\n¿Desea realizar la facturación? (s/n)\n= ");
        fgets(input, sizeof(input), stdin);

        // Verificamos si el input es válido
        if (sscanf(input, "%c", &option) == 1 && (option == 's' || option == 'S' || option == 'n' || option == 'N')) {
            break;
        } else {
            printf("❌ Entrada inválida. Por favor, ingrese 's' o 'n'.\n");
        }
    }

    if (option == 's' || option == 'S') {
        save_invoice();
    } else {
        invoices_menu();
    }
}


void save_invoice() {
    if (current_invoice->num_lines == 0) {
        printf("\n❌ No hay productos en la factura para guardar.\n");
        return;
    }

    MYSQL *conn = connect_to_db();
    if (!conn) {
        printf("\n❌ Error al conectar con la base de datos.\n");
        return;
    }

    create_invoice(conn, current_invoice);

    for (int i = 0; i < current_invoice->num_lines; i++) {
        current_invoice->lines[i].invoice_id = current_invoice->id;
        add_line_to_invoice(conn, &current_invoice->lines[i]);
    }

    printf("\n✅ Facturación guardada exitosamente.\n");
    close_db_connection(conn);
    print_general_submenu();
}

int get_last_inv_id() {
    
    MYSQL *conn = connect_to_db();
    if (!conn) {
        fprintf(stderr, "❌ Error al conectar con la base de datos.\n");
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
        printf("❌ Error: No hay cotización activa.\n");
        return;
    }

    // Crear nueva factura
    current_invoice = (Invoice *)malloc(sizeof(Invoice));
    if (current_invoice == NULL) {
        printf("❌ Error: No se pudo asignar memoria para la factura.\n");
        return;
    }

    adjust_quantities(quot);

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
        printf("❌ Error: No se pudo asignar memoria para las líneas de factura.\n");
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
                i + 1,
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

void adjust_quantities(Quotation *quot) {

    int product_stock;
    bool adjusted;

    for (int i = 0; i < quot->num_lines; i++) {

        MYSQL *conn = connect_to_db();

        Quotation_Line *ql = &quot->lines[i];

        MYSQL_RES *result = search_product(conn, ql->product_name);
        MYSQL_ROW product_row = mysql_fetch_row(result);


        product_stock = (product_row[5] ? atoi(product_row[5]) : 0);

        close_db_connection(conn);

        if (ql->quantity > product_stock) {
            adjusted = false;


            while (!adjusted) {
                printf("La línea N°%d %s cotiza %d unidades, pero en stock hay únicamente %d\n", 
                       ql->line_id, ql->product_name, ql->quantity, product_stock);
                printf("¿Desea ajustar la cantidad? (s/n)\n");
                printf("[S]í, quiero ajustar la cantidad\n");
                printf("[N]o, borrar de la factura\n");

                char option;
                char input[3];

                while (1) {
                    printf("Ingrese una opción: ");
                    fgets(input, sizeof(input), stdin);

                    if (sscanf(input, "%c", &option) == 1 && 
                        (option == 's' || option == 'S' || option == 'n' || option == 'N')) {
                        break; 
                    } else {
                        printf("❌ Entrada inválida. Por favor, ingrese 's' o 'n'.\n");
                    }


                }
            
                if (option == 's' || option == 'S') {

                    if (product_stock <= 0) {
                        printf("❌ Entrada inválida. No se puede ajustar a 0.\n");
                    } else {
                        ql->quantity = product_stock;

                        quot->sub_total -= ql->line_sub_total;
                        quot->total_taxes -= ql->line_total_taxes;
                        quot->total -= ql->line_sub_total + ql->line_total_taxes;
    
                        ql->line_sub_total = ql->quantity * ql->price;
                        ql->line_total_taxes = ql->line_sub_total * 0.13;
    
                        quot->sub_total += ql->line_sub_total;
                        quot->total_taxes += ql->line_total_taxes;
                        quot->total += ql->line_sub_total + ql->line_total_taxes;
    
                        adjusted = true;
                    }
                } else {
                    if (quot->num_lines <= 1) {
                        printf("\n⚠️ Advertencia: La factura no puede quedar sin líneas\n");
                    } else {
                        rm_product_from_quotation(ql->line_id);
                        i--;
                        adjusted = true;
                    }
                }
            }
        }
    }
}

void get_current_date(char *date_str) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date_str, "%04d-%02d-%02d %02d:%02d:%02d", 
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec);
}