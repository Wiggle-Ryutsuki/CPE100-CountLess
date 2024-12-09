# CPE100 — Final Project

## Count Less — The Inventory System

The goal of this project is to design and develop an inventory management system
capable of efficiently tracking, managing, and optimizing inventory within a simulated or real environment. The system should demonstrate capabilities, such
 as item recognition, stock level monitoring, and notifications for restocking.

## Inventory System Requirements

### 1. General Requirements

- Implement the system using **C programming language**.
- Store data in a **text-based database** (e.g., text or CSV files).
- Design a **menu-driven command-line interface (CLI)** to navigate between different features.
- Use **structs** for defining data such as **products, customers, and transactions**.

### 2. Owner Features

- **View Report**
  - Include an option to search and filter by price or product name (e.g., show sold products within a specified price range).
  - Such as daily summary, most sold product in a day, average . etc
- **Perform CRUD operations → Create, Read, Update and Delete**
  - Create a new product
  - View product (read)
  - Edit an existing product (update)
  - Delete product
- **Restocking**
  - Allow the owner to add more stock for any product.
  - If a product’s stock falls below a defined minimum threshold, display a notification alerting the owner that restocking is needed.
- **Logging**
  - Maintain a log file to track key actions such as:
    - Product restocks
    - Customer purchases
    - Report
- **Discount Coupons**
  - Create a coupon system that allows the owner to define discount coupons
    with Coupon code.
    - Coupon code has information including, discount percentage and
        expiration date.
  - Allow coupons to be applied at purchase for products.

### 3. Customer Features

- **View Products**
  - Provide an interface for customers to browse products with details such as
    product name, description, price, and available stock.
  - Include search functionality to filter products by category or price.
- **Purchase Products**
  - Allow customers to add products to a shopping cart and quantities.
  - Calculate the total cost, factoring in any applied discounts (via coupons) before checkout.
- **Update inventory levels after purchase to reflect the new stock levels.**
- **Discount Coupon Application**
  - Enable customers to apply a discount coupon at checkout to reduce the
    total purchase cost, based on valid coupon conditions.
    - Valid coupon conditions:
      - Coupons must not expire.

## Others Required features (to get the full score) (Do all of them)

### 1. Hybrid System Requirements

- The program must be **able to read and execute restock and purchase**
scripts from .txt or .csv files.
- For example, if a restock file contains product details along with quantities,
the program should automatically update the stock based on the quantities
specified in the file.

### 2. Automate System Requirements

- Assuming that some customer would like to purchase in the schedule, the
program must able to schedule the purchase of the product
  - For example, if the program sets the auto purchase to every
    Sunday when Sunday arrives, the quantity of the product should be
    decreased by the quantity set to the auto purchase.
  - Note: We can test this feature by changing the time within the
    computer. We suggest a library `<time.h>` to record and find the
    accurate date and time.
- The program must be able to restock the product according to the
threshold of the product quantity inside the inventory system.
  - For example, if the threshold is set to 30% of maximum quantity
    100 pieces of that product, when the quantity is below than 30 it
    should automatically restock that product to the set-value.

Note:

- You can include other features outside these requirements if possible.
- The program should be able to address/notify the validations.

## Deliverables

1. System architecture or diagram of your program
2. Implemented program
3. Report with technical details, faced problem, and testing results
4. Presentation slide
Presentation Date: To be announced. (it should be 18th December 2024)

## Team Distribution

### Project Lead/Coordinator

Name:
ID: 6707050____
Code Focus:

- Main Program File:
  - `main.c`:
    - Handles the program's main menu and overall flow.
    - Calls functions developed by other team members.
    - Manages integration and ensures smooth interaction between modules.
    - Report Writing

### Owner Features Developer

Name:
ID: 6707050____
Code Focus:

- File: `owner_features.c`
  - Implements **CRUD operations** for products
    - `addProduct()`
    - `viewProduct()`
    - `editProduct()`
    - `deleteProduct()`.
  - Develops the **restocking logic**.
    - `restockProduct()`
  - Adds **logging functionality**.
    - `logAction()`
  - Creates **discount coupon** functionality.
    - `createCoupon()`
    - `validateCoupon()`
    - `applyCoupon()`

Helper File:

- `owner_features.h`
  - Declares all related functions and structs for modularity.

### Customer Features Developer

Name: Iris
ID: 67070503478
Code Focus:

- File: `customer_features.c`
  - Implements **product browsing** and search.
    - `searchProduct()`
    - `browseProducts()`
  - Develops the **shopping cart**.
    - `addToCart()`
    - `viewCart()`
    - `checkoutCart()`
  - **Updates inventory** after purchase.
    - `updateInventoryAfterPurchase()`
  - Integrates **discount coupon application** during checkout
    - `applyCouponAtCheckout()`

Helper File:

- `customer_features.h`
  - Declares all related functions and structs.

### Hybrid and Automation Developer

Code Focus:

Name:
ID: 6707050____
- File: `automation.c`
  - Develops script execution for **restock and purchase**.
    - `executeRestockScript()`
    - `executePurchaseScript()`
  - Implements **auto-restocking** based on thresholds.
    - `autoRestock()`
  - Creates **purchase scheduling** functionality
    - `schedulePurchase()`
    - `autoPurchase()`

Helper File:

Name:
ID: 6707050____
- `automation.h`
  - Declares all related functions.

### Database and Interface Specialist

Code Focus:

- File: `database.c`
  - Designs and **manages text-based databases** for products, customers, and transactions.
    - `loadDatabase()`
    - `saveDatabase()`
  - **Handles file I/O** for `.txt` or `.csv` files.
  - Defines and **manages structs**.
    - `Product`
    - `Customer`
    - `Transaction`
- File: `interface.c`
  - Creates the **command-line interface**.
    - `displayMenu()`
    - `getMenuSelection()`
  - Handles **user inputs and outputs**
    - `getUserInput()`
    - `displayMessage()`

Helper Files:

- `database.h` and `interface.h`
  - Declare related functions and structs.
