#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct BookNode {
    char title[100];
    char author[100];
    int isbn;
    char genre[50];
    bool available;
    struct BookNode *left, *right;
} BookNode;

//Function declarations
BookNode* searchByIsbn(BookNode* root, int isbn);


BookNode* createBookNode(int isbn, char *title, char *author, char *genre, bool available) {
    BookNode* newNode = (BookNode*) malloc(sizeof(BookNode));
    newNode->isbn = isbn;
    strcpy(newNode->title, title);
    strcpy(newNode->author, author);
    strcpy(newNode->genre, genre);
    newNode->available = available;
    newNode->left = newNode->right = NULL;
    return newNode;
}

BookNode* insert(BookNode* node, int isbn, char *title, char *author, char *genre, bool available) {
    if (node == NULL) return createBookNode(isbn, title, author, genre, available);
    if (isbn < node->isbn)
        node->left = insert(node->left, isbn, title, author, genre, available);
    else if (isbn > node->isbn)
        node->right = insert(node->right, isbn, title, author, genre, available);
    return node;
}

BookNode* minValueNode(BookNode* node) {
    BookNode* current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

BookNode* deleteNode(BookNode* root, int isbn) {
    if (root == NULL) return root;
    if (isbn < root->isbn)
        root->left = deleteNode(root->left, isbn);
    else if (isbn > root->isbn)
        root->right = deleteNode(root->right, isbn);
    else {
        if (root->left == NULL) {
            BookNode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            BookNode* temp = root->left;
            free(root);
            return temp;
        }
        BookNode* temp = minValueNode(root->right);
        root->isbn = temp->isbn;
        strcpy(root->title, temp->title);
        strcpy(root->author, temp->author);
        strcpy(root->genre, temp->genre);
        root->available = temp->available;
        root->right = deleteNode(root->right, temp->isbn);
    }
    return root;
}

void inorderTraversal(BookNode* root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        printf("ISBN: %d, Title: %s, Author: %s, Genre: %s, Available: %s\n",
               root->isbn, root->title, root->author, root->genre, root->available ? "Yes" : "No");
        inorderTraversal(root->right);
    }
}

void checkOut(BookNode* root, int isbn) {
    BookNode* book = searchByIsbn(root, isbn);
    if (book != NULL && book->available) {
        book->available = false;
        printf("Book with ISBN %d has been checked out.\n", isbn);
    } else {
        printf("Book cannot be checked out.\n");
    }
}

void checkIn(BookNode* root, int isbn) {
    BookNode* book = searchByIsbn(root, isbn);
    if (book != NULL && !book->available) {
        book->available = true;
        printf("Book with ISBN %d has been checked in.\n", isbn);
    } else {
        printf("Book was not checked out.\n");
    }
}

BookNode* searchByIsbn(BookNode* root, int isbn) {
    if (root == NULL || root->isbn == isbn)
        return root;
    if (isbn < root->isbn)
        return searchByIsbn(root->left, isbn);
    return searchByIsbn(root->right, isbn);
}

void displayMenu() {
    printf("\nLibrary Management System\n");
    printf("1. Add Book\n");
    printf("2. Delete Book\n");
    printf("3. Check Out Book\n");
    printf("4. Check In Book\n");
    printf("5. List All Books\n");
    printf("6. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    BookNode* root = NULL;
    int choice, isbn;
    char title[100], author[100], genre[50];
    bool available;

    do {
        displayMenu();
        scanf("%d", &choice);
        getchar();  // consume newline

        switch (choice) {
            case 1:
                printf("Enter ISBN: ");
                scanf("%d", &isbn);
                getchar();
                printf("Enter Title: ");
                fgets(title, 100, stdin);
                title[strcspn(title, "\n")] = 0;  // remove newline
                printf("Enter Author: ");
                fgets(author, 100, stdin);
                author[strcspn(author, "\n")] = 0;  // remove newline
                printf("Enter Genre: ");
                fgets(genre, 50, stdin);
                genre[strcspn(genre, "\n")] = 0;  // remove newline
                printf("Is Available (1 for Yes, 0 for No): ");
                scanf("%d", &available);
                root = insert(root, isbn, title, author, genre, available);
                break;
            case 2:
                printf("Enter ISBN to delete: ");
                scanf("%d", &isbn);
                root = deleteNode(root, isbn);
                break;
            case 3:
                printf("Enter ISBN to check out: ");
                scanf("%d", &isbn);
                checkOut(root, isbn);
                break;
            case 4:
                printf("Enter ISBN to check in: ");
                scanf("%d", &isbn);
                checkIn(root, isbn);
                break;
            case 5:
                printf("Listing all books:\n");
                inorderTraversal(root);
                break;
            case 6:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice, please try again.\n");
        }
    } while (choice != 6);

    return 0;
}

