#include "./include/products.h"

void print_products() {
    MYSQL *conn = connect_to_db();
    if (conn == NULL) {
        fprintf(stderr, "Error: No se pudo conectar a la base de datos\n");
        return;
    }

    MYSQL_RES *res = get_all_products(conn);
    if (res == NULL) {
        mysql_close(conn);
        return;
    }

    if (mysql_num_rows(res) == 0) {
        printf("No hay productos disponibles en la base de datos.\n");
        mysql_free_result(res);
        mysql_close(conn);
        return;
    }

    MYSQL_ROW row;
    printf("Productos Disponibles:\n");

    printf("┌────────────┬─────────────────────────┬─────────────────────────┬──────────┬──────────┐\n");
    printf("│ ID         │ Producto                │ Descripción             │ Stock    │ Precio   │\n");
    printf("├────────────┼─────────────────────────┼─────────────────────────┼──────────┼──────────┤\n");
    
    int width_id = 10, width_product = 23, width_desc = 23, width_stock = 8, width_price = 8;

    while ((row = mysql_fetch_row(res)) != NULL) {
        
        printf("│ %-*s │ %-*s │ %-*s │ %-*s │ %-*s │\n", 
               width_id, row[0],    // ID o número
               width_product, row[1], // Nombre del producto
               width_desc, row[6],    // Descripción o familia del producto
               width_stock, row[4],   // Stock
               width_price, row[3]    // Precio
        );
    }

    printf("└────────────┴─────────────────────────┴─────────────────────────┴──────────┴──────────┘\n");

    
    mysql_free_result(res);
    mysql_close(conn);
}

void print_filtered_products() {
    char family_input[MAX_INPUT];

    printf("Ingrese el nombre de la familia de productos a filtrar: ");
    fgets(family_input, MAX_INPUT, stdin);
    family_input[strcspn(family_input, "\n")] = '\0';

    MYSQL *conn = connect_to_db();
    if (!conn) return;

    MYSQL_RES *result = get_products_by_family(conn, family_input);
    if (!result) {
        printf("No se encontraron productos para la familia ingresada.\n");
        close_db_connection(conn);
        return;
    }

    printf("\nPRODUCTOS EN LA FAMILIA '%s':\n", family_input);

    printf("┌────────────┬─────────────────────────┬─────────────────────────┬──────────┬──────────┐\n");
    printf("│ ID         │ Producto                │ Descripción             │ Stock    │ Precio   │\n");
    printf("├────────────┼─────────────────────────┼─────────────────────────┼──────────┼──────────┤\n");
    
    MYSQL_ROW row;

    int width_id = 10, width_product = 23, width_desc = 23, width_stock = 8, width_price = 8;

    while ((row = mysql_fetch_row(result)) != NULL) {
        
        printf("│ %-*s │ %-*s │ %-*s │ %-*s │ %-*s │\n", 
                width_id, row[0],    // ID o número
                width_product, row[1], // Nombre del producto
                width_desc, row[6],    // Descripción o familia del producto
                width_stock, row[4],   // Stock
                width_price, row[3]    // Precio
        );
    }

    printf("└────────────┴─────────────────────────┴─────────────────────────┴──────────┴──────────┘\n");

    mysql_free_result(result);
    close_db_connection(conn);
}

void set_product_family() {
    Product_Family product_family[MAX_LINES];
    char buffer[MAX_LENGTH];
    int count = 0;

    char filename[100];
    printf("Ingrese la ruta del archivo: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error al leer la ruta del archivo\n");
        return;
    }
    filename[strcspn(filename, "\n")] = 0; // Eliminar el salto de línea

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    while (fgets(buffer, sizeof(buffer), file) && count < MAX_LINES) {
        buffer[strcspn(buffer, "\n")] = 0; // Eliminar salto de línea

        // Separar la línea en código y nombre usando ","
        char *code = strtok(buffer, ",");
        char *name = strtok(NULL, ",");

        // Validar que ambos campos existen
        if (code == NULL || name == NULL) {
            fprintf(stderr, "Error: línea inválida en el archivo. Se esperaba 'código,nombre'.\n");
            continue; // Saltar esta línea si el formato no es correcto
        }

        // Validar que los datos no están vacíos
        if (strlen(code) == 0 || strlen(name) == 0) {
            fprintf(stderr, "Error: Código o nombre vacío en la línea. Se ignorará.\n");
            continue; // Ignorar esta línea si algún dato está vacío
        }

        // Copiar los valores a la estructura
        strncpy(product_family[count].code, code, sizeof(product_family[count].code) - 1);
        product_family[count].code[sizeof(product_family[count].code) - 1] = '\0';

        strncpy(product_family[count].name, name, sizeof(product_family[count].name) - 1);
        product_family[count].name[sizeof(product_family[count].name) - 1] = '\0';

        count++;
    }

    fclose(file);

    if (count == 0) {
        printf("No se cargaron datos debido a errores en el archivo.\n");
        return;
    }

    // Conectar a la base de datos
    MYSQL *conn = connect_to_db();
    if (conn == NULL) {
        fprintf(stderr, "Error: No se pudo conectar a la base de datos\n");
        return;
    }

    // Insertar en la base de datos
    for (int i = 0; i < count; i++) {
        if (!insert_product_family(conn, product_family[i].code, product_family[i].name)) {
            printf("%s %s no pudo ser cargado.\n", product_family[i].code, product_family[i].name);
        } else {
            printf("%s %s cargada correctamente.\n", product_family[i].code, product_family[i].name);
        }
    }

    // Cerrar conexión
    close_db_connection(conn);
}

