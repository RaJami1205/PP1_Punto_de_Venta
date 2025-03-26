#include <stdio.h>
#include <openssl/sha.h>
#include "./include/menu.h"
#include "./include/admin_menu.h"
#include "./include/general_menu.h"
#include "./include/products.h"
#include "./include/db_handler.h"

#define HASH_SIZE SHA256_DIGEST_LENGTH

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
            }
            break;
        case 2:
            print_general_submenu();
            break;
        case 3:
            printf("Gracias por utilizar el programa.\n");
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
    
    printf("\nContraseña: ");
    scanf("%s", password);

    hash_passwd(password, hashed_password);

    MYSQL *conn = connect_to_db();

    auth_success = validate_credentials(conn, username, hashed_password);

    close_db_connection(conn);

    return auth_success;
}