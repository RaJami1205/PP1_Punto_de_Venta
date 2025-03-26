#include <stdio.h>
#include "./include/admin_menu.h"
#include "./include/menu.h"
#include "./include/products.h"

void print_admin_submenu() {
    int subopcion_A;

    printf("\n");
    printf("╔════════════════════════════════════════════╗\n");
    printf("║        📦 Opciones de Administración       ║\n");
    printf("╠════════════════════════════════════════════╣\n");
    printf("║                  OPCIONES                  ║\n");
    printf("║                                            ║\n");
    printf("║   [1] 🏷️  Registrar familia de productos    ║\n");
    printf("║   [2] 🆕 Registrar nuevo producto          ║\n");
    printf("║   [3] ❌ Eliminar producto                 ║\n");
    printf("║   [4] 📦 Cargar inventario                 ║\n");
    printf("║   [5] 🧾 Consulta de facturas              ║\n");
    printf("║   [6] 📊 Estadísticas                      ║\n");
    printf("║   [7] 🔙 Volver                            ║\n");
    printf("╚════════════════════════════════════════════╝\n");

    printf("\nSeleccione una opción: ");
    scanf("%d", &subopcion_A);

    switch(subopcion_A) {
        case 1:
            printf("\n");
            set_product_family();
            print_admin_submenu();
            
        case 2:
            printf("\n");
            set_product();
            print_admin_submenu();

        case 3:
            delete_product();
            print_admin_submenu();
            break;

        case 4:
            printf("Has seleccionado Consulta de facturas.\n");
            break;

        case 5:
            printf("Has seleccionado Estadísticas.\n");
            break;

        case 6:
            print_main_menu();
            break;
        
        case 7:
            print_main_menu();
            break;

        default:
            printf("Opción inválida en el submenú de administración.\n");
            print_admin_submenu();
            break;
    }
}