// Muhammad Asad Aziz 67070503472
// This script is for programming all owner features

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// include other necessary libraries including from other scripts

// Struct for Product Information
typedef struct {
    char productID[10];
    char productName[50];
    char description[100];
    char category[50];
    float price;
    int stockQuantity;
    int minimumThreshold;
    int restockAmount;
    char lastUpdated[20];
} Product;

// Struct for Coupon Information
typedef struct {
    char code[20];
    int discountAmount;
    char discountType[20];
    char expiryDate[11];
    char description[100];
    int minTotal;
} Coupon;



void addProduct();
void viewProduct();
void editProduct();
void deleteProduct();
void restockProduct();
void logAction(const char *actionType, const char *productName, const char *category, const char *discountCode, float price, int stockQuantity);
void createCoupon();
void validateCoupon();
void applyCoupon();

// Helper functions
void getLastProductID();
int isValidDate();
int isLeapYear();
int isCouponExpired();
void parseProductLine(char *line, Product *product);
void printProduct(const Product *product);
void updateProductFile(Product *products, int count);

// Function to add a new product
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
    Product newProduct;
    char lastID[10];
    getLastProductID(lastID);

    // Increment the Product ID
    int numericID = atoi(lastID + 1);
    numericID++;
    sprintf(newProduct.productID, "P%03d", numericID);

    // Get the current date and time
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(newProduct.lastUpdated, "%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    // Prompt the user for product details
    printf("Enter Product Name: ");
    fgets(newProduct.productName, sizeof(newProduct.productName), stdin);
    newProduct.productName[strcspn(newProduct.productName, "\n")] = 0;

    printf("Enter Description: ");
    fgets(newProduct.description, sizeof(newProduct.description), stdin);
    newProduct.description[strcspn(newProduct.description, "\n")] = 0;

    // Check if the description contains a comma and enclose it in quotes if necessary
    char formattedDescription[200];
    if (strchr(newProduct.description, ',') != NULL) {
        sprintf(formattedDescription, "\"%s\"", newProduct.description);
    } else {
        strcpy(formattedDescription, newProduct.description);
    }

    printf("Enter Category: ");
    scanf("%s", newProduct.category);

    printf("Enter Price: ");
    scanf("%f", &newProduct.price);

    printf("Enter Stock Quantity: ");
    scanf("%d", &newProduct.stockQuantity);

    printf("Enter Minimum Threshold: ");
    scanf("%d", &newProduct.minimumThreshold);

    printf("Enter Restock Amount: ");
    scanf("%d", &newProduct.restockAmount);

    // Write the new product details to the file
    fprintf(file, "\n%s,%s,%s,%s,%.2f,%d,%d,%d,%s", newProduct.productID, newProduct.productName, formattedDescription, newProduct.category, newProduct.price, newProduct.stockQuantity, newProduct.minimumThreshold, newProduct.restockAmount, newProduct.lastUpdated);

    fclose(file); // Close the file
    printf("Product added successfully.\n"); // Give feedback

    // Log the action
    logAction("Product Added", newProduct.productName, newProduct.category, "-", newProduct.price, newProduct.stockQuantity);
}

// Function to view all products
void viewProduct(){
    // Open the file in read mode
    FILE *file = fopen("products.csv", "r");

    // Check if file exists
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    // Create an array of size 1024 (to read lines)
    char line[1024];

    // Skip the header line
    fgets(line, sizeof(line), file); // Skip header

    // Print Headers for better readability
    printf("%-10s %-25s %-45s %-15s %-10s %-10s %-10s %-10s %s\n", "ID", "Name", "Description", "Category", "Price", "Stock", "MinThresh", "Restock", "LastUpdated");
    printf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    // Read and print each line from the file
    while (fgets(line, sizeof(line), file)) {
        Product product;
        parseProductLine(line, &product);
        printProduct(&product);
    }

    fclose(file); // Close the file

    // Log the action (Privacy invasion tho)
    logAction("Product Viewed", "-", "-", "-", 0.0, 0);
}

