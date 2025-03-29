#include <stdio.h>
#include <time.h>
#include "./include/products.h"
#include "./include/general_menu.h"

int get_last_quo_id() {
    // TO-DO

    return 1;
}

void add_product_to_quotation() {
    // TO-DO

}

void rm_product_from_quotation() {
    // TO-DO

}

void filter_products() {
    
}

void save_quotation() {
    // TO-DO

}

void print_quotation() {
    // TO-DO

}

void new_quotation() {
    int new_q_id = get_last_quo_id() + 1;
    time_t t = time(NULL);
    struct tm date = *localtime(&t);

    printf("\nCotización          : %d\n", new_q_id);
    printf("Fecha de Creación   : %d-%02d-%d %02d:%02d:%02d\n\n", date.tm_mday, date.tm_mon + 1, date.tm_year + 1900, date.tm_hour, date.tm_min, date.tm_sec);

    print_products();
    
    int option;

    printf("[1] Seleccionar Producto\n");
    printf("[2] Filtrar por Familia\n");
    printf("[3] Guardar Cotización\n");
    printf("[4] Ver Cotización\n");
    printf("[5] Quitar Producto\n");
    printf("[6] Salir\n");
    
    printf("\nSeleccione una opción\n=");
    scanf("%d", &option);

    switch ((option))
    {
    case 1:
        add_product_to_quotation();
        break;
    case 2:
        filter_products();
        break;
    case 3:
        save_quotation();
        break;
    case 4:
        print_quotation();
        break;
    case 5:
        rm_product_from_quotation();
        break;
    case 6:
        print_general_submenu();
        break;
    default:
        printf("Opción Inválida, Intente de nuevo.\n");
        new_quotation();
        break;
    }
}

