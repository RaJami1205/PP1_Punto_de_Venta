/*
 * Archivo Principal: main.c
 *
 * Proyecto 1 de Lenguajes de Programación 
 *      
 * Autores:
 *      Raúl Alfaro
 *      Eyden Días
 */
#include <stdio.h>

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
    printf("5. Estadisticas\n");
    printf("6. Volver\n");
            
    printf("\n");
    printf("Ingrese la opcion: ");
    scanf("%d", &subopcion_A);

    switch(subopcion_A) {
        case 1:
            printf("Has seleccionado Registro de familias de productos.\n");
            break;
            
        case 2:
            printf("Has seleccionado Registro de productos.\n");
            break;

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
            printf("\n");
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

int main() {
    int opcion;

    printf("\n");
    printf("==================================\n");
    printf("Bienvenido a Supermercados Walmart\n");
    printf("==================================\n");
    printf("Menu de Opciones\n");
    printf("1. Administracion\n");
    printf("2. Generales\n");
    printf("3. Salir\n");

    printf("\n");
    printf("Ingrese la opcion: ");
    scanf("%d", &opcion);

    switch(opcion) {
        case 1:
            printSubMenuAdmin();

        case 2:
            printSubMenuGeneral();
            break;

        case 3:
            break;

        default:
            printf("Opcion invalida. Por favor, ingrese 1 o 2.\n");
            main();
            break;
    }

    return 0;
}