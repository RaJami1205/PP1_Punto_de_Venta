#ifndef MODELS_H
#define MODELS_H

#define MAX_LINES 100  
#define MAX_LENGTH 256 

// Estructura para el usuario administrador
typedef struct {
    char username[50];
    char password[100];
} User;

// Estructura para el punto de venta
typedef struct {
    char name[100];
    char legal_entity_id[100];
    char phone_number[50];
} Sales_Point;

// Estructura para la familia de productos
typedef struct {
    char code[50];
    char name[MAX_LENGTH];
} Product_Family;

// Estructura para los productos
typedef struct {
    char code[50];
    char name[100];
    float cost;
    float price;
    int stock;
    char product_family[50];
} Product;

// Estructura para los las líneas de cotizaciones
typedef struct {
    int line_id;
    int quotation_id;
    char product_name[100];
    int quantity;
    char line_sub_total[10];
    char line_total_taxes[10];
} Quotation_Line;

// Estructura para las cotizaciones
typedef struct {
    int id;
    char sub_total[10];
    char total_taxes[10];
    char total[10];
    char state[100];
    int num_lines;
    Quotation_Line *lines;
} Quotation;



// Estructura para las facturas
typedef struct {
    int quotation_reference_id;
    char date[10];
    char sub_total[10];
    char total_taxes[10];
    char total[10];
} Invoice;

// Estructura para las lineas de facturas
typedef struct {
    int invoice_id;
    int product_id;
    int quantity;
    char line_sub_total[10];
    char line_total_taxes[10];
} Invoice_Line;

#endif /* MODELS_H */