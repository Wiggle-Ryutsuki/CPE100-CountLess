#ifndef AUTOMATION_H 
#define AUTOMATION_H

void loadProducts();
void saveProducts();
void executeRestockScript();
void autoRestock();
void schedulePurchase();
void checkScheduledRestock();
void autoPurchase(char *productID, int quantity);

#endif