// Function that edits a products information | Change return type to appropriate type
void editProduct()
{
    // Open the file in read mode
    FILE *file = fopen("products.csv", "r");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    // Temporary storage for product data
    char lines[1000][1024];
    int lineCount = 0;

    // Read all lines from the file
    while (fgets(lines[lineCount], sizeof(lines[lineCount]), file))
    {
        lineCount++;
    }
    fclose(file);

    // Display products for selection
    printf("Select a product to edit by entering the Product ID:\n");
    viewProduct();

    // Get user selection
    char selectedProductID[10];
    printf("Enter the Product ID of the product you want to edit: ");
    fgets(selectedProductID, sizeof(selectedProductID), stdin);
    selectedProductID[strcspn(selectedProductID, "\n")] = 0; // Remove newline character

    // Find the product by ID
    int choice = -1;
    for (int i = 1; i < lineCount; i++)
    {
        char currentProductID[10];
        sscanf(lines[i], "%[^,]", currentProductID);
        if (strcmp(currentProductID, selectedProductID) == 0)
        {
            choice = i;
            break;
        }
    }

    if (choice == -1)
    {
        printf("Product ID not found.\n");
        return;
    }

    // Parse the selected line
    char productID[10], productName[50], description[100], category[50], lastUpdated[20];
    int stockQuantity, minimumThreshold, restockAmount;
    float price;

    // Handle descriptions with commas
    char *token = strtok(lines[choice], ",");
    strcpy(productID, token);

    token = strtok(NULL, ",");
    strcpy(productName, token);

    token = strtok(NULL, ",");
    if (token[0] == '"')
    {
        strcpy(description, token + 1);
        while (token[strlen(token) - 1] != '"')
        {
            token = strtok(NULL, ",");
            strcat(description, ",");
            strcat(description, token);
        }
        description[strlen(description) - 1] = '\0'; // Remove the closing quote
    }
    else
    {
        strcpy(description, token);
    }

    token = strtok(NULL, ",");
    strcpy(category, token);

    token = strtok(NULL, ",");
    price = atof(token);

    token = strtok(NULL, ",");
    stockQuantity = atoi(token);

    token = strtok(NULL, ",");
    minimumThreshold = atoi(token);

    token = strtok(NULL, ",");
    restockAmount = atoi(token);

    token = strtok(NULL, ",");
    strcpy(lastUpdated, token);

    // Prompt user for new details
    char input[100];

    printf("Enter new Product Name (current: %s) or press Enter to keep: ", productName);
    fgets(input, sizeof(input), stdin);
    if (input[0] != '\n')
    {
        strcpy(productName, input);
        productName[strcspn(productName, "\n")] = 0; // Remove newline character
    }

    printf("Enter new Description (current: %s) or press Enter to keep: ", description);
    fgets(input, sizeof(input), stdin);
    if (input[0] != '\n')
    {
        strcpy(description, input);
        description[strcspn(description, "\n")] = 0;
    }

    printf("Enter new Category (current: %s) or press Enter to keep: ", category);
    fgets(input, sizeof(input), stdin);
    if (input[0] != '\n')
    {
        strcpy(category, input);
        category[strcspn(category, "\n")] = 0;
    }

    printf("Enter new Price (current: %.2f) or press Enter to keep: ", price);
    fgets(input, sizeof(input), stdin);
    if (input[0] != '\n')
    {
        sscanf(input, "%f", &price);
    }

    printf("Enter new Stock Quantity (current: %d) or press Enter to keep: ", stockQuantity);
    fgets(input, sizeof(input), stdin);
    if (input[0] != '\n')
    {
        sscanf(input, "%d", &stockQuantity);
    }

    printf("Enter new Minimum Threshold (current: %d) or press Enter to keep: ", minimumThreshold);
    fgets(input, sizeof(input), stdin);
    if (input[0] != '\n')
    {
        sscanf(input, "%d", &minimumThreshold);
    }

    printf("Enter new Restock Amount (current: %d) or press Enter to keep: ", restockAmount);
    fgets(input, sizeof(input), stdin);
    if (input[0] != '\n')
    {
        sscanf(input, "%d", &restockAmount);
    }

    // Update the last updated time
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(lastUpdated, "%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    // Update the selected line
    if (strchr(description, ',') != NULL)
    { // Check if the description contains a comma
        sprintf(lines[choice], "%s,%s,\"%s\",%s,%.2f,%d,%d,%d,%s\n",
                productID, productName, description, category, price,
                stockQuantity, minimumThreshold, restockAmount, lastUpdated);
    }
    else
    {
        sprintf(lines[choice], "%s,%s,%s,%s,%.2f,%d,%d,%d,%s\n",
                productID, productName, description, category, price,
                stockQuantity, minimumThreshold, restockAmount, lastUpdated);
    }
    // Write the updated data back to the file
    file = fopen("products.csv", "w");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }
    for (int i = 0; i < lineCount; i++)
    {
        fputs(lines[i], file);
    }
    fclose(file);

    printf("Product updated successfully.\n");

    // Log the action
    logAction("Product Edited", productName, category, "-", price, stockQuantity);
}

