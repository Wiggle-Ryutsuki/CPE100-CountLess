// This script is for running and testing the entire program

#include <stdio.h>
#include "customer_features.h"
#include "automation.h"
// include other necessary libraries including from other scripts

int main(void) {
    int userRole;    
    int choice;
    int continueShopping = 1;

    // Step 1: Initialize product database
    printf("Loading product information...\n");
    productInformation();
    printf("Product information loaded successfully!\n");

    // Step 2: User role selection
    printf("\n--- Welcome to the Shopping System ---\n");
    printf("Are you a:\n");
    printf("1. Customer\n");
    printf("2. Owner\n");
    printf("Enter your choice: ");
    scanf("%d", &userRole);
    getchar(); // Clear newline character

    // Customer interaction loop
    if (userRole == 1) {
        // --- Customer Menu ---
        int continueShopping = 1;
        while (continueShopping) {
            printf("\n--- Customer Menu ---\n");
            printf("1. Browse Products\n");
            printf("2. Search for a Product\n");
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
                    applyCouponAtCheckout();
                    checkoutCart();
                    updateInventoryAfterPurchase();
                    printf("Thank you for your purchase!\n");
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
    } else if (userRole == 2) {
        // --- Owner Menu ---
        int continueManaging = 1;
        while (continueManaging) {
            printf("\n--- Owner Menu ---\n");
            printf("1. Schedule Auto-Restock\n");
            printf("2. Check Scheduled Restock\n");
            printf("3. Perform Auto-Purchase\n");
            printf("4. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            getchar();

            switch (choice) {
                case 1:
                    schedulePurchase();  // Allow the owner to schedule auto-restocks
                    break;
                case 2:
                    checkScheduledRestock();  // Check if a restock is scheduled for today
                    break;
                case 3:
                    {
                        // Perform an auto-purchase (you can hardcode or get input for productID and quantity)
                        char productID[10];
                        int quantity;
                        printf("Enter product ID to auto-purchase: ");
                        scanf("%s", productID);
                        printf("Enter quantity: ");
                        scanf("%d", &quantity);
                        autoPurchase(productID, quantity);  // Perform auto-purchase
                    }
                    break;
                case 4:
                    printf("Exiting... Thank you for managing!\n");
                    continueManaging = 0; // Exit the loop
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        }
    }
}