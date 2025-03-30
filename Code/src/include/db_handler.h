#ifndef DB_HANDLER_H
#define DB_HANDLER_H

#include <mysql/mysql.h>
#include <string.h>
#include "models.h"


// Función para conectar a la base de datos
MYSQL *connect_to_db();

// PRODUCTOS

// Función para insertar una nueva familia de productos
void insert_product_family(MYSQL *conn, const char *code, const char *name);

// Función para insertar un nuevo producto
void insert_product(MYSQL *conn, Product *product);

// Función para obtener producto por nombre o id
MYSQL_RES *search_product(MYSQL *conn, const char *input);

//Función para mostrar los productos disponibles
MYSQL_RES* get_all_products(MYSQL *conn);

// Función para obtener productos filtrados por familia
MYSQL_RES *get_products_by_family(MYSQL *conn, const char *family);

//Función para eliminar un producto
void drop_product(MYSQL *conn, const char *code);

// COTIZACIONES

// Función para obtener el último id de cotización creado
MYSQL_RES *get_last_quot_id(MYSQL *conn);

// Función para agregar la cotización
void create_quotation(MYSQL *conn, Quotation *quotation);

// Función para agregar nueva linea a la cotización
void add_line_to_quotation(MYSQL *conn, Quotation_Line *quotation_line);

// Función para transformar de hash a hexadecimal
void hash_to_hex(const unsigned char *hash, char *hex_str, size_t length);

// Función para verificar login
bool validate_credentials(MYSQL *conn, const char *username, unsigned char *password);

// Función para cerrar la conexión a la base de datos
void close_db_connection(MYSQL *conn);

#endif /* DB_HANDLER_H */