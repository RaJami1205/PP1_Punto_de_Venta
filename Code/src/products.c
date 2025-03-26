#include <stdio.h>
#include <string.h>
#include "./include/models.h"
#include "./include/db_handler.h"

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

    char buffer[MAX_LENGTH]; // Buffer para leer cada línea
    int count = 0;

    while (fgets(buffer, sizeof(buffer), file) && count < MAX_LINES) {
        buffer[strcspn(buffer, "\n")] = 0; // Eliminar salto de línea

        // Separar la línea en código y nombre usando ","
        char *code = strtok(buffer, ",");
        char *name = strtok(NULL, ",");

        if (code == NULL || name == NULL) {
            fprintf(stderr, "Error: línea inválida en el archivo\n");
            continue; // Saltar esta línea si el formato no es correcto
        }

        // Copiar los valores a la estructura
        strncpy(product_family[count].code, code, sizeof(product_family[count].code) - 1);
        product_family[count].code[sizeof(product_family[count].code) - 1] = '\0';

        strncpy(product_family[count].name, name, sizeof(product_family[count].name) - 1);
        product_family[count].name[sizeof(product_family[count].name) - 1] = '\0';

        count++;
    }

    fclose(file);

    // Conectar a la base de datos
    MYSQL *conn = connect_to_db();
    if (conn == NULL) {
        fprintf(stderr, "Error: No se pudo conectar a la base de datos\n");
        return;
    }

    // Insertar en la base de datos
    for (int i = 0; i < count; i++) {
        insert_product_family(conn, product_family[i].code, product_family[i].name);
    }

    // Cerrar conexión
    close_db_connection(conn);
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
    
    // Leer los productos del archivo
    while (fgets(buffer, sizeof(buffer), file) && count < MAX_LINES) {
        buffer[strcspn(buffer, "\n")] = 0;  // Eliminar el salto de línea

        // Usar sscanf correctamente para leer los valores
        int n = sscanf(buffer, "%49[^,],%99[^,],%f,%f,%d,%49s",
                        products[count].code,
                        products[count].name, 
                        &products[count].cost, 
                        &products[count].price, 
                        &products[count].stock, 
                        products[count].product_family_id);

        // Verificar que se leyeron todos los valores correctamente
        if (n != 6) {
            printf("Error al leer la línea: %s\n", buffer);
        } else {
            count++;
        }
    }

    fclose(file);

    // Conectar a la base de datos
    MYSQL *conn = connect_to_db();

    // Insertar los productos en la base de datos
    for (int i = 0; i < count; i++) {
        insert_product(conn, &products[i]);
    }

    // Cerrar la conexión a la base de datos
    close_db_connection(conn);

    // Imprimir los productos cargados
    printf("Productos cargados:\n");
    for (int i = 0; i < count; i++) {
        printf("Nombre: %s, Costo: %.2f, Precio: %.2f, Stock: %d, Familia: %s\n",
               products[i].name, 
               products[i].cost, 
               products[i].price, 
               products[i].stock, 
               products[i].product_family_id);
    }
}

void deleteProduct() {
    // Conectar a la base de datos
    MYSQL *conn_1 = connect_to_db();

    // Mostrar todos los productos
    get_all_products(conn_1);
    mysql_close(conn_1);
    
    // Pedir al usuario que ingrese el código del producto a eliminar
    char product_id[50];
    printf("Ingrese el ID del producto que desea eliminar: ");
    scanf("%49s", product_id);

    MYSQL *conn_2 = connect_to_db();

    delete_product(conn_2, product_id);

    // Cerrar la conexión
    mysql_close(conn_2);
}