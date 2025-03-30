#include <stdio.h>
#include <stdlib.h>
#include "./include/quotations.h"
#include "./include/products.h"
#include "./include/general_menu.h"

int get_last_quo_id() {
    // TO-DO

    return 1;
}


// NOT WORKING
bool product_exists(const char *input, char *product_name) {
    MYSQL *conn = connect_to_db();
    if (conn == NULL) {
        return false;
    }

    // Llamar a search_product
    MYSQL_RES *result = search_product(conn, input);
    if (result == NULL) {
        printf("Error al ejecutar la búsqueda o producto no encontrado.\n");
        close_db_connection(conn);
        return false;
    }

    // Comprobar si hay resultados
    if (mysql_num_rows(result) == 0) {
        printf("Producto no encontrado.\n");
        mysql_free_result(result);
        close_db_connection(conn);
        return false;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row != NULL) {
        strcpy(product_name, row[1]);  // Suponiendo que el nombre del producto está en la segunda columna
    }
    
    mysql_free_result(result);
    close_db_connection(conn);
    return true;
}

void quote_product(bool is_filtered) {
    int opt;
    
    if (!is_filtered) {
        print_products(); 
    }

    printf("\n\n[1] Seleccionar Producto\n");
    printf("[2] Filtrar por Familia\n");
    printf("[3] Regresar\n");
    
    printf("\nSeleccione una opción\n= ");
    scanf("%d", &opt);

    switch ((opt))
    {
    case 1:
        add_product_to_quotation();
        break;
    case 2:
        filter_products();
        break;
    case 3:
        print_quotation_menu();
        break;
    default:
        printf("\n\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\nOpción Inválida, Intente de nuevo.\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
        quote_product(false);
        break;
    }
}

void add_product_to_quotation() {

    char product_input[MAX_INPUT];
    int quantity;
    char product_name[100];

    while (getchar() != '\n' && getchar() != EOF);

    printf("\nIngrese el ID o nombre del producto que desea agregar (0 para cancelar): ");
    fgets(product_input, MAX_INPUT, stdin);
    product_input[strcspn(product_input, "\n")] = '\0';

    if (strcmp(product_input, "0") == 0) {
        printf("\nOperación cancelada.\n");
        return;
    }

    /*
    

    // Verificar si el producto existe en la base de datos
    if (!product_exists(product_input, product_name)) {
        printf("\nError: El producto '%s' no existe. Intente nuevamente.\n", product_input);
        add_product_to_quotation();
        return;
    }
    */
    printf("Ingrese la cantidad deseada: ");
    if (scanf("%d", &quantity) != 1 || quantity <= 0) {
        printf("\nError: Cantidad no válida. Intente nuevamente.\n");
        while (getchar() != '\n');
        add_product_to_quotation();
        return;
    }

    current_quotation.lines = realloc(current_quotation.lines, (current_quotation.num_lines + 1) * sizeof(Quotation_Line));
    if (!current_quotation.lines) {
        printf("\nError: No se pudo asignar memoria.\n");
        print_quotation_menu();
        return;
    }

    Quotation_Line *new_line = &current_quotation.lines[current_quotation.num_lines];
    new_line->line_id = current_quotation.num_lines + 1;
    new_line->quotation_id = current_quotation.id;
    strcpy(new_line->product_name, product_name);
    new_line->quantity = quantity;

    current_quotation.num_lines++;

    printf("\nProducto agregado correctamente.\n");

    char option;
    printf("\n¿Desea agregar otro producto? (s/n): ");
    while (getchar() != '\n');
    scanf("%c", &option);

    if (option == 's' || option == 'S') {
        while (getchar() != '\n');
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

    // Mostrar las líneas de la cotización antes de eliminar
    printf("\nLas siguientes líneas están en la cotización:\n");
    for (int i = 0; i < current_quotation.num_lines; i++) {
        printf("%d: %s (Cantidad: %d)\n", current_quotation.lines[i].line_id, current_quotation.lines[i].product_name, current_quotation.lines[i].quantity);
    }

    // Solicitar al usuario que ingrese el número de línea para eliminar
    printf("\nIngrese el número de la línea a eliminar (0 para cancelar): ");
    if (scanf("%d", &line_to_remove) != 1 || line_to_remove <= 0 || line_to_remove > current_quotation.num_lines) {
        printf("\nNúmero de línea inválido.\n");
        print_quotation_menu();
        return;
    }

    // Eliminar la línea especificada
    int index_to_remove = line_to_remove - 1;  // Ajustar índice para cero basado

    // Desplazar las líneas restantes para llenar el espacio vacío
    for (int i = index_to_remove; i < current_quotation.num_lines - 1; i++) {
        current_quotation.lines[i] = current_quotation.lines[i + 1];
    }

    // Reducir el número de líneas en la cotización
    current_quotation.num_lines--;

    // Reasignar memoria para las líneas de la cotización
    current_quotation.lines = realloc(current_quotation.lines, current_quotation.num_lines * sizeof(Quotation_Line));
    if (current_quotation.num_lines > 0 && current_quotation.lines == NULL) {
        printf("\nError al reasignar memoria.\n");
        print_quotation_menu();
        return;
    }

    printf("\nProducto eliminado correctamente.\n");

    print_quotation_menu();
}

void filter_products() {
    // TO-DO
}

void save_quotation() {
    // TO-DO
}

void print_quotation() {
    printf("\n                          COTIZACIÓN N°%d\n", current_quotation.id);
    printf("┌─────────┬──────────────────────────────┬─────────┬──────────┬──────────┐\n");
    printf("│ # Línea │ Producto                     │ Cant    │ Precio   │ IVA      │\n");
    printf("├─────────┼──────────────────────────────┼─────────┼──────────┼──────────┤\n");

    for (int i = 0; i < current_quotation.num_lines; i++) {
        printf("│ %-7d │ %-28s │ %-7d │ %-8s │ %-8s │\n",
               current_quotation.lines[i].line_id,
               current_quotation.lines[i].product_name,
               current_quotation.lines[i].quantity,
               current_quotation.lines[i].line_sub_total,
               current_quotation.lines[i].line_total_taxes);
    }
    printf("└─────────┴──────────────────────────────┴─────────┴──────────┴──────────┘\n");

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
        // VALIDACION DE HABER AL MENOS 1 PRODUCTO
        rm_product_from_quotation();
        break;
    case 3:
        // VALIDACION DE HABER AL MENOS 1 PRODUCTO
        save_quotation();
        break;
    case 4:
        // PREGUNTAR SI QUIERE GUARDAR SI: EXISTE AL MENOS 1 PRODUCTO
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