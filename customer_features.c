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

//For initializing product database
void productInformation();
//For initializing coupon database
void couponInformation();

//Information of items in cart
struct cart
{
    char productID[10];
    char productname[50];
    char category[50];
    float price;
    float Alltotal;
    int amount;
    int stock;
}InCart [1000];

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

//Coupon Information
struct coupon
{
    char code[20];
    char description[100];
    char discountType[20];
    float discount;
    float MinTotal;
    char expiryDate[11]; // Format: YYYY-MM-DD
} Coupon [100];

//information on selected coupon
struct MyCoupon
{
    char code[20];
    char discountType[20];
    float discount;
    float MinTotal;
    char expiryDate[11]; // Format: YYYY-MM-DD
} MyCoupon[2];


//Will be used to keeptrack of items in cart, at the start of the program it will be 0
int itemsInCart=0;
//Will be used to keep track of total amount of products for loops, start at 0
int itemsInProductsList=0;
//Will be used to keep track of total amount of coupons for loops, start at 0
int TotalCoupons=0;

//Reads product file and adds product information to struct, should be done at start of program
void productInformation(){

    //opens file
    FILE *file = fopen("products.csv","r");

    //check if file exists
    if (file == NULL){
        printf("Error: Unable to open the file.\n");
        return;
    }

    //variables
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
            strcpy(product[i].actualDescription, token); //For rewriting file

            token = strtok(NULL, ",");
            strcat(product[i].description, ",");
            strcat(product[i].actualDescription, ",");//for rewriting file
            strcat(product[i].description, token);
            strcat(product[i].actualDescription, token); //for rewriting file

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
            strcpy(product[i].actualDescription, token);//for file rewrite
        } 

        token = strtok(NULL, ",");
        strcpy(product[i].category, token);

        token = strtok(NULL, ",");
        product[i].price = atof(token);

        token = strtok(NULL, ",");
        product[i].stockquantity = atoi(token);

        token = strtok(NULL, ","); // Get next piece
        product[i].minimumThreshold = atoi(token); // minimumThreshold is an int, so we need to convert it to ASCII

        token = strtok(NULL, ","); // Get next piece
        product[i].restockAmount = atoi(token); // restockAmount is an int, so we need to convert it to ASCII

        token = strtok(NULL, ","); // Get next piece
        strcpy(product[i].lastUpdated, token); // lastUpdated is now whatever "token" was pointing at, "token" was pointing at the last piece of the string        

        itemsInProductsList++;
        i++;

    }

    //close the file
    fclose(file);

}

//Reads coupon file and adds coupon information to struct, should be done at start of program
void couponInformation(){

    //opens file
    FILE *file = fopen("coupons.csv","r");

    //check if file exists
    if (file == NULL){
        printf("Error: Unable to open coupon file.\n");
        return;
    }

    //variables
    char line[1000];
    int i=0, isFirstLine=1;

     while (fgets(line, sizeof(line), file))
    {
        //skip the header
        if (isFirstLine) {
            isFirstLine = 0;
            continue;
        }

        //this is not workingg
        sscanf(line, "%[^,],%f,%[^,],%[^,],%[^,],%f",
               Coupon[i].code, &Coupon[i].discount,
               Coupon[i].discountType, Coupon[i].expiryDate,
               Coupon[i].description, &Coupon[i].MinTotal);

        TotalCoupons++;
        i++;
    }


    //close the file
    fclose(file);

}

