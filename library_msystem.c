#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// structure to represent a user
struct User {
    char username[50];
    char password[50];
    struct User* next;
};

// structure to represent a book
struct Book {
    int bookId;
    char title[100];
    char author[100];
    int isIssued; // 1 if issued, 0 if not issued
    time_t issueDate; // Issue date (time in seconds since epoch)
    struct Book* next;
};

// create a new user node
struct User* createUserNode(char* username, char* password) {
    struct User* newUser = (struct User*)malloc(sizeof(struct User));
    if (newUser == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    strcpy(newUser->username, username);
    strcpy(newUser->password, password);
    newUser->next = NULL;
    return newUser;
}

// insert a new user into the linked list
void insertUser(struct User** head, struct User* newUser) {
    if (*head == NULL) {
        *head = newUser;
    } else {
        struct User* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newUser;
    }
}

// delete a user from the linked list
void deleteUser(struct User** head, char* username) {
    struct User* current = *head;
    struct User* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            if (prev == NULL) {
                *head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("\t\t\t\tUser '%s' not found in the system.\n", username);
}

// create a new book node
struct Book* createBookNode(int bookId, char* title, char* author) {
    struct Book* newBook = (struct Book*)malloc(sizeof(struct Book));
    if (newBook == NULL) {
        printf("\t\t\t\tMemory allocation failed.\n");
        exit(1);
    }
    newBook->bookId = bookId;
    strcpy(newBook->title, title);
    strcpy(newBook->author, author);
    newBook->isIssued = 0; // Initially, the book is not issued
    newBook->issueDate = 0; // Initialize issue date to 0
    newBook->next = NULL;
    return newBook;
}

// insert a new book into the linked list
void insertBook(struct Book** head, struct Book* newBook) {
    if (*head == NULL) {
        *head = newBook;
    } else {
        struct Book* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newBook;
    }
}

// delete a book from the linked list
void deleteBook(struct Book** head, int bookId) {
    struct Book* current = *head;
    struct Book* prev = NULL;

    while (current != NULL) {
        if (current->bookId == bookId) {
            if (prev == NULL) {
                *head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("\t\t\t\tBook with ID %d not found in the library.\n", bookId);
}

// View the list of books
void viewBooks(struct Book* head) {
    struct Book* current = head;
    if (current == NULL) {
        printf("\t\t\t\tThe library is empty.\n");
        return;
    }
    printf("\t\t\t\tList of books:\n");
    while (current != NULL) {
        printf("\t\t\t\tBook ID: %d\n", current->bookId);
        printf("\t\t\t\tTitle: %s\n", current->title);
        printf("\t\t\t\tAuthor: %s\n", current->author);
        printf("\t\t\t\tStatus: %s\n", current->isIssued ? "Issued" : "Available");
        if (current->isIssued) {
            printf("\t\t\t\tIssue Date: %s", ctime(&current->issueDate));
        }
        printf("\n");
        current = current->next;
    }
}


// issue a book
void issueBook(struct Book* head, int bookId) {
    struct Book* current = head;

    while (current != NULL) {
        if (current->bookId == bookId) {
            if (current->isIssued) {
                printf("\t\t\t\tBook with ID %d is already issued.\n", bookId);
            } else {
                current->isIssued = 1;
                current->issueDate = time(NULL); // Set the issue date to the current time
                printf("\t\t\t\tBook with ID %d has been issued.\n", bookId);
            }
            return;
        }
        current = current->next;
    }

    printf("\t\t\t\tBook with ID %d not found in the library.\n", bookId);
}

// view the list of issued books
void viewIssuedBooks(struct Book* head) {
    struct Book* current = head;
    if (current == NULL) {
        printf("\t\t\t\tNo books are issued.\n");
        return;
    }
    printf("\t\t\t\tList of issued books:\n");
    while (current != NULL) {
        if (current->isIssued) {
            printf("\t\t\t\tBook ID: %d\n", current->bookId);
            printf("\t\t\t\tTitle: %s\n", current->title);
            printf("\t\t\t\tAuthor: %s\n", current->author);
            printf("\t\t\t\tIssue Date: %s", ctime(&current->issueDate));
            printf("\n");
        }
        current = current->next;
    }
}

// Function to save user data to file
void saveUsersToFile(struct User* head, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("\t\t\t\tError opening file for writing.\n");
        exit(1);
    }

    struct User* current = head;
    while (current != NULL) {
        fprintf(file, "%s %s\n", current->username, current->password);
        current = current->next;
    }

    fclose(file);
}

// Function to load user data from file
void loadUsersFromFile(struct User** head, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return; // File doesn't
        return; // File doesn't exist or error opening file; assume no users initially
    }

    char username[50];
    char password[50];

    while (fscanf(file, "%s %s", username, password) == 2) {
        struct User* newUser = createUserNode(username, password);
        insertUser(head, newUser);
    }

    fclose(file);
}

void saveBooksToFile(struct Book* head, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("\t\t\t\tError opening file for writing.\n");
        exit(1);
    }

    struct Book* current = head;
    while (current != NULL) {
        fprintf(file, "%d,%s,%s,%d,%ld\n", current->bookId, current->title, current->author, current->isIssued, current->issueDate);
        current = current->next;
    }

    fclose(file);
}


// Function to load book data from file
void loadBooksFromFile(struct Book** head, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("\t\t\t\tError opening file for reading or file not found: %s\n", filename);
        return;
    }

    int bookId;
    char title[100];
    char author[100];
    int isIssued;
    time_t issueDate;

    while (fscanf(file, "%d,%99[^,],%99[^,],%d,%ld", &bookId, title, author, &isIssued, &issueDate) == 5) {
        struct Book* newBook = createBookNode(bookId, title, author);
        newBook->isIssued = isIssued;
        newBook->issueDate = issueDate;
        insertBook(head, newBook);
    }

    fclose(file);
}



int main() {
    struct User* userList = NULL; // Initialize empty user list
    struct Book* library = NULL; // Initialize empty book library

    char username[50];
    char password[50];
    int bookId;
    char title[100];
    char author[100];
    char choice;

    loadUsersFromFile(&userList, "users.txt");
    loadBooksFromFile(&library, "books.txt");

    do {
        printf("\n\n\n\t\t\t<===============Library Management System===============>\n");
        printf("\t\t\t\t<===============Main Menu===============>\n");
        printf("\t\t\t\t1. Login\n");
        printf("\t\t\t\t2. Add a user\n");
        printf("\t\t\t\t3. Delete a user\n");
        printf("\t\t\t\t4. Add a book\n");
        printf("\t\t\t\t5. Delete a book\n");
        printf("\t\t\t\t6. View books\n");
        printf("\t\t\t\t7. Issue a book\n");
        printf("\t\t\t\t8. View issued books\n");
        printf("\t\t\t\t9. Exit\n");
        printf("\t\t\t\t<==============xxxxxxxxxxxx==============>\n");
        printf("\t\t\t\tEnter your choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                printf("\t\t\t\tEnter your username: ");
                scanf("%s", username);
                printf("\t\t\t\tEnter your password: ");
                scanf("%s", password);

                // Check if the user exists
                struct User* currentUser = userList;
                int isUserFound = 0;
                while (currentUser != NULL) {
                    if (strcmp(currentUser->username, username) == 0 && strcmp(currentUser->password, password) == 0) {
                        isUserFound = 1;
                        break;
                    }
                    currentUser = currentUser->next;
                }

                if (isUserFound) {
                    printf("\t\t\t\tLogin successful. Welcome, %s!\n", username);
                } else {
                    printf("\t\t\t\tLogin failed. Incorrect username or password.\n");
                }
                break;
            case '2':
                printf("\t\t\t\tEnter the username for the new user: ");
                scanf("%s", username);

                // Check if the username already exists
                struct User* existingUser = userList;
                int isUsernameTaken = 0;
                while (existingUser != NULL) {
                    if (strcmp(existingUser->username, username) == 0) {
                        isUsernameTaken = 1;
                        break;
                    }
                    existingUser = existingUser->next;
                }

                if (isUsernameTaken) {
                    printf("\t\t\t\tUsername already exists. Please choose a different one.\n");
                } else {
                    printf("\t\t\t\tEnter the password for the new user: ");
                    scanf("%s", password);
                    struct User* newUser = createUserNode(username, password);
                    insertUser(&userList, newUser);
                    printf("\t\t\t\tUser '%s' has been added to the system.\n", username);
                }
                break;
            case '3':
                printf("\t\t\t\tEnter the username to delete: ");
                scanf("%s", username);
                deleteUser(&userList, username);
                break;
            case '4':
                printf("\t\t\t\tEnter the book ID: ");
                scanf("%d", &bookId);
                printf("\t\t\t\tEnter the book title: ");
                scanf("\t\t\t\t%[^\n]", title);
                printf("\t\t\t\tEnter the author: ");
                scanf("\t\t\t\t%[^\n]", author);
                struct Book* newBook = createBookNode(bookId, title, author);
                insertBook(&library, newBook);
                printf("\t\t\t\tBook with ID %d has been added to the library.\n", bookId);
                break;
            case '5':
                printf("\t\t\t\tEnter the book ID to delete: ");
                scanf("%d", &bookId);
                deleteBook(&library, bookId);
                break;
            case '6':
                viewBooks(library);
                break;
            case '7':
                printf("\t\t\t\tEnter the book ID to issue: ");
                scanf("%d", &bookId);
                issueBook(library, bookId);
                break;
            case '8':
                viewIssuedBooks(library);
                break;
            case '9':
                printf("\t\t\t\tExiting the program.\n");
                break;
            default:
                printf("\t\t\t\tInvalid choice. Please try again.\n");
        }
    } while (choice != '9');

    saveUsersToFile(userList, "users.txt");
    saveBooksToFile(library, "books.txt");

    // Free allocated memory
    while (userList != NULL) {
        struct User* temp = userList;
        userList = userList->next;
        free(temp);
    }

    while (library != NULL) {
        struct Book* temp = library;
        library = library->next;
        free(temp);
    }

    return 0;
}

