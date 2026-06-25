// Cafeteria-Managment-System.cpp 
// cafeteria management system 
//

#include <iostream>
#include <string>
#include <fstream>
#include <limits>
using namespace std;

// maximum number of items,stock and order
const int MAX_ITEMS = 15;
const int MAX_STOCK = 100;
const int MAX_ORDERS = 100;
const int MAX_ORDER_ITEMS = 50;

// Array to store inventory data
int itemID[MAX_ITEMS];
string itemName[MAX_ITEMS];
float itemPrice[MAX_ITEMS];
int itemStock[MAX_ITEMS];

struct Order {
    int orderNumber;
    int itemIDs[MAX_ORDER_ITEMS];
    string itemNames[MAX_ORDER_ITEMS];
    int quantities[MAX_ORDER_ITEMS];
    float costs[MAX_ORDER_ITEMS];
    int itemCount;
    float totalAmount;
    bool isActive;  // true if order is active, false if cancelled
};

Order orders[MAX_ORDERS];
int orderCount = 0;
int nextOrderNumber = 1;

// Count and Total Sales
int itemCount = 0;
float totalSales = 0;

// LOAD DATA FROM FILE (ifstream)
void loadFromFile() {
    ifstream file("items.txt");

    if (!file) {
        return;
    }

    file >> itemCount;
    file.ignore(); // Ignore the newline after itemCount

    for (int i = 0; i < itemCount; i++) {
        file >> itemID[i];
        file.ignore(); // Ignore space after ID
        getline(file, itemName[i], '\t'); // Read name until tab
        file >> itemPrice[i]
            >> itemStock[i];
        file.ignore(); // Ignore newline
    }

    file.close();

    // Load order tracking data
    ifstream orderFile("orders.txt");
    if (orderFile) {
        orderFile >> orderCount >> nextOrderNumber;

        // Read totalSales but recalculate it from active orders to ensure consistency
        float savedTotalSales;
        orderFile >> savedTotalSales;

        totalSales = 0;  // Reset and recalculate

        for (int i = 0; i < orderCount; i++) {
            orderFile >> orders[i].orderNumber
                >> orders[i].itemCount
                >> orders[i].totalAmount
                >> orders[i].isActive;

            // Only add active orders to totalSales
            if (orders[i].isActive) {
                totalSales += orders[i].totalAmount;
            }

            for (int j = 0; j < orders[i].itemCount; j++) {
                orderFile >> orders[i].itemIDs[j]
                    >> orders[i].itemNames[j]
                    >> orders[i].quantities[j]
                    >> orders[i].costs[j];
            }
        }
        orderFile.close();
    }
}

// SAVE DATA TO FILE (ofstream)
void saveToFile() {
    ofstream file("items.txt");

    if (!file) {
        cout << "Error saving file!" << endl;
        return;
    }

    file << itemCount << endl;

    for (int i = 0; i < itemCount; i++) {
        file << itemID[i] << "\t"
            << itemName[i] << "\t"
            << itemPrice[i] << "\t"
            << itemStock[i] << endl;
    }

    file.close();

    // Save order tracking data
    ofstream orderFile("orders.txt");
    if (orderFile) {
        orderFile << orderCount << " " << nextOrderNumber << " " << totalSales << endl;

        for (int i = 0; i < orderCount; i++) {
            orderFile << orders[i].orderNumber << " "
                << orders[i].itemCount << " "
                << orders[i].totalAmount << " "
                << orders[i].isActive << endl;

            for (int j = 0; j < orders[i].itemCount; j++) {
                orderFile << orders[i].itemIDs[j] << " "
                    << orders[i].itemNames[j] << " "
                    << orders[i].quantities[j] << " "
                    << orders[i].costs[j] << endl;
            }
        }
        orderFile.close();
    }
}

// function to add new item
void addItem() {
    if (itemCount >= MAX_ITEMS) {
        cout << "Inventory full. Cannot add more items." << endl;
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int newID;
    cout << "Enter Item ID: ";
    cin >> newID;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid ID! Item not added." << endl;
        return;
    }

    string newName;
    cout << "Enter Item Name: ";
    cin.ignore();
    getline(cin, newName);

    float newPrice;
    cout << "Enter Item Price: ";
    cin >> newPrice;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid price! Item not added." << endl;
        return;
    }

    int newStock;
    cout << "Enter Item Stock: ";
    cin >> newStock;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid stock! Item not added." << endl;
        return;
    }

    itemID[itemCount] = newID;
    itemName[itemCount] = newName;
    itemPrice[itemCount] = newPrice;
    itemStock[itemCount] = newStock;
    itemCount++;

    cout << "Item added successfully!" << endl;
}

