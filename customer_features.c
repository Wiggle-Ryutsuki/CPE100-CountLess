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

//test
int main(){

    productInformation();

    //browseProducts();
    searchProduct();
    searchProduct();
    //searchProduct();
    //addToCart();
    //addToCart();
    //addToCart();

    //viewCart();
    //browseProducts();
    //searchProduct();

    //checkoutCart();



    //printf("\n%d",itemsInCart);
}


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

        i++;

    }

    //close the file
    fclose(file);

}


// Function that searches for a product | Change return type to appropriate type
void searchProduct(){

    //variables
    char productName[50], category[50];
    float uppper_price,lower_price;
    int choice, i=0, j=0, done=0, match=0;

    //user interface to select search criteria
    printf("\n _______________________________");
    printf("\n|          Search Menu          |\n");
    printf("|_______________________________|");
    printf("\n| -Search by name     : Enter 1 |\n|                               |\n| -Search by category : Enter 2 |\n|                               |\n| -Search by price    : Enter 3 |\n");
    printf("|_______________________________|\n");

    //receive right input
    while(done==0){

        printf("\n  > Enter your selection: ");
        scanf("%d",&choice);
        getchar();
    
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
        done=1;
        //Enter product name
        printf("\n<------Search by name selected------>\n");
        printf("\n  > Enter product name: ");
        scanf("%[^\n]s",productName);
        getchar();

        printf("\n______________________________________________________________________________________________________________________________________________________________\n");
        printf("\n                                                                     Search results for %s\n", productName);
        printf("______________________________________________________________________________________________________________________________________________________________");
        printf("\n\n");                                                                     
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
                    match++;
                    printf("Product ID: %s | Product name: %-30s | %-45s | Price: %-8.2f Baht | Remaining: %d\n", product[i].productID, product[i].productname, product[i].description, product[i].price, product[i].stockquantity);
                }

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

        printf("\n______________________________________________________________________________________________________________________________________________________________\n");
        printf("\n                                                                    Search results for %s\n", category);
        printf("______________________________________________________________________________________________________________________________________________________________");                                                                 
        printf("\n\n");
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
                    match++;
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
        printf("\n<------Search by price selected------>\n");
        printf("\n  > Enter upper price range: ");
        scanf("%f",&uppper_price);
        getchar();
        printf("  > Enter lower price range: ");
        scanf("%f",&lower_price);
        getchar();        

        printf("\n______________________________________________________________________________________________________________________________________________________________\n");
        printf("\n                                                            Search results for prices from %.2f to %.2f\n", lower_price, uppper_price);
        printf("______________________________________________________________________________________________________________________________________________________________");                                                                 
        printf("\n\n");

        //loop through the product list
        i = 0;
        while(i<=30){

            //check if prices are in range
            if(product[i].price<=uppper_price && product[i].price>=lower_price){

                //print out product info
                match++;
                printf("Product ID: %s | Product name: %-30s | %-45s | Price: %-8.2f Baht | Remaining: %d\n", product[i].productID, product[i].productname, product[i].description, product[i].price, product[i].stockquantity);

            }

            i++;

        }

        break;

    default:

        printf("\nPlease enter valid option\n");

        break;
      
    }    

    if(match==0){

        printf("   -No items found...\n");

    }

    printf("______________________________________________________________________________________________________________________________________________________________\n");

}

// Function that calls for all products | Change return type to appropriate type
void browseProducts(){

    //variables
    int i=0;

    //print out list header
    printf("\n___________________________________________________________________________________________________________________________\n");
    printf("\nProduct ID |          Product name          |                  Description                  |      Price      |  Remaining\n");
    printf("___________________________________________________________________________________________________________________________\n");
    //loop to print out all products
    while(i<=29){

        printf(" %-9s | %-30s | %-45s | %-10.2f Baht | %7d\n", product[i].productID, product[i].productname, product[i].description, product[i].price, product[i].stockquantity);

        i++;
    }

}

