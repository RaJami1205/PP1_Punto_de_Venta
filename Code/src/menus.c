#include "./include/menus.h"


/*
==========================================================================
                              MAIN MENU
==========================================================================
*/
void print_main_menu() {

    int opcion;
    
    printf("\n\n╔════════════════════════════════════════╗\n");
    printf("║     🛒  SISTEMA DE PUNTO DE VENTA      ║\n");
    printf("╠════════════════════════════════════════╣\n");
    printf("║                OPCIONES                ║\n");
    printf("║                                        ║\n");
    printf("║   [1] 📦  Panel Administrativo         ║\n");
    printf("║   [2] 🛍️   Opciones Generales           ║\n");
    printf("║   [3] ❌  Salir del Sistema            ║\n");
    printf("╚════════════════════════════════════════╝\n");
    
    printf("\nSeleccione una opción: ");

    scanf("%d", &opcion);
    switch (opcion) {
        case 1:
            if (login()) {
                print_admin_submenu();
            } else {
                printf("Usuario inválido. Por favor, ingrese credenciales correctas.\n");
                print_main_menu();
            }
            break;
        case 2:
            print_general_submenu();
            break;
        case 3:
            printf("\nGracias por utilizar el programa.\n");
            return;
            break;
        default:
            printf("Opcion inválida. Por favor, ingrese una opción válida.\n");
            print_main_menu();
    }
}

void hash_passwd(const char *password, unsigned char *hashedPassword) {
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, password, strlen(password));
    SHA256_Final(hashedPassword, &sha256);
}

bool login() {
    char username[30];
    char password[30];
    unsigned char hashed_password[HASH_SIZE];
    bool auth_success = false;
    
    printf("\nUsuario: ");
    scanf("%s", username);
    
    printf("Contraseña: ");
    scanf("%s", password);

    hash_passwd(password, hashed_password);

    MYSQL *conn = connect_to_db();

    auth_success = validate_credentials(conn, username, hashed_password);

    close_db_connection(conn);

    return auth_success;
}

/*
==========================================================================
                             ADMIN MENU
==========================================================================
*/
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

        case 4:
            loadProductStock();
            print_admin_submenu();

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

/*
==========================================================================
                            GENERAL MENU
==========================================================================
*/
void print_general_submenu() {
    int subopcion_G;

    printf("\n");
    printf("╔════════════════════════════════════╗\n");
    printf("║        🛍️  Opciones Generales       ║\n");
    printf("╠════════════════════════════════════╣\n");
    printf("║             OPCIONES               ║\n");
    printf("║                                    ║\n");
    printf("║   [1] 📖 Consulta de Catálogo      ║\n");
    printf("║   [2] 💰 Crear Cotización          ║\n");
    printf("║   [3] ✏️  Modificar Cotización      ║\n");
    printf("║   [4] 🧾 Facturar                  ║\n");
    printf("║   [5] 🔙 Volver                    ║\n");
    printf("╚════════════════════════════════════╝\n");

    printf("\nIngrese la opcion: ");
    scanf("%d", &subopcion_G);

    switch(subopcion_G) {
        case 1:
            print_products();
            break;

        case 2:
            new_quotation();
            break;

        case 3:
            modify_quotation();
            break;

        case 4:
            invoices_menu();
            break;

        case 5:
            print_main_menu();
            break;

        default:
            printf("Opción inválida en el submenú de generales.\n");
            break;
    }
    print_general_submenu();
}
