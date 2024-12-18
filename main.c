// This script is for running and testing the entire program

#include <stdio.h>
// include other necessary libraries including from other scripts

int main(void) {
    int userRole;    

    printf("\n--- Welcome to the Shopping System ---\n");
    printf("Are you a:\n1. Customer\n2. Owner\n");
    printf("Enter your choice: "); scanf("%d", &userRole);
    getchar(); 

    // Customer interaction loop
    if (userRole == 1) {
        // --- Customer Menu ---
        int continueShopping = 1;
        while (continueShopping) {
            printf("Welcome, Customer!");
            continueShopping = 0; // End the loop after checkout
            break;
        }
    } else if (userRole == 2) { // Owner interaction loop
        // --- Owner Menu ---
        int continueManaging = 1;
        while (continueManaging) {
            printf("Welcome, Owner!");
            continueManaging = 0; // Exit the loop
            break;
        }
    }
}