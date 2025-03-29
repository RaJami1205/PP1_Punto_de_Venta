#ifndef DB_HANDLER_H
#define DB_HANDLER_H

#include <mysql/mysql.h>
#include <string.h>
#include "models.h"


// Función para conectar a la base de datos
MYSQL *connect_to_db();

// Función para insertar una nueva familia de productos
void insert_product_family(MYSQL *conn, const char *code, const char *name);

// Función para insertar un nuevo producto
void insert_product(MYSQL *conn, Product *product);

//Función para eliminar un producto
void drop_product(MYSQL *conn, const char *code);

//Función para mostrar los productos disponibles
MYSQL_RES* get_all_products(MYSQL *conn);

void hash_to_hex(const unsigned char *hash, char *hex_str, size_t length);

// Función para verificar login
bool validate_credentials(MYSQL *conn, const char *username, unsigned char *password);

// Función para cerrar la conexión a la base de datos
void close_db_connection(MYSQL *conn);

#endif /* DB_HANDLER_H */