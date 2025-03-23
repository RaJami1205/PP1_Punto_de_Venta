#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./include/models.h"

MYSQL *connect_to_db() {
    MYSQL *conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "Error al inicializar MySQL: %s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }

    if (mysql_real_connect(conn, "localhost", "root", "1234", "Sales_Points", 0, NULL, 0) == NULL) {
        fprintf(stderr, "Error al conectar a la base de datos: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }

    return conn;
}

void insert_product_family(MYSQL *conn, const char *name) {
    char query[200];
    snprintf(query, sizeof(query), "INSERT INTO Product_Family (name) VALUES ('%s')", name);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error en la consulta: %s\n", mysql_error(conn));
    }
}

void insert_product(MYSQL *conn, Product *product) {
    char query[500];
    snprintf(query, sizeof(query), "INSERT INTO Product (name, cost, price, stock, product_family_id) VALUES ('%s', '%s', '%s', %d, %d)",
             product->name, product->cost, product->price, product->stock, product->product_family_id);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error en la consulta: %s\n", mysql_error(conn));
    }
}

void close_db_connection(MYSQL *conn) {
    mysql_close(conn);
}