// function to display all items
void displayItems() {
    if (itemCount == 0) {
        cout << "No items in inventory." << endl;
        return;
    }
    cout << "\n----------Cafeteria Items----------" << endl;
    cout << "ID\tName\t\tPrice\tStock" << endl;
    cout << "---\t------------\t-----\t-----" << endl;

    for (int i = 0; i < itemCount; i++) {
        cout << itemID[i] << "\t";

        // Format name with proper spacing (pad to 12 characters)
        string displayName = itemName[i];
        if (displayName.length() < 12) {
            displayName += string(12 - displayName.length(), ' ');
        }
        cout << displayName << "\t";

        // Format price
        cout << (int)(itemPrice[i] + 0.5) << "\t";

        // Display stock
        cout << itemStock[i] << endl;
    }
    cout << "-----------------------------------\n" << endl;
}

// Function to search item by ID
int searchItem(int id) {
    for (int i = 0; i < itemCount; i++) {
        if (itemID[i] == id) {
            return i;
        }
    }
    return -1;
}

// Function to update stock
void updateStock() {
    if (itemCount == 0) {
        cout << "No items in inventory." << endl;
        return;
    }

    int id;
    cout << "Enter Item ID to update stock: ";
    cin >> id;

    int index = searchItem(id);
    if (index == -1) {
        cout << "Item not found." << endl;
        return;
    }

    int newstock;
    cout << "Enter new stock (0 - " << MAX_STOCK << "): ";
    cin >> newstock;

    if (newstock < 0) {
        cout << "Stock cannot be negative. Setting to 0." << endl;
        newstock = 0;
    }
    else if (newstock > MAX_STOCK) {
        cout << "Stock cannot exceed " << MAX_STOCK << ". Setting to " << MAX_STOCK << "." << endl;
        newstock = MAX_STOCK;
    }

    itemStock[index] = newstock;
    cout << "Stock updated successfully!" << endl;
    cout << "New stock for item " << itemName[index] << " is " << itemStock[index] << endl;
}

// Function to update price
void updatePrice() {
    if (itemCount == 0) {
        cout << "No items available to update." << endl;
        return;
    }

    int id;
    cout << "Enter Item ID to update price: ";
    cin >> id;

    int index = searchItem(id);
    if (index == -1) {
        cout << "Item not found." << endl;
        return;
    }

    float newPrice;
    cout << "Enter new price: ";
    cin >> newPrice;

    if (newPrice < 0.0f) {
        cout << "Price cannot be negative." << endl;
        return;
    }

    itemPrice[index] = newPrice;
    cout << "Price updated successfully!" << endl;
    cout << "New price for item " << itemName[index] << " is " << ((int)(itemPrice[index] * 100 + 0.5) / 100.0) << endl;
}

// Function to delete an item
void deleteItem() {
    if (itemCount == 0) {
        cout << "No items in inventory." << endl;
        return;
    }

    int id;
    cout << "Enter Item ID to delete: ";
    cin >> id;

    int itemNo = searchItem(id);
    if (itemNo == -1) {
        cout << "Item not found." << endl;
        return;
    }

    for (int i = itemNo; i < itemCount - 1; i++) {
        itemID[i] = itemID[i + 1];
        itemName[i] = itemName[i + 1];
        itemPrice[i] = itemPrice[i + 1];
        itemStock[i] = itemStock[i + 1];
    }
    itemCount--;
    cout << "Item deleted successfully!" << endl;
}

// function to Display all orders
void displayAllOrders() {
    if (orderCount == 0) {
        cout << "No orders placed yet." << endl;
        return;
    }

    cout << "\n---------- ALL ORDERS ----------" << endl;
    for (int i = 0; i < orderCount; i++) {
        cout << "Order #" << orders[i].orderNumber
            << " - Status: " << (orders[i].isActive ? "ACTIVE" : "CANCELLED")
            << " - Total: " << ((int)(orders[i].totalAmount * 100 + 0.5) / 100.0) << endl;

        for (int j = 0; j < orders[i].itemCount; j++) {
            cout << "  " << orders[i].itemNames[j]
                << " x" << orders[i].quantities[j]
                << " = " << ((int)(orders[i].costs[j] * 100 + 0.5) / 100.0) << endl;
        }
        cout << endl;
    }
    cout << "--------------------------------\n" << endl;
}

