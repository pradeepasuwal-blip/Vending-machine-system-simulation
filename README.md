# 🛒 Inventory & Billing Management System (C++)

A console-based inventory and billing management system written in C++.  
It supports **admin account management**, **product inventory control**, and **customer billing** with receipt generation.

---

##  Features
###  Admin Management
- Create and register admin accounts
- Secure login with 4-digit password
- Password recovery using security answer
- Multiple admins supported

### Inventory Control
- Add, update, delete products
- Track categories and product counts
- Auto-remove expired products
- Search products across admins
- Low/out-of-stock indicators

### Customer Billing
- Browse products by category
- Add items to cart
- Auto-calculated totals
- Receipt generation with timestamp
- Bills stored per admin

###  Validation
- Prevent duplicate product IDs and names
- Input validation for numbers and dates
- Expiry date checks

---

##  Requirements
- C++ compiler (e.g., g++, MinGW, MSVC)
- Windows OS (uses `conio.h` and `system("cls")`)
- Standard C++ libraries: `<iostream>`, `<fstream>`, `<vector>`, `<ctime>`, etc.

---

##  Compilation & Run
# Compile
g++ main.cpp -o inventory

---
## Run
./inventory

##  File Structure
main.cpp → Core program
admin.txt → Stores admin credentials
inventory_<admin>.dat → Binary file storing products for each admin
bills_<admin>.txt → Stores customer bills for each admin
Temporary files (temp_inv.dat, temp_admin.txt, etc.) → Used during updates/deletions

Example Workflow
## Admin Side
1.Register Admin
Create username, 4-digit password, and security answer
2.Login
Enter credentials
3.Manage Inventory
Add products with ID, name, category, price, quantity, expiry date
Update stock or price
Delete products
View inventory by category
Search products

## Customer Side
1.Select Category
Browse available product categories
2.Add to Cart
Choose products and quantities
3.Checkout
System generates receipt with:
Admin name
Customer name
Date & time
Itemized list with totals
4.Bill Storage
Receipt saved in bills_<admin>.txt
### sample output
## inventory view
CATEGORY: [ Snacks ] (3 product(s))
+---------+---------------------+----------+----------+------------------+------------+
|   ID    |     Product Name    | Quantity |  Price   |   Total Value    |   Expiry   |
+---------+---------------------+----------+----------+------------------+------------+
| 101     | Chips               | 10       | 20.00    | 200.00           | 2026/12/31 |
| 102     | Cookies             | 5 [LOW]  | 50.00    | 250.00           | 2026/11/15 |
| 103     | Juice               | 0 [OUT]  | 100.00   | 0.00             | 2026/10/01 |
+---------+---------------------+----------+----------+------------------+------------+

### License
This project is free to use for educational purposes.

# Run
./inventory
