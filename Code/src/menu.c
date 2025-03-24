#include <stdio.h>
#include "./include/menu.h"
#include "./include/products.h"

int printMainMenu() {
    int opcion;
    printf("\n==================================\n");
    printf("Bienvenido a Supermercados Walmart\n");
    printf("==================================\n");
    printf("1. Administración\n");
    printf("2. Generales\n");
    printf("3. Salir\n\n");
    printf("Ingrese la opción: ");
    scanf("%d", &opcion);
    return opcion;
}

bool login() {

    // TO-DO: LÓGICA PARA VERIFICAR CREDENCIALES
    printf("\nUsuario:\n");
    printf("Contraseña:\n");

    return true;

}

void printSubMenuAdmin() {
    int subopcion_A;

    printf("\n");
    printf("====================================\n");
    printf("    Opciones de Administracion\n");
    printf("====================================\n");
    printf("1. Registro de familias de productos\n");
    printf("2. Registro de productos\n");
    printf("3. Cargar inventario\n");
    printf("4. Consulta de facturas\n");
    printf("5. Crear cotización\n");
    printf("6. Finalizar factura\n");
    printf("7. Estadisticas\n");
    printf("6. Volver\n");
            
    printf("\n");
    printf("Ingrese la opcion: ");
    scanf("%d", &subopcion_A);

    switch(subopcion_A) {
        case 1:
            printf("\n");
            setProductFamily();
            printSubMenuAdmin();
            
        case 2:
            printf("\n");
            setProduct();
            printSubMenuAdmin();

        case 3:
            printf("Has seleccionado Cargar inventario.\n");
            break;

        case 4:
            printf("Has seleccionado Consulta de facturas.\n");
            break;

        case 5:
            printf("Has seleccionado Estadísticas.\n");
            break;

        case 6:
            main();
            break;

        default:
            printf("Opción inválida en el submenú de administración.\n");
            printSubMenuAdmin();
            break;
    }
}

void printSubMenuGeneral() {
    int subopcion_G;

    printf("\n");
    printf("=======================\n");
    printf("  Opciones Generales\n");
    printf("=======================\n");
    printf("1. Consulta de Catalogo\n");
    printf("2. Cotizar\n");
    printf("3. Facturar\n");
    printf("4. Volver\n");
            
    printf("\n");
    printf("Ingrese la opcion: ");
    scanf("%d", &subopcion_G);

    switch(subopcion_G) {
        case 1:
            printf("Has seleccionado Consulta de Catalogo.\n");
            break;

        case 2:
            printf("Has seleccionado Cotizar.\n");
            break;

        case 3:
            printf("Has seleccionado Facturar.\n");
            break;

        case 4:
            main();
            break;

        default:
            printf("Opción inválida en el submenú de generales.\n");
            printSubMenuGeneral();
            break;
    }
}
