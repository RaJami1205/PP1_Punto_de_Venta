#include <stdio.h>
#include <openssl/sha.h>
#include "./include/menu.h"
#include "./include/products.h"
#include "./include/db_handler.h"

#define HASH_SIZE SHA256_DIGEST_LENGTH

int printMainMenu() {
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
    return opcion;
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
    
    printf("\nContraseña: ");
    scanf("%s", password);

    hash_passwd(password, hashed_password);

    MYSQL *conn = connect_to_db();

    auth_success = validate_credentials(conn, username, hashed_password);

    close_db_connection(conn);

    return auth_success;
}

void printSubMenuAdmin() {
    int subopcion_A;

    printf("\n");
    printf("╔════════════════════════════════════════════╗\n");
    printf("║        📦 Opciones de Administración       ║\n");
    printf("╠════════════════════════════════════════════╣\n");
    printf("║                  OPCIONES                  ║\n");
    printf("║                                            ║\n");
    printf("║   [1] 🏷️ Registrar familia de productos     ║\n");
    printf("║   [2] 🆕 Registrar nuevo producto          ║\n");
    printf("║   [3] 📦 Cargar inventario                 ║\n");
    printf("║   [4] 🧾 Consulta de facturas              ║\n");
    printf("║   [5] 📊 Estadísticas                      ║\n");
    printf("║   [6] 🔙 Volver                            ║\n");
    printf("╚════════════════════════════════════════════╝\n");

    printf("\nSeleccione una opción: ");
    scanf("%d", &subopcion_A);

    switch(subopcion_A) {
        case 1:
            printf("\n");
            setProductFamily();
            printSubMenuAdmin();
            
        case 2:
            printf("\n");
            setProduct();
            printSubMenuAdmin();

        case 3:
            printf("Has seleccionado Cargar inventario.\n");
            break;

        case 4:
            printf("Has seleccionado Consulta de facturas.\n");
            break;

        case 5:
            printf("Has seleccionado Estadísticas.\n");
            break;

        case 6:
            main();
            break;

        default:
            printf("Opción inválida en el submenú de administración.\n");
            printSubMenuAdmin();
            break;
    }
}

void printSubMenuGeneral() {
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
            printf("Has seleccionado Consulta de Catalogo.\n");
            break;

        case 2:
            printf("Has seleccionado Cotizar.\n");
            break;

        case 3:
            printf("Has seleccionado Facturar.\n");
            break;

        case 4:
            main();
            break;

        default:
            printf("Opción inválida en el submenú de generales.\n");
            printSubMenuGeneral();
            break;
    }
}
