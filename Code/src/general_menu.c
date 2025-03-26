#include <stdio.h>
#include "./include/general_menu.h"
#include "./include/menu.h"
#include "./include/db_handler.h"
#include "./include/products.h"

void print_general_submenu() {
    int subopcion_G;

    printf("\n");
    printf("╔════════════════════════════════════╗\n");
    printf("║        🛍️  Opciones Generales       ║\n");
    printf("╠════════════════════════════════════╣\n");
    printf("║             OPCIONES               ║\n");
    printf("║                                    ║\n");
    printf("║   [1] 📖 Consulta de Catálogo      ║\n");
    printf("║   [2] 💰 Cotizar                   ║\n");
    printf("║   [3] 🧾 Facturar                  ║\n");
    printf("║   [4] 🔙 Volver                    ║\n");
    printf("╚════════════════════════════════════╝\n");

    printf("\nIngrese la opcion: ");
    scanf("%d", &subopcion_G);

    switch(subopcion_G) {
        case 1:
            print_products();
            break;

        case 2:
            printf("Has seleccionado Cotizar.\n");
            break;

        case 3:
            printf("Has seleccionado Facturar.\n");
            break;

        case 4:
            print_main_menu();
            break;

        default:
            printf("Opción inválida en el submenú de generales.\n");
            print_general_submenu();
            break;
    }
}
