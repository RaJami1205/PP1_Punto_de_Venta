#ifndef MENU_H
#define MENU_H

#include <stdbool.h>

void printSubMenuAdmin();
void hash_passwd(const char *password, unsigned char *hashed_password);
bool login();
void printSubMenuGeneral();

#endif /* MENU_H */