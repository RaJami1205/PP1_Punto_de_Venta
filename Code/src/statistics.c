#include "./include/statistics.h"

void print_pending_quotations(){
    // Conectar a la base de datos
    MYSQL *conn = connect_to_db();

    // Función que llama a la vista e imprime los datos
    view_pending_quotations(conn);

    // Cerrar conexión
    close_db_connection(conn);
};

void print_quotations_with_invoices(){
    // Conectar a la base de datos
    MYSQL *conn = connect_to_db();

    // Función que llama a la vista e imprime los datos
    view_quotations_with_invoices(conn);

    // Cerrar conexión
    close_db_connection(conn);
};

void print_total_purchase_average(){
    // Conectar a la base de datos
    MYSQL *conn = connect_to_db();

    // Función que llama a la vista e imprime los datos
    view_total_purchase_average(conn);

    // Cerrar conexión
    close_db_connection(conn);
};

void print_top_5_selling_products(){
    // Conectar a la base de datos
    MYSQL *conn = connect_to_db();

    // Función que llama a la vista e imprime los datos
    view_top_5_selling_products(conn);

    // Cerrar conexión
    close_db_connection(conn);
};

void print_top_product_per_family(){
    // Conectar a la base de datos
    MYSQL *conn = connect_to_db();

    // Función que llama a la vista e imprime los datos
    view_top_product_per_family(conn);

    // Cerrar conexión
    close_db_connection(conn);
};

void print_total_sales_per_family(){
    // Conectar a la base de datos
    MYSQL *conn = connect_to_db();

    // Función que llama a la vista e imprime los datos
    view_total_sales_per_family(conn);

    // Cerrar conexión
    close_db_connection(conn);
};