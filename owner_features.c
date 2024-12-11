// Muhammad Asad Aziz 67070503472
// This script is for programming all owner features

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// include other necessary libraries including from other scripts


void addProduct();
void viewProduct();
void editProduct();
void deleteProduct();
void restockProduct();
void logAction();
void createCoupon();
void validateCoupon();
void applyCoupon();

// Helper functions
void getLastProductID();

// Function that creates new product | Change return type to appropriate type
void addProduct()
{
    // Open the file in append mode
    FILE *file = fopen("products.csv", "a"); 
    
    // Check if file exists
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    // Variables
    char productName[50], description[100], category[50]; // The numbers means max word size
    int stockQuantity, minimumThreshold, restockAmount;
    float price;
    char lastID[10];
    getLastProductID(lastID);

    // Increment the Product ID
    int numericID = atoi(lastID + 1); // Convert the numeric part to an integer
    numericID++; // Increment
    char productID[10];
    sprintf(productID, "P%03d", numericID); // Format back to PXXX

    // Get the current date and time
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char lastUpdated[20];
    sprintf(lastUpdated, "%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    // Prompt the user for product details
    printf("Enter Product Name: ");
    fgets(productName, sizeof(productName), stdin);
    productName[strcspn(productName, "\n")] = 0; // Remove newline character

    printf("Enter Description: ");
    fgets(description, sizeof(description), stdin);
    description[strcspn(description, "\n")] = 0;

    printf("Enter Category: ");
    scanf("%s", category);

    printf("Enter Price: ");
    scanf("%f", &price);

    printf("Enter Stock Quantity: ");
    scanf("%d", &stockQuantity);

    printf("Enter Minimum Threshold: ");
    scanf("%d", &minimumThreshold);

    printf("Enter Restock Amount: ");
    scanf("%d", &restockAmount);

    // Write the new product details to the file
    fprintf(file, "%s,%s,%s,%s,%.2f,%d,%d,%d,%s\n", productID, productName, description, category, price, stockQuantity, minimumThreshold, restockAmount, lastUpdated);

    fclose(file); // Close the file
    printf("Product added successfully.\n"); // Give feedback
}

// Function that views a products information | Change return type to appropriate type
void viewProduct(){
    // Code here
}

// Function that edits a products information | Change return type to appropriate type
void editProduct(){
    // Code here
}

// Function that deletes a product | Change return type to appropriate type
void deleteProduct(){
    // Code here
}

// Function that restocks a product | Change return type to appropriate type
void restockProduct(){
    // Code here
}

// Function that logs action | Change return type to appropriate type
void logAction(){
    // Code here
}

// Function that creates coupon | Change return type to appropriate type
void createCoupon(){
    // Code here
}

// Function that validates coupon when customer uses a coupon | Change return type to appropriate type
void validateCoupon(){
    // Code here
}

// Function that applies coupon when validated | Change return type to appropriate type
void applyCoupon(){
    // Code here
}

// Helper functions
void getLastProductID(char *lastID)
{
    // Open the file in read mode
    FILE *file = fopen("products.csv", "r");

    // Check if the file exists
    if (file == NULL)
    {
        printf("Error opening file!\n");
        strcpy(lastID, "P000"); // Default start if file is empty
        return;
    }

    // Variable, an array of size 1024
    char line[1024];
    
    // Loop through the file line by line
    // fgets argument 1, line. This is the buffer array to store whatever is read.
    // fgets argument 2, sizeof line, which is 1024. it has to be the same or less than the size of the array, otherwise there would be an overflow.
    // fgets argument 3, file, input source is the file.
    while (fgets(line, sizeof(line), file))
    {
        // sscanf can read files
        sscanf(line, "%[^,]", lastID); // Read the Product ID from the line
    }

    fclose(file);
}

// Test
int main()
{
    addProduct();
    return 0;
}