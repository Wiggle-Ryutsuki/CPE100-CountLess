#include "interface.h"
#include <stdio.h>
#include <string.h>

// Display the main menu
void displayMenu() {
    printf("\nMenu:\n");
    printf("1. Load Products from File\n");
    printf("2. Save Products to File\n");
    printf("3. Add Product\n");
    printf("4. Display Products\n");
    printf("5. Add Transaction\n");
    printf("6. Display Transactions\n");
    printf("7. Exit\n");
}

// Get the user's menu selection
int getMenuSelection() {
    int choice;
    printf("Enter your choice: ");
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n'); // Clear invalid input
        return -1;
    }
    return choice;
}

// Get user input for a product
Product getUserInputForProduct() {
    Product p;

    printf("Enter product ID: ");
    scanf("%9s", p.product_id);

    printf("Enter name: ");
    scanf("%49s", p.name);

    printf("Enter description: ");
    scanf(" %[^\n]s", p.description);

    printf("Enter category: ");
    scanf("%49s", p.category);

    printf("Enter price: ");
    scanf("%lf", &p.price);

    printf("Enter stock quantity: ");
    scanf("%d", &p.stock_quantity);

    printf("Enter minimum threshold: ");
    scanf("%d", &p.minimum_threshold);

    printf("Enter restock amount: ");
    scanf("%d", &p.restock_amount);

    printf("Enter last updated date: ");
    scanf("%19s", p.last_updated);

    return p;
}

// Get user input for a transaction
Transaction getTransactionInput() {
    Transaction t;

    printf("Enter transaction ID: ");
    scanf("%9s", t.transaction_id);

    printf("Enter timestamp (e.g., 12/8/2024 10:45): ");
    scanf(" %[^\n]s", t.timestamp);

    strcpy(t.action_type, "Purchase Completed");
    strcpy(t.user, "Customer");

    printf("Enter product name: ");
    scanf("%49s", t.product_name);

    printf("Enter category: ");
    scanf("%49s", t.category);

    printf("Enter discount code (or '-' for none): ");
    scanf("%19s", t.discount_code);

    printf("Enter price: ");
    scanf("%lf", &t.price);

    printf("Enter quantity: ");
    scanf("%d", &t.quantity);

    // Calculate total price
    t.total = t.price * t.quantity;

    return t;
}

// Display a general message to the user
void displayMessage(const char *message) {
    printf("%s\n", message);
}
