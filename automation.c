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

// Global variables
Product products[MAX_PRODUCTS];
int productCount = 0;
int restockDay = 0; // Variable to store the user-selected restock day (0 = no schedule)

// Function declarations
void loadProducts();
void saveProducts();
void executeRestockScript();
void autoRestock();
void schedulePurchase();
void checkScheduledRestock();
void autoPurchase(char *productID, int quantity);

// Function that loads product data from the CSV file
void loadProducts() {
    FILE *file = fopen("product.csv", "r");
    if (!file) {
        perror("Failed to open product.csv");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    fgets(line, MAX_LINE_LENGTH, file); // Skip header line

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

// Function that saves product data to the CSV file
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

// Function that performs auto-restock
void autoRestock() {
    printf("Starting auto-restock\n");
    for (int i = 0; i < productCount; i++) {
        if (products[i].stockQuantity < products[i].minimumThreshold) {
            printf("Restocking %s. Current stock: %d, Threshold: %d\n",
                   products[i].productName, products[i].stockQuantity, products[i].minimumThreshold);
            products[i].stockQuantity += products[i].restockAmount;

            // Update last updated date
            time_t now = time(NULL);
            struct tm *t = localtime(&now);
            strftime(products[i].lastUpdated, sizeof(products[i].lastUpdated), "%Y-%m-%d", t);
        }
    }
    saveProducts();
    printf("Auto-restock completed.\n");
}

// Function to schedule a restock day
void schedulePurchase() {
    printf("Select the day of the week to auto-restock (1=Monday, 7=Sunday, 0=Disable): ");
    scanf("%d", &restockDay);

    if (restockDay < 0 || restockDay > 7) {
        printf("Invalid day. Schedule reset.\n");
        restockDay = 0;
    } else if (restockDay == 0) {
        printf("Auto-restock scheduling disabled.\n");
    } else {
        printf("Auto-restock scheduled for day %d of the week.\n", restockDay);
    }
}

// Function to check if auto-restock is scheduled for the current day
void checkScheduledRestock() {
    if (restockDay == 0) return; // No schedule set

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    int currentDay = t->tm_wday == 0 ? 7 : t->tm_wday; // Adjust Sunday as 7

    if (currentDay == restockDay) {
        printf("\nScheduled auto-restock triggered for today.\n");
        loadProducts();
        autoRestock();
    } else {
        printf("\nNo scheduled restock for today. Scheduled for day %d.\n", restockDay);
    }
}

// Function to process a purchase
void autoPurchase(char *productID, int quantity) {
    for (int i = 0; i < productCount; i++) {
        if (strcmp(products[i].productID, productID) == 0) {
            if (products[i].stockQuantity >= quantity) {
                products[i].stockQuantity -= quantity;
                printf("Purchased %d of %s. Remaining stock: %d\n",
                       quantity, products[i].productName, products[i].stockQuantity);

                // Update last updated date
                time_t now = time(NULL);
                struct tm *t = localtime(&now);
                strftime(products[i].lastUpdated, sizeof(products[i].lastUpdated), "%Y-%m-%d", t);

                saveProducts();
                return;
            } else {
                printf("Insufficient stock for %s. Available: %d\n",
                       products[i].productName, products[i].stockQuantity);
                return;
            }
        }
    }
    printf("Product ID %s not found.\n", productID);
}
