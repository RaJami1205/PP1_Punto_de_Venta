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