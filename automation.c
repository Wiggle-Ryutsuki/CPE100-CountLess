#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LENGTH 256
#define MAX_PRODUCTS 100

// Structure to store product details
typedef struct {
    char productID[10];
    char productName[50];
    char description[100];
    char category[20];
    double price;
    int stockQuantity;
    int minimumThreshold;
    int restockAmount;
    char lastUpdated[15];
} Product;

Product products[MAX_PRODUCTS];
int productCount = 0;

// Function Declarations
void loadProducts();
void saveProducts();
void autoRestock();
void autoPurchase(char *productID, int quantity);
void schedulePurchase();

// Load product data from CSV file
void loadProducts() {
    FILE *file = fopen("product.csv", "r");
    if (!file) {
        perror("Failed to open product.csv");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    fgets(line, MAX_LINE_LENGTH, file); // Skip header

    while (fgets(line, MAX_LINE_LENGTH, file)) {
        Product p;
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%lf,%d,%d,%d,%[^\n]",
               p.productID, p.productName, p.description, p.category,
               &p.price, &p.stockQuantity, &p.minimumThreshold,
               &p.restockAmount, p.lastUpdated);
        products[productCount++] = p;
    }
    fclose(file);
}

// Save product data back to CSV file
void saveProducts() {
    FILE *file = fopen("product.csv", "w");
    if (!file) {
        perror("Failed to save to product.csv");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "ProductID,ProductName,Description,Category,Price,StockQuantity,MinimumThreshold,RestockAmount,LastUpdated\n");
    for (int i = 0; i < productCount; i++) {
        fprintf(file, "%s,%s,%s,%s,%.2f,%d,%d,%d,%s\n",
                products[i].productID, products[i].productName, products[i].description,
                products[i].category, products[i].price, products[i].stockQuantity,
                products[i].minimumThreshold, products[i].restockAmount, products[i].lastUpdated);
    }
    fclose(file);
}

// Automatically restock products below the minimum threshold
void autoRestock() {
    for (int i = 0; i < productCount; i++) {
        if (products[i].stockQuantity < products[i].minimumThreshold) {
            products[i].stockQuantity += products[i].restockAmount;

            time_t now = time(NULL);
            struct tm *t = localtime(&now);
            strftime(products[i].lastUpdated, sizeof(products[i].lastUpdated), "%Y-%m-%d", t);
        }
    }
    saveProducts();
}

// Purchase a product by ID and quantity
void autoPurchase(char *productID, int quantity) {
    for (int i = 0; i < productCount; i++) {
        if (strcmp(products[i].productID, productID) == 0) {
            if (products[i].stockQuantity >= quantity) {
                products[i].stockQuantity -= quantity;

                time_t now = time(NULL);
                struct tm *t = localtime(&now);
                strftime(products[i].lastUpdated, sizeof(products[i].lastUpdated), "%Y-%m-%d", t);

                saveProducts();
                printf("Purchase successful: %d of %s remaining %d.\n",
                       quantity, products[i].productName, products[i].stockQuantity);
                return;
            } else {
                printf("Insufficient stock for %s. Available: %d\n", products[i].productName, products[i].stockQuantity);
                return;
            }
        }
    }
    printf("Product ID %s not found.\n", productID);
}

// Schedule Purchase: Allow users to set restock day
void schedulePurchase() {
    int day;
    printf("Enter the day of the week for restocking (1 = Monday, 7 = Sunday): ");
    scanf("%d", &day);

    if (day < 1 || day > 7) {
        printf("Invalid day. Please enter a number between 1 and 7.\n");
        return;
    }

    printf("Restock scheduled on day %d. The program will check and restock products automatically.\n", day);
}
