#include <stdio.h>
#include "./include/menu.h"

int main() {
    int opcion;

    do {
        opcion = printMainMenu();
        switch (opcion) {
            case 1:
                printSubMenuAdmin();
                break;
            case 2:
                printSubMenuGeneral();
                break;
            case 3:
                printf("Gracias por utilizar el programa.\n");
                break;
            default:
                printf("Opcion invalida. Por favor, ingrese una opción válida.\n");
        }
    } while (opcion != 3);

    return 0;
}