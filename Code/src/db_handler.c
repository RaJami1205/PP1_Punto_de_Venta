#include "./include/db_handler.h"

/*
==========================================================================
                              CONECTOR BD
==========================================================================
*/
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

void close_db_connection(MYSQL *conn) {
    mysql_close(conn);
}

/*
==========================================================================
                                PUNTO DE VENTA
==========================================================================
*/

Sales_Point get_sales_point(MYSQL *conn) {
    Sales_Point sales_point = {0, "", "", ""};
    if (!conn) {
        fprintf(stderr, "Error: Conexión a la base de datos no válida.\n");
        return sales_point;
    }

    if (mysql_query(conn, "CALL getSalesPoint()")) {
        fprintf(stderr, "Error al ejecutar getSalesPoint: %s\n", mysql_error(conn));
        return sales_point;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (!result) {
        fprintf(stderr, "Error al obtener resultados: %s\n", mysql_error(conn));
        return sales_point;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row) {
        sales_point.id = atoi(row[0]);
        strncpy(sales_point.name, row[1], sizeof(sales_point.name) - 1);
        strncpy(sales_point.legal_entity_id, row[2], sizeof(sales_point.legal_entity_id) - 1);
        strncpy(sales_point.phone_number, row[3], sizeof(sales_point.phone_number) - 1);
    }

    mysql_free_result(result);
    return sales_point;
}


/*
==========================================================================
                                PRODUCTOS
==========================================================================
*/

bool insert_product_family(MYSQL *conn, const char *code, const char *name) {
    char query[256];

    snprintf(query, sizeof(query),
             "CALL insertProductFamily('%s', '%s');",
             code, name);

    if (mysql_query(conn, query)) {
        //fprintf(stderr, "Error al insertar familia de productos: %s\n", mysql_error(conn));
        return false;
    }
    return true;
}


bool insert_product(MYSQL *conn, Product *product) {
    char query[500];

    // Llamar directamente al procedimiento almacenado insertNewProduct

    snprintf(query, sizeof(query), 
             "CALL insertNewProduct('%s', '%s', '%s', %.2f, %.2f, %d);",
             product->code,    // product_id
             product->name,    // name
             product->family, // product_family_name
             product->cost,    // cost
             product->price,   // price
             product->stock);  // stock

    if (mysql_query(conn, query)) {
        //fprintf(stderr, "Error al insertar producto: %s\n", mysql_error(conn));
        return false;
    }
    return true;
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

bool drop_product(MYSQL *conn, const char *code) {
    if (conn == NULL) {
        fprintf(stderr, "Error: Conexión a la base de datos no válida.\n");
        return false;
    }

    // Crear la consulta para llamar al procedimiento almacenado
    char query[200];
    snprintf(query, sizeof(query), "CALL deleteProductById('%s')", code);

    // Ejecutar la consulta
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error al ejecutar el procedimiento: %s\n", mysql_error(conn));
        return false;
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
    return true;
}

void update_stock_product(MYSQL *conn, Product *product, int *count) {
    for (int i = 0; i < *count; i++) {
        char query[500];
        MYSQL_RES *res;
        MYSQL_ROW row;
        int current_stock;  // Variable para almacenar el stock actual de la base de datos
        int new_stock;      // Nuevo stock que vamos a actualizar

        // Llamar al procedimiento almacenado para obtener el stock del producto
        snprintf(query, sizeof(query), "CALL GetProductStock('%s', @current_stock);", product[i].code);
        if (mysql_query(conn, query)) {
            fprintf(stderr, "Error al obtener el stock del producto %s: %s\n", product[i].code, mysql_error(conn));
            return;
        }
        
        // Obtener el valor de la variable de salida @current_stock
        if (mysql_query(conn, "SELECT @current_stock")) {
            fprintf(stderr, "Error al recuperar current_stock: %s\n", mysql_error(conn));
            return;
        }
        
        res = mysql_store_result(conn);
        if (res) {
            row = mysql_fetch_row(res);
            if (row) {
                current_stock = atoi(row[0]);  // Convertir el valor a un entero
            }
            mysql_free_result(res);
        }

        // Sumar el stock actual con el stock del producto
        new_stock = current_stock + product[i].stock;

        // Ahora, pasamos el nuevo stock al procedimiento almacenado para actualizarlo
        snprintf(query, sizeof(query), "CALL UpdateProductStock('%s', %d);", product[i].code, new_stock);
        
        if (mysql_query(conn, query)) {
            fprintf(stderr, "Error al actualizar el stock del producto %s: %s\n", product[i].code, mysql_error(conn));
            return;
        } else {
            printf("Stock actualizado para el producto %s: %d\n", product[i].code, new_stock);
        }
    }
}

/*
==========================================================================
                            COTIZACIONES
==========================================================================
*/

MYSQL_RES *get_last_quotation_id(MYSQL *conn) {
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
    snprintf(query, sizeof(query), "CALL addLineToQuotation(%d, %d, '%s', %d, %.2f, %.2f)",
             quotation_line->line_id, quotation_line->quotation_id, 
             quotation_line->product_name, quotation_line->quantity, 
             quotation_line->line_sub_total, quotation_line->line_total_taxes);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error al ejecutar addLineToQuotation: %s\n", mysql_error(conn));
    }
}

void update_quotation(MYSQL *conn, Quotation *quotation) {
    if (!conn || !quotation) {
        fprintf(stderr, "Error: Conexión o cotización no válida.\n");
        return;
    }

    char query[512];
    snprintf(query, sizeof(query), "CALL updateQuotation(%d, %.2f, %.2f)", 
             quotation->id, 
             quotation->sub_total, 
             quotation->total_taxes);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error al ejecutar updateQuotation: %s\n", mysql_error(conn));
    }
}


