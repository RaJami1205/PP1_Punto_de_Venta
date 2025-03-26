#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <stdbool.h>

// Función para imprimir el menu inicial
void print_main_menu();

// Función para cifrar contraseña ingresada por el usuario
void hash_passwd(const char *password, unsigned char *hashed_password);

// Función para iniciar sesión en modo administrador
bool login();

#endif /* MAIN_MENU_H */