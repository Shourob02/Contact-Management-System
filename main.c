#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Contact {
    char fName[50];
    char lName[50];
    char address[100];
    char email[50];
    char phNo[15];
    struct Contact* next;
} Contact;

typedef struct {
    Contact* head;
} ContactList;

void createContact(Contact* contact) {
    printf("Enter your first name: ");
    scanf("%s", contact->fName);
    printf("Enter your last name: ");
    scanf("%s", contact->lName);
    printf("Enter phone: ");
    scanf("%s", contact->phNo);
    printf("Enter address: ");
    scanf("%s", contact->address);
    printf("Enter email: ");
    scanf("%s", contact->email);
    contact->next = NULL;
}

void showContact(Contact* contact) {
    printf("Name: %s %s\n", contact->fName, contact->lName);
    printf("Phone: %s\n", contact->phNo);
    printf("Address: %s\n", contact->address);
    printf("Email: %s\n", contact->email);
}

void addContact(ContactList* list) {
    Contact* newContact = (Contact*)malloc(sizeof(Contact));
    createContact(newContact);
    newContact->next = list->head;
    list->head = newContact;
    printf("Contact has been successfully created...\n");
}

void swapContacts(Contact* a, Contact* b) {
    Contact temp = *a;
    *a = *b;
    *b = temp;

    // Ensure the next pointers are not swapped
    Contact* tempNext = a->next;
    a->next = b->next;
    b->next = tempNext;
}

void bubbleSortContacts(ContactList* list) {
    if (list->head == NULL) {
        return;
    }

    int swapped;
    Contact* ptr1;
    Contact* lptr = NULL;

    do {
        swapped = 0;
        ptr1 = list->head;

        while (ptr1->next != lptr) {
            if (strcmp(ptr1->phNo, ptr1->next->phNo) > 0) {
                swapContacts(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

void displayContacts(ContactList* list) {
    if (list->head == NULL) {
        printf("No contacts available.\n");
        return;
    }

    bubbleSortContacts(list);

    Contact* current = list->head;
    printf("\n================================\n");
    printf("LIST OF CONTACTS");
    printf("\n================================\n");
    while (current != NULL) {
        showContact(current);
        printf("\n================================\n");
        current = current->next;
    }
}

void searchContact(ContactList* list) {
    char phone[15];
    printf("Enter phone no.: ");
    scanf("%s", phone);
    Contact* current = list->head;
    while (current != NULL) {
        if (strcmp(current->phNo, phone) == 0) {
            showContact(current);
            return;
        }
        current = current->next;
    }
    printf("\n\nNo record found\n");
}

void deleteContact(ContactList* list) {
    char phone[15];
    printf("Enter phone no. to delete: ");
    scanf("%s", phone);

    Contact* current = list->head;
    Contact* prev = NULL;

    while (current != NULL && strcmp(current->phNo, phone) != 0) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("\n\nContact not found...\n");
        return;
    }

    if (prev == NULL) {
        list->head = current->next;
    } else {
        prev->next = current->next;
    }

    free(current);
    printf("\n\nContact deleted...\n");
}

void editContact(ContactList* list) {
    char phone[15];
    printf("Enter the phone number to be edited: ");
    scanf("%s", phone);

    Contact* current = list->head;
    while (current != NULL) {
        if (strcmp(current->phNo, phone) == 0) {
            printf("Enter new record\n");
            createContact(current);
            printf("\n\nContact successfully updated...\n");
            return;
        }
        current = current->next;
    }
    printf("\n\nNo record found\n");
}

void writeToFile(ContactList* list) {
    FILE* f1 = fopen("CMS.dat", "wb");
    if (f1 == NULL) {
        printf("Error opening file!\n");
        return;
    }
    Contact* current = list->head;
    while (current != NULL) {
        fwrite(current, sizeof(Contact), 1, f1);
        current = current->next;
    }
    fclose(f1);
}

void readFromFile(ContactList* list) {
    FILE* f2 = fopen("CMS.dat", "rb");
    if (f2 == NULL) {
        return;
    }
    Contact temp;
    while (fread(&temp, sizeof(Contact), 1, f2)) {
        Contact* newContact = (Contact*)malloc(sizeof(Contact));
        *newContact = temp;
        newContact->next = list->head;
        list->head = newContact;
    }
    fclose(f2);
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int main() {
    ContactList contacts;
    contacts.head = NULL;
    readFromFile(&contacts);

    printf("\n\n\n\n\n\n\n\n\t\t\t * WELCOME TO CONTACT MANAGEMENT SYSTEM *");
    getchar();

    while (1) {
        int choice;
        clearScreen();
        printf("\nCONTACT MANAGEMENT SYSTEM");
        printf("\n\nMAIN MENU");
        printf("\n=====================\n");
        printf("[1] Add a new Contact\n");
        printf("[2] List all Contacts\n");
        printf("[3] Search for contact\n");
        printf("[4] Delete a Contact\n");
        printf("[5] Edit a Contact\n");
        printf("[0] Exit");
        printf("\n=====================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        clearScreen();
        switch (choice) {
        case 1:
            addContact(&contacts);
            writeToFile(&contacts);
            break;
        case 2:
            displayContacts(&contacts);
            break;
        case 3:
            searchContact(&contacts);
            break;
        case 4:
            deleteContact(&contacts);
            writeToFile(&contacts);
            break;
        case 5:
            editContact(&contacts);
            writeToFile(&contacts);
            break;
        case 0:
            printf("\n\n\n\t\t\tThank you for using CMS.\n\n");
            exit(0);
            break;
        default:
            continue;
        }

        int opt;
        printf("\n\n..::Enter the Choice:\n[1] Main Menu\t\t[0] Exit\n");
        scanf("%d", &opt);

        if (opt == 0) {
            clearScreen();
            printf("\n\n\n\t\t\tThank you for using CMS.\n\n");
            exit(0);
        }
    }

    return 0;
}
