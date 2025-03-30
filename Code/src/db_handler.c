#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <openssl/sha.h>
#include "./include/models.h"
#include "./include/db_handler.h"

#define HASH_SIZE SHA256_DIGEST_LENGTH

MYSQL *connect_to_db() {
    MYSQL *conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "Error al inicializar MySQL: %s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }

    // Leer credenciales de las variables de entorno
    const char *db_host = getenv("DB_HOST");
    const char *db_user = getenv("DB_USER");
    const char *db_pass = getenv("DB_PASS");
    const char *db_name = getenv("DB_NAME");

    // Validar que todas las variables de entorno están definidas
    if (!db_host || !db_user || !db_pass || !db_name) {
        fprintf(stderr, "Error: Variables de entorno no definidas correctamente.\n");
        exit(EXIT_FAILURE);
    }

    // Conectar a la base de datos con las variables de entorno
    if (mysql_real_connect(conn, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL) {
        fprintf(stderr, "Error al conectar a la base de datos: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }

    // Configurar conjunto de caracteres a UTF-8
    if (mysql_set_character_set(conn, "utf8")) {
        fprintf(stderr, "Error al configurar el conjunto de caracteres: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }

    return conn;
}

void insert_product_family(MYSQL *conn, const char *code, const char *name) {
    char query[256];

    snprintf(query, sizeof(query),
             "INSERT INTO Product_Family (product_family_id, name) VALUES ('%s', '%s')",
             code, name);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error en la consulta: %s\n", mysql_error(conn));
    }
}

void insert_product(MYSQL *conn, Product *product) {
    char query[500];

    snprintf(query, sizeof(query), "INSERT INTO Product (product_id, name, cost, price, stock, product_family_id) "
                                   "VALUES ('%s', '%s', %.2f, %.2f, %d, '%s')",
             product->code,                // product_id
             product->name,                // name
             product->cost,                // cost (float)
             product->price,               // price (float)
             product->stock,               // stock (int)
             product->product_family_id);  // product_family_id

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error en la consulta: %s\n", mysql_error(conn));
    }
}

MYSQL_RES *search_product(MYSQL *conn, const char *search_value) {
    if (!conn) {
        return NULL;
    }

    char query[256];
    snprintf(query, sizeof(query), "CALL searchProduct('%s')", search_value);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error al ejecutar el procedimiento searchProduct: %s\n", mysql_error(conn));
        return NULL;
    }

    return mysql_store_result(conn);
}

MYSQL_RES *get_all_products(MYSQL *conn) {
    if (mysql_query(conn, "CALL GetAllProducts()")) {
        fprintf(stderr, "Error en la consulta: %s\n", mysql_error(conn));
        return NULL;
    }
    return mysql_store_result(conn);
}

MYSQL_RES *get_products_by_family(MYSQL *conn, const char *family) {
    char query[256];
    snprintf(query, sizeof(query), "CALL filterProductsByFamily('%s')", family);
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error al filtrar productos: %s\n", mysql_error(conn));
        return NULL;
    }
    return mysql_store_result(conn);
}

void drop_product(MYSQL *conn, const char *code) {
    if (conn == NULL) {
        fprintf(stderr, "Error: Conexión a la base de datos no válida.\n");
        return;
    }

    // Crear la consulta para llamar al procedimiento almacenado
    char query[200];
    snprintf(query, sizeof(query), "CALL deleteProductById('%s')", code);

    // Ejecutar la consulta
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error al ejecutar el procedimiento: %s\n", mysql_error(conn));
        return;
    }

    // Obtener el resultado del mensaje del procedimiento almacenado
    MYSQL_RES *result = mysql_store_result(conn);
    if (result) {
        MYSQL_ROW row = mysql_fetch_row(result);
        if (row) {
            printf("%s\n", row[0]);  // Imprimir el mensaje devuelto por MySQL
        }
        mysql_free_result(result);
    }
}

MYSQL_RES *get_last_quot_id(MYSQL *conn) {
    if (mysql_query(conn, "CALL getLastQuotId()")) {
        fprintf(stderr, "Error al ejecutar el procedimiento almacenado: %s\n", mysql_error(conn));
        return NULL;
    }
    return mysql_store_result(conn);
}

void create_quotation(MYSQL *conn, Quotation *quotation) {
    char query[256];
    snprintf(query, sizeof(query), "CALL createQuotation(%.2f, %.2f)", 
             quotation->sub_total, quotation->total_taxes);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error al ejecutar createQuotation: %s\n", mysql_error(conn));
    }
}

void add_line_to_quotation(MYSQL *conn, Quotation_Line *quotation_line) {
    char query[512];
    snprintf(query, sizeof(query), "CALL addLineToQuotation(%d, '%s', %d, %.2f, %.2f)",
             quotation_line->quotation_id, quotation_line->product_name,
             quotation_line->quantity, quotation_line->line_sub_total,
             quotation_line->line_total_taxes);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error al ejecutar addLineToQuotation: %s\n", mysql_error(conn));
    }
}


void hash_to_hex(const unsigned char *hash, char *hex_str, size_t length)
{
    for (size_t i = 0; i < length; i++) {
        sprintf(hex_str + (i * 2), "%02x", hash[i]);
    }
    hex_str[length * 2] = '\0';  // Asegura que la cadena termine en NULL
}

bool validate_credentials(MYSQL *conn, const char *username, unsigned char *password) {
    MYSQL_STMT *stmt;
    MYSQL_BIND bind[1], result_bind[1];
    char stored_password_hex[HASH_SIZE * 2 + 1];  // Hash en formato hex (64 caracteres + NULL)

    const char *query = "SELECT password FROM Users WHERE username = ?";
    stmt = mysql_stmt_init(conn);

    if (!stmt) {
        fprintf(stderr, "Error al inicializar consulta: %s\n", mysql_error(conn));
        return false;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query))) {
        fprintf(stderr, "Error al preparar consulta: %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return false;
    }

    memset(bind, 0, sizeof(bind));
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (char *)username;
    bind[0].buffer_length = strlen(username);
    mysql_stmt_bind_param(stmt, bind);

    if (mysql_stmt_execute(stmt)) {
        fprintf(stderr, "Error al ejecutar consulta: %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return false;
    }

    memset(result_bind, 0, sizeof(result_bind));
    result_bind[0].buffer_type = MYSQL_TYPE_STRING;
    result_bind[0].buffer = stored_password_hex;
    result_bind[0].buffer_length = sizeof(stored_password_hex);
    mysql_stmt_bind_result(stmt, result_bind);

    // Verifica si encontró un usuario
    if (mysql_stmt_fetch(stmt) == 0) {
        // Convierte el hash calculado a hexadecimal
        char input_password_hex[HASH_SIZE * 2 + 1];
        hash_to_hex(password, input_password_hex, HASH_SIZE);

        // Comparar las cadenas hexadecimales
        if (strcmp(stored_password_hex, input_password_hex) == 0) {
            printf("\n\nCredenciales válidas.\n");
            mysql_stmt_close(stmt);
            return true;
        }
    }

    mysql_stmt_close(stmt);
    return false;
}

void close_db_connection(MYSQL *conn) {
    mysql_close(conn);
}
