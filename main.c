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

int main() {
    int opcion;

    printf("==================================\n");
    printf("Bienvenido a Supermercados Walmart\n");
    printf("==================================\n");
    printf("Menu de Opciones\n");
    printf("1. Administracion\n");
    printf("2. Generales\n");

    printf("\n");
    printf("Ingrese la opcion: ");
    scanf("%d", &opcion);

    switch(opcion) {
        case 1:
            printf("Has seleccionado Administracion, sin embargo, no tienes permiso para acceder a esta opcion.\n");
            break;
        case 2:
            printf("Has seleccionado Generales.\n");
            break;
        default:
            printf("Opcion invalida. Por favor, ingrese 1 o 2.\n");
            break;
    }

    return 0;
}