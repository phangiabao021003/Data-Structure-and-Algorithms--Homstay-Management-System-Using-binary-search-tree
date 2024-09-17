#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#define ANSI_COLOR_BLUE   "\x1b[34m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_RED    "\x1b[31m"
#define ANSI_COLOR_RESET  "\x1b[0m"

// Dữ liệu kiểu ngày
typedef struct Date {
    int day;
    int month;
    int year;
} Date;

typedef struct Room {
    char RoomNumber[4];  // Assuming a maximum of 3 characters for room number
    char RoomType[20];
} Room;

typedef struct RoomNode {
    Room room;
    struct RoomNode* next;
    struct RoomNode* prev;
} RoomNode;

typedef struct Customer {
    char ID[12];       // Adjusted to 12 to accommodate the phone number
    char Name[50];
    char Email[50];
    char DOB[20];
    char Address[100];
} Customer;

typedef struct CustomerNode {
    Customer customer;
    struct CustomerNode* next;
    struct CustomerNode* prev;
} CustomerNode;

typedef struct Booking {
    char BookingID[9];
    char CustomerID[12];
    char BookingDate[20];
    Date CheckInDate;
    Date CheckOutDate;
} Booking;

// Tạo node cho booking
typedef struct BookingNode {
    Booking booking;
    struct BookingNode *left;
    struct BookingNode *right;
} BookingNode;

// Tạo binary search tree cho booking
typedef struct BookingTree {
    BookingNode *root;
} BookingTree;

// Structure to represent a node in a linked list of BookingNodes
typedef struct BookingNodeList {
    BookingNode* node;
    struct BookingNodeList* next;
} BookingNodeList;

// Function prototypes
void clearBuffer();

void insertRoom(RoomNode** head, Room room);
void displayRoomList(RoomNode* head);
void updateRoomType(RoomNode* head, const char* RoomNumber, const char* newRoomType);
RoomNode* loadRoomsFromFile(RoomNode** roomhead, const char* filename);
void saveRoomsToFile(RoomNode* roomhead, const char* filename);

void getCustomerInfo(Customer* customer);
void insertCustomer(CustomerNode** head, Customer customer);
CustomerNode* searchCustomerByID(CustomerNode* head, const char* customerID);
void displayCustomerNodeInfo(CustomerNode* node);
void displayCustomerList(CustomerNode* head);
CustomerNode* loadCustomersFromFile(CustomerNode** customerhead, const char* filename);
void saveCustomersToFile(CustomerNode* customerhead, const char* filename);

int compareDates(Date date1, Date date2);
void bstInit(BookingTree *tree);
void getBookingInfo(Booking* booking);
BookingNode *createNode(Booking key);
int bstInsert(BookingNode **root, Booking x);
int bstTreeInsert(BookingTree *tree, Booking x);
BookingNode* bstSearchByBookingID(BookingNode* root, const char* bookingID, BookingNode** parent);
void bstSearchByCustomerIDRecursive(BookingNode* root, const char* customerID, BookingNodeList** resultList);
BookingNodeList* bstSearchByCustomerID(BookingNode* root, const char* customerID);
BookingNode* bstTreeSearchByBookingID(BookingTree* tree, const char* bookingID);
void displayBookinginOrderTraversal(BookingNode *root);
void displayBookingNodeInfo(BookingNode* node);
void displayBookingNodeInfoinline (BookingNode* node);
void displayBookingList(BookingNodeList* resultList);
BookingNode* bstDeleteByID(BookingNode* root, const char* bookingID);
int checkEmptyRoom(BookingNode* root, Date CheckIn, Date CheckOut);
void displaystatus(BookingNode* root, const char* RoomNumber, Date CheckIn, Date CheckOut);
void updateBookingDates(BookingTree* tree, const char* bookingID, Date newCheckInDate, Date newCheckOutDate);
void loadBookingsFromFile(BookingTree *tree, const char *filename);
void writeBookingToFile(FILE *file, BookingNode *node);
void inOrderWriteToFile(FILE *file, BookingNode *node);
void saveBookingsToFile(BookingNode *root, const char *filename);
BookingTree* selectBookingTree(BookingTree* tree101, BookingTree* tree102, BookingTree* tree103, BookingTree* tree201, BookingTree* tree202,
    BookingTree* tree301, BookingTree* tree302, BookingTree* tree303, BookingTree* tree401, BookingTree* tree402);

// Thêm room node vào linked list
void insertRoom(RoomNode** head, Room room) {
    RoomNode* newRoomNode = (RoomNode*)malloc(sizeof(RoomNode));
    newRoomNode->room = room;
    newRoomNode->prev = NULL;
    newRoomNode->next = NULL;

    if (*head == NULL) {
        *head = newRoomNode;
    } else {
        RoomNode* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newRoomNode;
        newRoomNode->prev = current;
    }
}

// Function to display Room list
void displayRoomList(RoomNode* head) {
    printf("\n--- Room ---\n");
    printf("%-12s %-8s\n", "Room Number", "Room Type");
    printf("----------------------------------------\n");
    RoomNode* current = head;
    while (current != NULL) {
        printf("%-12s %-8s\n", current->room.RoomNumber, current->room.RoomType);
        current = current->next;
    }
}

// Function to update RoomType
void updateRoomType(RoomNode* head, const char* RoomNumber, const char* newRoomType) {
    RoomNode* current = head;
    while (current != NULL) {
        if (strcmp(current->room.RoomNumber, RoomNumber) == 0) {
            /*if (strlen(newRoomType) >= sizeof(current->room.RoomType)) {
                return;
            }*/
            strcpy(current->room.RoomType, newRoomType);
            printf("Update successful\n");
            return;
        }
        current = current->next;
    }
    printf("Room %s not found.\n", RoomNumber);
}

