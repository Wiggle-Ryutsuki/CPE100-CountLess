// This script is for programming all customer features

#include <stdio.h>
// include other necessary libraries including from other scripts


void searchProduct();
void browseProducts();
void addToCart();
void viewCart();
void checkoutCart();
void updateInventoryAfterPurchase();
void applyCouponAtCheckout();


// Function that searches for a product | Change return type to appropriate type
void searchProduct(){
    
    FILE *file = fopen("products.csv","r");

    if (file == NULL){
        printf("Error: Unable to open the file.\n");
        return 1;
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