void set_product() {
    Product products[MAX_LINES];
    char buffer[MAX_LENGTH]; // Buffer para leer cada línea
    int count = 0;

    char filename[100];
    printf("Ingrese la ruta del archivo: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error al leer la ruta del archivo\n");
        return;
    }
    filename[strcspn(filename, "\n")] = 0; // Eliminar el salto de línea al final de la ruta

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    // Leer los productos del archivo
    while (fgets(buffer, sizeof(buffer), file) && count < MAX_LINES) {
        buffer[strcspn(buffer, "\n")] = 0; // Eliminar salto de línea

        // Usar sscanf correctamente para leer los valores
        int n = sscanf(buffer, "%49[^,],%99[^,],%49[^,],%f,%f,%d", 
                        products[count].code,
                        products[count].name,
                        products[count].family,
                        &products[count].cost, 
                        &products[count].price, 
                        &products[count].stock);

        // Verificar que se leyeron todos los valores correctamente
        if (n != 6) {
            fprintf(stderr, "Error al leer la línea: %s\n", buffer);
            continue; // Continuar con la siguiente línea si hay error
        }

        // Validación adicional de los valores
        if (strlen(products[count].code) == 0 || strlen(products[count].name) == 0 ||
            strlen(products[count].family) == 0 || products[count].cost <= 0 ||
            products[count].price <= 0 || products[count].stock < 0) {
            fprintf(stderr, "Datos inválidos en la línea: %s\n", buffer);
            continue; // Continuar con la siguiente línea si los datos son inválidos
        }

        count++;
    }

    fclose(file);

    // Verificar si se cargaron productos
    if (count == 0) {
        printf("No se cargaron productos válidos desde el archivo.\n");
        return;
    }

    // Conectar a la base de datos
    MYSQL *conn = connect_to_db();
    if (conn == NULL) {
        fprintf(stderr, "Error: No se pudo conectar a la base de datos\n");
        return;
    }

    // Insertar los productos en la base de datos
    for (int i = 0; i < count; i++) {
        if (!insert_product(conn, &products[i])) {
            printf("%s %s no pudo ser cargado.\n", products[i].code, products[i].name);
        } else {
            printf("%s %s cargado correctamente.\n", products[i].code, products[i].name);
        }
    }

    // Cerrar la conexión a la base de datos
    close_db_connection(conn);
}

void delete_product() {
    print_products();

    // Pedir al usuario que ingrese el código del producto a eliminar
    char product_id[50];
    printf("Ingrese el ID del producto que desea eliminar: ");
    if (scanf("%49s", product_id) != 1) {
        fprintf(stderr, "Error al leer el ID del producto.\n");
        return;
    }

    MYSQL *conn = connect_to_db();
    if (conn == NULL) {
        fprintf(stderr, "Error: No se pudo conectar a la base de datos.\n");
        return;
    }

    // Intentar eliminar el producto y verificar si se realizó correctamente
    if (drop_product(conn, product_id)) {
        printf("Producto con ID %s eliminado correctamente.\n", product_id);
    } else {
        printf("No se pudo eliminar el producto con ID %s. Puede que no exista.\n", product_id);
    }

    // Cerrar la conexión
    mysql_close(conn);
}


void loadProductStock(){
    Product products[MAX_LINES];
    char buffer[MAX_LENGTH]; // Buffer para leer cada línea
    int count = 0;

    char filename[100];
    printf("Ingrese la ruta del archivo: ");
    scanf("%99s", filename);
    
    // Eliminar el salto de línea que fgets agrega
    filename[strcspn(filename, "\n")] = 0;
    
    printf("Ruta ingresada: %s\n", filename); // Depuración

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    while (fgets(buffer, MAX_LENGTH, file) && count < MAX_LINES) {
        char code[50];
        int stock;
        
        if (sscanf(buffer, "%49[^,],%d", code, &stock) >= 1) {
            strcpy(products[count].code, code);
            products[count].stock = stock;
            count++;
        }
    }

    fclose(file);

    MYSQL *conn = connect_to_db();

    update_stock_product(conn, products, &count);  

    mysql_close(conn);
}
