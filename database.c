#include "database.h"

// Load products from a CSV file
int loadDatabase(const char *filename, Product products[], int *productCount) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return -1;
    }

    *productCount = 0;
    char buffer[512];
    while (fgets(buffer, sizeof(buffer), file)) {
        if (*productCount >= MAX_PRODUCTS) {
            fprintf(stderr, "Maximum product limit reached.\n");
            break;
        }
        Product *p = &products[*productCount];
        sscanf(buffer, "%9[^,],%49[^,],%99[^,],%49[^,],%lf,%d,%d,%d,%19[^\n]",
               p->product_id, p->name, p->description, p->category,
               &p->price, &p->stock_quantity, &p->minimum_threshold,
               &p->restock_amount, p->last_updated);
        (*productCount)++;
    }

    fclose(file);
    return 0;
}

// Save products to a CSV file
int saveDatabase(const char *filename, Product products[], int productCount) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file");
        return -1;
    }

    for (int i = 0; i < productCount; i++) {
        Product *p = &products[i];
        fprintf(file, "%s,%s,%s,%s,%.2lf,%d,%d,%d,%s\n",
                p->product_id, p->name, p->description, p->category,
                p->price, p->stock_quantity, p->minimum_threshold,
                p->restock_amount, p->last_updated);
    }

    fclose(file);
    return 0;
}

// Add a transaction
void addTransaction(Transaction transactions[], int *transactionCount, Transaction newTransaction) {
    if (*transactionCount >= MAX_TRANSACTIONS) {
        fprintf(stderr, "Transaction list is full.\n");
        return;
    }
    transactions[(*transactionCount)++] = newTransaction;
}

// Display transactions
void displayTransactions(const Transaction transactions[], int transactionCount) {
    printf("\n%-20s %-20s %-10s %-20s %-15s %-15s %-10s %-10s %-10s\n",
           "Timestamp", "ActionType", "User", "ProductName", "Category",
           "DiscountCode", "Price", "Quantity", "Total");
    printf("--------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < transactionCount; i++) {
        const Transaction *t = &transactions[i];
        printf("%-20s %-20s %-10s %-20s %-15s %-15s %-10.2lf %-10d %-10.2lf\n",
               t->timestamp, t->action_type, t->user, t->product_name,
               t->category, t->discount_code, t->price, t->quantity, t->total);
    }
}
