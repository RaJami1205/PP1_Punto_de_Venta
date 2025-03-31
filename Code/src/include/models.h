#ifndef MODELS_H
#define MODELS_H

// Estructura para el usuario administrador
typedef struct {
    char username[50];
    char password[100];
} User;

// Estructura para el punto de venta
typedef struct {
    int id;
    char name[100];
    char legal_entity_id[100];
    char phone_number[50];
} Sales_Point;

// Estructura para la familia de productos
typedef struct {
    char code[50];
    char name[100];
} Product_Family;

// Estructura para los productos
typedef struct {
    char code[50];
    char name[100];
    char family[100];
    float cost;
    float price;
    int stock;
} Product;

// Estructura para los las líneas de cotizaciones
typedef struct {
    int line_id;
    int quotation_id;
    char product_name[100];
    int quantity;
    float price;
    float line_sub_total;
    float line_total_taxes;
} Quotation_Line;

// Estructura para las cotizaciones
typedef struct {
    int id;
    float sub_total;
    float total_taxes;
    float total;
    char state[100];
    int num_lines;
    Quotation_Line *lines;
} Quotation;

// Estructura para las lineas de facturas
typedef struct {
    int line_id;
    int invoice_id;
    char product_name[100];
    int quantity;
    float price;
    float line_sub_total;
    float line_total_taxes;
} Invoice_Line;

// Estructura para las facturas
typedef struct {
    int id;
    char customer_name[100]; 
    char date[11];
    int quotation_reference_id;
    double sub_total;
    double total_taxes;
    double total;
    int num_lines;
    Invoice_Line *lines;
} Invoice;

#endif /* MODELS_H */