// Made this helper function to figure out if a coupon is expired or not
int isCouponExpired(const char *expiryDate)
{
    // This struct will hold the expiry date of a coupon.
    struct tm expiry = {0};

    // A variable called now with a type time_t
    time_t now;

    // A pointer called current, points to a structure that represents the current date and time.
    struct tm *current;

    // Reads the expiry date from a string called expiryDate in the format "YYYY-MM-DD" and stores the year, month, and day into the respective fields of the expiry structure.
    sscanf(expiryDate, "%d-%d-%d", &expiry.tm_year, &expiry.tm_mon, &expiry.tm_mday);

    // Adjusts the year field in the expiry structure. The tm_year field expects the number of years since 1900, so this line converts the full year (e.g., 2024) to the expected format by subtracting 1900.
    expiry.tm_year -= 1900; // Adjust year

    // Adjusts the month field in the expiry structure. The tm_mon field expects months in the range 0-11 (where 0 is January), so this line converts the month from the usual 1-12 range to 0-11 by subtracting 1.
    expiry.tm_mon -= 1;     // Adjust month

    // This function call gets the current time and stores it in the now variable.
    time(&now);

    // Converts the time stored in "now" to local time and stores the result in the current pointer. This pointer now points to a struct "tm" that represents the current date and time.
    current = localtime(&now);

    // Compares the expiry date with the current date. mktime converts the struct tm to time_t for comparison. difftime calculates the difference in seconds between the expiry date and the current date. If the result is less than 0, it means the expiry date is in the past.
    if (difftime(mktime(&expiry), mktime(current)) < 0)
    {
        return 1; // Expired
    }
    return 0; // Not expired
}