// Function to load the room data from file to linked list
RoomNode* loadRoomsFromFile(RoomNode** roomhead, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file for reading\n");
        return NULL;
    }

    char line[100];  // Adjust the size based on your actual data
    while (fgets(line, sizeof(line), file)) {
        Room room;
        // Adjust the format specifier to read both room number and room type correctly
        if (sscanf(line, "%3[^,], %19[^\n]", room.RoomNumber, room.RoomType) == 2) {
            insertRoom(roomhead, room);
        }
    }

    fclose(file);
    return *roomhead;
}

// Function to save the room data from linked list to a file
void saveRoomsToFile(RoomNode* roomhead, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing\n");
        return;
    }

    RoomNode* current = roomhead;
    while (current != NULL) {
        fprintf(file, "%s, %s\n",
                current->room.RoomNumber, current->room.RoomType);
        current = current->next;
    }

    fclose(file);
}

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Function to get customer information
void getCustomerInfo(Customer* customer) {
    printf("Enter Name: ");
    scanf(" %[^\n]", customer->Name);
    clearBuffer();

    printf("Enter Email: ");
    scanf("%49s", customer->Email);
    clearBuffer();

    printf("Enter DOB: ");
    scanf("%11s", customer->DOB);
    clearBuffer();

    printf("Enter Address: ");
    scanf(" %[^\n]", customer->Address);
    clearBuffer();
}

// Function to insert a customer node at the end of the double-linked list
void insertCustomer(CustomerNode** head, Customer customer) {
    CustomerNode* newCustomerNode = (CustomerNode*)malloc(sizeof(CustomerNode));
    newCustomerNode->customer = customer;
    newCustomerNode->prev = NULL;
    newCustomerNode->next = NULL;

    if (*head == NULL) {
        *head = newCustomerNode;
    } else {
        CustomerNode* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newCustomerNode;
        newCustomerNode->prev = current;
    }
}

//Function to search node by ID
CustomerNode* searchCustomerByID(CustomerNode* head, const char* customerID) {
    CustomerNode* current = head;

    while (current != NULL) {
        if (strcmp(current->customer.ID, customerID) == 0) {
            // Khách hàng được tìm thấy
            return current;
        }
        current = current->next;
    }

    // Không tìm thấy khách hàng với CustomerID cụ thể
    return NULL;
}

// Function to display Customer Node info
void displayCustomerNodeInfo(CustomerNode* node) {
    if (node != NULL) {
        printf("ID: %s, Name: %s, Email: %s, DOB: %s, Address: %s\n", 
            node->customer.ID, node->customer.Name, node->customer.Email, node->customer.DOB, node->customer.Address);
    } else {
        printf("CustomerID not found.\n");
    }
}

// Function to display the customer list
void displayCustomerList(CustomerNode* head) {
    printf("\n--- Customer List ---\n");
    printf("%-12s %-30s %-30s %-12s %-50s\n",
           "ID", "Name", "Email", "DOB", "Address");
    printf("-------------------------------------------------------------------------------------------------\n");

    CustomerNode* current = head;
    while (current != NULL) {
        printf("%-12s %-30s %-30s %-12s %-50s\n",
               current->customer.ID, current->customer.Name,
               current->customer.Email, current->customer.DOB,
               current->customer.Address);
        current = current->next;
    }
}

//Function to load the customer data from a file to linked list
CustomerNode* loadCustomersFromFile(CustomerNode** customerhead, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file for reading\n");
        return NULL;
    }

    char line[200];  // Assuming a maximum line length of 200 characters
    while (fgets(line, sizeof(line), file)) {
        Customer customer;
        if (sscanf(line, "%11[^,], %49[^,], %49[^,], %19[^,], %99[^\n]",
                   customer.ID, customer.Name, customer.Email,
                   customer.DOB, customer.Address) == 5) {
            insertCustomer(customerhead, customer);
        }
    }

    fclose(file);
    return *customerhead;
}

// Function to save the customer data from linked list to a file
void saveCustomersToFile(CustomerNode* customerhead, const char* filename) {
    FILE* file = fopen("customerlist.txt", "w");
    if (file == NULL) {
        printf("Error opening file for writing\n");
        return;
    }

    CustomerNode* current = customerhead;
    while (current != NULL) {
        fprintf(file, "%s, %s, %s, %s, %s\n",
                current->customer.ID, current->customer.Name,
                current->customer.Email, current->customer.DOB,
                current->customer.Address);
        current = current->next;
    }

    fclose(file);
}

// Compare two dates
// Returns -1 if date1 is earlier, 0 if they are equal, and 1 if date2 is earlier
int compareDates(Date date1, Date date2) {
    if (date1.year < date2.year)
        return -1;
    else if (date1.year > date2.year)
        return 1;

    // If years are equal, compare months
    if (date1.month < date2.month)
        return -1;
    else if (date1.month > date2.month)
        return 1;

    // If years and months are equal, compare days
    if (date1.day < date2.day)
        return -1;
    else if (date1.day > date2.day)
        return 1;

    // If all components are equal, the dates are equal
    return 0;
}

// Initialize a BST
void bstInit(BookingTree *tree) {
    tree->root = NULL;
}

// Function to get booking information from user input
void getBookingInfo(Booking* booking) {
    printf("Enter Booking ID: ");
    scanf("%s", booking->BookingID);

    printf("Enter Customer ID: ");
    scanf("%s", booking->CustomerID);

    printf("Enter Booking Date (DD/MM/YYYY): ");
    scanf("%s", booking->BookingDate);

    printf("Enter Check-In Date (DD/MM/YYYY): ");
    scanf("%d/%d/%d", &booking->CheckInDate.day, &booking->CheckInDate.month, &booking->CheckInDate.year);

    printf("Enter Check-Out Date (DD/MM/YYYY): ");
    scanf("%d/%d/%d", &booking->CheckOutDate.day, &booking->CheckOutDate.month, &booking->CheckOutDate.year);
}

// Create a new node for the BST
BookingNode *createNode(Booking key) {
    BookingNode *newNode = (BookingNode *)malloc(sizeof(BookingNode));
    newNode->booking = key;
    newNode->left = newNode->right = NULL;
    return newNode;
}

