#ifndef DATABASE_H
#define DATABASE_H

#include <stdio.h>

#define MAX_PRODUCT_NAME 50
#define MAX_DESCRIPTION 100
#define MAX_CATEGORY 50
#define MAX_PRODUCT_ID 10
#define MAX_DATE 20
#define MAX_PRODUCTS 1000

typedef struct {
    char product_id[MAX_PRODUCT_ID];
    char name[MAX_PRODUCT_NAME];
    char description[MAX_DESCRIPTION];
    char category[MAX_CATEGORY];
    float price;
    int stock_quantity;
    int minimum_threshold;
    int restock_amount;
    char last_updated[MAX_DATE];
} Product;

int loadDatabase(const char *filename, Product products[], int *productCount);
int saveDatabase(const char *filename, Product products[], int productCount);
int addProduct(Product products[], int *productCount, Product newProduct);
int updateProduct(Product products[], int productCount, const char *productId, Product updatedProduct);

#endif // DATABASE_H