// function to Find order by number 
int findOrder(int orderNum) {
    for (int i = 0; i < orderCount; i++) {
        if (orders[i].orderNumber == orderNum) {
            return i;
        }
    }
    return -1;
}

// function to Cancel entire order 
void cancelWholeOrder() {
    if (orderCount == 0) {
        cout << "No orders to cancel." << endl;
        return;
    }

    displayAllOrders();

    int orderNum;
    cout << "Enter Order Number to cancel: ";
    cin >> orderNum;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input!" << endl;
        return;
    }

    int orderIndex = findOrder(orderNum);
    if (orderIndex == -1) {
        cout << "Order not found!" << endl;
        return;
    }

    if (!orders[orderIndex].isActive) {
        cout << "This order is already cancelled!" << endl;
        return;
    }

    // Return items to stock
    for (int i = 0; i < orders[orderIndex].itemCount; i++) {
        int itemIndex = searchItem(orders[orderIndex].itemIDs[i]);
        if (itemIndex != -1) {
            itemStock[itemIndex] += orders[orderIndex].quantities[i];
            if (itemStock[itemIndex] > MAX_STOCK) {
                itemStock[itemIndex] = MAX_STOCK;
            }
        }
    }

    // Deduct from total sales
    totalSales -= orders[orderIndex].totalAmount;
    if (totalSales < 0) totalSales = 0;

    // Mark order as cancelled
    orders[orderIndex].isActive = false;

    cout << "\nOrder #" << orderNum << " cancelled successfully!" << endl;
    cout << "Items returned to stock and sales updated." << endl;
    cout << "Refund amount: " << ((int)(orders[orderIndex].totalAmount * 100 + 0.5) / 100.0) << endl;
}

// function to Cancel specific item from order
void cancelItemFromOrder() {
    if (orderCount == 0) {
        cout << "No orders available." << endl;
        return;
    }

    displayAllOrders();

    int orderNum;
    cout << "Enter Order Number: ";
    cin >> orderNum;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input!" << endl;
        return;
    }

    int orderIndex = findOrder(orderNum);
    if (orderIndex == -1) {
        cout << "Order not found!" << endl;
        return;
    }

    if (!orders[orderIndex].isActive) {
        cout << "Cannot modify a cancelled order!" << endl;
        return;
    }

    if (orders[orderIndex].itemCount == 0) {
        cout << "No items in this order!" << endl;
        return;
    }

    // Display items in this order
    cout << "\nItems in Order #" << orderNum << ":" << endl;
    cout << "Index\tID\tName\t\tQty\tCost" << endl;
    for (int i = 0; i < orders[orderIndex].itemCount; i++) {
        cout << i + 1 << "\t"
            << orders[orderIndex].itemIDs[i] << "\t"
            << orders[orderIndex].itemNames[i] << "\t"
            << orders[orderIndex].quantities[i] << "\t"
            << ((int)(orders[orderIndex].costs[i] * 100 + 0.5) / 100.0) << endl;
    }

    int itemIndex;
    cout << "\nEnter item index to cancel (1-" << orders[orderIndex].itemCount << "): ";
    cin >> itemIndex;

    if (cin.fail() || itemIndex < 1 || itemIndex > orders[orderIndex].itemCount) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid item index!" << endl;
        return;
    }

    itemIndex--; // Convert to 0-based index

    // Return item to stock
    int invIndex = searchItem(orders[orderIndex].itemIDs[itemIndex]);
    if (invIndex != -1) {
        itemStock[invIndex] += orders[orderIndex].quantities[itemIndex];
        if (itemStock[invIndex] > MAX_STOCK) {
            itemStock[invIndex] = MAX_STOCK;
        }
    }

    // Update total sales and order total
    float refundAmount = orders[orderIndex].costs[itemIndex];
    totalSales -= refundAmount;
    if (totalSales < 0) totalSales = 0;
    orders[orderIndex].totalAmount -= refundAmount;
    if (orders[orderIndex].totalAmount < 0) orders[orderIndex].totalAmount = 0;

    cout << "\nItem '" << orders[orderIndex].itemNames[itemIndex]
        << "' cancelled from order." << endl;
    cout << "Refund amount: " << ((int)(refundAmount * 100 + 0.5) / 100.0) << endl;

    // Remove item from order by shifting remaining items
    for (int i = itemIndex; i < orders[orderIndex].itemCount - 1; i++) {
        orders[orderIndex].itemIDs[i] = orders[orderIndex].itemIDs[i + 1];
        orders[orderIndex].itemNames[i] = orders[orderIndex].itemNames[i + 1];
        orders[orderIndex].quantities[i] = orders[orderIndex].quantities[i + 1];
        orders[orderIndex].costs[i] = orders[orderIndex].costs[i + 1];
    }
    orders[orderIndex].itemCount--;

    // If no items left, mark order as cancelled
    if (orders[orderIndex].itemCount == 0) {
        orders[orderIndex].isActive = false;
        cout << "All items removed. Order marked as cancelled." << endl;
    }
    else {
        cout << "Updated order total: " << ((int)(orders[orderIndex].totalAmount * 100 + 0.5) / 100.0) << endl;
    }
}

