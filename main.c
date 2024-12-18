// This script is for running and testing the entire program

#include <stdio.h>
#include "customer_features.h"
#include "owner_features.h"

int main(void) {
    int userRole;    
    int choice;
    int continueShopping = 1;

    // Step 1: Initialize product database
    printf("Loading product information...\n");
    productInformation();
    couponInformation();
    printf("Product information loaded successfully!\n");

    // Step 2: User role selection
    printf("\n--- Welcome to the Shopping System ---\n");
    printf("Are you a:\n1. Customer\n2. Owner\n");
    printf("Enter your choice: "); scanf("%d", &userRole);
    getchar(); 

    // Customer interaction loop
    if (userRole == 1) {
        // --- Customer Menu ---
        int continueShopping = 1;
        while (continueShopping) {
            printf("\n--- Shop Menu ---\n");
            printf("1. Browse Products\n");
            printf("2. Search Product\n");
            printf("3. Add to Cart\n");
            printf("4. View Cart\n");
            printf("5. Checkout\n");
            printf("6. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            getchar();
            
            switch (choice) {
                case 1:
                    browseProducts();
                    break;
                case 2:
                    searchProduct();
                    break;
                case 3:
                    addToCart();
                    break;
                case 4:
                    viewCart();
                    break;
                case 5:
                    checkoutCart();
                    continueShopping = 0; // End the loop after checkout
                    break;
                case 6:
                    printf("Exiting... Thank you for visiting!\n");
                    continueShopping = 0; // Exit the loop
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        }
    } else if (userRole == 2) { // Owner interaction loop
        // --- Owner Menu ---
        int continueManaging = 1;
        while (continueManaging) {
            printf("\n--- Shop Menu ---\n");
            printf("1. View Product\n");
            printf("2. Edit Product\n");
            printf("3. Add a New Product\n");
            printf("4. Delete Product From Inventory\n");
            printf("5. Restock Product\n");
            printf("6. Create a Coupon\n");
            printf("7. Check Coupon Validation\n");
            printf("8. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            getchar();
            
            switch (choice) {
                case 1:
                    viewProduct();
                    break;
                case 2:
                    editProduct();
                    break;
                case 3:
                    addProduct();
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
                case 8:
                    printf("Exiting...");
                    continueManaging = 0; // Exit the loop
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        }
    }
}