int bstInsert(BookingNode **root, Booking x) {
    if (*root == NULL) {
        *root = createNode(x);
        return 1; // Insertion successful
    }

    // Compare BookingIDs to ensure uniqueness
    int idComparison = strcasecmp(x.BookingID, (*root)->booking.BookingID);

    if (idComparison == 0) {
        // Duplicate BookingID, handle accordingly
        printf("Duplicate BookingID: %s\n", x.BookingID);
        return 0; // Insertion failed
    } else if ((compareDates(x.CheckOutDate, (*root)->booking.CheckInDate) == -1) || (compareDates(x.CheckOutDate, (*root)->booking.CheckInDate) == 0)) {
        return bstInsert(&((*root)->left), x);
    } else if ((compareDates(x.CheckInDate, (*root)->booking.CheckOutDate) == 1) || (compareDates(x.CheckInDate, (*root)->booking.CheckOutDate) == 0)) {
        return bstInsert(&((*root)->right), x);
    } else {
        printf("The room is unavailable on that day.\n");
        return 0; // Insertion failed
    }
}

// Public function to insert into the BST
int bstTreeInsert(BookingTree *tree, Booking x) {
    return bstInsert(&(tree->root), x);
}

// Function to search for a node by BookingID in the BST
BookingNode* bstSearchByBookingID(BookingNode* root, const char* bookingID, BookingNode** parent) {
    if (root != NULL) {
        // First, search the left subtree
        BookingNode* leftResult = bstSearchByBookingID(root->left, bookingID, parent);
        // If found in the left subtree, return it
        if (leftResult != NULL) {
            return leftResult;
        }
        // Check the current node
        if (strcasecmp(root->booking.BookingID, bookingID) == 0) {
            return root;
        }
        // If not found in the left subtree or the current node, search the right subtree
        *parent = root; // Update the parent before going to the right subtree
        return bstSearchByBookingID(root->right, bookingID, parent);
    }
    // BookingID not found
    return NULL;
}

// Recursive helper function for searching by CustomerID
void bstSearchByCustomerIDRecursive(BookingNode* root, const char* customerID, BookingNodeList** resultList) {
    if (root != NULL) {
        // First, search the left subtree
        bstSearchByCustomerIDRecursive(root->left, customerID, resultList);

        // Check the current node's CustomerID
        if (strcasecmp(root->booking.CustomerID, customerID) == 0) {
            // If there's a match, create a new node for the result list
            BookingNodeList* newNode = (BookingNodeList*)malloc(sizeof(BookingNodeList));
            if (newNode != NULL) {
                newNode->node = root;
                newNode->next = *resultList;
                *resultList = newNode;
            }
        }

        // Continue searching the right subtree
        bstSearchByCustomerIDRecursive(root->right, customerID, resultList);
    }
}

// Function to search for all nodes with a specific CustomerID in the BST
BookingNodeList* bstSearchByCustomerID(BookingNode* root, const char* customerID) {
    // Initialize the result list
    BookingNodeList* resultList = NULL;

    // Start the recursive search
    bstSearchByCustomerIDRecursive(root, customerID, &resultList);

    return resultList;
}

// Public function to search for a node by BookingID in the BST
BookingNode* bstTreeSearchByBookingID(BookingTree* tree, const char* bookingID) {
    BookingNode* parent = NULL;  // Declare the parent variable
    return bstSearchByBookingID(tree->root, bookingID, &parent);
}

// In-order traversal of the BST
void displayBookinginOrderTraversal(BookingNode *root) {
    if (root != NULL) {
        displayBookinginOrderTraversal(root->left);
        printf("%-12s %-12s %-12s %02d/%02d/%04d   %02d/%02d/%04d\n", 
        root->booking.BookingID, root->booking.CustomerID, root->booking.BookingDate, 
        root->booking.CheckInDate.day, root->booking.CheckInDate.month, root->booking.CheckInDate.year,
        root->booking.CheckOutDate.day, root->booking.CheckOutDate.month, root->booking.CheckOutDate.year);
        displayBookinginOrderTraversal(root->right);
    }
}

//Function to display Booking node info
void displayBookingNodeInfo(BookingNode* node) {
    if (node != NULL) {
        printf("BookingID: %s\n", node->booking.BookingID);
        printf("CustomerID: %s\n", node->booking.CustomerID);
        printf("BookingDate: %s\n", node->booking.BookingDate);
        printf("CheckInDate: %02d/%02d/%04d\n", node->booking.CheckInDate.day, node->booking.CheckInDate.month, node->booking.CheckInDate.year);
        printf("CheckOutDate: %02d/%02d/%04d\n", node->booking.CheckOutDate.day, node->booking.CheckOutDate.month, node->booking.CheckOutDate.year);
    } else {
        printf("BookingID not found.\n");
    }
}

// Function to display Booking node info in a line
void displayBookingNodeInfoinline (BookingNode* node) {
    if (node != NULL) {
        printf ("BookingID: %s, CustomerID: %s, BookingDate: %s, CheckInDate: %02d/%02d/%04d, CheckOutDate: %02d/%02d/%04d\n", 
        node->booking.BookingID, node->booking.CustomerID, node->booking.BookingDate,
        node->booking.CheckInDate.day, node->booking.CheckInDate.month, node->booking.CheckInDate.year,
        node->booking.CheckOutDate.day, node->booking.CheckOutDate.month, node->booking.CheckOutDate.year);
    }
}

// Function to display Booking list
void displayBookingList(BookingNodeList* resultList) {
    while (resultList != NULL) {
        // Process the current node (e.g., display information)
        displayBookingNodeInfoinline(resultList->node);
        // Move to the next node
        resultList = resultList->next;
    }
}