// Function to delete a product
void deleteProduct()
{
    // Open the file in read mode
    FILE *file = fopen("products.csv", "r");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    // Temporary storage for product data
    char lines[1000][1024];
    int lineCount = 0;

    // Read all lines from the file
    while (fgets(lines[lineCount], sizeof(lines[lineCount]), file))
    {
        lineCount++;
    }
    fclose(file);

    // Display products for selection
    printf("Select a product to delete by entering the Product ID:\n");
    viewProduct();

    // Get user selection
    char input[15]; // Increased size to accommodate extra character
    printf("\nEnter the Product ID of the product you want to delete: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0; // Remove newline character

    // Check for force delete character
    int forceDelete = 0;
    char selectedProductID[10];
    if (strstr(input, " F") != NULL)
    { // Check if " F" is in the input
        sscanf(input, "%s", selectedProductID);
        forceDelete = 1;
    }
    else
    {
        sscanf(input, "%s", selectedProductID);
    }

    // Find the product by ID
    int choice = -1;
    char productName[50], category[50];
    for (int i = 1; i < lineCount; i++)
    {
        char currentProductID[10];
        sscanf(lines[i], "%[^,]", currentProductID);
        if (strcmp(currentProductID, selectedProductID) == 0)
        {
            choice = i;

            // Extract productName and category
            char *token = strtok(lines[i], ",");
            token = strtok(NULL, ",");  // Skip productID
            strcpy(productName, token); // Get productName
            token = strtok(NULL, ",");  // Skip description
            token = strtok(NULL, ",");  // Get category
            strcpy(category, token);

            break;
        }
    }

    if (choice == -1)
    {
        printf("Product ID not found.\n");
        return;
    }

    // Confirmation step
    if (!forceDelete)
    {
        char confirmation[4];
        printf("Are you sure you want to delete this product? (yes/no): ");
        fgets(confirmation, sizeof(confirmation), stdin);
        confirmation[strcspn(confirmation, "\n")] = 0; // Remove newline character

        if (strcmp(confirmation, "yes") != 0)
        {
            printf("Deletion cancelled.\n");
            return;
        }
    }

    // Write the updated data back to the file, excluding the deleted product
    file = fopen("products.csv", "w");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    // Write the header
    fputs(lines[0], file);

    // Write all lines except the one to be deleted
    for (int i = 1; i < lineCount; i++)
    {
        if (i != choice)
        {
            fputs(lines[i], file);
        }
    }
    fclose(file);

    printf("Product deleted successfully.\n");

    // Log the action
    logAction("Product Deleted", productName, category, "-", 0.0, 0);
}

// Function to restock a product
void restockProduct()
{
    // Open the file in read mode
    FILE *file = fopen("products.csv", "r");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    // Temporary storage for product data
    char lines[1000][1024];
    int lineCount = 0;

    // Read all lines from the file
    while (fgets(lines[lineCount], sizeof(lines[lineCount]), file))
    {
        lineCount++;
    }
    fclose(file);

    // Display products for selection
    printf("Select a product to restock by entering the Product ID:\n");
    for (int i = 1; i < lineCount; i++)
    {
        printf("%s", lines[i]); // Display each product
    }

    // Get user selection
    char selectedProductID[10];
    printf("\nEnter the Product ID of the product you want to restock: ");
    fgets(selectedProductID, sizeof(selectedProductID), stdin);
    selectedProductID[strcspn(selectedProductID, "\n")] = 0; // Remove newline character

    // Find the product by ID
    int choice = -1;
    for (int i = 1; i < lineCount; i++)
    {
        char currentProductID[10];
        sscanf(lines[i], "%[^,]", currentProductID);
        if (strcmp(currentProductID, selectedProductID) == 0)
        {
            choice = i;
            break;
        }
    }

    if (choice == -1)
    {
        printf("Product ID not found.\n");
        return;
    }

    // Parse the selected line
    Product product;
    parseProductLine(lines[choice], &product);

    // Ask user for the restock amount
    int restockAmount;
    printf("Enter the amount to restock: ");
    scanf("%d", &restockAmount);
    getchar(); // Consume leftover newline character

    // Update stock quantity and last updated date
    product.stockQuantity += restockAmount;
    product.restockAmount = restockAmount; // Update the restock amount field

    // Get the current date and time
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(product.lastUpdated, "%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);


    // Update the selected line
    if (strchr(product.description, ',') != NULL)
    { // Check if the description contains a comma
        sprintf(lines[choice], "%s,%s,\"%s\",%s,%.2f,%d,%d,%d,%s\n",
                product.productID, product.productName, product.description, product.category,
                product.price, product.stockQuantity, product.minimumThreshold, product.restockAmount, product.lastUpdated);
    }
    else
    {
        sprintf(lines[choice], "%s,%s,%s,%s,%.2f,%d,%d,%d,%s\n",
                product.productID, product.productName, product.description, product.category,
                product.price, product.stockQuantity, product.minimumThreshold, product.restockAmount, product.lastUpdated);
    }

    // Write the updated data back to the file
    file = fopen("products.csv", "w");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }
    for (int i = 0; i < lineCount; i++)
    {
        fputs(lines[i], file);
    }
    fclose(file);

    printf("Product restocked successfully.\n");

    // Log the action
    logAction("Product Restocked", product.productName, product.category, "-", product.price, product.stockQuantity);
}


// Function to log actions
void logAction(const char *actionType, const char *productName, const char *category, const char *discountCode, float price, int stockQuantity) {
    FILE *file = fopen("logs.csv", "a");
    if (file == NULL) {
        printf("Error opening logs file!\n");
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char timestamp[20];
    sprintf(timestamp, "%02d/%02d/%d %02d:%02d", tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900, tm.tm_hour, tm.tm_min);

    fprintf(file, "%s,%s,Owner,%s,%s,%s,%.2f,%d\n", timestamp, actionType, productName, category, discountCode, price, stockQuantity);

    fclose(file);
}


// Function to create a coupon
// Function that creates coupon
void createCoupon()
{
    // Open the file in append mode
    FILE *file = fopen("coupons.csv", "a");

    // Check if the file was opened successfully
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    // Variables to store coupon details
    char couponName[20], discountType[20], expiryDate[11], description[100];
    int amountOfDiscount, minTotal, discountChoice;
    char inputBuffer[100]; // Buffer for reading input

    // Prompt the user for coupon details
    printf("Enter Coupon Name: ");
    fgets(couponName, sizeof(couponName), stdin);
    couponName[strcspn(couponName, "\n")] = 0; // Remove newline character

    printf("Enter Amount of Discount: ");
    fgets(inputBuffer, sizeof(inputBuffer), stdin);
    sscanf(inputBuffer, "%d", &amountOfDiscount);

    // Prompt the user to enter a choice for discount type
    printf("Enter Discount Type (1 for Percentage, 2 for Flat Value): ");
    fgets(inputBuffer, sizeof(inputBuffer), stdin);
    sscanf(inputBuffer, "%d", &discountChoice);

    // Assign the discount type based on user choice
    if (discountChoice == 1)
    {
        strcpy(discountType, "Percentage");
    }
    else if (discountChoice == 2)
    {
        strcpy(discountType, "Flat Value");
    }
    else
    {
        printf("Invalid choice! Defaulting to Percentage.\n");
        strcpy(discountType, "Percentage");
    }

    // Prompt the user for a valid expiry date
    do
    {
        // Clear the input buffer to ensure no leftover newline characters
        fflush(stdin);
        
        printf("Enter Expiry Date (YYYY-MM-DD): ");
        fgets(expiryDate, sizeof(expiryDate), stdin);
        expiryDate[strcspn(expiryDate, "\n")] = 0; // Remove newline character

        if (!isValidDate(expiryDate))
        {
            printf("Invalid date format! Please enter a valid date.\n");
        }
    } while (!isValidDate(expiryDate));

    // Clear the input buffer to ensure no leftover newline characters
    fflush(stdin);

    printf("Enter Description: ");
    fgets(description, sizeof(description), stdin);
    description[strcspn(description, "\n")] = 0; // Remove newline character

    printf("Enter Minimum Total (Baht): ");
    fgets(inputBuffer, sizeof(inputBuffer), stdin);
    sscanf(inputBuffer, "%d", &minTotal);

    // Write the new coupon details to the file
    fprintf(file, "%s,%d,%s,%s,%s,%d\n", couponName, amountOfDiscount, discountType, expiryDate, description, minTotal);

    // Close the file
    fclose(file);

    printf("Coupon added successfully.\n");

    // Log the action
    logAction("Coupon Created", "-", "-", couponName, 0.0, 0);
}


// Function to validate coupons
void validateCoupon() {
    FILE *file = fopen("coupons.csv", "r");
    if (file == NULL) {
        printf("Error: Unable to open coupon file.\n");
        return;
    }

    char line[256];
    int isFirstLine = 1;

    while (fgets(line, sizeof(line), file)) {
        if (isFirstLine) {
            isFirstLine = 0;
            continue;
        }

        char name[20], expiryDate[11];
        sscanf(line, "%[^,],%*d,%*[^,],%[^,]", name, expiryDate);

        if (isCouponExpired(expiryDate)) {
            printf("The coupon %s is expired.\n", name);
        } else {
            printf("The coupon %s is valid.\n", name);
        }
    }

    fclose(file);
}

// Helper functions
// Helper function to parse a product line
void parseProductLine(char *line, Product *product) {
    char *token = strtok(line, ",");
    strcpy(product->productID, token);

    token = strtok(NULL, ",");
    strcpy(product->productName, token);

    token = strtok(NULL, ",");
    if (token[0] == '"') {
        strcpy(product->description, token + 1);
        while (token[strlen(token) - 1] != '"') {
            token = strtok(NULL, ",");
            strcat(product->description, ",");
            strcat(product->description, token);
        }
        product->description[strlen(product->description) - 1] = '\0';
    } else {
        strcpy(product->description, token);
    }

    token = strtok(NULL, ",");
    strcpy(product->category, token);

    token = strtok(NULL, ",");
    product->price = atof(token);

    token = strtok(NULL, ",");
    product->stockQuantity = atoi(token);

    token = strtok(NULL, ",");
    product->minimumThreshold = atoi(token);

    token = strtok(NULL, ",");
    product->restockAmount = atoi(token);

    token = strtok(NULL, ",");
    strcpy(product->lastUpdated, token);
}

// Helper function to print a product
void printProduct(const Product *product) {
    printf("%-10s %-25s %-45s %-15s %-10.2f %-10d %-10d %-10d %s\n",
           product->productID, product->productName, product->description,
           product->category, product->price, product->stockQuantity,
           product->minimumThreshold, product->restockAmount, product->lastUpdated);
}

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

// Function to check if a date is valid
int isValidDate(const char *date)
{
    int year, month, day;
    // Use %d for parsing integers, which will handle both single and double-digit numbers
    if (sscanf(date, "%4d-%2d-%2d", &year, &month, &day) != 3)
    {
        return 0; // Invalid format
    }
    // Basic validation of date components
    if (year < 1900 || month < 1 || month > 12 || day < 1)
    {
        return 0; // Invalid date
    }

    // Array with the number of days in each month
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Adjust for leap year
    if (month == 2 && isLeapYear(year))
    {
        daysInMonth[1] = 29;
    }

    // Check if the day is within the valid range for the month
    if (day > daysInMonth[month - 1])
    {
        return 0; // Invalid day for the month
    }

    return 1; // Valid date
}

// Function to check if a year is a leap year
int isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Function to check if a coupon is expired
int isCouponExpired(const char *expiryDate) {
    struct tm expiry = {0};
    time_t now;
    struct tm *current;
    sscanf(expiryDate, "%d-%d-%d", &expiry.tm_year, &expiry.tm_mon, &expiry.tm_mday);
    expiry.tm_year -= 1900;
    expiry.tm_mon -= 1;
    time(&now);
    current = localtime(&now);
    if (difftime(mktime(&expiry), mktime(current)) < 0) {
        return 1; // Expired
    }
    return 0; // Not expired
}

// UI
void displayOwnerMenu() {
    printf("\n--- Owner Features Menu ---\n");
    printf("1. Add Product\n");
    printf("2. View Products\n");
    printf("3. Edit Product\n");
    printf("4. Delete Product\n");
    printf("5. Restock Product\n");
    printf("6. Create Coupon\n");
    printf("7. Validate Coupons\n");
    printf("0. Exit\n");
    printf("---------------------------\n");
}

void handleOwnerSelection(int choice) {
    switch (choice) {
        case 1:
            addProduct();
            break;
        case 2:
            viewProduct();
            break;
        case 3:
            editProduct();
            break;
        case 4:
            deleteProduct();
            break;
        case 5:
            restockProduct();
            break;
        case 6:
            createCoupon();
            break;
        case 7:
            validateCoupon();
            break;
        case 0:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
    }
}

int main(void) {
    int choice;
    do {
        displayOwnerMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline character left by scanf
        handleOwnerSelection(choice);
    } while (choice != 0); // Exit when the user chooses 0

    return 0;
}