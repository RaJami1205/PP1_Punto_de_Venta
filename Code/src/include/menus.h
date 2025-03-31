#ifndef MENUS_H
#define MENUS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include "products.h"
#include "quotations.h"
#include "invoices.h"
#include "models.h"
#include "db_handler.h"

#define HASH_SIZE SHA256_DIGEST_LENGTH
#define MAX_USERNAME 30
#define MAX_PASSWORD 30

/*
        MAIN MENU
*/

// Función para imprimir el menu inicial
void print_main_menu();

// Función para cifrar contraseña ingresada por el usuario
void hash_passwd(const char *password, unsigned char *hashed_password);

// Función para iniciar sesión en modo administrador
bool login();

/*
        ADMIN MENU
*/

// Función para imprimir el menú administrativo
void print_admin_submenu();

/*
        GENERAL MENU
*/

void print_general_submenu();

#endif /* MENUS_H */