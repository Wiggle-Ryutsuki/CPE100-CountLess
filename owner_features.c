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
    // Open the file in read mode
    FILE *file = fopen("products2.csv", "r");

    // Check if file exists
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;

    }

    // Create an array of size 1024 (to read lines)
    char line[1024];

    // Skip the header line
    fgets(line, sizeof(line), file);

    // Print Headers for better readability
    printf("%-10s %-25s %-45s %-15s %-10s %-10s %-10s %-10s %s\n", "ID", "Name", "Description", "Category", "Price", "Stock", "MinThresh", "Restock", "LastUpdated");
    printf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    // Read and print each line from the file
    while (fgets(line, sizeof(line), file))
    {
        // Variables
        char productID[10], productName[50], description[100], category[50], lastUpdated[20];
        int stockQuantity, minimumThreshold, restockAmount;
        float price;

        // Use strtok to handle fields
        // strtok is a function that can split strings into multiple pieces called tokens
        // The first argument for strtok is a pointer, the second argument are delimiters, basically character to look for to make a split in a string, in this case it's a comma
        char *token = strtok(line, ",");
        strcpy(productID, token); //productionID is now whatever "token" was pointing at, "token" was pointing at whatever was before the first comma.

        token = strtok(NULL, ","); // We input NULL as the first argument to get more pieces of the same string, basically whatever is after the first comma and before the second comma.
        strcpy(productName, token); // productName is now whatever "token" was pointing at, "token" was pointing at the second peice of the string

        // The description can contain commas that are NOT supposed to be seperators. This is here to fix any issues relating to commas
        // Check if the description is quoted ""
        token = strtok(NULL, ","); // Get next peice
        if (token[0] == '"') {
            // Handle quoted description
            strcpy(description, token + 1); // Skip the opening quote
            while (token[strlen(token) - 1] != '"') {
                token = strtok(NULL, ",");
                strcat(description, ",");
                strcat(description, token);
            }
            description[strlen(description) - 1] = '\0'; // Remove the closing quote

            // Check for double quotes at the end and replace with a single quote. Because descriptions can be like this: "Pleated Black Skirt, 38"""
            size_t len = strlen(description);
            if (len >= 2 && strcmp(&description[len - 2], "\"\"") == 0)
            {
                description[len - 2] = '"';
                description[len - 1] = '\0';
            }
        } 
        // Descriptions with no commas
        else {
            strcpy(description, token);
        }

        token = strtok(NULL, ","); // Get next peice
        strcpy(category, token);   // category is now whatever "token" was pointing at, "token" was pointing at the fourth peice of the string

        token = strtok(NULL, ","); // Get next peice
        price = atof(token);       // price is a float, so we need to convert it to ASCII

        token = strtok(NULL, ","); // Get next peice
        stockQuantity = atoi(token); // stockQuantity is an int, so we need to convert it to ASCII

        token = strtok(NULL, ","); // Get next peice
        minimumThreshold = atoi(token); // minimumThreshold is an int, so we need to convert it to ASCII

        token = strtok(NULL, ","); // Get next peice
        restockAmount = atoi(token); // restockAmount is an int, so we need to convert it to ASCII

        token = strtok(NULL, ","); // Get next peice
        strcpy(lastUpdated, token); // lastUpdated is now whatever "token" was pointing at, "token" was pointing at the last peice of the string

        // Print the product details in a formatted way
        printf("%-10s %-25s %-45s %-15s %-10.2f %-10d %-10d %-10d %s\n", productID, productName, description, category, price, stockQuantity, minimumThreshold, restockAmount, lastUpdated);
    }

    fclose(file); // Close the file
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
int main(void)
{
    viewProduct();
}