// funtion to Place an order 
void placeOrder() {
    if (itemCount == 0) {
        cout << "No items available to order." << endl;
        return;
    }

    //Check max orders limit 
    if (orderCount >= MAX_ORDERS) {
        cout << "Maximum orders reached!" << endl;
        return;
    }

    char more;
    float orderTotal = 0.0f;

    Order newOrder;
    newOrder.orderNumber = nextOrderNumber++;
    newOrder.itemCount = 0;
    newOrder.totalAmount = 0.0f;
    newOrder.isActive = true;

    displayItems();

    do {
        if (newOrder.itemCount >= MAX_ORDER_ITEMS) {
            cout << "Maximum items per order reached!" << endl;
            break;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        int choiceID, quantity;
        cout << "Enter item ID to order: ";
        cin >> choiceID;

        int i = searchItem(choiceID);
        if (i == -1) {
            cout << "Invalid item ID!" << endl;
        }
        else {
            cout << "Enter Quantity: ";
            cin >> quantity;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid quantity!" << endl;
                continue;
            }

            if (quantity <= 0) {
                cout << "Quantity must be at least 1." << endl;
            }
            else if (quantity > itemStock[i]) {
                cout << "Not enough stock available: " << itemStock[i] << endl;
            }
            else {
                itemStock[i] -= quantity;
                float cost = quantity * itemPrice[i];
                orderTotal += cost;

                newOrder.itemIDs[newOrder.itemCount] = itemID[i];
                newOrder.itemNames[newOrder.itemCount] = itemName[i];
                newOrder.quantities[newOrder.itemCount] = quantity;
                newOrder.costs[newOrder.itemCount] = cost;
                newOrder.itemCount++;

                cout << "Added " << quantity << " x " << itemName[i]
                    << " to your order. Item cost: " << ((int)(cost * 100 + 0.5) / 100.0) << endl;
            }
        }

        cout << "Do you want to order another item? (y/n): ";
        cin >> more;
    } while (more == 'y' || more == 'Y');

    // PRINT BILL and save order 
    if (orderTotal > 0.0f) {
        newOrder.totalAmount = orderTotal;
        totalSales += orderTotal;


        orders[orderCount] = newOrder;
        orderCount++;

        cout << "\n------------- CUSTOMER BILL -------------\n";
        cout << "Order Number: #" << newOrder.orderNumber << endl;
        cout << "ID\tItem\t\tQty\tCost\n";

        for (int k = 0; k < newOrder.itemCount; k++) {
            cout << newOrder.itemIDs[k] << "\t"
                << newOrder.itemNames[k] << "\t"
                << newOrder.quantities[k] << "\t"
                << ((int)(newOrder.costs[k] * 100 + 0.5) / 100.0) << endl;
        }

        cout << "\nTotal Bill: " << ((int)(orderTotal * 100 + 0.5) / 100.0) << endl;
        cout << "-----------------------------------------\n";
        cout << "** Please note your Order Number: #" << newOrder.orderNumber << " **\n";
    }
    else {
        cout << "No items were ordered." << endl;
    }
}

// Function to display total sales
void displayTotalSales() {
    cout << "---------------------------" << endl;
    cout << "Total Sales: " << ((int)(totalSales * 100 + 0.5) / 100.0) << endl;
    cout << "---------------------------\n" << endl;
}

