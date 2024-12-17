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
void logAction(const char *actionType, const char *productName, const char *category, const char *discountCode, float price, int stockQuantity);
void createCoupon();
void validateCoupon();
void applyCoupon();

// Helper functions
void getLastProductID();
int isValidDate();
int isLeapYear();

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

    // Log the action
    logAction("Product Added", productName, category, "-", price, stockQuantity);
}

// Function that views a products information | Change return type to appropriate type
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
        // The first argument for strtok is a pointer, the second argument are delimiters, basically characters to look for to make a split in a string, in this case it's a comma
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
    sprintf(lastUpdated, "%02d-%02d-%02d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

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

// Function that deletes a product | Change return type to appropriate type
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
    for (int i = 1; i < lineCount; i++)
    {
        printf("%s", lines[i]); // Display each product
    }

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

// Function that restocks a product | Change return type to appropriate type
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
    char productID[10], productName[50], description[100], category[50], lastUpdated[20];
    int stockQuantity, minimumThreshold;
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

    // Ask user for the restock amount
    int restockAmount;
    printf("Enter the amount to restock: ");
    scanf("%d", &restockAmount);

    // Update stock quantity and last updated date
    stockQuantity += restockAmount;

    // Get the current date and time
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(lastUpdated, "%02d-%02d-%02d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    // Update the selected line without changing the RestockAmount
    if (strchr(description, ',') != NULL)
    { // Check if the description contains a comma
        sprintf(lines[choice], "%s,%s,\"%s\",%s,%.2f,%d,%d,%s\n",
                productID, productName, description, category, price,
                stockQuantity, minimumThreshold, lastUpdated);
    }
    else
    {
        sprintf(lines[choice], "%s,%s,%s,%s,%.2f,%d,%d,%s\n",
                productID, productName, description, category, price,
                stockQuantity, minimumThreshold, lastUpdated);
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
    logAction("Product Restocked", productName, category, "-", price, stockQuantity);
}

// Function that logs action | Change return type to appropriate type
// Function to log actions to logs.csv
void logAction(const char *actionType, const char *productName, const char *category, const char *discountCode, float price, int stockQuantity)
{
    // Open the logs.csv file in append mode
    FILE *file = fopen("logs.csv", "a");
    if (file == NULL)
    {
        printf("Error opening logs file!\n");
        return;
    }

    // Get the current date and time
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char timestamp[20];
    sprintf(timestamp, "%02d/%02d/%d %02d:%02d", tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900, tm.tm_hour, tm.tm_min);

    // Write the log entry to the file
    /*
    Examples:
    logAction("Product Updated", "Pen", "Stationery", "-", 5.00, 100);
    logAction("Product Restocked", "Sketchbook", "Art Supplies", "-", 20.00, 50);
    logAction("Purchase Completed", "Textbook", "Educational", "STRAIGHTA5", 300.00, 64);
    logAction("Discount Created", "-", "Electronics", "FALL20", 0.00, 0);

    */
    fprintf(file, "%s,%s,Owner,%s,%s,%s,%.2f,%d\n", timestamp, actionType, productName, category, discountCode, price, stockQuantity);

    // Close the file
    fclose(file);
    //printf("Action logged successfully.\n");
}

// Function that creates coupon | Change return type to appropriate type
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

    // Prompt the user for coupon details
    printf("Enter Coupon Name: ");
    fgets(couponName, sizeof(couponName), stdin);
    couponName[strcspn(couponName, "\n")] = 0; // Remove newline character

    printf("Enter Amount of Discount: ");
    scanf("%d", &amountOfDiscount);
    getchar(); // Consume newline character left by scanf

    // Prompt the user to enter a choice for discount type
    printf("Enter Discount Type (1 for Percentage, 2 for Flat Value): ");
    scanf("%d", &discountChoice);
    getchar(); // Consume newline character left by scanf

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
        printf("Enter Expiry Date (YYYY-MM-DD): ");
        fgets(expiryDate, sizeof(expiryDate), stdin);
        expiryDate[strcspn(expiryDate, "\n")] = 0; // Remove newline character

        if (!isValidDate(expiryDate))
        {
            printf("Invalid date format! Please enter a valid date.\n");
        }
    } while (!isValidDate(expiryDate));

    printf("Enter Description: ");
    fgets(description, sizeof(description), stdin);
    description[strcspn(description, "\n")] = 0; // Remove newline character

    printf("Enter Minimum Total (Baht): ");
    scanf("%d", &minTotal);
    getchar(); // Consume newline character left by scanf

    // Write the new coupon details to the file
    fprintf(file, "%s,%d,%s,%s,%s,%d\n", couponName, amountOfDiscount, discountType, expiryDate, description, minTotal);

    // Close the file
    fclose(file);

    printf("Coupon added successfully.\n");
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

// Function to check if a date is valid
// Function to check if a date is valid
int isValidDate(const char *date)
{
    int year, month, day;
    if (sscanf(date, "%d-%d-%d", &year, &month, &day) != 3)
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

// Test
int main(void)
{
    createCoupon();
}