// This script is for programming all customer features

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
// include other necessary libraries including from other scripts

void searchProduct();
void browseProducts();
void addToCart();
void viewCart();
void checkoutCart();
void updateInventoryAfterPurchase();
void applyCouponAtCheckout();
void productInformation();

//Information of items in cart
struct cart
{
    char productID[10];
    char productname[50];
    float price;
    int amount;
    int stock;
}InCart [1000];

//Will be used to keeptrack of items in cart, at the start of the program it will be 0
int itemsInCart=0;
//Will be used to keep track of total amount of products for loops, start at 0
int itemsInProductsList=0;

//Information of products
struct products
{
    char productID[10];
    char productname[50];
    char description[100];
    char actualDescription[100];
    char category[50];
    float price;
    int stockquantity;
    int minimumThreshold;
    int restockAmount;
    char lastUpdated[20];
}product [1000];

//Reads product file and adds product information to struct, should be done at start of program
void productInformation(){
    // Code here
}


// Function that searches for a product | Change return type to appropriate type
void searchProduct(){
    // Code here
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
    // code here
}

// Function that applies coupon | Change return type to appropriate type
void applyCouponAtCheckout(){
    // Code here
}