// function for admin menu
void adminMenu() {
    int choice;
    do {
        cout << "\n------Admin------" << endl;
        cout << "1. Add Item" << endl;
        cout << "2. Display Items" << endl;
        cout << "3. Update Stock" << endl;
        cout << "4. Update Price" << endl;
        cout << "5. Delete Item" << endl;
        cout << "6. Display Total Sales" << endl;
        cout << "7. Display All Orders" << endl;
        cout << "8. Cancel Whole Order" << endl;
        cout << "9. Cancel Item from Order" << endl;
        cout << "10. Exit Admin Menu" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number." << endl;
            continue;
        }

        switch (choice) {
        case 1:
            addItem();
            break;
        case 2:
            displayItems();
            break;
        case 3:
            updateStock();
            break;
        case 4:
            updatePrice();
            break;
        case 5:
            deleteItem();
            break;
        case 6:
            displayTotalSales();
            break;
        case 7:
            displayAllOrders();
            break;
        case 8:
            cancelWholeOrder();
            break;
        case 9:
            cancelItemFromOrder();
            break;
        case 10:
            cout << "Exiting Admin Menu." << endl;
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 10);
}

// Function for customer menu
void customerMenu() {
    int choice;
    do {
        cout << "\n------Customer Menu------" << endl;
        cout << "1. Display Items" << endl;
        cout << "2. Place Order" << endl;
        cout << "3. Exit Customer Menu" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number." << endl;
            continue;
        }

        switch (choice) {
        case 1:
            displayItems();
            break;
        case 2:
            placeOrder();
            break;
        case 3:
            cout << "Exiting Customer Menu." << endl;
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 3);
}

// Main function
int main() {
    loadFromFile();

    if (itemCount == 0) {
        itemCount = 15;
        itemID[0] = 1; itemName[0] = "Sandwich"; itemPrice[0] = 150; itemStock[0] = 50;
        itemID[1] = 2; itemName[1] = "Burger"; itemPrice[1] = 200; itemStock[1] = 50;
        itemID[2] = 3; itemName[2] = "Pasta"; itemPrice[2] = 300; itemStock[2] = 50;
        itemID[3] = 4; itemName[3] = "Fries"; itemPrice[3] = 100; itemStock[3] = 50;
        itemID[4] = 5; itemName[4] = "Cake"; itemPrice[4] = 150; itemStock[4] = 50;
        itemID[5] = 6; itemName[5] = "Pastries"; itemPrice[5] = 120; itemStock[5] = 50;
        itemID[6] = 7; itemName[6] = "Donuts"; itemPrice[6] = 100; itemStock[6] = 30;
        itemID[7] = 8; itemName[7] = "Cookies"; itemPrice[7] = 50; itemStock[7] = 50;
        itemID[8] = 9; itemName[8] = "Juice"; itemPrice[8] = 80; itemStock[8] = 50;
        itemID[9] = 10; itemName[9] = "Tea"; itemPrice[9] = 70; itemStock[9] = 25;
        itemID[10] = 11; itemName[10] = "Cold Drink"; itemPrice[10] = 100; itemStock[10] = 50;
        itemID[11] = 12; itemName[11] = "Americano"; itemPrice[11] = 250; itemStock[11] = 50;
        itemID[12] = 13; itemName[12] = "Cold Coffee"; itemPrice[12] = 300; itemStock[12] = 50;
        itemID[13] = 14; itemName[13] = "Lattechino"; itemPrice[13] = 450; itemStock[13] = 50;
        itemID[14] = 15; itemName[14] = "Pizza"; itemPrice[14] = 350; itemStock[14] = 35;
    }
    displayItems();

    int choice;
    do {
        cout << "\n------Cafeteria System------" << endl;
        cout << "1. Admin Menu" << endl;
        cout << "2. Customer Menu" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number." << endl;
            continue;
        }

        switch (choice) {
        case 1:
        {
            string adminPass;
            cout << "Enter admin password: ";
            cin >> adminPass;
            if (adminPass == "jia") {
                adminMenu();
            }
            else {
                cout << "Wrong password! Returning to main menu.\n";
            }
            break;
        }
        case 2:
            customerMenu();
            break;
        case 3:
            saveToFile();
            cout << "Exiting Cafeteria System. Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 3);

    return 0;
}
