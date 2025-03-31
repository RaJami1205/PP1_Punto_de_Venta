#include "./include/quotations.h"

int get_last_quo_id() {
    MYSQL *conn = connect_to_db();
    if (!conn) {
        fprintf(stderr, "Error al conectar con la base de datos.\n");
        return -1;
    }
    
    MYSQL_RES *result = get_last_quot_id(conn);
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

bool product_exists(const char *input, Product *product) {
    MYSQL *conn = connect_to_db();
    if (conn == NULL) {
        return false;
    }

    MYSQL_RES *result = search_product(conn, input);
    if (result == NULL) {
        printf("Error al ejecutar la búsqueda o producto no encontrado.\n");
        close_db_connection(conn);
        return false;
    } else {
        MYSQL_ROW row = mysql_fetch_row(result);
        if (!row) {
            printf("Producto no encontrado.\n");
            mysql_free_result(result);
            close_db_connection(conn);
            return false;
        }

        strncpy(product->code, row[0], sizeof(product->code) - 1);
        strncpy(product->name, row[1], sizeof(product->name) - 1);
        strncpy(product->family, row[2], sizeof(product->family) - 1);
        product->cost = row[3] ? atof(row[3]) : 0.0;
        product->price = row[4] ? atof(row[4]) : 0.0;
        product->stock = row[5] ? atoi(row[5]) : 0;
    
        mysql_free_result(result);
        close_db_connection(conn);
        return true;
    }
}

void quote_product(bool is_filtered) {
    int opt;
    
    if (!is_filtered) {
        print_products(); 
    } else {
        print_filtered_products();
    }

    printf("\n\n[1] Seleccionar Producto\n");
    printf("[2] Filtrar por Familia\n");
    printf("[3] Mostrar Todos los Productos\n");
    printf("[4] Regresar\n");
    
    printf("\nSeleccione una opción\n= ");
    scanf("%d", &opt);

    switch ((opt))
    {
    case 1:
        add_product_to_quotation();
        break;
    case 2:
        quote_product(true);
        break;
    case 3:
        quote_product(false);
        break;
    case 4:
        print_quotation_menu();
        break;
    default:
        printf("\n\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\nOpción Inválida, Intente de nuevo.\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
        quote_product(false);
        break;
    }
}

Quotation_Line *find_product_line(const char *product_name) {
    for (int i = 0; i < current_quotation.num_lines; i++) {
        if (strcmp(current_quotation.lines[i].product_name, product_name) == 0) {
            return &current_quotation.lines[i];
        }
    }
    return NULL;
}

void add_product_to_quotation() {

    char product_input[MAX_INPUT];
    int quantity;
    Product selected_product;

    while (getchar() != '\n' && getchar() != EOF);

    printf("\nIngrese el ID o nombre del producto que desea agregar (0 para cancelar): ");
    fgets(product_input, MAX_INPUT, stdin);
    product_input[strcspn(product_input, "\n")] = '\0';

    if (strcmp(product_input, "0") == 0) {
        printf("\nOperación cancelada.\n");
        print_quotation_menu();
        return;
    }

    if (!product_exists(product_input, &selected_product)) {
        printf("\nError: El producto '%s' no existe. Intente nuevamente.\n", product_input);
        add_product_to_quotation();
        return;
    }

    bool correct_quantity = false;

    while (!correct_quantity) {
        printf("Ingrese la cantidad deseada: ");
        if (scanf("%d", &quantity) != 1 || quantity <= 0 || quantity > selected_product.stock) {

            printf("\nError: Cantidad no válida. Intente nuevamente.\n");
            while (getchar() != '\n');
        } else {
            correct_quantity = true;
        }
    }

    Quotation_Line *existing_line = find_product_line(selected_product.name);

    if (existing_line) {
        if (selected_product.stock - existing_line->quantity <= 0) {
            printf("\nError: No hay más cantidad en stock para agregar.\n");
            quote_product(false);
            return;
        }
        // Si el producto ya está en la cotización, actualizar cantidad y total


        if (existing_line->quantity + quantity > selected_product.stock) {
            printf("\nError: No hay más cantidad en stock para agregar.\n");
            quote_product(false);
            return;
        }

        current_quotation.sub_total -= existing_line->line_sub_total;
        current_quotation.total_taxes -= existing_line->line_total_taxes;

        existing_line->quantity += quantity;
        existing_line->line_sub_total = existing_line->price * existing_line->quantity;
        existing_line->line_total_taxes = existing_line->line_sub_total * 0.13;

        current_quotation.sub_total += current_quotation.lines->line_sub_total;
        current_quotation.total_taxes += current_quotation.lines->line_total_taxes;
        current_quotation.total = current_quotation.sub_total + current_quotation.total_taxes;

        printf("\nCantidad actualizada en la cotización.\n");
    } else {
        // Agregar nuevo producto a la cotización
        current_quotation.lines = realloc(current_quotation.lines, (current_quotation.num_lines + 1) * sizeof(Quotation_Line));
        if (!current_quotation.lines) {
            printf("\nError: No se pudo asignar memoria.\n");
            print_quotation_menu();
            return;
        }

        Quotation_Line *new_line = &current_quotation.lines[current_quotation.num_lines];
        new_line->line_id = current_quotation.num_lines + 1;
        new_line->quotation_id = current_quotation.id;
        strcpy(new_line->product_name, selected_product.name);
        new_line->quantity = quantity;
        new_line->price = selected_product.price;
        new_line->line_sub_total = selected_product.price * quantity;
        new_line->line_total_taxes = new_line->line_sub_total * 0.13;

        current_quotation.sub_total += current_quotation.lines->line_sub_total;
        current_quotation.total_taxes += current_quotation.lines->line_total_taxes;
        current_quotation.total = current_quotation.sub_total + current_quotation.total_taxes;
        current_quotation.num_lines++;

        printf("\nProducto agregado correctamente.\n");
    }

    char option;
    printf("\n¿Desea agregar otro producto? (s/n): ");
    while (getchar() != '\n');
    scanf("%c", &option);

    if (option == 's' || option == 'S') {
        print_products();
        add_product_to_quotation();
    } else {
        print_quotation_menu();
    }
}

void rm_product_from_quotation() {
    int line_to_remove;

    if (current_quotation.num_lines == 0) {
        printf("\nNo hay productos en la cotización para eliminar.\n");
        print_quotation_menu();
        return;
    }

    print_quotation();

    printf("\nLas siguientes líneas están en la cotización:\n");
    for (int i = 0; i < current_quotation.num_lines; i++) {
        printf("%d: %s (Cantidad: %d)\n", current_quotation.lines[i].line_id, current_quotation.lines[i].product_name, current_quotation.lines[i].quantity);
    }

    printf("\nIngrese el número de la línea a eliminar (0 para cancelar): ");
    if (scanf("%d", &line_to_remove) != 1 || line_to_remove <= 0 || line_to_remove > current_quotation.num_lines) {
        printf("\nNúmero de línea inválido.\n");
        print_quotation_menu();
        return;
    }

    int index_to_remove = line_to_remove - 1;
    Quotation_Line *line = &current_quotation.lines[index_to_remove];

    // Actualizamos los totales de la cotización
    current_quotation.sub_total -= line->line_sub_total;
    current_quotation.total_taxes -= line->line_total_taxes;
    current_quotation.total = current_quotation.sub_total + current_quotation.total_taxes;

    // Desplazar las líneas restantes para llenar el espacio vacío
    for (int i = index_to_remove; i < current_quotation.num_lines - 1; i++) {
        current_quotation.lines[i] = current_quotation.lines[i + 1];
    }

    // Acomodar los IDs de las líneas restantes
    for (int i = index_to_remove; i < current_quotation.num_lines - 1; i++) {
        current_quotation.lines[i].line_id = i + 1;  // Reasignar los IDs (empezando desde 1)
    }

    current_quotation.num_lines--;

    // Reasignar memoria para evitar desperdicio
    current_quotation.lines = realloc(current_quotation.lines, current_quotation.num_lines * sizeof(Quotation_Line));
    if (current_quotation.num_lines > 0 && current_quotation.lines == NULL) {
        printf("\nError al reasignar memoria.\n");
        print_quotation_menu();
        return;
    }

    printf("\nLínea eliminada correctamente.\n");

    print_quotation_menu();
}



void ask_save_quotation() {
    if (current_quotation.num_lines == 0) {
        return;
    }

    print_quotation();

    char option;
    printf("\n¿Salir sin guardar? (s/n): ");
    while (getchar() != '\n');
    scanf("%c", &option);

    if (option == 's' || option == 'S') {
        while (getchar() != '\n');
        print_general_submenu();
    } else {
        save_quotation();
    }
}

void save_quotation() {
    if (current_quotation.num_lines == 0) {
        printf("\nNo hay productos en la cotización para guardar.\n");
        return;
    }

    char option;
    printf("\n¿Desea guardar la cotización? (s/n): ");
    while (getchar() != '\n');
    scanf("%c", &option);

    if (option == 'n' || option == 'N') {
        while (getchar() != '\n');
        print_quotation_menu();
    }

    MYSQL *conn = connect_to_db();
    if (!conn) {
        printf("\nError al conectar con la base de datos.\n");
        return;
    }

    if (edit_quotation) {

        // Insertar la cotización en la base de datos
        update_quotation(conn, &current_quotation);
    
        for (int i = 0; i < current_quotation.num_lines; i++) {
            current_quotation.lines[i].quotation_id = current_quotation.id;
            update_quotation_lines(conn, &current_quotation.lines[i]);
        }

        delete_modified_quotation_lines(conn, &current_quotation);
    
        printf("\nCotización actualizada exitosamente.\n");
    } else {

    
        // Insertar la cotización en la base de datos
        create_quotation(conn, &current_quotation);
    
        for (int i = 0; i < current_quotation.num_lines; i++) {
            current_quotation.lines[i].quotation_id = current_quotation.id;
            add_line_to_quotation(conn, &current_quotation.lines[i]);
        }
    
        printf("\nCotización guardada exitosamente.\n");
    }
    close_db_connection(conn);
    print_general_submenu();
}

void print_all_quotations(bool show_all) {
    MYSQL *conn = connect_to_db();
    if (!conn) {
        printf("\nError al conectar con la base de datos.\n");
        return;
    }

    MYSQL_RES *result = get_quotations(conn, show_all);
    if (!result) {
        printf("\nError al obtener las cotizaciones.\n");
        close_db_connection(conn);
        return;
    }

    MYSQL_ROW row;
    printf("\nListado de Cotizaciones:\n");
    printf("┌───────────────┬───────────────┐\n");
    printf("│ ID Cotización │ Total         │\n");
    printf("├───────────────┼───────────────┤\n");

    while ((row = mysql_fetch_row(result))) {
        printf("│ %-13s │ %-13s │\n", row[0], row[1]);
    }

    printf("└───────────────┴───────────────┘\n");

    mysql_free_result(result);
    close_db_connection(conn);
}


void print_quotation() {
    printf("\n                                      COTIZACIÓN N°%d\n", current_quotation.id);
    printf("┌─────────┬──────────────────────────────┬─────────┬─────────────┬───────────────┬───────────┐\n");
    printf("│ # Línea │ Producto                     │ Cant    │ Precio      │ Subtotal      │ IVA       │\n");
    printf("├─────────┼──────────────────────────────┼─────────┼─────────────┼───────────────┼───────────┤\n");

    for (int i = 0; i < current_quotation.num_lines; i++) {
        printf("│ %-7d │ %-28s │ %-7d │ %-11.2f │ %-13.2f │ %-9.2f │\n",
               current_quotation.lines[i].line_id,
               current_quotation.lines[i].product_name,
               current_quotation.lines[i].quantity,
               current_quotation.lines[i].price,
               current_quotation.lines[i].line_sub_total,
               current_quotation.lines[i].line_total_taxes);
    }
    printf("└─────────┴──────────────────────────────┴─────────┴─────────────┴───────────────┴───────────┘\n");
    printf("\n                                                                   Subtotal :      %-9.2f  \n", current_quotation.sub_total);
    printf("                                                                   IVA Total:      %-9.2f  \n", current_quotation.total_taxes);
    printf("                                                                   Total    :      %-9.2f  \n", current_quotation.total);

}

void print_quotation_menu() {

    if (current_quotation.num_lines > 0) {
        print_quotation();
    }
    
    int option;

    printf("\n\n[1] Agregar Producto\n");
    printf("[2] Quitar Producto\n");
    printf("[3] Guardar Cotización\n");
    printf("[4] Salir\n");
    
    printf("\nSeleccione una opción\n= ");
    scanf("%d", &option);

    switch ((option))
    {
    case 1:
        quote_product(false);
        break;
    case 2:
        rm_product_from_quotation();
        break;
    case 3:
        save_quotation();
        break;
    case 4:
        ask_save_quotation();
        print_general_submenu();
        break;
    default:
        printf("\n\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\nOpción Inválida, Intente de nuevo.\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
        print_quotation_menu();
        break;
    }
}

void new_quotation() {

    current_quotation.id = get_last_quo_id() + 1;
    current_quotation.num_lines = 0;
    current_quotation.lines = NULL;

    printf("\nCOTIZACIÓN N°%d", current_quotation.id);
    print_quotation_menu();
}

void search_quotation(int quotation_id) {
    MYSQL *conn = connect_to_db();
    if (!conn) {
        printf("\nError al conectar con la base de datos.\n");
        return;
    }

    // Obtener cotización
    MYSQL_RES *quotation_result = get_quotation_by_id(conn, quotation_id);
    if (!quotation_result) {
        printf("\nNo se encontró la cotización con ID %d.\n", quotation_id);
        close_db_connection(conn);
        return;
    }

    // Asignar datos de la cotización a current_quotation
    MYSQL_ROW row = mysql_fetch_row(quotation_result);
    if (!row) {
        printf("\nNo se encontraron datos para la cotización.\n");
        mysql_free_result(quotation_result);
        close_db_connection(conn);
        return;
    }

    current_quotation.id = atoi(row[0]);
    current_quotation.sub_total = atof(row[1]);
    current_quotation.total_taxes = atof(row[2]);
    current_quotation.total = atof(row[3]);
    strncpy(current_quotation.state, row[4], sizeof(current_quotation.state) - 1);
    current_quotation.num_lines = 0;
    current_quotation.lines = NULL;

    mysql_free_result(quotation_result);
    close_db_connection(conn);
}

void search_quotation_lines(int quotation_id) {
    MYSQL *conn = connect_to_db();
    MYSQL_ROW row;
    // Obtener líneas de cotización
    MYSQL_RES *lines_result = get_quotation_lines(conn, quotation_id);
    if (!lines_result) {
        printf("\nError al obtener las líneas de la cotización.\n");
        close_db_connection(conn);
        return;
    }

    int num_lines = mysql_num_rows(lines_result);
    if (num_lines > 0) {
        current_quotation.lines = malloc(num_lines * sizeof(Quotation_Line));
        if (!current_quotation.lines) {
            printf("\nError al asignar memoria.\n");
            mysql_free_result(lines_result);
            close_db_connection(conn);
            return;
        }
        
        int i = 0;
        while ((row = mysql_fetch_row(lines_result))) {
            current_quotation.lines[i].line_id = atoi(row[0]);
            current_quotation.lines[i].quotation_id = atoi(row[1]);
            strncpy(current_quotation.lines[i].product_name, row[2], sizeof(current_quotation.lines[i].product_name) - 1);
            current_quotation.lines[i].quantity = atoi(row[3]);
            current_quotation.lines[i].price = atof(row[4]);
            current_quotation.lines[i].line_sub_total = atof(row[5]);
            current_quotation.lines[i].line_total_taxes = atof(row[6]);
            i++;
        }
        current_quotation.num_lines = num_lines;
        printf("%d", current_quotation.num_lines);

    }

    mysql_free_result(lines_result);
    close_db_connection(conn);
}

void modify_quotation() {
    edit_quotation = true;

    print_all_quotations(false);

    int quotation_id;
    printf("\nModificar Cotización\n");
    printf("Seleccione una cotización mediante su ID\n= ");
    
    if (scanf("%d", &quotation_id) != 1 || quotation_id <= 0) {
        printf("\nID inválido. Intente de nuevo.\n");
        return;
    }

    search_quotation(quotation_id);
    search_quotation_lines(quotation_id);

    printf("\nCotización cargada correctamente.\n");
    print_quotation_menu();
}
