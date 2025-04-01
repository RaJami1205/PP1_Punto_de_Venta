#ifndef DB_HANDLER_H
#define DB_HANDLER_H

#include <mysql/mysql.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <openssl/sha.h>
#include "models.h"

#define HASH_SIZE SHA256_DIGEST_LENGTH

/*
==========================================================================
                              CONECTOR BD
==========================================================================
*/
// Función para conectar a la base de datos
MYSQL *connect_to_db();

// Función para cerrar la conexión a la base de datos
void close_db_connection(MYSQL *conn);

/*
==========================================================================
                                PUNTO DE VENTA
==========================================================================
*/

// Función para obtener la información del punto de venta
Sales_Point get_sales_point(MYSQL *conn);

/*
==========================================================================
                                PRODUCTOS
==========================================================================
*/
// Función para insertar una nueva familia de productos
bool insert_product_family(MYSQL *conn, const char *code, const char *name);

// Función para insertar un nuevo producto
bool insert_product(MYSQL *conn, Product *product);

// Función para obtener producto por nombre o id
MYSQL_RES *search_product(MYSQL *conn, const char *input);

//Función para mostrar los productos disponibles
MYSQL_RES* get_all_products(MYSQL *conn);

// Función para obtener productos filtrados por familia
MYSQL_RES *get_products_by_family(MYSQL *conn, const char *family);

//Función para eliminar un producto
void drop_product(MYSQL *conn, const char *code);

// Función para aumentar la cantidad en stock de los productos
void update_stock_product(MYSQL *conn, Product *product, int *count);

/*
==========================================================================
                              COTIZACIONES
==========================================================================
*/
// Función para obtener el último id de cotización creado
MYSQL_RES *get_last_quotation_id(MYSQL *conn);

// Función para agregar la cotización
void create_quotation(MYSQL *conn, Quotation *quotation);

// Función para agregar nueva linea a la cotización
void add_line_to_quotation(MYSQL *conn, Quotation_Line *quotation_line);

// Función para modificar la cotización
void update_quotation(MYSQL *conn, Quotation *quotation);

// Función para modificar linea de la cotización
void update_quotation_lines(MYSQL *conn, Quotation_Line *quotation_line);

// Función para eliminar las líneas antiguas de cotización modificada
void delete_modified_quotation_lines(MYSQL *conn, Quotation *quotation);

// Función para recuperar cotizaciones de la bd
MYSQL_RES *get_quotations(MYSQL *conn, bool show_all);

// Función para buscar cotización
MYSQL_RES *get_quotation_by_id(MYSQL *conn, int quotation_id);

// Función para obtener líneas de cotización
MYSQL_RES *get_quotation_lines(MYSQL *conn, int quotation_id);

/*
==========================================================================
                              FACTURACIÓN
==========================================================================
*/

// Función para obtener el último id de factura creada
MYSQL_RES *get_last_invoice_id(MYSQL *conn);

// Función para agregar la factura
void create_invoice(MYSQL *conn, Invoice *invoice);

// Función para agregar nueva linea a la factura
void add_line_to_invoice(MYSQL *conn, Invoice_Line *invoice_line);

/*
==========================================================================
                                LOGIN
==========================================================================
*/
// Función para transformar de hash a hexadecimal
void hash_to_hex(const unsigned char *hash, char *hex_str, size_t length);

// Función para verificar login
bool validate_credentials(MYSQL *conn, const char *username, unsigned char *password);

/*
==========================================================================
                              ESTADÍSTICAS
==========================================================================
*/
// Función para llamar a la vista de la cantidad de cotizaciones pendientes
void view_pending_quotations(MYSQL *conn);

// Función para llamar a la vista de la cantidad de cotizaciones facturadas
void view_quotations_with_invoices(MYSQL *conn);

// Función para llamar a la vista del promedio de compra total
void view_total_purchase_average(MYSQL *conn);

// Función para llamar a la vista de los 5 productos más vendidos
void view_top_5_selling_products(MYSQL *conn);

// Función para llamar a la vista de los productos más vendidos por familia
void view_top_product_per_family(MYSQL *conn);

// Función para llamar a la vista de los montos vendidos por familia
void view_total_sales_per_family(MYSQL *conn);

#endif /* DB_HANDLER_H */