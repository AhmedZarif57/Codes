#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50
#define ADMIN_PASSWORD "admin123"

// ---------------- Password Hashing ----------------
unsigned long hashPassword(const char* str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

// ---------------- Structures ----------------
typedef struct MenuItem
{
    int id;
    char name[MAX_NAME];
    float price;
    struct MenuItem* next;
} MenuItem;

typedef struct Order
{
    int orderId;
    int itemId;
    int quantity;
    int express;
    struct Order* next;
} Order;

typedef struct OrderHistory
{
    int orderId;
    int itemId;
    int quantity;
    struct OrderHistory* next;
} OrderHistory;

typedef struct Customer
{
    char username[MAX_NAME];
    unsigned long passwordHash;
    struct Customer* left;
    struct Customer* right;
} Customer;

// ---------------- Globals ----------------
MenuItem* menuHead = NULL;
Order* front = NULL;
Order* rear = NULL;
OrderHistory* historyTop = NULL;
Customer* customerRoot = NULL;
int orderCounter = 1;
int adminLoggedIn = 0;

// ---------------- Menu Functions ----------------
int menuItemExists(int id)
{
    MenuItem* current = menuHead;
    while (current)
    {
        if (current->id == id)
            return 1;
        current = current->next;
    }
    return 0;
}

void addMenuItem(int id, char* name, float price, int showMessage)
{
    if (menuItemExists(id))
    {
        if (showMessage) printf("Menu item with ID %d already exists.\n", id);
        return;
    }
    MenuItem* newItem = (MenuItem*)malloc(sizeof(MenuItem));
    newItem->id = id;
    strcpy(newItem->name, name);
    newItem->price = price;
    newItem->next = menuHead;
    menuHead = newItem;
    if (showMessage) printf("Menu item added successfully.\n");
}

void viewMenu()
{
    MenuItem* current = menuHead;
    printf("\n--- MENU ---\n");
    while (current)
    {
        printf("ID: %d | %s - $%.2f\n", current->id, current->name, current->price);
        current = current->next;
    }
}

void alterMenuItemPrice(int itemId, float newPrice)
{
    MenuItem* current = menuHead;
    while (current)
    {
        if (current->id == itemId)
        {
            current->price = newPrice;
            printf("Price updated for %s to $%.2f\n", current->name, newPrice);
            return;
        }
        current = current->next;
    }
    printf("Item ID not found.\n");
}

void removeMenuItem(int itemId)
{
    MenuItem *current = menuHead, *prev = NULL;
    while (current)
    {
        if (current->id == itemId)
        {
            if (prev) prev->next = current->next;
            else menuHead = current->next;
            free(current);
            printf("Menu item removed.\n");
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Item ID not found.\n");
}

// ---------------- Queue Functions ----------------
void enqueueOrder(int itemId, int quantity, int express)
{
    Order* newOrder = (Order*)malloc(sizeof(Order));
    newOrder->orderId = orderCounter++;
    newOrder->itemId = itemId;
    newOrder->quantity = quantity;
    newOrder->express = express;
    newOrder->next = NULL;

    if (express)
    {
        newOrder->next = front;
        front = newOrder;
        if (rear == NULL) rear = newOrder;
    }
    else
    {
        if (rear == NULL)
        {
            front = rear = newOrder;
        }
        else
        {
            rear->next = newOrder;
            rear = newOrder;
        }
    }

    printf("Order #%d placed (%s)!\n", newOrder->orderId, express ? "Express" : "Normal");
}

void serveOrder()
{
    if (front == NULL)
    {
        printf("No orders to serve.\n");
        return;
    }

    Order* temp = front;
    printf("Serving Order #%d\n", temp->orderId);
    front = front->next;

    OrderHistory* hist = (OrderHistory*)malloc(sizeof(OrderHistory));
    hist->orderId = temp->orderId;
    hist->itemId = temp->itemId;
    hist->quantity = temp->quantity;
    hist->next = historyTop;
    historyTop = hist;

    free(temp);
    if (front == NULL) rear = NULL;
}

void rejectOrder()
{
    if (front == NULL)
    {
        printf("No orders to reject.\n");
        return;
    }

    Order* temp = front;
    printf("Rejecting Order #%d\n", temp->orderId);
    front = front->next;
    free(temp);
    if (front == NULL) rear = NULL;
}

void viewPendingOrders()
{
    Order* current = front;
    printf("\n--- PENDING ORDERS ---\n");
    while (current)
    {
        printf("Order #%d: Item ID %d x%d [%s]\n", current->orderId, current->itemId, current->quantity, current->express ? "Express" : "Normal");
        current = current->next;
    }
}

// ---------------- Stack Functions ----------------
void viewOrderHistory()
{
    OrderHistory* current = historyTop;
    printf("\n--- ORDER HISTORY ---\n");
    while (current)
    {
        printf("Order #%d: Item ID %d x%d\n", current->orderId, current->itemId, current->quantity);
        current = current->next;
    }
}

// ---------------- Customer Functions ----------------
Customer* insertCustomer(Customer* root, char* username, char* password)
{
    if (root == NULL)
    {
        Customer* newCust = (Customer*)malloc(sizeof(Customer));
        strcpy(newCust->username, username);
        newCust->passwordHash = hashPassword(password);
        newCust->left = newCust->right = NULL;
        return newCust;
    }
    int cmp = strcmp(username, root->username);
    if (cmp < 0)
        root->left = insertCustomer(root->left, username, password);
    else
        root->right = insertCustomer(root->right, username, password);
    return root;
}

Customer* loginCustomer(Customer* root, char* username, char* password)
{
    if (root == NULL) return NULL;
    int cmp = strcmp(username, root->username);
    if (cmp == 0)
    {
        if (root->passwordHash == hashPassword(password))
            return root;
        else
            return NULL;
    }
    else if (cmp < 0)
        return loginCustomer(root->left, username, password);
    else
        return loginCustomer(root->right, username, password);
}

void inorderCustomers(Customer* root)
{
    if (root)
    {
        inorderCustomers(root->left);
        printf("Customer: %s\n", root->username);
        inorderCustomers(root->right);
    }
}


// Tree-based search to display customer info by username
void searchCustomerByUsername(Customer* root, char* username)
{
    if (root == NULL)
    {
        printf("Customer not found.\n");
        return;
    }

    int cmp = strcmp(username, root->username);
    if (cmp == 0)
    {
        printf("Customer found: %s (Password hash: %lu)\n", root->username, root->passwordHash);
    }
    else if (cmp < 0)
    {
        searchCustomerByUsername(root->left, username);
    }
    else
    {
        searchCustomerByUsername(root->right, username);
    }
}


// ---------------- Main ----------------
int main()
{
    int choice;
    Customer* currentCustomer = NULL;
    int loggedIn = 0;

    addMenuItem(1, "Biriyani", 8.5, 0);
    addMenuItem(2, "Burger", 5.99, 0);
    addMenuItem(3, "Khichuri", 6.0, 0);
    addMenuItem(4, "Soda", 1.99, 0);
    addMenuItem(5, "Water", 0.99, 0);

    while (1)
    {
        printf("\n--- Online Restaurant System ---\n");
        printf("1. Admin Login\n2. Customer Sign Up\n3. Customer Login\n4. Exit\nEnter choice: ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            char adminPass[MAX_NAME];
            printf("Enter admin password: ");
            scanf(" %s", adminPass);
            if (strcmp(adminPass, ADMIN_PASSWORD) == 0)
            {
                printf("Admin logged in successfully.\n");
                int adminChoice;
                do
                {
                    printf("\nAdmin Options:\n1. View Orders\n2. Alter Menu Item Price\n3. Reject Order\n4. Accept Order\n5. View Customer Info\n6. View Instant & Normal Orders\n7. View Order History\n8. Add Menu Item\n9. Remove Menu Item\n10. Logout\nEnter choice: ");
                    scanf("%d", &adminChoice);
                    switch (adminChoice)
                    {
                    case 1:
                        viewPendingOrders();
                        break;
                    case 2:
                    {
                        int itemId;
                        float price;
                        printf("Enter item ID: ");
                        scanf("%d", &itemId);
                        printf("Enter new price: ");
                        scanf("%f", &price);
                        alterMenuItemPrice(itemId, price);
                        break;
                    }
                    case 3:
                        rejectOrder();
                        break;
                    case 4:
                        serveOrder();
                        break;

                    case 5:
                    {
                        int subChoice;
                        printf("\n1. View All Customers (Inorder)\n2. Search Customer by Username\nEnter choice: ");
                        scanf("%d", &subChoice);
                        if (subChoice == 1)
                        {
                            inorderCustomers(customerRoot);
                        }
                        else if (subChoice == 2)
                        {
                            char searchName[MAX_NAME];
                            printf("Enter username to search: ");
                            scanf(" %s", searchName);
                            searchCustomerByUsername(customerRoot, searchName);
                        }
                        else
                        {
                            printf("Invalid choice.\n");
                        }
                        break;
                    }

                    case 6:
                        viewPendingOrders();
                        break;
                    case 7:
                        viewOrderHistory();
                        break;
                    case 8:
                    {
                        int id;
                        char name[MAX_NAME];
                        float price;
                        printf("Enter new item ID: ");
                        scanf("%d", &id);
                        printf("Enter item name: ");
                        scanf(" %s", name);
                        printf("Enter item price: ");
                        scanf("%f", &price);
                        addMenuItem(id, name, price, 1);
                        break;
                    }
                    case 9:
                    {
                        int itemId;
                        printf("Enter item ID to remove: ");
                        scanf("%d", &itemId);
                        removeMenuItem(itemId);
                        break;
                    }
                    case 10:
                        printf("Admin logged out.\n");
                        break;
                    default:
                        printf("Invalid option.\n");
                    }
                }
                while (adminChoice != 10);
            }
            else
            {
                printf("Incorrect admin password.\n");
            }
        }
        else if (choice == 2)
        {
            char username[MAX_NAME], password[MAX_NAME];
            printf("Username: ");
            scanf(" %s", username);
            printf("Password: ");
            scanf(" %s", password);
            customerRoot = insertCustomer(customerRoot, username, password);
            printf("Signup successful!\n");
        }
        else if (choice == 3)
        {
            char username[MAX_NAME], password[MAX_NAME];
            printf("Username: ");
            scanf(" %s", username);
            printf("Password: ");
            scanf(" %s", password);
            currentCustomer = loginCustomer(customerRoot, username, password);
            if (currentCustomer)
            {
                printf("Welcome, %s!\n", currentCustomer->username);
                int custChoice;
                do
                {
                    printf("\nCustomer Options:\n1. View Menu\n2. Order\n3. Logout\nEnter choice: ");
                    scanf("%d", &custChoice);
                    if (custChoice == 1) viewMenu();
                    else if (custChoice == 2)
                    {
                        int itemId, quantity, express;
                        printf("Enter item ID: ");
                        scanf("%d", &itemId);
                        printf("Enter quantity: ");
                        scanf("%d", &quantity);
                        printf("Express delivery? (1 = Yes, 0 = No): ");
                        scanf("%d", &express);

                        MenuItem* temp = menuHead;
                        float totalPrice = 0;
                        while (temp)
                        {
                            if (temp->id == itemId)
                            {
                                totalPrice = quantity * temp->price;
                                break;
                            }
                            temp = temp->next;
                        }
                        printf("Total Price: $%.2f\nConfirm order? (1 = Yes, 0 = No): ", totalPrice);
                        int confirm;
                        scanf("%d", &confirm);
                        if (confirm)
                            enqueueOrder(itemId, quantity, express);
                        else
                            printf("Order cancelled.\n");
                    }
                    else if (custChoice == 3) printf("Customer logged out.\n");
                    else printf("Invalid choice.\n");
                }
                while (custChoice != 3);
            }
            else
            {
                printf("Login failed.\n");
            }
        }
        else if (choice == 4)
        {
            printf("Exiting...\n");
            break;
        }
        else
        {
            printf("Invalid choice.\n");
        }
    }

    return 0;
}