// Function that searches for a product | Change return type to appropriate type
void searchProduct(){

    //variables
    char productNameOrID[50], category[50];
    float uppper_price,lower_price;
    int choice, i=0, j=0, done=0, match=0;

    //user interface to select search criteria
    printf("\n _________________________________");
    printf("\n|           Search Menu           |\n");
    printf("|_________________________________|");
    printf("\n| -Search by name or ID : Enter 1 |\n|                                 |\n| -Search by category   : Enter 2 |\n|                                 |\n| -Search by price      : Enter 3 |\n");
    printf("|_________________________________|\n");

    //receive correct input
    while(done==0){
        
        //enter your selection
        printf("\n  > Enter your selection: ");
        scanf("%d",&choice);
        getchar();
    
        //if input is correct stop loop, else ask for reinput
        if(choice==1 || choice==2 || choice==3){

            done=1;

        }else{

            printf("\n<<---------Please enter a valid option!--------->>\n"); 

        }

    }
    
    switch (choice)
    {
    //search product by name
    case 1:
        
        //Enter product name
        printf("\n<------Search by name selected------>\n");
        printf("\n  > Enter product name or ID: ");
        scanf("%[^\n]s",productNameOrID);
        getchar();

        //search result header
        printf("\n______________________________________________________________________________________________________________________________________________________________\n");
        printf("\n                                                                     Search results for %s\n", productNameOrID);
        printf("______________________________________________________________________________________________________________________________________________________________");
        printf("\n\n");                                                                     
        //loop through the names
        i = 0;
        while(i<itemsInProductsList){

            //If string lentghs are the same, then it can be the product we are looking for            
            if(strlen(productNameOrID)==strlen(product[i].productname)){

                //variable to compare the product name without modifying the actual struct data
                char compareName[50];
                char enteredName[50];

                //It takes the name of the current product
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

                //change current product name to lower case to prepare for the comparison
                j=0;
                while(compareName[j] != '\0'){

                    if(compareName[j] >= 'A' && compareName[j] <= 'Z' ){

                        compareName[j]+=32;

                    }
                    j++;

                }

                //product name comparisons, if it is the same, print out the information
                if(strcmp(enteredName, compareName)==0){

                    match++; //item was found
                    printf("Product ID: %s | Product name: %-30s | %-45s | Price: %-8.2f baht | Remaining: %d\n", product[i].productID, product[i].productname, product[i].description, product[i].price, product[i].stockquantity);

                    break;

                }

            }

            //If product Ids are the same, proceed
            if(strcmp(productNameOrID,product[i].productID) == 0){
                
                //item was found
                match++;
                printf("Product ID: %s | Product name: %-30s | %-45s | Price: %-8.2f baht | Remaining: %d\n", product[i].productID, product[i].productname, product[i].description, product[i].price, product[i].stockquantity);    
                
                break;

            }

            i++;

        }
        
        break;

    //search product by category
    case 2:
        
        //Enter product category
        printf("\n<------Search by category selected------>\n");        
        printf("\n  > Enter product category: ");
        scanf("%[^\n]s",category);
        getchar();

        //search result header
        printf("\n______________________________________________________________________________________________________________________________________________________________\n");
        printf("\n                                                                    Search results for %s\n", category);
        printf("______________________________________________________________________________________________________________________________________________________________");                                                                 
        printf("\n\n");
        //loop through product list
        i = 0;
        while(i<itemsInProductsList){

            //If string lentghs are the same, then it can be the category we are looking for
            if(strlen(category)==strlen(product[i].category)){

                //variable to compear category without modifying the actual struct data
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

                    match++; //item was found
                    printf("Product ID: %s | Product name: %-30s | %-45s | Price: %-8.2f baht | Remaining: %d\n", product[i].productID, product[i].productname, product[i].description, product[i].price, product[i].stockquantity);
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
        printf("\n<------Search by price selected------>\n");
        printf("\n  > Enter upper price range: "); //enter upper price range
        scanf("%f",&uppper_price);
        getchar();
        printf("  > Enter lower price range: "); //enter lower price range
        scanf("%f",&lower_price);
        getchar();        

        //if upper price range is less than lower price range
        if(uppper_price<lower_price){

            //switch values
            int temp = uppper_price;
            uppper_price = lower_price;
            lower_price = temp;

        }

        //search result header
        printf("\n______________________________________________________________________________________________________________________________________________________________\n");
        printf("\n                                                            Search results for prices from %.2f to %.2f\n", lower_price, uppper_price);
        printf("______________________________________________________________________________________________________________________________________________________________");                                                                 
        printf("\n\n");

        //loop through the product list
        i = 0;
        while(i<itemsInProductsList){

            //check if prices are in range
            if(product[i].price<=uppper_price && product[i].price>=lower_price){

                //print out product info
                match++; //item was found
                printf("Product ID: %s | Product name: %-30s | %-45s | Price: %-8.2f baht | Remaining: %d\n", product[i].productID, product[i].productname, product[i].description, product[i].price, product[i].stockquantity);

            }

            i++;

        }

        break;

    default:

        printf("\nPlease enter valid option\n");

        break;
      
    }    

    //If no items were found
    if(match==0){

        printf(" ....No items found....\n");

    }

    printf("______________________________________________________________________________________________________________________________________________________________\n");

}

// Function that calls for all products | Change return type to appropriate type
void browseProducts(){

    //variables
    int i=0;

    //print out list header
    printf("\n__________________________________________________________________________________________________________________________________________________\n");
    printf("\nProduct ID |          Product name          |                  Description                  |       Category       |      Price      |  Remaining\n");
    printf("__________________________________________________________________________________________________________________________________________________\n");
    //loop to print out all products
    while(i<itemsInProductsList){

        printf(" %6s    | %-30s | %-45s | %-20s | %-10.2f baht | %7d\n", product[i].productID, product[i].productname, product[i].description, product[i].category, product[i].price, product[i].stockquantity);

        i++;
    }

    printf("__________________________________________________________________________________________________________________________________________________\n");

}

// Function that adds product to shopping cart | Change return type to appropriate type
void addToCart(){

    //variables
    char productNameOrID[50];
    int choice, i=0, j=0, k=0, quantity, match=0, AlreadyInCart=0, done=0;

    //Enter item of purchase
    printf("\n<------What would you like to add to cart?------>\n");
    printf("\n  > Please enter product name or ID: ");

    scanf("%[^\n]s",productNameOrID);
    getchar();

    //Check if input is correct, quantity shouldnt be less than or equal to 0
    while(done==0){

        //enter quantity
        printf("  > Please enter quantity of purchase: ");

        scanf("%d",&quantity);
        getchar();

        //If input in incorrect, ask for reinput
        if(quantity<=0){

            printf("\n<<---------Please enter valid amount!--------->>\n");

        }else{

            done=1;

        }

    }

    //loop through product list
    i = 0;
    while(i<itemsInProductsList){

        //check if item already in cart
        for(j=0;j<itemsInCart;j++){

            //check if names are same lenght
            if(strlen(productNameOrID)==strlen(InCart[j].productname)){
                //variable to coampre category without modifying the actual struct data
                char compareName[50];
                char enteredName[50];

                //It takes the name of the current product name
                strcpy(compareName, InCart[j].productname);
                strcpy(enteredName, productNameOrID);

                //change entered product name to lower case to make it easier to compare to the other product name
                k=0;
                while(enteredName[k] != '\0'){

                    if(enteredName[k] >= 'A' && enteredName[k] <= 'Z' ){

                        enteredName[k]+=32;

                    }
                    k++;
                }

                //change current product name to lower case to prepare for the comparison
                k=0;
                while(compareName[k] != '\0'){

                    if(compareName[k] >= 'A' && compareName[k] <= 'Z' ){

                        compareName[k]+=32;

                    }
                    k++;
                }

                //Name comparisons, if it is the same, proceed
                if(strcmp(enteredName, compareName)==0){

                    match++; //item was found
                    AlreadyInCart=1; //item is already in cart
                    //Check if there are enough items in stock    
                    if((quantity + InCart[j].amount) <= InCart[j].stock){

                        //if there are enough items, add item to cart
                        InCart[j].amount += quantity;

                        printf("\n  > %d of item %s %s successfully added to cart <\n", quantity, InCart[j].productID, InCart[j].productname);
                        
                    }else{

                        //if there are not enough items, print out error message
                        printf("\n<<---------Unable to purchase, not enough in stock--------->>\n");

                    }    

                    break;
                }                
            }

            //check if product IDs are the same
            if(strcmp(productNameOrID,InCart[j].productID) == 0){

                match++; //item was found
                AlreadyInCart=1; //item is already in cart              
                //Check if there are enough items in stock               
                if((quantity + InCart[j].amount) <= InCart[j].stock){

                    //if there are enough items, add item to cart                
                    InCart[j].amount+=quantity;
                            
                    printf("\n  > %d of item %s %s successfully added to cart < \n",quantity , InCart[j].productID, InCart[j].productname);

                }else{

                    //if there are not enough items, print out error message
                    printf("\n<<---------Unable to purchase, not enough in stock--------->\n");

                }    

                break;

            }

        }

        //If item already in cart no need to continue
        if(AlreadyInCart==1){

            break;

        }

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

            //Name comparisons, if it is the same, proceed
            if(strcmp(enteredName, compareName)==0){

                match++; //item was found
                //Check if there are enough items in stock    
                if(quantity<=product[i].stockquantity){

                    //if there are enough items, add item to cart
                    strcpy(InCart[itemsInCart].productID, product[i].productID);
                    strcpy(InCart[itemsInCart].productname, product[i].productname);
                    strcpy(InCart[itemsInCart].category, product[i].category);
                    InCart[itemsInCart].price = product[i].price;
                    InCart[itemsInCart].amount = quantity;
                    InCart[itemsInCart].stock = product[i].stockquantity;                    

                    printf("\n  > %d of item %s %s successfully added to cart <\n",InCart[itemsInCart].amount , InCart[itemsInCart].productID, InCart[itemsInCart].productname);

                    itemsInCart++; //item in cart increases
                    
                }else{

                    //if there are not enough items, print out error message
                    printf("\n<<--------Unable to purchase, not enough in stock--------->>\n");

                }    

                break;

            } 
        }

        //If product Ids are the same, proceed
        if(strcmp(productNameOrID,product[i].productID) == 0){
            
            match++;// item was found
            //Check if there are enough items in stock               
            if(quantity<=product[i].stockquantity){

                //if there are enough items, add item to cart                
                strcpy(InCart[itemsInCart].productID, product[i].productID);
                strcpy(InCart[itemsInCart].productname, product[i].productname);
                strcpy(InCart[itemsInCart].category, product[i].category);                
                InCart[itemsInCart].price = product[i].price;
                InCart[itemsInCart].amount = quantity;
                InCart[itemsInCart].stock = product[i].stockquantity;
                        
                printf("\n  > %d of item %s %s successfully added to cart <\n",InCart[itemsInCart].amount , InCart[itemsInCart].productID, InCart[itemsInCart].productname);

                itemsInCart++; //item in cart increases

            }else{

                //if there are not enough items, print out error message
                printf("\n<<---------Unable to purchase, not enough in stock--------->>\n");

            }    

            break;

        }

        i++;         
    }

    //If no products were found, print out error message
    if(match==0){

        printf("\n<---------No items found, unable to purchase--------->\n");

    }

}

// Function that calls for all items in cart | Change return type to appropriate type
void viewCart(){
    //variables
    int i=0;

    //cart item header
    printf("\n___________________________________________________________________________________________________________________________\n");
    printf("\n                                                     Items in Cart\n");
    printf("___________________________________________________________________________________________________________________________");
    printf("\n\n");

    //loop for printing everything in cart
    while(i<itemsInCart){

        printf("%4d. Product ID: %s | Product name: %-30s | Amount: %-10d | Product total: %-8.2f baht\n", (i+1), InCart[i].productID, InCart[i].productname, InCart[i].amount, (InCart[i].price*InCart[i].amount));

        i++;

    }

    //If there are no items in cart
    if(itemsInCart==0){

        printf("  ....No Items in cart....\n");

    }

    printf("\n___________________________________________________________________________________________________________________________\n");

}

// Function that proceeds to checkout point (calculates total and prompts to use coupon) | Change return type to appropriate type
void checkoutCart(){
    
    //variables
    float total=0, productTotal;
    int choice, confirm, i=0, j=0, valid=0, loop=0;

    //Exit if no items in cart
    if(itemsInCart==0){

        printf("\n\n<<---------Unable to purchase, no items in cart--------->>\n");

        return;

    }

    printf("\n\n                                       -----List of Purchasing Items-----");
    printf("\n________________________________________________________________________________________________________________\n");
    printf("\nItem No. | Product ID |          Product name           |    Price per unit    |  Amount\n");
    printf("_________________________________________________________________________________________________________________\n");
    printf("\n");

    //loop for printing everything in cart
    while(i<itemsInCart){

        printf("%5d.   |    %s     | %-30s | %-13.2f baht   | %5d \n", (i+1), InCart[i].productID, InCart[i].productname,InCart[i].price, InCart[i].amount);

        i++;

    }

    printf("_________________________________________________________________________________________________________________\n");

    //Calculates total value of items in cart
    i=0;
    while (i<itemsInCart){
        
        productTotal = (InCart[i].amount*InCart[i].price);

        total+=productTotal;

        i++;

    }

    //Total before any coupons applied
    printf("\n  >>> Your total is %.2f baht\n", total);

    //asks if you would like to use coupon
    printf("\n<------Would you like to use your coupon?------>\n");
    printf("\n  -Yes: Enter 1\n  -No : Enter 2\n");
    printf("<---------------------------------------------->\n");

    //if input invalid, ask for reinput
    while(valid==0){
        //enter choice
        printf("\n  > Use coupon?: ");
        scanf("%d",&choice);

        //Incase if char was entered, so program is not stuck in infinity loop
        if(loop>10){
            
            printf("<<---------Error--------->>");
            return;

        }

        //checks if input valid
        if(choice==1 || choice == 2){

            valid = 1;

        }else{

            printf("\n<<---------Please enter valid input!--------->>\n");

        }
        
        loop++;

    }

    //default coupon code value
    strcpy(MyCoupon[1].code, "-");

    //Slection, use coupon or no coupon
    switch (choice)
    {
    case 1:
        //if coupon was selected

        InCart[1].Alltotal=total;

        //apply your coupon
        applyCouponAtCheckout();

        //displays total after coupon is used
        printf("\n  >>> Your total is now %.2f baht\n", InCart[1].Alltotal);

        break;
    
    case 2:
        //if no coupon selected

        printf("\n  > No coupon selected <\n");

        break;

    default:

        printf("\n<----------Please enter valid input!(Order Cancled)--------->\n");

        return;
    }

    int done=0;
    //Checks for valid input, if invalid loop for reinput            
    while(done==0){

        //display purchase options
        printf("\n<------Confirm purchase?------>\n");
        printf("\n  - Confirm: Enter 1\n  - Cancel : Enter 2\n");

        //enter choice
        printf("\n  > Confirm?: ");
        scanf("%d",&confirm);

        //If confirm purchase
        if(confirm==1){

            done=1;

            // Get the current date and time, to update last update
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            char lastUpdated[20];
            sprintf(lastUpdated, "%d-%02d-%02d\n", tm.tm_year + 1900, tm.tm_mon + 1 ,tm.tm_mday); 

            i=0;
            //loops through items in cart and match it with product in list
            while(i<itemsInCart){

                while (j<itemsInProductsList){
                    
                    //If product ID of item in cart and product list match, update information
                    if(strcmp(InCart[i].productID, product[j].productID)==0){

                        product[j].stockquantity-=InCart[i].amount;
                        InCart[i].stock=product[j].stockquantity;
                        strcpy(product[j].lastUpdated, lastUpdated);

                        break;

                    }

                    j++;
                }
                    
                i++;
            }

            //updates the required csv files
            updateInventoryAfterPurchase();

            //Printing out transaction record
            printf("\n\n                                       -----Transaction Record-----");
            printf("\n________________________________________________________________________________________________________________\n");
            printf("\nItem No. | Product ID |          Product name           |    Price per unit    |  Amount\n");
            printf("_________________________________________________________________________________________________________________\n");
            printf("\n");

            //loop for printing everything in cart
            i=0;
            while(i<itemsInCart){

                printf("%5d.   |    %s     | %-30s | %-13.2f baht   | %5d \n", (i+1), InCart[i].productID, InCart[i].productname,InCart[i].price, InCart[i].amount);

                i++;

            }

            printf("\n                                                         # Coupon Code used: %s ", MyCoupon[1].code);
            printf("\n                                                         # Transaction total: %.2f baht\n                                                         # Purchased on %s ", InCart[1].Alltotal, lastUpdated);          
            printf("_________________________________________________________________________________________________________________\n");

            //reset cart
            memset(InCart, 0, sizeof(InCart));
            itemsInCart=0;

            //Closing message
            printf("\n\n    >> Purchase successful! <<\n");
            printf("\n >> Thank you for your patronage <<");
            printf("\n__________________________________");            

        }else if(confirm==2){

            //If cancle was chosen
            done=1;
            printf("\n  > Order canceled <\n");
            printf("_________________________________________________________________________________________________________________\n");

        }else{

            printf("\n<----------Please enter valid input!--------->\n");

        }

    }

}

// Function that updates inventory | Change return type to appropriate type
void updateInventoryAfterPurchase(){


    //update the products file
    FILE *file = fopen("products.csv","w");

    //check if file exists
    if (file == NULL){
        printf("Error: Unable to open product file.\n");
        return;
    }

    //print out the new information
    fprintf(file,"ProductID,ProductName,Description,Category,Price,StockQuantity,MinimumThreshold,RestockAmount,LastUpdated\n");
    for (int j = 0; j < itemsInProductsList; j++){
        fprintf(file,"%s,%s,%s,%s,%.2f,%d,%d,%d,%s",
                product[j].productID, product[j].productname,
                product[j].actualDescription, product[j].category,
                product[j].price, product[j].stockquantity,
                product[j].minimumThreshold, product[j].restockAmount,
                product[j].lastUpdated);
    }

    fclose(file);

    // Get the current date and time to the minute for logs and transactions
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char TimeStamp[20];
    sprintf(TimeStamp, "%02d/%02d/%d %d:%d", tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900, tm.tm_hour, tm.tm_min);

    //open the Logs file for update
    FILE *lPtr = fopen("logs.csv","a");

    //check if file exists
    if (lPtr == NULL){
        printf("Error: Unable to open logs file.\n");
        return;
    }

    //write purchased item info
    for (int j = 0; j < itemsInCart; j++){
        fprintf(file,"%s,Purchase Completed,Customer,%s,%s,%s,%.2f,%d\n",
                TimeStamp, InCart[j].productname,
                InCart[j].category, MyCoupon[1].code, InCart[j].price,
                InCart[j].stock);
    }    

    //close the logs file
    fclose(lPtr);

    //open the transactions file for update
    FILE *tPtr = fopen("transactions2.csv","a");

    //check if file exists
    if (tPtr == NULL){
        printf("Error: Unable to open transactions file.\n");
        return;
    }

    //write out transaction info
    for (int j = 0; j < itemsInCart; j++){
        fprintf(file,"%s,Purchase Completed,Customer,%s,%s,%s,%.2f,%d,%.2f,%.2f\n",
                TimeStamp, InCart[j].productname,
                InCart[j].category, MyCoupon[1].code, InCart[j].price,
                InCart[j].amount, (InCart[j].price*InCart[j].amount), InCart[1].Alltotal);
    }    

    //close the transactions file
    fclose(tPtr);

}

// Function that applies coupon | Change return type to appropriate type
void applyCouponAtCheckout(){
    //variables
    char enteredCode[20];
    int i=0, done=0;
    float total, saved=0; 

    //print out list header
    printf("\n_____________________________________________________________________________________________________\n");
    printf("\nCoupon Code |                  Description                       |  Expiration Date (Year-Month-Day)\n");
    printf("______________________________________________________________________________________________________\n");
    //loop to print out all products
    while(i<TotalCoupons){

        printf(" %-10s | %-50s | %20s\n", Coupon[i].code, Coupon[i].description, Coupon[i].expiryDate);

        i++;
    }

    printf("\n______________________________________________________________________________________________________\n");

    printf("\n<-----Select your coupon----->\n");
    
    //checks if input is valid, else loop for reinput
    while(done==0){
        //enter coupon code
        printf("\n  > Enter coupon code: ");
        scanf("%s",enteredCode);
        getchar();

        int match=0;
        //loop and check with all coupon codes
        for(i=0;i<TotalCoupons;i++){

            //if code is the same, update customer coupon
            if(strcmp(enteredCode, Coupon[i].code)==0){

                done=1;
                match++; //coupon found
                strcpy(MyCoupon[1].discountType, Coupon[i].discountType);
                strcpy(MyCoupon[1].code, Coupon[i].code);
                strcpy(MyCoupon[1].expiryDate, Coupon[i].expiryDate);
                MyCoupon[1].discount=Coupon[i].discount;
                MyCoupon[1].MinTotal=Coupon[i].MinTotal;

                break;

            }

        }

        //if no code match
        if(match==0){

            printf("\n<---------Please enter valid code--------->\n");

        }
    }

    //display selected coupon
    printf("\n  > Selected coupon: %s <", MyCoupon[1].code);

    //checks for expiry date, if expired, coupon cant be used
    if (isCouponExpired(MyCoupon[1].expiryDate))
    {
        printf("\n\n<--------The coupon is expired.-------->\n");
        printf("<--------Discount unapplicable--------->\n");

        //set code used to "-"
        strcpy(MyCoupon[1].code, "-");       

        return;
    }

    //checks if purchase meets the required minimum total to use the coupon
    if(InCart[1].Alltotal < MyCoupon[1].MinTotal){

        printf("\n\n<-------Minimum total not reached-------->\n");
        printf("<---------Discount unapplicable---------->\n");

        //set code used to "-"
        strcpy(MyCoupon[1].code, "-");       

        return;        

    }

    //checks if coupon is percentage or flat amount type
    if(strcmp(MyCoupon[1].discountType, "Percentage")==0){

        printf("\n  > The coupon is valid. <\n");

        //calulate total after applying percentage coupon
        total=InCart[1].Alltotal;
        total=((total*MyCoupon[1].discount)/100);
        saved=InCart[1].Alltotal-total;

        InCart[1].Alltotal=total;

        //displays amount saved
        printf("\n >> You Saved %.2f baht! <<\n", saved);

        return;

    }else{

        printf("\n  > The coupon is valid. <\n");

        //calculates total after applying flat value coupon
        total=InCart[1].Alltotal;
        total=total-MyCoupon[1].discount;
        saved=InCart[1].Alltotal-total;

        InCart[1].Alltotal=total;

        //displays amount saved
        printf("\n >> You Saved %.2f baht! <<\n", saved);

        return;

    }

}