// Function to delete a node from the BST
BookingNode* bstDeleteByID(BookingNode* root, const char* bookingID) {
    // Dummy node to handle the case where the root itself needs to be deleted
    BookingNode dummy = {
    {"", "", "", {0, 0, 0}, {0, 0, 0}},
    NULL,
    NULL};
    dummy.left = root;

    // Use the search function to find the node to be deleted and its parent
    BookingNode* parent = &dummy;
    BookingNode* toDelete = bstSearchByBookingID(root, bookingID, &parent);

    if (toDelete != NULL) {
        if (toDelete->left == NULL && toDelete->right == NULL) {
            // Case 1: Node with no child
            if (parent->left == toDelete) {
                parent->left = NULL;
            } else {
                parent->right = NULL;
            }
            free(toDelete);
        } else if (toDelete->left == NULL || toDelete->right == NULL) {
            // Case 2: Node with one child
            BookingNode* child = (toDelete->left != NULL) ? toDelete->left : toDelete->right;
            if (parent->left == toDelete) {
                parent->left = child;
            } else {
                parent->right = child;
            }
            free(toDelete);
        } else {
            // Case 3: Node with two children
            // Find the in-order successor (smallest node in the right subtree)
            BookingNode* successor = toDelete->right;
            parent = toDelete;
            while (successor->left != NULL) {
                parent = successor;
                successor = successor->left;
            }
            // Copy the data from the in-order successor
            strcpy(toDelete->booking.BookingID, successor->booking.BookingID);
            toDelete->booking.CheckInDate = successor->booking.CheckInDate;
            toDelete->booking.CheckOutDate = successor->booking.CheckOutDate;

            // Delete the in-order successor
            if (parent->left == successor) {
                parent->left = bstDeleteByID(parent->left, successor->booking.BookingID);
            } else {
                parent->right = bstDeleteByID(parent->right, successor->booking.BookingID);
            }
        }
        printf ("Successfully deleted booking\n");
    }

    // Update the root of the tree
    return dummy.left;
}

// Function to check empty room on specific day
int checkEmptyRoom(BookingNode* root, Date CheckIn, Date CheckOut) {
    // Cây rỗng, nên có phòng trống
    if (root == NULL) 
        return 1;

    int compare1 = compareDates(CheckIn, root->booking.CheckInDate);
    int compare2 = compareDates(CheckOut, root->booking.CheckInDate);
    int compare3 = compareDates(CheckIn, root->booking.CheckOutDate);
    int compare4 = compareDates(CheckOut, root->booking.CheckOutDate);

    if ((compare1 <= 0 && compare2 > 0) || (compare3 < 0 && compare4 >= 0)) {
        return 0;
    }

    if (compare2 <= 0) {
        // Kiểm tra ở nhánh trái và trả về kết quả của đệ quy
        return checkEmptyRoom(root->left, CheckIn, CheckOut);
    } else {
        // Kiểm tra ở nhánh phải và trả về kết quả của đệ quy
        return checkEmptyRoom(root->right, CheckIn, CheckOut);
    }
}

// Function to display if room is available on a specific day
void displaystatus(BookingNode* root, const char* RoomNumber, Date CheckIn, Date CheckOut) {
    if (checkEmptyRoom(root, CheckIn, CheckOut) == 1) {
        printf ("%-12s %-12s\n", RoomNumber, "Available");
    } else {
        printf ("%-12s %-12s\n", RoomNumber, "Unavailable");
    }
}

// Function to update new check-in, check-out date
void updateBookingDates(BookingTree* tree, const char* bookingID, Date newCheckInDate, Date newCheckOutDate) {
    BookingNode* toUpdate = bstTreeSearchByBookingID(tree, bookingID);

    // Check if the node with the given bookingID exists
    if (toUpdate != NULL) {
        // Create a new booking with updated dates
        if (checkEmptyRoom (tree->root, newCheckInDate, newCheckOutDate) == 1) {
            Booking newInfo;
            strcpy(newInfo.BookingID, toUpdate->booking.BookingID);
            strcpy(newInfo.CustomerID, toUpdate->booking.CustomerID);
            strcpy(newInfo.BookingDate, toUpdate->booking.BookingDate);
            newInfo.CheckInDate = newCheckInDate;
            newInfo.CheckOutDate = newCheckOutDate;

            // Delete the existing node
            tree->root = bstDeleteByID(tree->root, bookingID);

            // Insert the updated booking information
            bstTreeInsert(tree, newInfo);
            system ("clear");
            printf("Update successfully.\n");
        } else {
            printf ("The room is not available on that day.\n");
            printf ("Update failed.\n");
        }
    }
}

// Function to read data from file and insert into the binary search tree
void loadBookingsFromFile(BookingTree *tree, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    char line[200];  // Assuming a maximum line length of 200 characters
    while (fgets(line, sizeof(line), file)) {
        Booking booking;
        if (sscanf(line, "%7[^,], %11[^,], %19[^,], %d/%d/%d, %d/%d/%d",
                  booking.BookingID, booking.CustomerID, booking.BookingDate,
                  &booking.CheckInDate.day, &booking.CheckInDate.month, &booking.CheckInDate.year,
                  &booking.CheckOutDate.day, &booking.CheckOutDate.month, &booking.CheckOutDate.year) == 9) {
            bstTreeInsert(tree, booking);
        }
    }
    fclose(file);
}

// Helper function to write a single booking to the file
void writeBookingToFile(FILE *file, BookingNode *node) {
    fprintf(file, "%s, %s, %s, %02d/%02d/%04d, %02d/%02d/%04d\n",
            node->booking.BookingID, node->booking.CustomerID, node->booking.BookingDate,
            node->booking.CheckInDate.day, node->booking.CheckInDate.month, node->booking.CheckInDate.year,
            node->booking.CheckOutDate.day, node->booking.CheckOutDate.month, node->booking.CheckOutDate.year);
}

// Helper function for in-order traversal to write each booking to the file
void inOrderWriteToFile(FILE *file, BookingNode *node) {
    if (node != NULL) {
        inOrderWriteToFile(file, node->left);
        writeBookingToFile(file, node);
        inOrderWriteToFile(file, node->right);
    }
}

// Function to save data from the binary search tree to a file
void saveBookingsToFile(BookingNode *root, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }

    // Start the in-order traversal from the root
    inOrderWriteToFile(file, root);

    fclose(file);
}

