#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <openssl/sha.h>
#include "./include/models.h"

#define HASH_SIZE SHA256_DIGEST_LENGTH

MYSQL *connect_to_db() {
    MYSQL *conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "Error al inicializar MySQL: %s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }

    // TO-DO: CREAR VARIABLES DE ENTORNO PARA MANEJO DE CLAVE Y USUARIO DE BD
    if (mysql_real_connect(conn, "localhost", "root", "1234", "Sales_Points", 0, NULL, 0) == NULL) {
        fprintf(stderr, "Error al conectar a la base de datos: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }

    return conn;
}

void insert_product_family(MYSQL *conn, const char *name) {
    char query[200];
    // TO-DO: CAMBIAR LA FORMA DE INSERCION A STORED PROCEDURE O FUNCION
    snprintf(query, sizeof(query), "INSERT INTO Product_Family (name) VALUES ('%s')", name);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error en la consulta: %s\n", mysql_error(conn));
    }
}

void insert_product(MYSQL *conn, Product *product) {
    char query[500];
    // TO-DO: CAMBIAR LA FORMA DE INSERCION A STORED PROCEDURE O FUNCION
    snprintf(query, sizeof(query), "INSERT INTO Product (name, cost, price, stock, product_family_id) VALUES ('%s', '%s', '%s', %d, %d)",
             product->name, product->cost, product->price, product->stock, product->product_family_id);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error en la consulta: %s\n", mysql_error(conn));
    }
}

bool validate_credentials(MYSQL *conn, const char *username, unsigned char *password) {

    MYSQL_STMT *stmt;
    MYSQL_BIND bind[2], result_bind[1];
    unsigned char stored_password[HASH_SIZE];

    const char *query = "SELECT password FROM Users WHERE username = ?";
    stmt = mysql_stmt_init(conn);

    if (!stmt) {
        fprintf(stderr, "Error al inicializar consulta: %s\n", mysql_error(conn));
        mysql_close(conn);
        return false;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query))) {
        fprintf(stderr, "Error al preparar consulta: %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        mysql_close(conn);
        return false;
    }

    memset(bind, 0, sizeof(bind));
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = username;
    bind[0].buffer_length = strlen(username);
    mysql_stmt_bind_param(stmt, bind);

    if (mysql_stmt_execute(stmt)) {
        fprintf(stderr, "Error al ejecutar consulta: %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        mysql_close(conn);
        return false;
    }

    memset(result_bind, 0, sizeof(result_bind));
    result_bind[0].buffer_type = MYSQL_TYPE_BLOB;
    result_bind[0].buffer = stored_password;
    result_bind[0].buffer_length = HASH_SIZE;
    mysql_stmt_bind_result(stmt, result_bind);

    if (mysql_stmt_fetch(stmt) == 0) {
        if (memcmp(stored_password, password, HASH_SIZE)) {
            mysql_stmt_close(stmt);
            mysql_close(conn);
            return true;
        }
    }

}

void close_db_connection(MYSQL *conn) {
    mysql_close(conn);
}
