#include<iostream>
#include<fstream>
#include<cstring>
#include<conio.h>
#include<cstdlib>
#include<vector>
#include<limits>
#include<ctime>

using namespace std;

string currentAdmin = "";

struct Product
{
    int id;
    char name[30];
    float price;
    int quantity;
    char category[30];
    int expDay;
    int expMonth;
    int expYear;
};

struct CartItem
{
    char name[30];
    int qty;
    float price;
    float total;
};

int getIntInput(string prompt)
{
    int value;
    if(!prompt.empty()) cout << prompt;
    while(!(cin >> value))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  [!] Invalid! Enter a whole number: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return value;
}

float getFloatInput(string prompt)
{
    float value;
    if(!prompt.empty()) cout << prompt;
    while(!(cin >> value) || value < 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  [!] Invalid! Enter a positive number: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return value;
}

void clearScreen()
{
    system("cls");
}

string inventoryFile(string admin)
{
    if(admin == "") return "inventory_default.dat";
    return "inventory_" + admin + ".dat";
}

string billFile(string admin)
{
    if(admin == "") return "bills_default.txt";
    return "bills_" + admin + ".txt";
}

void printLine(char c, int len)//border
{
    for(int i = 0; i < len; i++) cout << c;
    cout << "\n";
}

void header(string title)//border
{
    cout << "\n";
    printLine('=', 80);
    int padding = (80 - (int)title.length()) / 2;
    if(padding < 0) padding = 0;
    for(int i = 0; i < padding; i++) cout << " ";
    cout << title << "\n";
    printLine('=', 80);
    cout << "\n";
}

void subHeader(string title)//border
{
    cout << "\n";
    printLine('-', 80);
    cout << "  >> " << title << "\n";
    printLine('-', 80);
}

void successMsg(string msg) { cout << "\n  [OK]  " << msg << "\n"; }
void errorMsg(string msg)   { cout << "\n  [!!]  " << msg << "\n"; }
void infoMsg(string msg)    { cout << "\n  [i]   " << msg << "\n"; }

string getPassword()
{
    string pass = "";
    char ch;
    while(true)
    {
        pass = "";
        while((ch = _getch()) != 13)//13 is the ascii value of enter// enter will terminate
        {
            if(ch == 8 && !pass.empty())// 8 is the ascii code for backspace
            {
                pass.pop_back();
                cout << "\b \b";
            }
            else if(isdigit(ch) && pass.length() < 4)//checks the digit (0-9)
            {
                pass += ch;
                cout << "*";
            }
        }
        cout << "\n";
        if(pass.length() == 4)
            return pass;
        else
            cout << "  [!] Password must be exactly 4 digits! Try again.\n  Enter Password: ";
    }
}

bool usernameExists(string username)
{
    ifstream file("admin.txt");
    string user, pass, sec;// sec=security password
    while(file >> user >> pass >> sec)
    {
        if(user == username) //remake
        return true;
    }
    return false;//attempts remaining will show 
}

int listAllAdmins(vector<string>* adminList)//vector ma call pass garya xa vanae matra admin name store huni vayo else null , and it will return int 
{
    ifstream file("admin.txt");
    if(!file)
    {
        infoMsg("No admin accounts registered yet.");
        return 0;
    }
    string user, pass, sec;
    int count = 0;
    cout << "\n  +-----+------------------------+\n";
    cout << "  | No. |    Admin Username      |\n";
    cout << "  +-----+------------------------+\n";
    while(file >> user >> pass >> sec)
    {
        count++;
        if(adminList) adminList->push_back(user);//admin name input , vector ma push call vaxa vanae store huni vayo (addd garxa admin) and allows admin selection
        cout << "  | " << count;//prints the count number then adds space to keep columns aligned
        for(int i = to_string(count).length(); i < 3; i++) cout << " ";    
        cout << " | " << user;//same as count but for username
        for(int i = user.length(); i < 22; i++) cout << " ";//this is just for alignment to make the tables aligned
        cout << " |\n";
    }
    if(count == 0)
        cout << "  |       No admins found.         |\n";//file bhitra admin xaina vanae

    cout << "  +-----+------------------------+\n";
    file.close();
    return count;
}

bool registerAdmin()
{
    clearScreen();
    header("CREATE ADMIN ACCOUNT");

    string user, pass, sec;

    while(true)//
    {
        cout << "  Enter Username          : ";
        cin >> user;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if(usernameExists(user))
            errorMsg("Username already exists! Try a different one.");
        else
            break;
    }

    cout << "  Enter 4-Digit Password  : ";
    pass = getPassword();

    cout << "  Enter Security Answer   : ";
    cin >> sec;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    ofstream file("admin.txt", ios::app);//append is ka dekhi left gareko tya bata nai begin garxa
    file << user << " " << pass << " " << sec << "\n";
    file.close();

    successMsg("Admin account created successfully!");
    system("pause");
    return true;
}

void forgotPassword(string knownUser)
{
    clearScreen();
    header("FORGOT PASSWORD");

    string user, fuser, fpass, fsec, ans, newpass;
    bool found = false;

    if(knownUser != "")//usernmae wrong haleko bela
    {
        user = knownUser;
        cout << "  Username : " << user << "  (from failed login)\n\n";
    }
    else//username is correct
    {
        cout << "  Enter Username : ";
        cin >> user;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    ifstream file("admin.txt");//read
    ofstream temp("temp_admin.txt");//write

    while(file >> fuser >> fpass >> fsec)
    {
        if(user == fuser)
        {
            found = true;
            cout << "  Security Answer : ";
            cin >> ans;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');//limit header bata ayo
//input lirakhxa until the enter key is not pressed
//once the enter is clicked it reads the data that we have pressed
            if(ans == fsec)
            {
                cout << "  New 4-Digit Password : ";
                newpass = getPassword();
                temp << fuser << " " << newpass << " " << fsec << "\n";
                successMsg("Password updated! You can now login with your new password.");
            }
            else
            {
                temp << fuser << " " << fpass << " " << fsec << "\n";
                errorMsg("Wrong security answer! Password not changed.");
            }
        }
        else
            temp << fuser << " " << fpass << " " << fsec << "\n";
    }

    file.close();
    temp.close();
    remove("admin.txt");
    rename("temp_admin.txt", "admin.txt");

    if(!found) errorMsg("Username not found!");
    system("pause");
}

bool loginAdmin()
{
    int attempts = 3;
    string lastUser = "";

    while(attempts > 0)
    {
        clearScreen();
        header("ADMIN LOGIN");

        string user, pass, fuser, fpass, fsec;

        cout << "  Attempts remaining : " << attempts << "\n\n";
        cout << "  Username : ";
        cin >> user;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        lastUser = user;//last attempt ko usernmae save garxa

        cout << "  Password : ";
        pass = getPassword();

        ifstream file("admin.txt");
        bool matched = false;
        while(file >> fuser >> fpass >> fsec)
        {
            if(user == fuser && pass == fpass)
            {
                matched = true;
                currentAdmin = fuser;
                file.close();
                return true;
            }
        }
        file.close();

        attempts--;
        if(attempts > 0)
        {
            errorMsg("Wrong credentials! Attempts left: " + to_string(attempts));
            system("pause");
        }
    }

    clearScreen();
    header("LOGIN FAILED");
    errorMsg("3 failed attempts!");
    cout << "\n  Would you like to reset your password using your security answer?\n";
    cout << "  (y/n): ";
    char f;
    cin >> f;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if(f == 'y' || f == 'Y')
        forgotPassword(lastUser);

    return false;
}

bool checkDuplicateID(int id, string admin)
{
    Product p;
    ifstream file(inventoryFile(admin), ios::binary);
    while(file.read((char*)&p, sizeof(p)))
    {
        if(p.id == id) { file.close();
             return true; }
    }
    file.close();
    return false;
}

bool checkDuplicateName(string name, string admin)
{
    Product p;
    ifstream file(inventoryFile(admin), ios::binary);
    while(file.read((char*)&p, sizeof(p)))
    {
        if(name == p.name) { file.close(); return true; }
    }
    file.close();
    return false;
}

bool isExpired(const Product& p)
{
    time_t now = time(0);
    tm* lt = localtime(&now);
    int todayYear  = lt->tm_year + 1900;
    int todayMonth = lt->tm_mon + 1;
    int todayDay   = lt->tm_mday;

    if(p.expYear < todayYear) return true;
    if(p.expYear == todayYear && p.expMonth < todayMonth) return true;
    if(p.expYear == todayYear && p.expMonth == todayMonth && p.expDay < todayDay) return true;
    return false;
}

int removeExpiredProducts(string admin)
{
    Product p;
    ifstream fin(inventoryFile(admin), ios::binary);
    if(!fin) return 0;

    ofstream fout("temp_exp.dat", ios::binary);
    int removed = 0;

    while(fin.read((char*)&p, sizeof(p)))
    {
        if(isExpired(p))
            removed++;
        else
            fout.write((char*)&p, sizeof(p));
    }
    fin.close();
    fout.close();

    remove(inventoryFile(admin).c_str());
    rename("temp_exp.dat", inventoryFile(admin).c_str());
    return removed;
}

string expiryStr(const Product& p)
{
    char buf[20];
    sprintf(buf, "%04d/%02d/%02d", p.expYear, p.expMonth, p.expDay);
    return string(buf);
}

vector<string> getCategories(string admin)
{
    vector<string> cats;
    Product p;
    ifstream file(inventoryFile(admin), ios::binary);
    while(file.read((char*)&p, sizeof(p)))
    {
        string cat = p.category;
        bool found = false;
        for(int i = 0; i < (int)cats.size(); i++)
            if(cats[i] == cat) { found = true; break; }
        if(!found) cats.push_back(cat);
    }
    file.close();
    return cats;
}

int countProductsInCategory(string admin, string category)
{
    Product p;
    int count = 0;
    ifstream file(inventoryFile(admin), ios::binary);
    while(file.read((char*)&p, sizeof(p)))
    {
        if(string(p.category) == category) count++;
    }
    file.close();
    return count;
}

void viewProductsAdmin(string admin)
{
    int removed = removeExpiredProducts(admin);
    if(removed > 0)
        cout << "\n  [AUTO]  " << removed << " expired product(s) removed from inventory.\n";

    clearScreen();
    header("PRODUCT INVENTORY  [ Admin: " + admin + " ]");

    Product p;
    ifstream file(inventoryFile(admin), ios::binary);

    if(!file)
    {
        infoMsg("No inventory file found. Add products first.");
        return;
    }

    vector<string> cats = getCategories(admin);

    if(cats.empty())
    {
        infoMsg("No products in inventory yet.");
        file.close();
        return;
    }

    cout << "  Total Categories : " << cats.size() << "\n";
    int totalProducts = 0;
    for(int c = 0; c < (int)cats.size(); c++)
        totalProducts += countProductsInCategory(admin, cats[c]);
    cout << "  Total Products   : " << totalProducts << "\n\n";

    for(int c = 0; c < (int)cats.size(); c++)
    {
        int catCount = countProductsInCategory(admin, cats[c]);
        cout << "\n  CATEGORY: [ " << cats[c] << " ]  (" << catCount << " product(s))\n";
        cout << "  +---------+---------------------+----------+----------+------------------+------------+\n";
        cout << "  |   ID    |     Product Name    | Quantity |  Price   |   Total Value    |   Expiry   |\n";
        cout << "  +---------+---------------------+----------+----------+------------------+------------+\n";

        bool hasAny = false;
        file.clear();
        file.seekg(0, ios::beg);

        while(file.read((char*)&p, sizeof(p)))
        {
            if(string(p.category) != cats[c]) continue;
            hasAny = true;

            string lowFlag = (p.quantity <= 5 && p.quantity > 0) ? " [LOW]" : "";
            string outFlag = (p.quantity == 0) ? " [OUT]" : "";

            string idStr = to_string(p.id);
            cout << "  | " << idStr;
            for(int i = idStr.length(); i < 7; i++) cout << " ";
            cout << " | ";

            string name = p.name;
            if(name.length() > 19) name = name.substr(0, 19);
            cout << name;
            for(int i = name.length(); i < 19; i++) cout << " ";
            cout << " | ";

            string qStr = to_string(p.quantity) + lowFlag + outFlag;
            cout << qStr;
            for(int i = qStr.length(); i < 8; i++) cout << " ";
            cout << " | ";

            char priceBuf[20];
            sprintf(priceBuf, "%.2f", p.price);
            string priceStr = priceBuf;
            cout << priceStr;
            for(int i = priceStr.length(); i < 8; i++) cout << " ";
            cout << " | ";

            float totalVal = p.price * p.quantity;
            char tvBuf[20];
            sprintf(tvBuf, "%.2f", totalVal);
            string tvStr = tvBuf;
            cout << tvStr;
            for(int i = tvStr.length(); i < 16; i++) cout << " ";
            cout << " | ";

            string exp = expiryStr(p);
            cout << exp;
            for(int i = exp.length(); i < 10; i++) cout << " ";
            cout << " |\n";
        }

        if(!hasAny)
            cout << "  |                         No products in this category.                          |\n";

        cout << "  +---------+---------------------+----------+----------+------------------+------------+\n";
    }

    file.close();
}

void viewProductsByCategory(string admin, string category)
{
    Product p;
    ifstream file(inventoryFile(admin), ios::binary);

    int catCount = countProductsInCategory(admin, category);
    cout << "\n  CATEGORY: [ " << category << " ]  (" << catCount << " product(s))\n";
    cout << "  +---------+---------------------+----------+----------------+\n";
    cout << "  |   ID    |     Product Name    | Quantity |  PerUnit (Rs.) |\n";
    cout << "  +---------+---------------------+----------+----------------+\n";

    bool hasAny = false;
    while(file.read((char*)&p, sizeof(p)))
    {
        if(string(p.category) != category) continue;
        hasAny = true;

        string name = p.name;
        if(name.length() > 19) name = name.substr(0, 19);

        cout << "  | " << p.id;
        for(int i = to_string(p.id).length(); i < 7; i++) cout << " ";
        cout << " | ";

        cout << name;
        for(int i = name.length(); i < 19; i++) cout << " ";
        cout << " | ";

        cout << p.quantity;
        for(int i = to_string(p.quantity).length(); i < 8; i++) cout << " ";
        cout << " | ";

        char priceBuf[20];
        sprintf(priceBuf, "%.2f", p.price);
        string priceStr = priceBuf;
        cout << priceStr;
        for(int i = priceStr.length(); i < 12; i++) cout << " ";
        cout << " |\n";
    }

    if(!hasAny)
        cout << "  |             No products in this category.              |\n";

    cout << "  +---------+---------------------+----------+----------------+\n\n";
    file.close();
}
string selectOrCreateCategory(string admin)
{
    vector<string> cats = getCategories(admin);

    cout << "\n  -- Select Category --\n";

    if(!cats.empty())
    {
        cout << "\n  Existing categories:\n";
        for(int i = 0; i < (int)cats.size(); i++)
            cout << "  [" << (i+1) << "] " << cats[i] << "\n";
        cout << "  [" << (cats.size()+1) << "] Create new category\n\n";

        int ch;
        while(true)  // keep looping until valid input
        {
            ch = getIntInput("  Choose: ");

            if(ch >= 1 && ch <= (int)cats.size())
                return cats[ch - 1];
            else if(ch == (int)cats.size() + 1)
                break;  // proceed to create new category
            else
                errorMsg("Invalid choice. Enter a number from 1 to " 
                         + to_string(cats.size() + 1) + ".");
        }
    }
    else
    {
        cout << "  No categories yet. You must create one.\n";
    }

    char newCat[30];
    cout << "  Enter New Category Name : ";
    cin >> newCat;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return string(newCat);
}

string selectCategoryCustomer(string admin)
{
    vector<string> cats = getCategories(admin);

    if(cats.empty()) return "";

    cout << "\n";
    printLine('-', 80);
    cout << "  SELECT A CATEGORY\n";
    printLine('-', 80);
    for(int i = 0; i < (int)cats.size(); i++)
    {
        int count = countProductsInCategory(admin, cats[i]);
        cout << "  [" << (i+1) << "] " << cats[i];
        for(int j = cats[i].length(); j < 25; j++) cout << " ";
        cout << "(" << count << " product(s))\n";
    }
    printLine('-', 80);

    int ch;
    while(true)
    {
        ch = getIntInput("  Choose category: ");
        if(ch >= 1 && ch <= (int)cats.size()) break;
        errorMsg("Invalid choice. Please pick a number from the list.");
    }
    return cats[ch - 1];
}

void getExpiryDate(Product& p)
{
    time_t now = time(0);
    tm* lt = localtime(&now);
    int todayYear  = lt->tm_year + 1900;
    int todayMonth = lt->tm_mon + 1;
    int todayDay   = lt->tm_mday;

    while(true)
    {
        cout << "  Expiry Date (YYYY/MM/DD) : ";
        string input;
        cin >> input;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        int yr = 0, mo = 0, dy = 0;
        bool validFormat = false;

        if(input.length() == 10 && input[4] == '/' && input[7] == '/')
        {
            string yearPart  = input.substr(0, 4);
            string monthPart = input.substr(5, 2);
            string dayPart   = input.substr(8, 2);

            bool allDigits = true;
            for(int i = 0; i < (int)(yearPart + monthPart + dayPart).length(); i++)
                if(!isdigit((yearPart + monthPart + dayPart)[i])) { allDigits = false; break; }

            if(allDigits)
            {
                yr = atoi(yearPart.c_str());
                mo = atoi(monthPart.c_str());
                dy = atoi(dayPart.c_str());
                validFormat = true;
            }
        }

        if(!validFormat)
        {
            errorMsg("Invalid format! Use YYYY/MM/DD  e.g. 2026/12/31");
            continue;
        }

        if(mo < 1 || mo > 12 || dy < 1 || dy > 31)
        {
            errorMsg("Invalid date values. Month: 01-12, Day: 01-31");
            continue;
        }

        if(yr < todayYear ||
          (yr == todayYear && mo < todayMonth) ||
          (yr == todayYear && mo == todayMonth && dy < todayDay))
        {
            errorMsg("Expiry date is already in the past! Enter a future date.");
            continue;
        }

        p.expYear  = yr;
        p.expMonth = mo;
        p.expDay   = dy;
        break;
    }
}

void addProduct()
{
    clearScreen();
    header("ADD NEW PRODUCT");

    Product p;

    string cat = selectOrCreateCategory(currentAdmin);
    strncpy(p.category, cat.c_str(), 29);
    p.category[29] = '\0';
    cout << "  Category set to: " << p.category << "\n\n";

    while(true)
    {
        p.id = getIntInput("  Enter Product ID   : ");
        if(p.id <= 0) { errorMsg("ID must be a positive number."); continue; }
        if(checkDuplicateID(p.id, currentAdmin)) 
        { errorMsg("Duplicate ID! That ID already exists."); 
            continue; }
        break;
    }

    while(true)
    {
        cout << "  Enter Product Name  : ";
        cin >> p.name;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if(checkDuplicateName(p.name, currentAdmin))
         { errorMsg("A product with that name already exists!"); 
            continue; }
        break;
    }

    p.price = getFloatInput("  Enter Price (Rs.)  : ");

    while(true)
    {
        p.quantity = getIntInput("  Enter Quantity     : ");
        if(p.quantity < 0) { errorMsg("Quantity cannot be negative."); continue; }
        break;
    }

    getExpiryDate(p);

    ofstream file(inventoryFile(currentAdmin), ios::binary | ios::app);
    file.write((char*)&p, sizeof(p));
    file.close();

    successMsg("Product added successfully!");
    system("pause");
}

void updateStock()
{
    clearScreen();
    header("UPDATE STOCK");
    viewProductsAdmin(currentAdmin);
    cout << "\n";

    Product p;
    bool found = false;
    int id = getIntInput("  Enter Product ID to update stock : ");

    fstream file(inventoryFile(currentAdmin), ios::binary | ios::in | ios::out);
    while(file.read((char*)&p, sizeof(p)))
    {
        if(p.id == id)
        {
            found = true;
            subHeader("Product Found");
            cout << "  Name          : " << p.name << "\n";
            cout << "  Current Stock : " << p.quantity << "\n\n";

            while(true)
            {
                p.quantity = getIntInput("  Enter New Quantity : ");
                if(p.quantity < 0) { errorMsg("Quantity cannot be negative."); continue; }
                break;
            }

            streampos pos = file.tellg();
            file.seekp(pos - streampos(sizeof(p)));
            file.write((char*)&p, sizeof(p));
            break;
        }
    }
    file.close();

    if(found) successMsg("Stock updated!");
    else      errorMsg("Product not found.");
    system("pause");
}

void updatePrice()
{
    clearScreen();
    header("UPDATE PRICE");
    viewProductsAdmin(currentAdmin);
    cout << "\n";

    Product p;
    bool found = false;
    int id = getIntInput("  Enter Product ID to update price : ");

    fstream file(inventoryFile(currentAdmin), ios::binary | ios::in | ios::out);
    while(file.read((char*)&p, sizeof(p)))
    {
        if(p.id == id)
        {
            found = true;
            subHeader("Product Found");
            cout << "  Name          : " << p.name << "\n";
            cout << "  Current Price : Rs. " << p.price << "\n\n";

            p.price = getFloatInput("  Enter New Price   : ");

            streampos pos = file.tellg();
            file.seekp(pos - streampos(sizeof(p)));
            file.write((char*)&p, sizeof(p));
            break;
        }
    }
    file.close();

    if(found) successMsg("Price updated!");
    else      errorMsg("Product not found.");
    system("pause");
}

void deleteProduct()
{
    clearScreen();
    header("DELETE PRODUCT");
    viewProductsAdmin(currentAdmin);
    cout << "\n";

    Product p;
    bool found = false;
    int id = getIntInput("  Enter Product ID to delete : ");

    ifstream fin(inventoryFile(currentAdmin), ios::binary);
    ofstream fout("temp_inv.dat", ios::binary);

    while(fin.read((char*)&p, sizeof(p)))
    {
        if(p.id != id)
            fout.write((char*)&p, sizeof(p));
        else
        {
            found = true;
            cout << "  Deleting: " << p.name << "\n";
        }
    }
    fin.close();
    fout.close();

    remove(inventoryFile(currentAdmin).c_str());
    rename("temp_inv.dat", inventoryFile(currentAdmin).c_str());

    if(found) successMsg("Product deleted.");
    else      errorMsg("Product not found.");
    system("pause");
}

void searchProduct()
{
    clearScreen();
    header("SEARCH PRODUCT");

    cout << "  Enter product name to search : ";
    string keyword;
    cin >> keyword;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string keyLower = keyword;
    for(int i = 0; i < (int)keyLower.length(); i++) keyLower[i] = tolower(keyLower[i]);

    vector<string> adminList;
    int count = listAllAdmins(&adminList);
    if(count == 0)
    {
        infoMsg("No admin accounts exist yet.");
        system("pause");
        return;
    }

    int adminChoice;
    while(true)
    {
        adminChoice = getIntInput("\n  Enter Admin Number to search in : ");
        if(adminChoice >= 1 && adminChoice <= count) break;
        errorMsg("Invalid number. Choose between 1 and " + to_string(count) + ".");
    }

    string chosenAdmin = adminList[adminChoice - 1];

    clearScreen();
    header("SEARCH RESULTS  [ Admin: " + chosenAdmin + " ]");
    cout << "  Keyword: \"" << keyword << "\"\n\n";

    Product p;
    ifstream file(inventoryFile(chosenAdmin), ios::binary);
    bool anyFound = false;

    cout << "  +---------+---------------------+----------+----------------+-----------+\n";
    cout << "  |   ID    |     Product Name    | Quantity |  PerUnit (Rs.) | Category  |\n";
    cout << "  +---------+---------------------+----------+----------------+-----------+\n";

    while(file.read((char*)&p, sizeof(p)))
    {
        string pname = p.name;
        string pnameLower = pname;
        for(int i = 0; i < (int)pnameLower.length(); i++) pnameLower[i] = tolower(pnameLower[i]);

        if(pnameLower.find(keyLower) != string::npos)
        {
            anyFound = true;

            string dispName = pname;
            if(dispName.length() > 19) dispName = dispName.substr(0, 19);

            string catStr = p.category;
            if(catStr.length() > 9) catStr = catStr.substr(0, 9);

            cout << "  | " << p.id;
            for(int i = to_string(p.id).length(); i < 7; i++) cout << " ";
            cout << " | ";

            cout << dispName;
            for(int i = dispName.length(); i < 19; i++) cout << " ";
            cout << " | ";

            string avail = (p.quantity == 0) ? "OUT" : to_string(p.quantity);
            cout << avail;
            for(int i = avail.length(); i < 8; i++) cout << " ";
            cout << " | ";

            char priceBuf[20];
            sprintf(priceBuf, "%.2f", p.price);
            string priceStr = priceBuf;
            cout << priceStr;
            for(int i = priceStr.length(); i < 14; i++) cout << " ";
            cout << " | ";

            cout << catStr;
            for(int i = catStr.length(); i < 9; i++) cout << " ";
            cout << " |\n";
        }
    }

    if(!anyFound)
        cout << "  |              No matching products found.                               |\n";

    cout << "  +---------+---------------------+----------+----------------+-----------+\n";

    file.close();
    system("pause");
}

void printReceipt(string customer, vector<CartItem>& cart, float grandTotal)
{
    time_t now = time(0);
    char timeBuf[30];
    strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d  %H:%M:%S", localtime(&now));

    clearScreen();

    cout << "\n";
    printLine('*', 80);
    cout << "                            RECEIPT\n";
    printLine('*', 80);
    cout << "  Admin    : " << currentAdmin << "\n";
    cout << "  Customer : " << customer << "\n";
    cout << "  Date     : " << timeBuf << "\n";
    printLine('-', 80);
    cout << "  +-----------------------+-------+------------+------------+\n";
    cout << "  | Item                  |  Qty  |  Price(Rs.)|  Total(Rs.)|\n";
    cout << "  +-----------------------+-------+------------+------------+\n";

    for(int i = 0; i < (int)cart.size(); i++)
    {
        string iname = cart[i].name;
        if(iname.length() > 21) iname = iname.substr(0, 21);

        cout << "  | " << iname;
        for(int j = iname.length(); j < 21; j++) cout << " ";
        cout << " | ";

        string qStr = to_string(cart[i].qty);
        cout << qStr;
        for(int j = qStr.length(); j < 5; j++) cout << " ";
        cout << " | ";

        char pb[15], tb[15];
        sprintf(pb, "%.2f", cart[i].price);
        sprintf(tb, "%.2f", cart[i].total);
        string ps = pb, ts = tb;

        cout << ps;
        for(int j = ps.length(); j < 10; j++) cout << " ";
        cout << " | ";

        cout << ts;
        for(int j = ts.length(); j < 10; j++) cout << " |\n";
    }

    cout << "  +-----------------------+-------+------------+------------+\n";
    char gb[15];
    sprintf(gb, "%.2f", grandTotal);
    cout << "  | GRAND TOTAL                               | Rs. " << gb;
    for(int j = string(gb).length(); j < 6; j++) cout << " ";
    cout << "     |\n";
    cout << "  +-----------------------+-------+------------+------------+\n";
    printLine('*', 80);
    cout << "          Thank you for your purchase!\n";
    printLine('*', 80);

    ofstream bill(billFile(currentAdmin), ios::app);
    bill << "\n";
    bill << "================================================================================\n";
    bill << "                                  RECEIPT\n";
    bill << "================================================================================\n";
    bill << "  Admin    : " << currentAdmin << "\n";
    bill << "  Customer : " << customer << "\n";
    bill << "  Date     : " << timeBuf << "\n";
    bill << "--------------------------------------------------------------------------------\n";
    bill << "  +-----------------------+-------+------------+------------+\n";
    bill << "  | Item                  |  Qty  |  Price(Rs.)|  Total(Rs.)|\n";
    bill << "  +-----------------------+-------+------------+------------+\n";

    for(int i = 0; i < (int)cart.size(); i++)
    {
        string iname = cart[i].name;
        if(iname.length() > 21) iname = iname.substr(0, 21);

        bill << "  | " << iname;
        for(int j = iname.length(); j < 21; j++) bill << " ";
        bill << " | ";

        string qStr = to_string(cart[i].qty);
        bill << qStr;
        for(int j = qStr.length(); j < 5; j++) bill << " ";
        bill << " | ";

        char pb[15], tb[15];
        sprintf(pb, "%.2f", cart[i].price);
        sprintf(tb, "%.2f", cart[i].total);
        string ps = pb, ts = tb;

        bill << ps;
        for(int j = ps.length(); j < 10; j++) bill << " ";
        bill << " | ";

        bill << ts;
        for(int j = ts.length(); j < 10; j++) bill << " ";
        bill << " |\n";
    }

    bill << "  +-----------------------+-------+------------+------------+\n";
    bill << "  | GRAND TOTAL                               | Rs. " << gb;
    for(int j = string(gb).length(); j < 6; j++) bill << " ";
    bill << "     |\n";
    bill << "  +-----------------------+-------+------------+------------+\n";
    bill << "================================================================================\n";
    bill << "GRAND TOTAL: " << grandTotal << "\n";
    bill.close();
}

void placeOrder()
{
    clearScreen();
    header("PLACE ORDER  [ Admin: " + currentAdmin + " ]");

    string customer;
    cout << "  Customer Name : ";
    cin >> customer;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<CartItem> cart;
    float grandTotal = 0;

    while(true)
    {
        removeExpiredProducts(currentAdmin);

        clearScreen();
        header("PLACE ORDER  [ Admin: " + currentAdmin + " ]");

        if(!cart.empty())
        {
            subHeader("Current Cart");
            cout << "  +-----------------------+-------+------------+\n";
            cout << "  | Item                  |  Qty  | Subtotal   |\n";
            cout << "  +-----------------------+-------+------------+\n";
            for(int i = 0; i < (int)cart.size(); i++)
            {
                string iname = cart[i].name;
                if(iname.length() > 21) iname = iname.substr(0, 21);
                cout << "  | " << iname;
                for(int j = iname.length(); j < 21; j++) cout << " ";

                string qStr = to_string(cart[i].qty);
                cout << " | " << qStr;
                for(int j = qStr.length(); j < 5; j++) cout << " ";

                char tb[15]; sprintf(tb, "%.2f", cart[i].total);
                string ts = tb;
                cout << " | " << ts;
                for(int j = ts.length(); j < 10; j++) cout << " ";
                cout << " |\n";
            }
            cout << "  +-----------------------+-------+------------+\n";
            char gb[15]; sprintf(gb, "%.2f", grandTotal);
            cout << "  Running Total : Rs. " << gb << "\n";
            printLine('-', 80);
        }

        cout << "\n  [1] Order a product\n";
        cout << "  [2] Show receipt\n";
        cout << "  [3] Cancel order\n\n";

        int action = getIntInput("  Choose: ");
        if(action == 1)
        {
            vector<string> cats = getCategories(currentAdmin);
            if(cats.empty())
            {
                errorMsg("No products available.");
                system("pause");
                continue;
            }

            string chosenCat = selectCategoryCustomer(currentAdmin);
            if(chosenCat == "")
            {
                errorMsg("No categories found.");
                system("pause");
                continue;
            }

            clearScreen();
            header("PLACE ORDER  [ Admin: " + currentAdmin + " ]");
            viewProductsByCategory(currentAdmin, chosenCat);

            int id = getIntInput("  Enter Product ID : ");

            Product p;
            bool found = false;

            fstream file(inventoryFile(currentAdmin), ios::binary | ios::in | ios::out);
            while(file.read((char*)&p, sizeof(p)))
            {
                if(p.id == id)
                {
                    if(string(p.category) != chosenCat)
                    {
                        errorMsg("That product is not in the selected category!");
                        file.close();
                        system("pause");
                        found = true;
                        break;
                    }

                    found = true;
                    cout << "\n  Product  : " << p.name << "\n";
                    cout << "  Category : " << p.category << "\n";
                    cout << "  Price    : Rs. " << p.price << "\n";
                    cout << "  In Stock : " << p.quantity << "\n\n";

                    if(p.quantity == 0)
                    {
                        errorMsg("This product is out of stock!");
                        file.close();
                        system("pause");
                        break;
                    }

                    int qty;
                    while(true)
                    {
                        qty = getIntInput("  Quantity : ");
                        if(qty <= 0) { errorMsg("Quantity must be at least 1."); continue; }
                        if(qty > p.quantity)
                        {
                            errorMsg("Only " + to_string(p.quantity) + " in stock!");
                            continue;
                        }
                        break;
                    }

                    float itemTotal = qty * p.price;

                    p.quantity -= qty;
                    streampos pos = file.tellg();
                    file.seekp(pos - streampos(sizeof(p)));
                    file.write((char*)&p, sizeof(p));

                    bool merged = false;
                    for(int i = 0; i < (int)cart.size(); i++)
                    {
                        if(string(cart[i].name) == string(p.name))
                        {
                            cart[i].qty   += qty;
                            cart[i].total += itemTotal;
                            grandTotal    += itemTotal;
                            merged = true;
                            char tb[15]; sprintf(tb, "%.2f", itemTotal);
                            successMsg(string(p.name) + " x" + to_string(qty) + " merged in cart  (Rs. " + tb + ")");
                            break;
                        }
                    }

                    if(!merged)
                    {
                        grandTotal += itemTotal;
                        CartItem ci;
                        strncpy(ci.name, p.name, 30);
                        ci.qty   = qty;
                        ci.price = p.price;
                        ci.total = itemTotal;
                        cart.push_back(ci);
                        char tb[15]; sprintf(tb, "%.2f", itemTotal);
                        successMsg(string(p.name) + " x" + to_string(qty) + " added  (Rs. " + tb + ")");
                    }

                    system("pause");
                    break;
                }
            }
            file.close();
            if(!found)
            {
                errorMsg("Product not found.");
                system("pause");
            }
        }
        else if(action == 2)
        {
            if(cart.empty())
            {
                errorMsg("Cart is empty! Add at least one product.");
                system("pause");
                continue;
            }
            printReceipt(customer, cart, grandTotal);
            system("pause");
            break;
        }
        else if(action == 3)
        {
            for(int i = 0; i < (int)cart.size(); i++)
            {
                Product p;
                fstream file(inventoryFile(currentAdmin), ios::binary | ios::in | ios::out);
                while(file.read((char*)&p, sizeof(p)))
                {
                    if(string(p.name) == string(cart[i].name))
                    {
                        p.quantity += cart[i].qty;
                        streampos pos = file.tellg();
                        file.seekp(pos - streampos(sizeof(p)));
                        file.write((char*)&p, sizeof(p));
                        break;
                    }
                }
                file.close();
            }
            infoMsg("Order cancelled. All stock restored.");
            system("pause");
            break;
        }
        else
        {
            errorMsg("Invalid choice.");
            system("pause");
        }
    }
}
void viewBillsByDate(string admin)
{
    clearScreen();
    header("VIEW BILLS BY DATE  [ Admin: " + admin + " ]");

    cout << "  Enter date to search (YYYY-MM-DD) : ";
    string searchDate;
    cin >> searchDate;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    ifstream file(billFile(admin));
    if(!file)
    {
        infoMsg("No bill records found.");
        system("pause");
        return;
    }
    string line;
    bool inBlock = false;
    bool blockMatches = false;
    vector<string> currentBlock;
    bool anyPrinted = false;

    while(getline(file, line))
    {
        if(line.rfind("GRAND TOTAL:", 0) == 0)
        {
            if(inBlock && blockMatches)
            {
                for(int i = 0; i < (int)currentBlock.size(); i++)
                    cout << currentBlock[i] << "\n";
                anyPrinted = true;
            }
            inBlock = false;
            blockMatches = false;
            currentBlock.clear();
            continue;
        }

        if(line.find("================================================================================") != string::npos && !inBlock)
        {
            inBlock = true;
            blockMatches = false;
            currentBlock.clear();
        }

        if(inBlock)
        {
            currentBlock.push_back(line);
            // Check if this line contains the date we want
            if(line.find("Date") != string::npos && line.find(searchDate) != string::npos)
                blockMatches = true;
        }
    }

    if(inBlock && blockMatches)
    {
        for(int i = 0; i < (int)currentBlock.size(); i++)
            cout << currentBlock[i] << "\n";
        anyPrinted = true;
    }

    file.close();

    if(!anyPrinted)
        infoMsg("No receipts found for date: " + searchDate);

    system("pause");
}

void billsAndEarningsPanel()
{
    int ch;
    do {
        clearScreen();
        header("BILLS & EARNINGS  [ Admin: " + currentAdmin + " ]");

        cout << "  [1]  View All Bill Records\n";
        cout << "  [2]  View Bills by Date\n";       
        cout << "  [3]  View Total Earnings\n";
        cout << "  [4]  Back\n\n";

        ch = getIntInput("  Choose: ");

        if(ch == 1)
        {
            clearScreen();
            header("BILL RECORDS  [ Admin: " + currentAdmin + " ]");

            ifstream file(billFile(currentAdmin));
            if(!file)
            {
                infoMsg("No bill records found.");
                system("pause");
                continue;
            }

            string line;
            bool hasRecords = false;
            while(getline(file, line))
            {
                if(line.rfind("GRAND TOTAL:", 0) == 0) continue;
                hasRecords = true;
                cout << line << "\n";
            }
            if(!hasRecords) infoMsg("No records yet.");
            file.close();
            system("pause");
        }
        else if(ch == 2)
        {
            viewBillsByDate(currentAdmin);   
        }
        else if(ch == 3)
        {
            clearScreen();
            header("TOTAL EARNINGS  [ Admin: " + currentAdmin + " ]");

            ifstream file(billFile(currentAdmin));
            if(!file)
            {
                infoMsg("No records found. Earnings = Rs. 0.00");
                system("pause");
                continue;
            }

            float earn = 0;
            int receiptCount = 0;
            string line;
            while(getline(file, line))
            {
                if(line.rfind("GRAND TOTAL:", 0) == 0)
                {
                    receiptCount++;
                    float val = 0;
                    sscanf(line.c_str(), "GRAND TOTAL: %f", &val);
                    earn += val;
                }
            }
            file.close();

            char buf[30]; sprintf(buf, "%.2f", earn);
            if(receiptCount > 0)
            {
                cout << "  Total Receipts : " << receiptCount << "\n";
                cout << "  Total Earnings : Rs. " << buf << "\n\n";
            }
            else
                infoMsg("No completed receipts yet. Earnings = Rs. 0.00");

            system("pause");
        }
        else if(ch == 4)
            break;
        else
        {
            errorMsg("Invalid choice.");
            system("pause");
        }
    } while(ch != 4);
}

void inventoryMenu()
{
    int ch;
    do {
        clearScreen();
        header("INVENTORY MANAGEMENT  [ Admin: " + currentAdmin + " ]");

        cout << "  [1]  Add Product\n";
        cout << "  [2]  View Products\n";
        cout << "  [3]  Update Stock\n";
        cout << "  [4]  Update Price\n";
        cout << "  [5]  Delete Product\n";
        cout << "  [6]  Back\n\n";

        ch = getIntInput("  Choose: ");

        switch(ch)
        {
            case 1: addProduct();    break;
            case 2:
                viewProductsAdmin(currentAdmin);
                system("pause");
                break;
            case 3: updateStock();   break;
            case 4: updatePrice();   break;
            case 5: deleteProduct(); break;
            case 6: break;
            default: errorMsg("Invalid choice."); system("pause");
        }
    } while(ch != 6);
}

void adminDashboard()
{
    int ch;
    do {
        clearScreen();
        header("ADMIN DASHBOARD  [ " + currentAdmin + " ]");

        cout << "  [1]  Inventory Management\n";
        cout << "  [2]  Bills & Earnings\n";
        cout << "  [3]  Logout\n\n";

        ch = getIntInput("  Choose: ");

        switch(ch)
        {
            case 1: inventoryMenu();         break;
            case 2: billsAndEarningsPanel(); break;
            case 3: break;
            default: errorMsg("Invalid choice."); system("pause");
        }
    } while(ch != 3);
}

void customerMenu()
{
    int ch;
    do {
        clearScreen();
        header("CUSTOMER PANEL");

        cout << "  [1]  Show All Admins\n";
        cout << "  [2]  Choose Admin & Shop\n";
        cout << "  [3]  Search Product\n";
        cout << "  [4]  Exit to Main Menu\n\n";

        ch = getIntInput("  Choose: ");

        if(ch == 1)
        {
            clearScreen();
            header("REGISTERED ADMINS");
            int count = listAllAdmins(NULL);
            if(count == 0)
                infoMsg("No admins registered yet.");
            system("pause");
        }
        else if(ch == 2)
        {
            clearScreen();
            header("CHOOSE ADMIN TO SHOP FROM");

            vector<string> adminList;
            int count = listAllAdmins(&adminList);
            if(count == 0)
            {
                infoMsg("No admin accounts exist yet.");
                system("pause");
                continue;
            }

            int adminChoice;
            while(true)
            {
                adminChoice = getIntInput("\n  Enter Admin Number : ");
                if(adminChoice >= 1 && adminChoice <= count) break;
                errorMsg("Invalid number. Choose between 1 and " + to_string(count) + ".");
            }

            string chosenAdmin = adminList[adminChoice - 1];
            cout << "\n  Selected Admin : " << chosenAdmin << "\n";

            ifstream checkFile(inventoryFile(chosenAdmin), ios::binary);
            if(!checkFile)
            {
                errorMsg("Admin '" + chosenAdmin + "' has no inventory yet.");
                system("pause");
                continue;
            }
            Product tmp;
            bool hasItems = false;
            while(checkFile.read((char*)&tmp, sizeof(tmp))) hasItems = true;
            checkFile.close();

            if(!hasItems)
            {
                errorMsg("Admin '" + chosenAdmin + "' has no products yet.");
                system("pause");
                continue;
            }

            currentAdmin = chosenAdmin;
            placeOrder();
            currentAdmin = "";
        }
        else if(ch == 3)
        {
            searchProduct();
        }
        else if(ch == 4)
            break;
        else
        {
            errorMsg("Invalid choice.");
            system("pause");
        }
    } while(ch != 4);
}

void adminPanel()
{
    int ch;
    do {
        clearScreen();
        header("ADMIN OPTIONS");

        cout << "  [1]  Create Admin Account\n";
        cout << "  [2]  Admin Login\n";
        cout << "  [3]  Forgot Password\n";
        cout << "  [4]  Back to main menu\n\n";

        ch = getIntInput("  Choose: ");

        if(ch == 1)
        {
            registerAdmin();
        }
        else if(ch == 2)
        {
            bool success = loginAdmin();
            if(success)
            {
                successMsg("Welcome, " + currentAdmin + "!");
                system("pause");
                adminDashboard();
                currentAdmin = "";
            }
        }
        else if(ch == 3)
        {
            forgotPassword("");
        }
        else if(ch == 4)
        {
            break;
        }
        else
        {
            errorMsg("Invalid choice.");
            system("pause");
        }
    } while(ch != 4);
}

int main()
{
    int choice;

    while(true)
    {
        clearScreen();
        header("VENDING MACHINE SYSTEM");

        cout << "  [1]  Admin Panel\n";
        cout << "  [2]  Customer Panel\n";
        cout << "  [3]  Exit\n";

        choice = getIntInput("  Choose: ");

        if(choice == 1)
        {
            adminPanel();
        }
        else if(choice == 2)
        {
            customerMenu();
        }
        else if(choice == 3)
        {
            clearScreen();
            cout << "\n  Thank you! Goodbye.\n\n";
            break;
        }
        else
        {
            errorMsg("Invalid choice.");
            system("pause");
        }
    }

    return 0;
}