// Function to select a BookingTree based on user input
BookingTree* selectBookingTree(BookingTree* tree101, BookingTree* tree102, BookingTree* tree103, BookingTree* tree201, BookingTree* tree202,
    BookingTree* tree301, BookingTree* tree302, BookingTree* tree303, BookingTree* tree401, BookingTree* tree402) {
    int selectedTree;
        printf("Enter RoomNumber: ");
        scanf("%d", &selectedTree);

    if (selectedTree == 101) {
        return tree101;
    } else if (selectedTree == 102) {
        return tree102;
    } else if (selectedTree == 103) {
        return tree103;
    } else if (selectedTree == 201) {
        return tree201;
    } else if (selectedTree == 202) {
        return tree202;
    } else if (selectedTree == 301) {
        return tree301;
    } else if (selectedTree == 302) {
        return tree302;
    } else if (selectedTree == 303) {
        return tree303;
    } else if (selectedTree == 401) {
        return tree401;
    } else if (selectedTree == 402) {
        return tree402;
    }else {
        printf("Invalid selection.\n");
        return NULL;
    }
}

int main() {
    char expectedUsername[50] = "admin";
    char expectedPassword[50] = "group11";

    char enteredUsername[50];
    char enteredPassword[50];

    printf(ANSI_COLOR_BLUE "\n");
    printf("#######################################################\n");
    printf("#                                                     #\n");
    printf("#    " ANSI_COLOR_YELLOW "WELCOME TO HOMESTAY MANAGEMENT SYSTEM PROJECT" ANSI_COLOR_BLUE "    #\n");
    printf("#                                                     #\n");
    printf("#######################################################\n");
    printf(ANSI_COLOR_RESET);

    // Enter username and password from the user
    printf("\nLog in to the system:\n\n");
    printf("Username: ");
    scanf("%49s", enteredUsername);
    clearBuffer(); // Clear input buffer
    
    printf("Password: ");
    scanf("%49s", enteredPassword);
    // Check login information
    if (strcmp(enteredUsername, expectedUsername) == 0 && strcmp(enteredPassword, expectedPassword) == 0) {
        // User has successfully logged in, allow access to the system
        system("clear");
        int choice;
        RoomNode* roomhead = NULL;
        roomhead = loadRoomsFromFile(&roomhead, "roomlist.txt");

        CustomerNode* customerhead = NULL;
        customerhead = loadCustomersFromFile(&customerhead, "customerlist.txt");

        BookingTree tree101;
        BookingTree tree102;
        BookingTree tree103;
        BookingTree tree201;
        BookingTree tree202;
        BookingTree tree301;
        BookingTree tree302;
        BookingTree tree303;
        BookingTree tree401;
        BookingTree tree402;

        bstInit(&tree101);
        bstInit(&tree102);
        bstInit(&tree103);
        bstInit(&tree201);
        bstInit(&tree202);
        bstInit(&tree301);
        bstInit(&tree302);
        bstInit(&tree303);
        bstInit(&tree401);
        bstInit(&tree402);

        loadBookingsFromFile(&tree101, "101.txt");
        loadBookingsFromFile(&tree102, "102.txt");
        loadBookingsFromFile(&tree103, "103.txt");
        loadBookingsFromFile(&tree201, "201.txt");
        loadBookingsFromFile(&tree202, "202.txt");
        loadBookingsFromFile(&tree301, "301.txt");
        loadBookingsFromFile(&tree302, "302.txt");
        loadBookingsFromFile(&tree303, "303.txt");
        loadBookingsFromFile(&tree401, "401.txt");
        loadBookingsFromFile(&tree402, "402.txt");

        do {
            // Display menu options
            printf(ANSI_COLOR_BLUE "\n");
            printf("#######################################################\n");
            printf("#                                                     #\n");
            printf("#    " ANSI_COLOR_YELLOW "WELCOME TO HOMESTAY MANAGEMENT SYSTEM PROJECT" ANSI_COLOR_BLUE "    #\n");
            printf("#                                                     #\n");
            printf("#######################################################\n");
            printf(ANSI_COLOR_RESET);
            printf("\nPress 1 to Manage Booking\n");
            printf("Press 2 to Manage Room\n");
            printf("Press 3 to Manage Customer\n");
            printf("Press 4 to Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            clearBuffer(); // Clear input buffer
            system("clear");

            switch (choice) {
                case 1: { //Booking management 
                    int BookingChoice;
                    system("clear");
                    do{
                    printf("\n");
                    printf("             Homestay Management System\n");
                    printf("-----------------------------------------------------------\n");
                    printf("        1. Add Booking\n");
                    printf("        2. Search Booking (by BookingID)\n");
                    printf("        3. Delete Booking (by BookingID)\n");
                    printf("        4. Update Booking (by BookingID)\n");
                    printf("        5. Display all Booking\n");
                    printf("        6. Exit\n");
                    printf("-----------------------------------------------------------\n");
                    printf("Enter your choice: ");
                    scanf("%d", &BookingChoice);
                    clearBuffer(); // Clear input buffer
                    system("clear");

                    switch (BookingChoice) {
                        case 1: { //add booking
                            displayRoomList(roomhead);
                            BookingTree* selectedTreePtr = selectBookingTree(&tree101, &tree102, &tree103, &tree201, &tree202, &tree301, &tree302, &tree303, &tree401, &tree402);
                            while (selectedTreePtr == NULL) {
                                system ("clear");
                                displayRoomList(roomhead);
                                printf("Invalid Room Number.\n");
                                selectedTreePtr = selectBookingTree(&tree101, &tree102, &tree103, &tree201, &tree202, &tree301, &tree302, &tree303, &tree401, &tree402);;  // Exit with an error code
                            }

                            system("clear");
                            Booking newBooking;
                            printf ("Enter Booking Information\n"); 
                            getBookingInfo (&newBooking);
                            system("clear");

                            // Attempt to insert the new booking into the selected tree
                            int insertionSuccessful = bstTreeInsert(selectedTreePtr, newBooking);

                            if (insertionSuccessful) {
                                // Rest of your code for handling successful insertion
                                const char* customerID = newBooking.CustomerID;
                                CustomerNode* foundnode = searchCustomerByID(customerhead, customerID);
                                if (foundnode == NULL) {
                                    printf ("Enter Customer Information\n");
                                    Customer newCustomer;
                                    strcpy(newCustomer.ID, customerID);
                                    getCustomerInfo(&newCustomer);
                                    insertCustomer(&customerhead, newCustomer);
                                    system ("clear");
                                    printf ("Booking successful.\n");
                                    printf("Enter customer information successfully.\n");
                                    // Proceed with additional operations related to the customer
                                } else {
                                    printf ("Booking successful.\n");
                                    printf("Customer information already exists\n");
                                    // Handle the case where the customer information already exists
                                }
                            }
                            else {
                                printf("Booking failed");
                            }

                            clearBuffer();
                            printf("\nPress any key to continue...");
                            getchar(); // Đọc một ký tự từ bàn phím (bất kỳ phím nào)
                            system("clear");
                            break;
                        }
                        case 2: { //search booking
                            // Provide the targetBookingID
                            char targetBookingID[9]; 
                            printf ("Enter BookingID to search: ");
                            scanf("%8s", targetBookingID);
                            clearBuffer(); // Clear input buffer

                            BookingNode* foundNode = bstTreeSearchByBookingID(&tree101, targetBookingID);
                            if (foundNode == NULL) {
                                foundNode = bstTreeSearchByBookingID(&tree102, targetBookingID);
                            } if (foundNode == NULL) {
                                foundNode = bstTreeSearchByBookingID(&tree103, targetBookingID);
                            } if (foundNode == NULL) {
                                foundNode = bstTreeSearchByBookingID(&tree201, targetBookingID);
                            } if (foundNode == NULL) {
                                foundNode = bstTreeSearchByBookingID(&tree202, targetBookingID);
                            } if (foundNode == NULL) {
                                foundNode = bstTreeSearchByBookingID(&tree301, targetBookingID);
                            } if (foundNode == NULL) {
                                foundNode = bstTreeSearchByBookingID(&tree302, targetBookingID);
                            } if (foundNode == NULL) {
                                foundNode = bstTreeSearchByBookingID(&tree302, targetBookingID);
                            } if (foundNode == NULL) {
                                foundNode = bstTreeSearchByBookingID(&tree401, targetBookingID);
                            } if (foundNode == NULL) {
                                foundNode = bstTreeSearchByBookingID(&tree402, targetBookingID);
                            }
                            system ("clear");
                            if (foundNode != NULL) {
                                printf("Information of Booking with ID %s:\n", targetBookingID);
                                displayBookingNodeInfo(foundNode);
                            } else {
                                printf ("Booking is not found.\n");
                            }

                            printf("\nPress any key to continue...");
                            getchar(); // Đọc một ký tự từ bàn phím (bất kỳ phím nào)
                            system("clear");                         
                            break;
                        }
                        case 3: { //delete booking
                            char targetBookingID[9];
                            printf("\nEnter BookingID to delete: ");
                            scanf("%8s", targetBookingID);
                            clearBuffer(); // Clear input buffer

                            tree101.root = bstDeleteByID(tree101.root, targetBookingID);
                            tree102.root = bstDeleteByID(tree102.root, targetBookingID);
                            tree103.root = bstDeleteByID(tree103.root, targetBookingID);
                            tree201.root = bstDeleteByID(tree201.root, targetBookingID);
                            tree202.root = bstDeleteByID(tree202.root, targetBookingID);
                            tree301.root = bstDeleteByID(tree301.root, targetBookingID);
                            tree302.root = bstDeleteByID(tree302.root, targetBookingID);
                            tree303.root = bstDeleteByID(tree303.root, targetBookingID);
                            tree401.root = bstDeleteByID(tree401.root, targetBookingID);
                            tree402.root = bstDeleteByID(tree402.root, targetBookingID);

                            printf("\nPress any key to continue...");
                            getchar(); // Đọc một ký tự từ bàn phím (bất kỳ phím nào)
                            system("clear");
                            break;
                        }
                        case 4: { //update booking
                            char targetBookingID[9];
                            printf("\nEnter BookingID to update: ");
                            scanf("%8s", targetBookingID);
                            clearBuffer(); // Clear input buffer

                            Date newCheckIn, newCheckOut;
                            printf ("Enter new CheckIn Date (DD/MM/YYYY): ");
                            scanf("%d/%d/%d", &newCheckIn.day, &newCheckIn.month, &newCheckIn.year);
                            clearBuffer();
                            printf ("Enter new CheckOut Date (DD/MM/YYYY): ");
                            scanf("%d/%d/%d", &newCheckOut.day, &newCheckOut.month, &newCheckOut.year);
                            clearBuffer();

                            updateBookingDates (&tree101, targetBookingID, newCheckIn, newCheckOut);
                            updateBookingDates (&tree102, targetBookingID, newCheckIn, newCheckOut);
                            updateBookingDates (&tree103, targetBookingID, newCheckIn, newCheckOut);
                            updateBookingDates (&tree201, targetBookingID, newCheckIn, newCheckOut);
                            updateBookingDates (&tree202, targetBookingID, newCheckIn, newCheckOut);
                            updateBookingDates (&tree301, targetBookingID, newCheckIn, newCheckOut);
                            updateBookingDates (&tree302, targetBookingID, newCheckIn, newCheckOut);
                            updateBookingDates (&tree303, targetBookingID, newCheckIn, newCheckOut);
                            updateBookingDates (&tree401, targetBookingID, newCheckIn, newCheckOut);
                            updateBookingDates (&tree402, targetBookingID, newCheckIn, newCheckOut);

                            printf("\nPress any key to continue...");
                            getchar(); // Đọc một ký tự từ bàn phím (bất kỳ phím nào)
                            system("clear");
                            break;
                        }
                        case 5: { //display all booking
                            if (tree101.root != NULL) {
                                printf("Booking List of Room 101\n");
                                printf("%-12s %-12s %-12s %-12s %-12s\n", "BookingID", "CustomerID", "BookingDate", "CheckInDate", "CheckOutDate");
                                displayBookinginOrderTraversal(tree101.root);
                                printf ("--------------------------------------------------------------\n");
                            }
                            if (tree102.root != NULL) {
                                printf("Booking List of Room 102\n");
                                printf("%-12s %-12s %-12s %-12s %-12s\n", "BookingID", "CustomerID", "BookingDate", "CheckInDate", "CheckOutDate");
                                displayBookinginOrderTraversal(tree102.root);
                                printf ("--------------------------------------------------------------\n");
                            }
                            if (tree103.root != NULL) {
                                printf("Booking List of Room 103\n");
                                printf("%-12s %-12s %-12s %-12s %-12s\n", "BookingID", "CustomerID", "BookingDate", "CheckInDate", "CheckOutDate");
                                displayBookinginOrderTraversal(tree103.root);
                                printf ("--------------------------------------------------------------\n");
                            }
                            if (tree201.root != NULL) {
                                printf("Booking List of Room 201\n");
                                printf("%-12s %-12s %-12s %-12s %-12s\n", "BookingID", "CustomerID", "BookingDate", "CheckInDate", "CheckOutDate");
                                displayBookinginOrderTraversal(tree201.root);
                                printf ("--------------------------------------------------------------\n");
                            }
                            if (tree202.root != NULL) {
                                printf("Booking List of Room 202\n");
                                printf("%-12s %-12s %-12s %-12s %-12s\n", "BookingID", "CustomerID", "BookingDate", "CheckInDate", "CheckOutDate");
                                displayBookinginOrderTraversal(tree202.root);
                                printf ("--------------------------------------------------------------\n");
                            }
                            if (tree301.root != NULL) {
                                printf("Booking List of Room 301\n");
                                printf("%-12s %-12s %-12s %-12s %-12s\n", "BookingID", "CustomerID", "BookingDate", "CheckInDate", "CheckOutDate");
                                displayBookinginOrderTraversal(tree301.root);
                                printf ("--------------------------------------------------------------\n");
                            }
                            if (tree302.root != NULL) {
                                printf("Booking List of Room 302\n");
                                printf("%-12s %-12s %-12s %-12s %-12s\n", "BookingID", "CustomerID", "BookingDate", "CheckInDate", "CheckOutDate");
                                displayBookinginOrderTraversal(tree302.root);
                                printf ("--------------------------------------------------------------\n");
                            }
                            if (tree303.root != NULL) {
                                printf("Booking List of Room 303\n");
                                printf("%-12s %-12s %-12s %-12s %-12s\n", "BookingID", "CustomerID", "BookingDate", "CheckInDate", "CheckOutDate");
                                displayBookinginOrderTraversal(tree303.root);
                                printf ("--------------------------------------------------------------\n");
                            }
                            if (tree401.root != NULL) {
                                printf("Booking List of Room 401\n");
                                printf("%-12s %-12s %-12s %-12s %-12s\n", "BookingID", "CustomerID", "BookingDate", "CheckInDate", "CheckOutDate");
                                displayBookinginOrderTraversal(tree401.root);
                                printf ("--------------------------------------------------------------\n");
                            }
                            if (tree402.root != NULL) {
                                printf("Booking List of Room 402\n");
                                printf("%-12s %-12s %-12s %-12s %-12s\n", "BookingID", "CustomerID", "BookingDate", "CheckInDate", "CheckOutDate");
                                displayBookinginOrderTraversal(tree402.root);
                                printf ("--------------------------------------------------------------\n");
                            }
                            printf("\nPress any key to continue...");
                            getchar(); // Đọc một ký tự từ bàn phím (bất kỳ phím nào)
                            system("clear");
                            break;
                        }
                        case 6: { //exit
                            system("clear");
                            break;
                        }
                        default:
                            printf("Invalid choice. Please try again.\n");
                    }
                    } while (BookingChoice != 6);
                    break;
                }
                case 2: { //Room management
                    int RoomChoice;
                    system("clear");
                    do{
                    printf("\n");
                    printf("             Homestay Management System\n");
                    printf("-----------------------------------------------------------\n");
                    printf("        1. Check Room Status (available/unavailable)\n");
                    printf("        2. Update Room Type\n");
                    printf("        3. Display all Rooms\n");
                    printf("        4. Exit\n");
                    printf("-----------------------------------------------------------\n");
                    printf("Enter your choice: ");
                    scanf("%d", &RoomChoice);
                    clearBuffer(); // Clear input buffer
                    system ("clear");

                    switch (RoomChoice) { 
                        case 1: { //check room status
                            Date CheckIn, CheckOut;
                            printf ("Enter CheckIn Date (DD/MM/YYYY): ");
                            scanf("%d/%d/%d", &CheckIn.day, &CheckIn.month, &CheckIn.year);
                            printf ("Enter CheckOut Date (DD/MM/YYYY): ");
                            scanf("%d/%d/%d", &CheckOut.day, &CheckOut.month, &CheckOut.year);
                            printf("------Room Status-----\n");
                            printf("%-12s %-12s\n", "Room Number", "Status");
                            
                            displaystatus(tree101.root, "101", CheckIn, CheckOut);
                            displaystatus(tree102.root, "102", CheckIn, CheckOut);
                            displaystatus(tree103.root, "103", CheckIn, CheckOut);
                            displaystatus(tree201.root, "201", CheckIn, CheckOut);
                            displaystatus(tree202.root, "202", CheckIn, CheckOut);
                            displaystatus(tree301.root, "301", CheckIn, CheckOut);
                            displaystatus(tree302.root, "302", CheckIn, CheckOut);
                            displaystatus(tree303.root, "303", CheckIn, CheckOut);
                            displaystatus(tree401.root, "401", CheckIn, CheckOut);
                            displaystatus(tree402.root, "402", CheckIn, CheckOut);
                        
                            clearBuffer();
                            printf("\nPress any key to continue...");
                            getchar(); // Đọc một ký tự từ bàn phím (bất kỳ phím nào)
                            system("clear");
                            break;
                        }
                        case 2: { //update room type
                            char roomnumber[4];
                            char newRoomType [20];
                            printf("\nEnter RoomNumber to update: ");
                            scanf("%3s", roomnumber);
                            printf("Enter new RoomType to update: ");
                            scanf("%19s", newRoomType);
                            clearBuffer();
                            updateRoomType(roomhead, roomnumber, newRoomType);
                            
                            printf("\nPress any key to continue...");
                            getchar(); // Đọc một ký tự từ bàn phím (bất kỳ phím nào)
                            system("clear");
                            break;
                        }
                        case 3: { //display room list
                            displayRoomList(roomhead);

                            printf("\nPress any key to continue...");
                            getchar(); // Đọc một ký tự từ bàn phím (bất kỳ phím nào)
                            system("clear");
                            break;
                        }
                        case 4: { //exit
                            system("clear");
                            break;
                        }
                        default:
                            printf("Invalid choice. Please try again.\n");
                    }
                    } while (RoomChoice != 4);
                    break;
                }
                case 3: { //Customer management
                    int CustomerChoice;
                    system("clear");
                    do{
                    printf("\n");
                    printf("             Homestay Management System\n");
                    printf("-----------------------------------------------------------\n");
                    printf("        1. Search Booking History of Customer\n");
                    printf("        2. Search Customer (by CustomerID)\n");
                    printf("        3. Display all Customers\n");
                    printf("        4. Exit\n");
                    printf("-----------------------------------------------------------\n");
                    printf("Enter your choice: ");
                    scanf("%d", &CustomerChoice);
                    clearBuffer(); // Clear input buffer
                    system("clear");

                    switch (CustomerChoice) {
                        case 1: { //search booking history of a customer
                            char targetCustomerID[12]; 
                            printf ("Enter CustomerID to search booking history: ");
                            scanf("%11s", targetCustomerID);
                            clearBuffer();

                            BookingNodeList* resultList101 = bstSearchByCustomerID(tree101.root, targetCustomerID);
                            BookingNodeList* resultList102 = bstSearchByCustomerID(tree102.root, targetCustomerID);
                            BookingNodeList* resultList103 = bstSearchByCustomerID(tree103.root, targetCustomerID);
                            BookingNodeList* resultList201 = bstSearchByCustomerID(tree201.root, targetCustomerID);
                            BookingNodeList* resultList202 = bstSearchByCustomerID(tree202.root, targetCustomerID);
                            BookingNodeList* resultList301 = bstSearchByCustomerID(tree301.root, targetCustomerID);
                            BookingNodeList* resultList302 = bstSearchByCustomerID(tree302.root, targetCustomerID);
                            BookingNodeList* resultList303 = bstSearchByCustomerID(tree303.root, targetCustomerID);
                            BookingNodeList* resultList401 = bstSearchByCustomerID(tree401.root, targetCustomerID);
                            BookingNodeList* resultList402 = bstSearchByCustomerID(tree402.root, targetCustomerID);

                            displayBookingList(resultList101);
                            displayBookingList(resultList102);
                            displayBookingList(resultList103);
                            displayBookingList(resultList201);
                            displayBookingList(resultList202);
                            displayBookingList(resultList301);
                            displayBookingList(resultList302);
                            displayBookingList(resultList303);
                            displayBookingList(resultList401);
                            displayBookingList(resultList402);

                            if (resultList101 == NULL && resultList102 == NULL && resultList103 == NULL && resultList201 == NULL && resultList202 == NULL
                            && resultList301 == NULL && resultList302 == NULL && resultList303 == NULL && resultList401 == NULL && resultList402 == NULL) {
                                printf("No booking found.\n");
                            }
                            
                            printf("\nPress any key to continue...");
                            getchar(); // Đọc một ký tự từ bàn phím (bất kỳ phím nào)
                            system("clear");
                            break;
                        }
                        case 2: { //search customer by customerID
                            char customerID[12];
                            printf("Enter CustomerID to search: ");
                            scanf("%11s", customerID);
                            clearBuffer();

                            CustomerNode* foundNode = searchCustomerByID(customerhead, customerID);
                            if (foundNode != NULL) {
                                displayCustomerNodeInfo(foundNode);
                            }
                            else {
                                printf ("Not found.\n");
                            }
                            
                            printf("\nPress any key to continue...");
                            getchar(); // Đọc một ký tự từ bàn phím (bất kỳ phím nào)
                            system("clear");
                            break;
                        }
                        case 3: { //display all customers
                            displayCustomerList(customerhead);
                            
                            printf("\nPress any key to continue...");
                            getchar(); // Đọc một ký tự từ bàn phím (bất kỳ phím nào)
                            system("clear");
                            break;
                        }
                        case 4: { //exit
                            system("clear");
                            break;
                        }
                        default:
                            printf("Invalid choice. Please try again.\n");
                    }
                    } while (CustomerChoice != 4);
                    break;
                }              
                case 4: { //save and exit
                    printf("Exiting...\n");

                    saveCustomersToFile(customerhead, "customerlist.txt");
                    saveRoomsToFile(roomhead, "roomlist.txt");

                    saveBookingsToFile(tree101.root, "101.txt");
                    saveBookingsToFile(tree102.root, "102.txt");
                    saveBookingsToFile(tree103.root, "103.txt");
                    saveBookingsToFile(tree201.root, "201.txt");
                    saveBookingsToFile(tree202.root, "202.txt");
                    saveBookingsToFile(tree301.root, "301.txt");
                    saveBookingsToFile(tree302.root, "302.txt");
                    saveBookingsToFile(tree303.root, "303.txt");
                    saveBookingsToFile(tree401.root, "401.txt");
                    saveBookingsToFile(tree402.root, "402.txt");
                    
                    system("clear");
                    break;
                }
                default:
                    printf("Invalid choice. Please try again.\n");
            }
            } while (choice != 4);
    } else {
        printf("\nInvalid username or password. Access denied.\n");
    }
    return 0;
}
