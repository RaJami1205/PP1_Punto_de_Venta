#include "./include/menus.h"


/*
==========================================================================
                              MAIN MENU
==========================================================================
*/

void print_main_menu() {
    char input[10];
    int opcion;

    while (1) {
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
        
        fgets(input, sizeof(input), stdin);
        
        char *endptr;
        opcion = strtol(input, &endptr, 10);

        if (*endptr != '\n' && *endptr != '\0') {
            printf("❌ Entrada inválida. Por favor, ingrese un número.\n");
            continue;
        }

        switch (opcion) {
            case 1:
                if (login()) {
                    print_admin_submenu();
                }
                break;
            case 2:
                print_general_submenu();
                break;
            case 3:
                printf("\nGracias por utilizar el programa.\n");
                return;
            default:
                printf("❌ Opción inválida. Por favor, ingrese un número del 1 al 3.\n");
        }
    }
}


void hash_passwd(const char *password, unsigned char *hashedPassword) {
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, password, strlen(password));
    SHA256_Final(hashedPassword, &sha256);
}

bool login() {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    unsigned char hashed_password[HASH_SIZE];
    bool auth_success = false;
    
    printf("\nUsuario: ");
    if (fgets(username, sizeof(username), stdin) == NULL) {
        printf("❌ Error al leer el usuario.\n");
        return false;
    }
    username[strcspn(username, "\n")] = 0;  // Eliminar salto de línea

    printf("Contraseña: ");
    if (fgets(password, sizeof(password), stdin) == NULL) {
        printf("❌ Error al leer la contraseña.\n");
        return false;
    }
    password[strcspn(password, "\n")] = 0;  // Eliminar salto de línea

    if (strlen(username) == 0 || strlen(password) == 0) {
        printf("❌ Usuario o contraseña no pueden estar vacíos.\n");
        return false;
    }

    hash_passwd(password, hashed_password);

    MYSQL *conn = connect_to_db();
    if (conn == NULL) {
        printf("❌ Error al conectar con la base de datos.\n");
        return false;
    }

    auth_success = validate_credentials(conn, username, hashed_password);
    close_db_connection(conn);

    if (!auth_success) {
        printf("❌ Credenciales incorrectas. \n");
    }

    return auth_success;
}

/*
==========================================================================
                             ADMIN MENU
==========================================================================
*/

void print_admin_submenu() {
    char input[10];
    int subopcion_A;

    while (1) {
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
        fgets(input, sizeof(input), stdin);

        char *endptr;
        subopcion_A = strtol(input, &endptr, 10);

        if (*endptr != '\n' && *endptr != '\0') {
            printf("❌ Entrada inválida. Por favor, ingrese un número.\n");
            continue;
        }

        switch (subopcion_A) {
            case 1:
                set_product_family();
                break;
            case 2:
                set_product();
                break;
            case 3:
                delete_product();
                break;
            case 4:
                loadProductStock();
                break;
            case 5:
                printf("Has seleccionado consultar factuuras.\n");
                break;
            case 6:
                print_statistics_submenu();
                break;
            case 7:
                print_main_menu();
                return;
            default:
                printf("❌ Opción inválida en el submenú de administración.\n");
        }
    }
}

/*
==========================================================================
                            GENERAL MENU
==========================================================================
*/

void print_general_submenu() {
    char input[10];
    int subopcion_G;

    while (1) {
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

        printf("\nIngrese la opción: ");
        fgets(input, sizeof(input), stdin);

        char *endptr;
        subopcion_G = strtol(input, &endptr, 10);

        if (*endptr != '\n' && *endptr != '\0') {
            printf("❌ Entrada inválida. Por favor, ingrese un número.\n");
            continue;
        }

        switch (subopcion_G) {
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
                return;
            default:
                printf("❌ Opción inválida en el submenú de generales.\n");
        }
    }
}

/*
==========================================================================
                            Estadísticas MENU
==========================================================================
*/

void print_statistics_submenu() {
    char input[10];
    int subopcion_E;

    while (1) {
        printf("\n");
        printf("╔════════════════════════════════════════════════╗\n");
        printf("║            📊 Estadísticas Generales           ║\n");
        printf("╠════════════════════════════════════════════════╣\n");
        printf("║                    OPCIONES                    ║\n");
        printf("║                                                ║\n");
        printf("║   [1] 📋 Cantidad de Cotizaciones Pendientes   ║\n");
        printf("║   [2] 💼 Cantidad de Cotizaciones Facturadas   ║\n");
        printf("║   [3] 📈 Promedio de Compra                    ║\n");
        printf("║   [4] 🔝 Top 5 Productos más Vendidos          ║\n");
        printf("║   [5] 🏷️  Productos más Vendidos por Familia    ║\n");
        printf("║   [6] 💵 Monto Total Vendido por Familia       ║\n");
        printf("║   [7] 🔙 Regresar                              ║\n");
        printf("╚════════════════════════════════════════════════╝\n");

        printf("\nIngrese la opción: ");
        fgets(input, sizeof(input), stdin);

        char *endptr;
        subopcion_E = strtol(input, &endptr, 10);

        if (*endptr != '\n' && *endptr != '\0') {
            printf("❌ Entrada inválida. Por favor, ingrese un número.\n");
            continue;
        }

        switch (subopcion_E) {
            case 1:
                print_pending_quotations();
                break;
            case 2:
                print_quotations_with_invoices();
                break;
            case 3:
                print_total_purchase_average();
                break;
            case 4:
                print_top_5_selling_products();
                break;
            case 5:
                print_top_product_per_family();
                break;
            case 6:
                print_total_sales_per_family();
                break;
            case 7:
                print_admin_submenu();
                break;
            default:
                printf("❌ Opción inválida en el submenú de estadísticas.\n");
        }
    }
}