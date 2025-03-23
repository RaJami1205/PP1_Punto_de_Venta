#ifndef DB_HANDLER_H
#define DB_HANDLER_H

#include <mysql/mysql.h>
#include "models.h"

// Función para conectar a la base de datos
MYSQL *connect_to_db();

// Función para insertar una nueva familia de productos
void insert_product_family(MYSQL *conn, const char *name);

// Función para insertar un nuevo producto
void insert_product(MYSQL *conn, Product *product);

// Función para cerrar la conexión a la base de datos
void close_db_connection(MYSQL *conn);

#endif /* DB_HANDLER_H */