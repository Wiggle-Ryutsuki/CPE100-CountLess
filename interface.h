#ifndef INTERFACE_H
#define INTERFACE_H

#include "database.h"

// Menu display and selection 
void displayMenu();
int getMenuSelection();

// Input and output for products
Product getUserInputForProduct();

// Input for transactions
Transaction getTransactionInput();

// General purpose message display
void displayMessage(const char *message);

#endif // INTERFACE_H

