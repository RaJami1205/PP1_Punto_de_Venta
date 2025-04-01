#include "./include/quotations.h"

Quotation *get_current_quotation() {
    return &current_quotation;
}

int get_last_quo_id() {
    MYSQL *conn = connect_to_db();
    if (!conn) {
        fprintf(stderr, "Error al conectar con la base de datos.\n");
        return -1;
    }
    
    MYSQL_RES *result = get_last_quotation_id(conn);
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

    // TO-DO SOLUCIONAR DOBLE ENTER
    while (1) {
        if (!is_filtered) {
            print_products(); 
        } else {
            print_filtered_products();
        }

        char input[10];
        int opt;

        printf("\nOpciones:\n");
        printf("\n[1] Seleccionar Producto\n");
        printf("[2] Filtrar por Familia\n");
        printf("[3] Mostrar Todos los Productos\n");
        printf("[4] Regresar\n");
        printf("\nSeleccione una opción\n= ");
        fgets(input, sizeof(input), stdin);

        char *endptr;
        opt = strtol(input, &endptr, 10);

        if (*endptr != '\n' && *endptr != '\0') {
            printf("❌ Entrada inválida. Por favor, ingrese un número.\n");
            continue;
        }

        switch (opt) {
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
                return;
            default:
                printf("❌ Opción Inválida, Intente de nuevo.\n");
        }
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


    printf("\nIngrese el ID o nombre del producto que desea agregar (0 para cancelar): ");
    fgets(product_input, MAX_INPUT, stdin);
    product_input[strcspn(product_input, "\n")] = '\0';

    if (strcmp(product_input, "0") == 0) {
        printf("\nOperación cancelada.\n");
        print_quotation_menu();
        return;
    }

    if (!product_exists(product_input, &selected_product)) {
        printf("\n❌ Error: El producto '%s' no existe. Intente nuevamente.\n", product_input);
        quote_product(false);
        return;
    }

    bool correct_quantity = false;
    char input[20];

    
    while (!correct_quantity) {
        printf("Ingrese la cantidad deseada: ");
        fgets(input, sizeof(input), stdin);

        char *endptr;
        quantity = strtol(input, &endptr, 10);

        if (*endptr != '\n' && *endptr != '\0') {
            printf("\n❌ Error: Entrada no válida. Intente nuevamente.\n");
            continue;
        }

        if (quantity <= 0 || quantity > selected_product.stock) {
            printf("\n❌ Error: Cantidad no válida. Debe estar entre 1 y %d.\n", selected_product.stock);
        } else {
            correct_quantity = true;
        }
    }

    Quotation_Line *existing_line = find_product_line(selected_product.name);

    // Si el producto ya está en la cotización, actualizar cantidad y total
    if (existing_line) {

        if (existing_line->quantity + quantity > selected_product.stock) {
            printf("\n❌ Error: No hay suficiente stock disponible.\n");
            quote_product(false);
            return;
        }

        current_quotation.sub_total -= existing_line->line_sub_total;
        current_quotation.total_taxes -= existing_line->line_total_taxes;

        existing_line->quantity += quantity;
        existing_line->line_sub_total = existing_line->price * existing_line->quantity;
        existing_line->line_total_taxes = existing_line->line_sub_total * 0.13;

        current_quotation.sub_total += existing_line->line_sub_total;
        current_quotation.total_taxes += existing_line->line_total_taxes;
        current_quotation.total = current_quotation.sub_total + current_quotation.total_taxes;

        printf("\n✅ Cantidad actualizada en la cotización.\n");
    } else {
        // Agregar nuevo producto a la cotización
        current_quotation.lines = realloc(current_quotation.lines, (current_quotation.num_lines + 1) * sizeof(Quotation_Line));

        if (!current_quotation.lines) {
            printf("\n❌ Error: No se pudo asignar memoria.\n");
            print_quotation_menu();
            return;
        }

        Quotation_Line *new_line = &current_quotation.lines[current_quotation.num_lines];
        new_line->line_id = current_quotation.num_lines + 1;
        new_line->quotation_id = current_quotation.id;
        strcpy(new_line->product_name, selected_product.name);
        new_line->quantity = quantity;
        new_line->price = selected_product.price;
        new_line->line_sub_total = selected_product.price * new_line->quantity;
        new_line->line_total_taxes = new_line->line_sub_total * 0.13;

        current_quotation.sub_total += new_line->line_sub_total;
        current_quotation.total_taxes += new_line->line_total_taxes;
        current_quotation.total = current_quotation.sub_total + current_quotation.total_taxes;
        current_quotation.num_lines++;

        printf("\n✅ Producto agregado correctamente.\n");
    }

    char option[10];

    while (1) {
        printf("\n¿Desea agregar otro producto? (s/n): ");
        
        fgets(option, sizeof(option), stdin);

        if (option[0] == 's' || option[0] == 'S') {
            print_products();
            add_product_to_quotation();
            return;
        } else if (option[0] == 'n' || option[0] == 'N') {
            print_quotation_menu();
            return;
        } else {
            printf("❌ Entrada inválida. Por favor, ingrese 's' o 'n'.\n");
        }
    }
}

void rm_product_from_quotation(int line_to_remove) {
    if (current_quotation.num_lines == 0) {
        printf("\n❌ No hay productos en la cotización para eliminar.\n");
        print_quotation_menu();
        return;
    }

    if (current_quotation.num_lines == 1) {
        printf("\n❌ No se puede eliminar la única línea en la cotización.\n");
        print_quotation_menu();
        return;
    }

    char input[20];

    if (line_to_remove == 0) {
        print_quotation();

        printf("\n📌 Líneas en la cotización:\n");
        for (int i = 0; i < current_quotation.num_lines; i++) {
            printf(" [%d] %s (Cantidad: %d)\n", i + 1, current_quotation.lines[i].product_name, current_quotation.lines[i].quantity);
        }

        while (1) {
            printf("\n📝 Ingrese el número de la línea a eliminar (0 para cancelar): ");
            fgets(input, sizeof(input), stdin);

            if (sscanf(input, "%d", &line_to_remove) == 1 && line_to_remove >= 0 && line_to_remove <= current_quotation.num_lines) {
                break; // Entrada válida, salir del bucle
            } else {
                printf("\n❌ Número de línea inválido. Intente nuevamente.\n");
            }
        }

        if (line_to_remove == 0) {
            printf("\n🔙 Operación cancelada.\n");
            print_quotation_menu();
            return;
        }
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
        current_quotation.lines[i].line_id = i + 1;
    }

    current_quotation.num_lines--;

    // Reasignar memoria para evitar desperdicio
    current_quotation.lines = realloc(current_quotation.lines, current_quotation.num_lines * sizeof(Quotation_Line));
    if (current_quotation.num_lines > 0 && current_quotation.lines == NULL) {
        printf("\n⚠️ Error al reasignar memoria.\n");
        print_quotation_menu();
        return;
    }

    printf("\n✅ Línea eliminada correctamente.\n");

    print_quotation_menu();
}

void seek_quotation() {
    set_current_quot(); 
    print_quotation();
}

void ask_save_quotation() {
    if (current_quotation.num_lines == 0) {
        return;
    }

    print_quotation();

    char option[10];

    while (1) {
        printf("\n¿Salir sin guardar? (s/n): ");
        fgets(option, sizeof(option), stdin);

        if (option[0] == 's' || option[0] == 'S') {
            print_general_submenu();
            return;
        } else if (option[0] == 'n' || option[0] == 'N') {
            save_quotation();
            return;
        } else {
            printf("\n❌ Entrada inválida. Por favor, ingrese 's' o 'n'.\n");
        }
    }
}


void save_quotation() {
    if (current_quotation.num_lines == 0) {
        printf("\n❌ No hay productos en la cotización para guardar.\n");
        return;
    }

    char option[10];

    while (1) {
        printf("\n¿Desea guardar la cotización? (s/n): ");
        fgets(option, sizeof(option), stdin);

        if (option[0] == 'n' || option[0] == 'N') {
            print_quotation_menu();
            return;
        } else if (option[0] == 's' || option[0] == 'S') {
            break;
        } else {
            printf("\n❌ Entrada inválida. Por favor, ingrese 's' o 'n'.\n");
        }
    }

    // Conectar con la base de datos
    MYSQL *conn = connect_to_db();
    if (!conn) {
        printf("\n❌ Error al conectar con la base de datos.\n");
        return;
    }

    if (edit_quotation) {

        update_quotation(conn, &current_quotation);

        for (int i = 0; i < current_quotation.num_lines; i++) {
            current_quotation.lines[i].quotation_id = current_quotation.id;
            update_quotation_lines(conn, &current_quotation.lines[i]);
        }

        delete_modified_quotation_lines(conn, &current_quotation);

        printf("\n✅ Cotización actualizada exitosamente.\n");
    } else {
        create_quotation(conn, &current_quotation);

        for (int i = 0; i < current_quotation.num_lines; i++) {
            current_quotation.lines[i].quotation_id = current_quotation.id;
            add_line_to_quotation(conn, &current_quotation.lines[i]);
        }

        printf("\n✅ Cotización guardada exitosamente.\n");
    }

    close_db_connection(conn);
    print_general_submenu();
}


bool print_all_quotations(bool show_all_quots) {
    MYSQL *conn = connect_to_db();
    if (!conn) {
        printf("\n❌ Error al conectar con la base de datos.\n");
        return false;
    }

    MYSQL_RES *result = get_quotations(conn, show_all_quots);
    if (!result) {
        printf("\n❌ Error al obtener las cotizaciones.\n");
        close_db_connection(conn);
        return false;
    }

    if (mysql_num_rows(result) == 0) {
        printf("\n❌ No hay cotizaciones disponibles.\n");
        mysql_free_result(result);
        close_db_connection(conn);
        return false;
    }

    MYSQL_ROW row;
    printf("\nListado de Cotizaciones:\n");
    printf("┌───────────────┬───────────────┐\n");
    printf("│ ID Cotización │ Total         │\n");
    printf("├───────────────┼───────────────┤\n");

    while ((row = mysql_fetch_row(result))) {
        printf("│ %-13s │ %-13s │\n", row[0], row[3]);
    }

    printf("└───────────────┴───────────────┘\n");

    mysql_free_result(result);
    close_db_connection(conn);
    return true;
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
    while (1) {
        if (current_quotation.num_lines > 0) {
            print_quotation();
        }

        char input[10];
        int option;

        printf("\nOpciones:\n");
        printf("\n[1] Agregar Producto\n");
        printf("[2] Quitar Producto\n");
        printf("[3] Guardar Cotización\n");
        printf("[4] Salir\n");

        printf("\nSeleccione una opción\n= ");
        fgets(input, sizeof(input), stdin);

        char *endptr;
        option = strtol(input, &endptr, 10);

        if (*endptr != '\n' && *endptr != '\0') {
            printf("\n❌ Entrada inválida. Ingrese un número entre 1 y 4.\n");
            continue;
        }

        switch (option) {
            case 1:
                quote_product(false);
                break;
            case 2:
                rm_product_from_quotation(0);
                break;
            case 3:
                save_quotation();
                break;
            case 4:
                ask_save_quotation();
                print_general_submenu();
                return;
            default:
                printf("\n❌ Opción inválida. Intente de nuevo.\n");
                break;
        }
    }
}

void new_quotation() {

    current_quotation.id = get_last_quo_id() + 1;
    current_quotation.num_lines = 0;
    current_quotation.lines = NULL;
    edit_quotation = false;

    printf("\nCOTIZACIÓN N°%d\n", current_quotation.id);
    print_quotation_menu();
}

bool search_quotation(int quotation_id) {
    MYSQL *conn = connect_to_db();
    if (!conn) {
        printf("\n❌ Error al conectar con la base de datos.\n");
        return false;
    }

    // Obtener cotización
    MYSQL_RES *quotation_result = get_quotation_by_id(conn, quotation_id);
    if (!quotation_result) {
        printf("\n❌ No se encontró la cotización con ID %d.\n", quotation_id);
        close_db_connection(conn);
        return false;
    }

    // Asignar datos de la cotización a current_quotation
    MYSQL_ROW row = mysql_fetch_row(quotation_result);
    if (!row) {
        printf("\n❌ No se encontraron datos para la cotización.\n");
        mysql_free_result(quotation_result);
        close_db_connection(conn);
        return false;
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
    return true;
}

void search_quotation_lines(int quotation_id) {
    MYSQL *conn = connect_to_db();
    MYSQL_ROW row;
    // Obtener líneas de cotización
    MYSQL_RES *lines_result = get_quotation_lines(conn, quotation_id);
    if (!lines_result) {
        printf("\n❌ Error al obtener las líneas de la cotización.\n");
        close_db_connection(conn);
        return;
    }

    int num_lines = mysql_num_rows(lines_result);
    if (num_lines > 0) {
        current_quotation.lines = malloc(num_lines * sizeof(Quotation_Line));
        if (!current_quotation.lines) {
            printf("\n❌ Error al asignar memoria.\n");
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

    }

    mysql_free_result(lines_result);
    close_db_connection(conn);
}

bool set_current_quot() {
    int quotation_id;
    char input[20];

    while (1) {

        printf("Seleccione una cotización mediante su ID\n= ");
        fgets(input, sizeof(input), stdin);

        if (sscanf(input, "%d", &quotation_id) == 1 && quotation_id > 0) {
            break;
        } else {
            printf("\n\n❌ ID inválido. Intente de nuevo.\n\n");
        }
    }

    if (search_quotation(quotation_id)) {
        search_quotation_lines(quotation_id);
        return true;
    }
    return false;
}


void modify_quotation() {
    edit_quotation = true;

    if(!print_all_quotations(false)) {
        print_general_submenu();
        return;
    }
    printf("\nModificar Cotización\n");

    if (set_current_quot()) {
        print_quotation_menu();
    }
}