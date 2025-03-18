#ifndef SALES_POINTS_H
#define SALES_POINTS_H

#define MAX_LINES 100  
#define MAX_LENGTH 100 

typedef struct {
    char username[50];
    char password[100];
} User;

typedef struct {
    char name[100];
    char legal_entity_id[100];
    char phone_number[50];
} Sales_Point;

typedef struct {
    char name[MAX_LENGTH];
} Product_Family;

typedef struct {
    char name[100];
    char cost[10];
    char price[10];
    int stock;
    int product_family_id;
} Product;

typedef struct {
    char sub_total[10];
    char total_taxes[10];
    char total[10];
    char state[100];
} Quotation;

typedef struct {
    int quotation_id;
    int product_id;
    int quantity;
    char line_sub_total[10];
    char line_total_taxes[10];
} Quotation_Line;

typedef struct {
    int quotation_reference_id;
    char date[10];
    char sub_total[10];
    char total_taxes[10];
    char total[10];
} Invoice;

typedef struct {
    int invoice_id;
    int product_id;
    int quantity;
    char line_sub_total[10];
    char line_total_taxes[10];
} Invoice_Line;

void setProductFamily();
void setProduct();
void deleteProduct();
void setQuotation();
void setQuotationLine();
void setInvoice();
void setInvoiceLine();
void setUser();
void setSalesPoint();

#endif