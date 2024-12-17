#include "database.h"
#include <string.h>
#include <stdlib.h>

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
        sscanf(buffer, "%9s,%49[^,],%99[^,],%49[^,],%f,%d,%d,%d,%19[^\n]",
               p->product_id, p->name, p->description, p->category,
               &p->price, &p->stock_quantity, &p->minimum_threshold,
               &p->restock_amount, p->last_updated);
        (*productCount)++;
    }

    fclose(file);
    return 0;
}

int saveDatabase(const char *filename, Product products[], int productCount) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file");
        return -1;
    }

    for (int i = 0; i < productCount; i++) {
        Product *p = &products[i];
        fprintf(file, "%s,%s,%s,%s,%.2f,%d,%d,%d,%s\n",
                p->product_id, p->name, p->description, p->category,
                p->price, p->stock_quantity, p->minimum_threshold,
                p->restock_amount, p->last_updated);
    }

    fclose(file);
    return 0;
}

int addProduct(Product products[], int *productCount, Product newProduct) {
    if (*productCount >= MAX_PRODUCTS) return -1;
    products[(*productCount)++] = newProduct;
    return 0;
}

int updateProduct(Product products[], int productCount, const char *productId, Product updatedProduct) {
    for (int i = 0; i < productCount; i++) {
        if (strcmp(products[i].product_id, productId) == 0) {
            products[i] = updatedProduct;
            return 0;
        }
    }
    return -1;
}
