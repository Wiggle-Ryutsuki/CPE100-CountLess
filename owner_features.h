#ifndef OWNER_FEATURES_H 
#define OWNER_FEATURES_H

typedef struct {
    char productID[10];
    char productName[50];
    char description[100];
    char category[50];
    float price;
    int stockQuantity;
    int minimumThreshold;
    int restockAmount;
    char lastUpdated[20];
} Product;


void addProduct();
void viewProduct();
void editProduct();
void deleteProduct();
void restockProduct();
void logAction(const char *actionType, const char *productName, const char *category, const char *discountCode, float price, int stockQuantity);
void createCoupon();
void validateCoupon();

// Helper functions
void getLastProductID();
int isValidDate();
int isLeapYear();
int isCouponExpired();
void parseProductLine(char *line, Product *product);
void printProduct(const Product *product);

#endif 