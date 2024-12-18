#ifndef DATABASE_H
#define DATABASE_H

#include <stdio.h>
#include <string.h>

#define MAX_PRODUCT_NAME 50
#define MAX_DESCRIPTION 100
#define MAX_CATEGORY 50
#define MAX_PRODUCT_ID 10
#define MAX_DATE 20
#define MAX_PRODUCTS 1000
#define MAX_TRANSACTION_ID 10
#define MAX_CUSTOMER_ID 10
#define MAX_TRANSACTIONS 1000

// Product structure
typedef struct {
    char productID[MAX_PRODUCT_ID];
    char productName[MAX_PRODUCT_NAME];
    char description[MAX_DESCRIPTION];
    char category[MAX_CATEGORY];
    double price; // Changed to double for better precision
    int stockQuantity;
    int minimumThreshold;
    int restockAmount;
    char lastUpdated[MAX_DATE];
} Product;

// Transaction structure
typedef struct {
    char transaction_id[MAX_TRANSACTION_ID];
    char timestamp[MAX_DATE];   // e.g., "12/8/2024 10:45"
    char action_type[30];       // e.g., "Purchase Completed"
    char user[20];              // e.g., "Customer"
    char product_name[MAX_PRODUCT_NAME];
    char category[MAX_CATEGORY];
    char discount_code[20];     // e.g., "STRAIGHTA5" or "-"
    double price;               // Price per unit
    int quantity;               // Quantity purchased
    double total;               // Total price = price * quantity
} Transaction;

// Product-related functions
int loadDatabase(const char *filename, Product products[], int *productCount);
int saveDatabase(const char *filename, Product products[], int productCount);

// Transaction-related functions
void addTransaction(Transaction transactions[], int *transactionCount, Transaction newTransaction);
void displayTransactions(const Transaction transactions[], int transactionCount);

#endif // DATABASE_H