// Function that adds product to shopping cart | Change return type to appropriate type
void addToCart(){

    //variables
    char productNameOrID[50];
    int choice, i=0, j=0, k=0, quantity, match=0, AlreadyInCart=0, done=0;

    //Enter item of purchase
    printf("\nWhat would you like to add to cart?\n");
    printf("\nPlease enter product name or ID: ");

    scanf("%[^\n]s",productNameOrID);
    getchar();

    while(done==0){

        printf("\nPlease enter quantity of purchase: ");

        scanf("%d",&quantity);
        getchar();

        if(quantity<=0){

            printf("\nPlease enter valid amount\n");

        }else{

            done=1;

        }

    }

    //loop through product list
    i = 0;
    while(i<=30){

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

                    match++;
                    AlreadyInCart=1;
                    //Check if there are enough items in stock    
                    printf("\n%d\n",InCart[j].stock);
                    if((quantity + InCart[j].amount) <= InCart[j].stock){

                        //if there are enough items, add item to cart
                        InCart[j].amount += quantity;

                        printf("\n%d of item %s %s successfully added to cart\n",InCart[j].amount , InCart[j].productID, InCart[j].productname);
                        
                    }else{

                        //if there are not enough items, print out error message
                        printf("\nUnable to purchase, not enough in stock\n");

                    }    

                    break;
                }                
            }

            //check if product IDs are the same
            if(strcmp(productNameOrID,InCart[j].productID) == 0){

                match++;  
                AlreadyInCart=1;              
                //Check if there are enough items in stock               
                if((quantity + InCart[j].amount) <= InCart[j].stock){

                    //if there are enough items, add item to cart                
                    InCart[j].amount+=quantity;
                            
                    printf("\n%d of item %s %s successfully added to cart\n",InCart[j].amount , InCart[j].productID, InCart[j].productname);

                }else{

                    //if there are not enough items, print out error message
                    printf("\nUnable to purchase, not enough in stock\n");

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

                match++;
                //Check if there are enough items in stock    
                if(quantity<=product[i].stockquantity){

                    //if there are enough items, add item to cart
                    strcpy(InCart[itemsInCart].productID, product[i].productID);
                    strcpy(InCart[itemsInCart].productname, product[i].productname);
                    InCart[itemsInCart].price = product[i].price;
                    InCart[itemsInCart].amount = quantity;
                    InCart[itemsInCart].stock = product[i].stockquantity;                    

                    printf("\n%d of item %s %s successfully added to cart\n",InCart[itemsInCart].amount , InCart[itemsInCart].productID, InCart[itemsInCart].productname);

                    itemsInCart++;
                    
                }else{

                    //if there are not enough items, print out error message
                    printf("\nUnable to purchase, not enough in stock\n");

                }    

                break;

            } 
        }

        //If product Ids are the same, proceed
        if(strcmp(productNameOrID,product[i].productID) == 0){
            
            match++;
            //Check if there are enough items in stock               
            if(quantity<=product[i].stockquantity){

                //if there are enough items, add item to cart                
                strcpy(InCart[itemsInCart].productID, product[i].productID);
                strcpy(InCart[itemsInCart].productname, product[i].productname);
                InCart[itemsInCart].price = product[i].price;
                InCart[itemsInCart].amount = quantity;
                InCart[itemsInCart].stock = product[i].stockquantity;
                        
                printf("\n%d of item %s %s successfully added to cart\n",InCart[itemsInCart].amount , InCart[itemsInCart].productID, InCart[itemsInCart].productname);

                itemsInCart++;

            }else{

                //if there are not enough items, print out error message
                printf("\nUnable to purchase, not enough in stock\n");

            }    

            break;

        }

        i++;         
    }

    //If no products were found, print out error message
    if(match==0){

        printf("\nNo items found, unable to purchase\n");

    }

}

// Function that calls for all items in cart | Change return type to appropriate type
void viewCart(){
    //variables
    int i=0;

    //loop for printing everything in cart
    while(i<itemsInCart){

        printf("%d. Product ID: %s | Product name: %-30s | Amount: %-10d | Product total: %-8.2f Baht\n", (i+1), InCart[i].productID, InCart[i].productname, InCart[i].amount, (InCart[i].price*InCart[i].amount));

        i++;

    }

}

// Function that proceeds to checkout point (calculates total and prompts to use coupon) | Change return type to appropriate type
void checkoutCart(){
    
    //variables
    float total=0, productTotal;
    int choice, confirm, i=0, j=0;

    //Calculates total value of items in cart
    while (i<itemsInCart){
        
        productTotal = (InCart[i].amount*InCart[i].price);

        total+=productTotal;

        i++;

    }

    printf("\nYour total is %.2f\n", total);

    printf("\nWould you like to use your coupon?\n");
    printf("Yes: Enter 1\nNo: Enter 2\n");

    scanf("%d",&choice);

    //Slection, use coupon or no coupon
    switch (choice)
    {
    case 1:
        
        break;
    
    case 2:

        printf("\nNo coupon selected\n");
        
        int done=0;
            
        while(done==0){

            printf("\nConfirm purchase?\n");

            printf("\nConfirm: Enter 1\nCancel: Enter 2\n");
            scanf("%d",&confirm);

            if(confirm==1){

                done=1;
                //update inventory

                // Get the current date and time
                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                char lastUpdated[20];
                sprintf(lastUpdated, "%02d/%02d/%d\n", tm.tm_mday,  tm.tm_mon + 1, tm.tm_year + 1900); // i changed this, hope it works tomorrow

                i=0;
                while(i<itemsInCart){

                    while (j<30){
                        
                        if(strcmp(InCart[i].productID, product[j].productID)==0){

                            product[j].stockquantity-=InCart[i].amount;
                            strcpy(product[j].lastUpdated, lastUpdated);

                            break;

                        }

                        j++;
                    }
                    
                    i++;
                }

                updateInventoryAfterPurchase();

            }else if(confirm==2){

                done=1;
                printf("\nOrder canceled\n");

            }else{

                printf("\nPlease enter valid input\n");

            }

        }

        break;

    default:
        break;
    }

}

// Function that updates inventory | Change return type to appropriate type
void updateInventoryAfterPurchase(){


    //update the products file
    FILE *file = fopen("products3.csv","w");

        //check if file exists
    if (file == NULL){
        printf("Error: Unable to open the file.\n");
        return;
    }


    fprintf(file,"ProductID,ProductName,Description,Category,Price,StockQuantity,MinimumThreshold,RestockAmount,LastUpdated\n");
    for (int j = 0; j < 30; j++){
        fprintf(file,"%s,%s,%s,%s,%.2f,%d,%d,%d,%s",
                product[j].productID, product[j].productname,
                product[j].actualDescription, product[j].category,
                product[j].price, product[j].stockquantity,
                product[j].minimumThreshold, product[j].restockAmount,
                product[j].lastUpdated);
    }

    fclose(file);

    printf("\nPurchase successful\n");

}

// Function that applies coupon | Change return type to appropriate type
void applyCouponAtCheckout(){
    // Code here
}

