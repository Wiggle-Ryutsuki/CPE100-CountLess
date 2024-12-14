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

//For initializing product database
void productInformation();

struct cart
{
    char productID[10];
    char productname[50];
    float price;
    int amount;
}InCart [1000];

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

//test
int main(){

    productInformation();
    searchProduct();
}

void productInformation(){

    FILE *file = fopen("products.csv","r");

    //check if file exists
    if (file == NULL){
        printf("Error: Unable to open the file.\n");
        return;
    }

    char line[1000];
    int i=0, isFirstLine=1;

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

        i++;

    }

    //close the file
    fclose(file);

}


// Function that searches for a product | Change return type to appropriate type
void searchProduct(){

    //variables
    char productName[50], category[50];
    float price;
    int choice, i=0, j=0;

    //user interface to select search criteria
    printf("\nSearch by name: Enter 1\nSearch by category: Enter 2\nSearch by price: Enter 3\n\nYour selection: ");
    scanf("%d",&choice);
    
    switch (choice)
    {
    //search product by name
    case 1:
        //Enter product name
        printf("\nEnter product name: ");
        scanf("\n%[^\n]s",productName);

        //loop through the names
        i = 0;
        while(i<=30){

            //If string lentghs are the same, then it can be the product we are looking for            
            if(strlen(productName)==strlen(product[i].productname)){

                //variable to compare the product name without modifying the actual struct data
                char compareName[50];

                //It takes the name of the current product
                strcpy(compareName, product[i].productname);

                //change entered product name to lower case to make it easier to compare to the other product name
                j=0;
                while(productName[j] != '\0'){

                    if(productName[j] >= 'A' && productName[j] <= 'Z' ){

                        productName[j]+=32;

                    }
                    j++;
                }

                //change current product name to lower case to prepare for the comparison
                j=0;
                while(compareName[j] != '\0'){

                    if(compareName[j] >= 'A' && compareName[j] <= 'Z' ){

                        compareName[j]+=32;

                    }
                    j++;

                }

                //product name comparisons, if it is the same, print out the information
                if(strcmp(productName, compareName)==0){
                    printf("Product ID: %s | Product name: %-30s | %-45s | Price: %-8.2f Baht | Remaining: %d\n", product[i].productID, product[i].productname, product[i].description, product[i].price, product[i].stockquantity);
                }

            }else{

                //If string lentghs are not the same then skip this iteration
                i++;
                continue;
            }

            i++;

        }

        break;
    
    //search product by category
    case 2:
        //Enter product category
        printf("\nEnter product category: ");
        scanf("\n%[^\n]s",category);

        //loop through product list
        i = 0;
        while(i<=30){

            //If string lentghs are the same, then it can be the category we are looking for
            if(strlen(category)==strlen(product[i].category)){

                //variable to coampre category without modifying the actual struct data
                char compareCategory[50];

                //It takes the name of the current product category
                strcpy(compareCategory, product[i].category);

                //change entered product category to lower case to make it easier to compare to the other product name
                j=0;
                while(category[j] != '\0'){

                    if(category[j] >= 'A' && category[j] <= 'Z' ){

                        category[j]+=32;

                    }
                    j++;
                }

                //change current product category to lower case to prepare for the comparison
                j=0;
                while(compareCategory[j] != '\0'){

                    if(compareCategory[j] >= 'A' && compareCategory[j] <= 'Z' ){

                        compareCategory[j]+=32;

                    }
                    j++;

                }

                //category comparisons, if it is the same, print out the information
                if(strcmp(category, compareCategory)==0){
                    printf("Product ID: %s | Product name: %-30s | %-45s | Price: %-8.2f Baht | Remaining: %d\n", product[i].productID, product[i].productname, product[i].description, product[i].price, product[i].stockquantity);
                }

            }else{

                //If string lentghs are not the same then skip this iteration
                i++;
                continue;
            }

            i++;

        }

        break;

    //search product by price
    case 3:

        //Enter product price
        printf("\nEnter product price: ");
        scanf("%f",&price);

        //loop through the product list
        i = 0;
        while(i<=30){

            //check if prices are the same
            if(price==product[i].price){

                //print out product info
                printf("Product ID: %s | Product name: %-30s | %-45s | Price: %-8.2f Baht | Remaining: %d\n", product[i].productID, product[i].productname, product[i].description, product[i].price, product[i].stockquantity);

            }

            i++;

        }

        break;

    default:

        printf("\nPlease enter valid option\n");

        break;
    }

}

// Function that calls for all products | Change return type to appropriate type
void browseProducts(){

    //variables
    int i=0;

    while(i<=29){

        printf("Product ID: %s | Product name: %-30s | %-45s | Price: %-8.2f Baht | Remaining: %d\n", product[i].productID, product[i].productname, product[i].description, product[i].price, product[i].stockquantity);

        i++;
    }

}

// Function that adds product to shopping cart | Change return type to appropriate type
void addToCart(){

    //variables
    char productNameOrID[50];
    int choice, i=0, j=0,k=0, quantity;

    //Enter item of purchase
    printf("\nWhat would you like to purchase?\n");
    printf("Please enter product name or ID: ");

    scanf("%[^\n]s",productNameOrID);

    printf("Please enter quantity of purhase: ");

    scanf("%d",&quantity);

        //loop through product list
        i = 0;
        while(i<=30){

            if(strlen(productNameOrID)==strlen(product[i].productname)){
                //variable to coampre category without modifying the actual struct data
                char compareName[50];
                char enteredName[50];

                //It takes the name of the current product name
                strcpy(compareName, product[i].productname);
                strcpy(enteredName, productNameOrID);

                //change entered product name to lower case to make it easier to compare to the other product name
                j=0;
                while(enteredName[j] != '\0'){

                    if(enteredName[j] >= 'A' && enteredName[j] <= 'Z' ){

                        enteredName[j]+=32;

                    }
                    j++;
                }

                //change current product category to lower case to prepare for the comparison
                j=0;
                while(compareName[j] != '\0'){

                    if(compareName[j] >= 'A' && compareName[j] <= 'Z' ){

                        compareName[j]+=32;

                    }
                    j++;

                }

                //category comparisons, if it is the same, print out the information
                if(strcmp(enteredName, compareName)==0){
                    
                    strcpy(InCart[k].productID, product[i].productID);
                    strcpy(InCart[k].productname, product[i].productname);
                    InCart[k].price = product[i].price;
                    InCart[k].amount = quantity;

                    printf("\n%d of items %s %s successfully added to cart\n",InCart[k].amount , InCart[k].productID, InCart[k].productname);

                    k++;

                    break;

                } 
            }

            if(strcmp(productNameOrID,product[i].productID) == 0){

                strcpy(InCart[k].productID, product[i].productID);
                strcpy(InCart[k].productname, product[i].productname);
                InCart[k].price = product[i].price;
                InCart[k].amount = quantity;
                    
                printf("\n%d of items %s %s successfully added to cart\n",InCart[k].amount , InCart[k].productID, InCart[k].productname);

                k++;

                break;

            }

            i++;         
        }
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

