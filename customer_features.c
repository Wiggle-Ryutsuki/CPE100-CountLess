// This script is for programming all customer features

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// include other necessary libraries including from other scripts


void searchProduct();
void browseProducts();
void addToCart();
void viewCart();
void checkoutCart();
void updateInventoryAfterPurchase();
void applyCouponAtCheckout();

//test
int main(){
    searchProduct();
}


// Function that searches for a product | Change return type to appropriate type
void searchProduct(){
    //open file for reading
    FILE *file = fopen("products.csv","r");

    //check if file exists
    if (file == NULL){
        printf("Error: Unable to open the file.\n");
        return;
    }

    //variables
    char line[1000]; //for the struct
    char productName[50], category[50];
    float price;
    int choice, i=0, isFirstLine=1;

    //get product database
    struct products
    {
        char productID[10];
        char productname[50];
        char description[100];
        char category[50];
        float price;
        int stockquantity;
    }product [1000];

    while(fgets(line, sizeof(line), file)){
        
        //skip the header
        if (isFirstLine) {
            isFirstLine = 0;
            continue;
        }

        char *token = strtok(line, ",");
        strcpy(product[i].productID, token);

        token = strtok(NULL, ",");
        strcpy(product[i].productname, token);

        token = strtok(NULL, ","); 
        if (token[0] == '"'){
            // Handle quoted description
            strcpy(product[i].description, token + 1); // Skip the opening quote
            
            token = strtok(NULL, ",");
            strcat(product[i].description, ",");
            strcat(product[i].description, token);

            product[i].description[strlen(product[i].description) - 1] = '\0'; // Remove the closing quote

            // Check for double quotes at the end and replace with a single quote. Because descriptions can be like this: "Pleated Black Skirt, 38"""
            size_t len = strlen(product[i].description);
            if (len >= 2 && strcmp(&product[i].description[len - 2], "\"\"") == 0)
            {
                product[i].description[len - 2] = '"';
                product[i].description[len - 1] = '\0';
            }
        }else{
            strcpy(product[i].description, token);
        } 

        token = strtok(NULL, ",");
        strcpy(product[i].category, token);

        token = strtok(NULL, ",");
        product[i].price = atof(token);

        token = strtok(NULL, ",");
        product[i].stockquantity = atoi(token);

        printf("%s %s %s %s %.2f %d\n",product[i].productID, product[i].productname, product[i].description, product[i].category, product[i].price, product[i].stockquantity);
        i++;

    }

    fclose(file);

    //user interface to select search criteria
    printf("Search by name: Enter 1\nSearch by category: Enter 2\nSearch by price: Enter 3\n\nYour selection: ");
    scanf("%d",&choice);
    
    switch (choice)
    {
    case 1:
        //Enter product name
        printf("Enter product name: ");
        scanf("%s",productName);

        //search by name
        i = 0;
        while(i<=30){

            //compare names
            if( strcmp(productName, product[i].productname)==0){

                printf("Product ID: %7s | Product name: %7s | %7s | Price: %.2f Baht | Remaining: %d\n", product[i].productID, product[i].productname, product[i].description, product[i].price, product[i].stockquantity);
           
            }
            i++;

        }
        break;
    
    default:
        break;
    }

}

// Function that calls for all products | Change return type to appropriate type
void browseProducts(){
    // Code here
}

// Function that adds product to shopping cart | Change return type to appropriate type
void addToCart(){
    // Code here
}

// Function that calls for all items in cart | Change return type to appropriate type
void viewCart(){
    // Code here
}

// Function that proceeds to checkout point (calculates total and prompts to use coupon) | Change return type to appropriate type
void checkoutCart(){
    // Code here
}

// Function that updates inventory | Change return type to appropriate type
void updateInventoryAfterPurchase(){
    // Code here
}

// Function that applies coupon | Change return type to appropriate type
void applyCouponAtCheckout(){
    // Code here
}

