#include <stdio.h>
#include <string.h>
#include "Sales_Points.h"

#define MAX_LINES 100  
#define MAX_LENGTH 100

void setProductFamily() {
    Product_Family product_family[MAX_LINES];
    
    char filename[100];
    printf("Ingrese el nombre del archivo: ");
    scanf("%99s", filename);
    
    FILE *file = fopen(filename, "r"); 
    if (file == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    char buffer[MAX_LENGTH];
    int count = 0;
    while (fgets(buffer, sizeof(buffer), file) && count < MAX_LINES) {
        buffer[strcspn(buffer, "\n")] = 0; 
        strcpy(product_family[count].name, buffer); 
        count++;
    }

    fclose(file);

    // Función para almacenar en la base de datos
}

void setProduct() {
    Product products[MAX_LINES]; 

    char filename[100];
    printf("Ingrese el nombre del archivo: ");
    scanf("%99s", filename);
    
    FILE *file = fopen(filename, "r"); 
    if (file == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    char buffer[MAX_LENGTH];
    int count = 0;
    
    while (fgets(buffer, sizeof(buffer), file) && count < MAX_LINES) {
        buffer[strcspn(buffer, "\n")] = 0;

        sscanf(buffer, "%99[^,],%9[^,],%9[^,],%d,%d", 
               products[count].name, 
               products[count].cost, 
               products[count].price, 
               &products[count].stock, 
               &products[count].product_family_id);
        
        count++;
    }

    fclose(file);

    printf("Productos cargados:\n");
    for (int i = 0; i < count; i++) {
        printf("Nombre: %s, Costo: %s, Precio: %s, Stock: %d, Familia: %d\n",
               products[i].name, 
               products[i].cost, 
               products[i].price, 
               products[i].stock, 
               products[i].product_family_id);
    }
}