void update_quotation_lines(MYSQL *conn, Quotation_Line *quotation_line) {
    if (!conn || !quotation_line) {
        fprintf(stderr, "Error: Conexión o línea de cotización no válida.\n");
        return;
    }

    char query[512];
    snprintf(query, sizeof(query), "CALL updateQuotationLine(%d, %d, '%s', %d, %.2f, %.2f)",
            quotation_line->line_id, 
            quotation_line->quotation_id,
            quotation_line->product_name,
            quotation_line->quantity,
            quotation_line->line_sub_total,
            quotation_line->line_total_taxes);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error al ejecutar updateQuotationLine: %s\n", mysql_error(conn));
    }
}

void delete_modified_quotation_lines(MYSQL *conn, Quotation *quotation) {
    if (!conn) {
        fprintf(stderr, "Error: Conexión a la base de datos no válida.\n");
        return;
    }

    char query[256];
    snprintf(query, sizeof(query), "CALL deleteQuotationLinesFromId(%d, %d)", quotation->id, quotation->num_lines + 1);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error al ejecutar deleteQuotationLinesFromId: %s\n", mysql_error(conn));
    }
}

MYSQL_RES *get_quotations(MYSQL *conn, bool show_all) {
    if (!conn) {
        fprintf(stderr, "Error: Conexión a la base de datos no válida.\n");
        return NULL;
    }

    char query[256];
    snprintf(query, sizeof(query), "CALL getQuotations(%d)", show_all);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error al ejecutar getQuotations: %s\n", mysql_error(conn));
        return NULL;
    }

    return mysql_store_result(conn);
}

MYSQL_RES *get_quotation_by_id(MYSQL *conn, int quotation_id) {
    if (!conn) {
        fprintf(stderr, "Error: Conexión a la base de datos no válida.\n");
        return NULL;
    }

    char query[256];
    snprintf(query, sizeof(query), "CALL searchQuotation(%d)", quotation_id);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error al ejecutar searchQuotation: %s\n", mysql_error(conn));
        return NULL;
    }
    
    return mysql_store_result(conn);
}

MYSQL_RES *get_quotation_lines(MYSQL *conn, int quotation_id) {
    if (!conn) {
        fprintf(stderr, "Error: Conexión a la base de datos no válida.\n");
        return NULL;
    }

    char query[256];
    snprintf(query, sizeof(query), "CALL searchQuotationLines(%d)", quotation_id);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error al ejecutar searchQuotationLines: %s\n", mysql_error(conn));
        return NULL;
    }    
    return mysql_store_result(conn);
}

/*
==========================================================================
                              FACTURACIÓN
==========================================================================
*/

MYSQL_RES *get_last_invoice_id(MYSQL *conn) {
    if (mysql_query(conn, "CALL getLastInvoiceId()")) {
        fprintf(stderr, "Error al ejecutar el procedimiento almacenado: %s\n", mysql_error(conn));
        return NULL;
    }
    return mysql_store_result(conn);
}

void create_invoice(MYSQL *conn, Invoice *invoice) {
    char query[256];
    snprintf(query, sizeof(query), "CALL createInvoice(%d, '%s', '%s', %.2f, %.2f)", 
                invoice->quotation_reference_id, invoice->date, invoice->customer_name,      
                invoice->sub_total, invoice->total_taxes);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error al ejecutar createInvoice: %s\n", mysql_error(conn));
    }
}

void add_line_to_invoice(MYSQL *conn, Invoice_Line *invoice_line) {
    char query[512];

    snprintf(query, sizeof(query), "CALL addLineToInvoice(%d, %d, '%s', %d, %.2f, %.2f)",
        invoice_line->line_id, invoice_line->invoice_id, 
        invoice_line->product_name, invoice_line->quantity, 
        invoice_line->line_sub_total, invoice_line->line_total_taxes);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error al ejecutar addLineToInvoice: %s\n", mysql_error(conn));
    }
}

MYSQL_RES *get_invoices(MYSQL *conn) {
    if (!conn) {
        fprintf(stderr, "Error: Conexión a la base de datos no válida.\n");
        return NULL;
    }

    char query[256];
    snprintf(query, sizeof(query), "CALL getInvoices()");

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error al ejecutar getInvoices: %s\n", mysql_error(conn));
        return NULL;
    }

    return mysql_store_result(conn);
}

MYSQL_RES *get_invoice_by_id(MYSQL *conn, int invoice_id) {
    if (!conn) {
        fprintf(stderr, "Error: Conexión a la base de datos no válida.\n");
        return NULL;
    }

    char query[256];
    snprintf(query, sizeof(query), "CALL searchInvoice(%d)", invoice_id);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error al ejecutar searchInvoice: %s\n", mysql_error(conn));
        return NULL;
    }
    
    return mysql_store_result(conn);
}

MYSQL_RES *get_invoice_lines(MYSQL *conn, int invoice_id) {
    if (!conn) {
        fprintf(stderr, "Error: Conexión a la base de datos no válida.\n");
        return NULL;
    }

    char query[256];
    snprintf(query, sizeof(query), "CALL searchInvoiceLines(%d)", invoice_id);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error al ejecutar searchInvoiceLines: %s\n", mysql_error(conn));
        return NULL;
    }    
    return mysql_store_result(conn);
}

/*
 ==========================================================================
                                 LOGIN
 ==========================================================================
 */

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
