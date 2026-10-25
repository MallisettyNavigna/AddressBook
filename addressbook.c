#include "addressbook.h"

int search_flag = 0;

/*
 * Function: pull_file_data
 * Purpose : Read contact records from the CSV file into the AddressBook structure.
 * Input   : AddressBook pointer and file name.
 */
void pull_file_data(struct AddressBook *addressBook, char *file)
{
    FILE *fp;

    /* Open the CSV file in read mode. */
    fp = fopen(file, "r");

    if (fp == NULL)
    {
        return;
    }

    /* Start loading contacts from the first record. */
    addressBook->contactCount = 0;

    /* Read each CSV record until EOF or the address book reaches capacity. */
    while (addressBook->contactCount < 100 &&
           fscanf(fp, " %[^,],%[^,],%[^\n]",
                  addressBook->contacts[addressBook->contactCount].name,
                  addressBook->contacts[addressBook->contactCount].phone,
                  addressBook->contacts[addressBook->contactCount].mail) == 3)
    {
        addressBook->contactCount++;
    }

    fclose(fp);
}


/*
 * Function: push_data_file
 * Purpose : Write all contact records from memory back to the CSV file.
 * Input   : AddressBook pointer and file name.
 */
void push_data_file(struct AddressBook *addressBook, char *file)
{
    FILE *fp;
    int i;

    /* Open the CSV file in write mode to save the current records. */
    fp = fopen(file, "w");

    if (fp == NULL)
                                    printf("File cannot be opened\n");
        return;


    /* Write every stored contact as a comma-separated record. */
    for (i = 0; i <= addressBook->contactCount; i++)
    {
        fprintf(fp, "%s,%s,%s\n",
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].mail);
    }

    fclose(fp);
}


/*
 * Function: name_verify
 * Purpose : Validate that the name contains at least four alphabetic characters.
 * Return  : 1 if valid, 0 if invalid.
 */
int name_verify(char name[])
{
    int i;

    if (strlen(name) < 4)
    {
        printf("Name must contain at least 4 characters\n");
        return 0;
    }

    for (i = 0; name[i] != '\0'; i++)
    {
        if (!((name[i] >= 'A' && name[i] <= 'Z') ||
              (name[i] >= 'a' && name[i] <= 'z')))
        {
            printf("Name should contain only alphabets\n");
            return 0;
        }
    }

    return 1;
}

                               /* Validate phone */
int mobile_verify(char mobile[])
{
    int i;

    if (strlen(mobile) != 10)
    {
        printf("Phone number must contain exactly 10 digits\n");
        return 0;
    }

    if (mobile[0] < '6' || mobile[0] > '9')
    {
        printf("First digit must be between 6 and 9\n");
        return 0;
    }

    for (i = 0; i < 10; i++)
    {
        if (mobile[i] < '0' || mobile[i] > '9')
        {
            printf("Symbols are not allowed in phone number\n");
            return 0;
        }
    }

    return 1;
}


/*
 * Function: gmail_verify
 * Purpose : Validate the basic format of an email address.
 * Return  : 1 if valid, 0 if invalid.
 */
int gmail_verify(char gmail[])
{
    int i;
    int at_count = 0;
    int at_position = -1;
    int dot_position = -1;
    int last_dot = -1;

                         for (i = 0; gmail[i] != '\0'; i++)
    {
        if (gmail[i] == '@')
        {
            at_count++;
            at_position = i;
        }

        if (gmail[i] == '.')
        {
            dot_position = i;
            last_dot = i;
        }
    }

    if (at_count == 0)
    {
        printf("Email ID must contain @ symbol\n");
        return 0;
    }

    if (at_count > 1)
    {
        printf("Multiple @ symbols are not allowed\n");
        return 0;
    }

    if (dot_position == -1)
    {
        printf("Email ID must contain a dot\n");
        return 0;
    }

    if (dot_position < at_position)
    {
        printf("Dot must appear after @\n");
        return 0;
    }

                             if (dot_position == at_position + 1)
    {
        printf("There must be at least one character between @ and dot\n");
        return 0;
    }

    if (at_position == 0)
    {
        printf("Domain is missing\n");
        return 0;
    }

    if (last_dot == (int)strlen(gmail) - 1)
    {
        printf("Domain extension is missing\n");
        return 0;
    }

    for (i = 0; gmail[i] != '\0'; i++)
    {
        if (!((gmail[i] >= 'A' && gmail[i] <= 'Z') ||
              (gmail[i] >= 'a' && gmail[i] <= 'z') ||
              (gmail[i] >= '0' && gmail[i] <= '9') ||
              gmail[i] == '@' ||
              gmail[i] == '.'))
        {
            printf("Invalid symbol in email ID\n");
            return 0;
        }
    }

    for (i = last_dot + 1; gmail[i] != '\0'; i++)
    {
        if (!((gmail[i] >= 'A' && gmail[i] <= 'Z') ||
              (gmail[i] >= 'a' && gmail[i] <= 'z') ||
              (gmail[i] >= '0' && gmail[i] <= '9')))
        {
            printf("Symbols are not allowed after domain extension\n");
            return 0;
                                  }
    }

    return 1;
}


/*
 * Function: add_contact
 * Purpose : Accept a new contact, validate its details and store it in memory.
 */
void add_contact(struct AddressBook *addressBook)
{
    int index;

    if (addressBook->contactCount >= 100)
    {
        printf("Address book is full\n");
        return;
    }

    index = addressBook->contactCount;

    /* Repeatedly accept input until valid contact details are entered. */
    do
    {
        printf("Enter name : ");
        scanf(" %[^\n]", addressBook->contacts[index].name);

    } while (name_verify(addressBook->contacts[index].name) == 0);

    do
    {
        printf("Enter phone : ");
        scanf("%s", addressBook->contacts[index].phone);

    } while (mobile_verify(addressBook->contacts[index].phone) == 0);

    do
    {
        printf("Enter email : ");
        scanf("%s", addressBook->contacts[index].mail);

    } while (gmail_verify(addressBook->contacts[index].mail) == 0);
                                                            addressBook->contactCount++;

    printf("Contact added successfully\n");
}


/*
 * Function: search_contact
 * Purpose : Search for contacts using name, phone number or email address.
 *          Stores matching record indexes for later edit/delete operations.
 */
void search_contact(struct AddressBook *addressBook)
{
    int choice;
    char name[50];
    char phone[15];
    char gmail[50];

    search_flag = 0;
    addressBook->ir_size = 0;

    printf("\nSearch by:\n");
    printf("1. Name\n");
    printf("2. Phone\n");
    printf("3. Email\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");

    if (scanf("%d", &choice) != 1)
    {
        printf("Invalid choice\n");
        scanf("%*s");
        return;
    }

    /* Perform the search according to the selected field. */
    switch (choice)
    {
        case 1:

            while (1)
            {
                printf("Enter name to search : ");
                scanf(" %[^\n]", name);

                                          if (name_verify(name) == 0)
                    continue;

                search_flag = 0;
                addressBook->ir_size = 0;

                search_by_name(addressBook, name);

                if (search_flag == 1)
                    return;

                printf("Contact not found\n");
            }


        case 2:

            while (1)
            {
                printf("Enter phone to search : ");
                scanf("%s", phone);

                if (mobile_verify(phone) == 0)
                    continue;

                search_flag = 0;
                addressBook->ir_size = 0;

                search_by_phone(addressBook, phone);

                if (search_flag == 1)
                    return;

                printf("Contact not found\n");
            }


        case 3:

            while (1) {
                printf("Enter email to search : ");
                scanf("%s", gmail);

                if (gmail_verify(gmail) == 0)
                    continue;

                search_flag = 0;
                addressBook->ir_size = 0;

                search_by_gmail(addressBook, gmail);

                if (search_flag == 1)
                    return;

                printf("Contact not found\n");
            }


        case 4:

            search_flag = 0;
            addressBook->ir_size = 0;
            return;


        default:

            printf("Invalid choice\n");
            return;
    }
}


/* Search the address book for contacts matching the given name. */
void search_by_name(struct AddressBook *addressBook, char name[])
{
    int i;
    int serial_no = 0;

                                     for (i = 0; i < addressBook->contactCount; i++)
    {
        if (strcmp(addressBook->contacts[i].name, name) == 0)
        {
            addressBook->index_record[addressBook->ir_size] = i;
            addressBook->ir_size++;

            search_flag = 1;
            serial_no++;

            printf("\n%d. Name : %s\n",
                   serial_no,
                   addressBook->contacts[i].name);

            printf("   Phone : %s\n",
                   addressBook->contacts[i].phone);

            printf("   Email : %s\n",
                   addressBook->contacts[i].mail);
        }
    }
}


/* Search the address book for a contact matching the given phone number. */
void search_by_phone(struct AddressBook *addressBook, char phone[])
{
    int i;
    int serial_no = 0;

    for (i = 0; i < addressBook->contactCount; i++)
    {
        if (strcmp(addressBook->contacts[i].phone, phone) == 0)
        {
            addressBook->index_record[addressBook->ir_size] = i;
            addressBook->ir_size++;

            search_flag = 1;
            serial_no++;

                               printf("\n%d. Name : %s\n",
                   serial_no,
                   addressBook->contacts[i].name);

            printf("   Phone : %s\n",
                   addressBook->contacts[i].phone);

            printf("   Email : %s\n",
                   addressBook->contacts[i].mail);
        }
    }
}


/* Search the address book for a contact matching the given email address. */
void search_by_gmail(struct AddressBook *addressBook, char gmail[])
{
    int i;
    int serial_no = 0;

    for (i = 0; i < addressBook->contactCount; i++)
    {
        if (strcmp(addressBook->contacts[i].mail, gmail) == 0)
        {
            addressBook->index_record[addressBook->ir_size] = i;
            addressBook->ir_size++;

            search_flag = 1;
            serial_no++;

            printf("\n%d. Name : %s\n",
                   serial_no,
                   addressBook->contacts[i].name);

            printf("   Phone : %s\n",
                   addressBook->contacts[i].phone);

            printf("   Email : %s\n",
                   addressBook->contacts[i].mail);
        }
                                                  }
}


/*
 * Function: edit_contact
 * Purpose : Search for a contact and modify its name, phone number or email.
 */
void edit_contact(struct AddressBook *addressBook)
{
    int opt;
    int i;
    int choice;

    search_contact(addressBook);

    if (search_flag == 0)
        return;

    printf("\nEdit Menu\n");
    printf("1. Edit Name\n");
    printf("2. Edit Phone\n");
    printf("3. Edit Email\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");

    if (scanf("%d", &choice) != 1)
    {
        printf("Invalid choice\n");
        scanf("%*s");
        return;
    }

    if (choice == 4)
    {
        search_flag = 0;
        addressBook->ir_size = 0;
        return;
    }

    if (choice < 1 || choice > 3)
    {
        printf("Invalid choice\n");
                               return;
    }

    printf("Enter one s.no contact: ");
    scanf("%d", &opt);

    if (opt < 1 || opt > addressBook->ir_size)
    {
        printf("Invalid s_no\n");
        return;
    }

    /* Convert the displayed serial number into the actual array index. */
    i = addressBook->index_record[opt - 1];

    switch (choice)
    {
        case 1:

            do
            {
                printf("Enter new name : ");
                scanf(" %[^\n]", addressBook->contacts[i].name);

            } while (name_verify(addressBook->contacts[i].name) == 0);

            break;


        case 2:

            do
            {
                printf("Enter new phone : ");
                scanf("%s", addressBook->contacts[i].phone);

            } while (mobile_verify(addressBook->contacts[i].phone) == 0);

            break;

                                           case 3:

            do
            {
                printf("Enter new email : ");
                scanf("%s", addressBook->contacts[i].mail);

            } while (gmail_verify(addressBook->contacts[i].mail) == 0);

            break;
    }

    printf("Contact edited successfully\n");

    search_flag = 0;
    addressBook->ir_size = 0;
}


/*
 * Function: delete_contact
 * Purpose : Search for a contact, confirm deletion and remove it from the array.
 */
void delete_contact(struct AddressBook *addressBook)
{
    int opt;
    int i;
    int j;
    char ch;

    search_contact(addressBook);

    if (search_flag == 0)
        return;

    printf("\nDelete Menu\n");
    printf("1. Delete selected contact\n");
    printf("2. Exit\n");
    printf("Enter your choice: ");

    scanf("%d", &opt);

                                              if (opt == 2)
    {
        search_flag = 0;
        addressBook->ir_size = 0;
        return;
    }

    if (opt != 1)
    {
        printf("Invalid choice\n");
        return;
    }

    printf("Enter one s.no contact: ");
    scanf("%d", &opt);

    if (opt < 1 || opt > addressBook->ir_size)
    {
        printf("Invalid s_no\n");
        return;
    }

    /* Convert the displayed serial number into the actual array index. */
    i = addressBook->index_record[opt - 1];

    printf("Do you want to delete? (y/n): ");
    scanf(" %c", &ch);

    if (ch == 'y' || ch == 'Y')
    {
        /* Shift all records after the deleted contact one position left. */
        for (j = i; j < addressBook->contactCount - 1; j++)
        {
            addressBook->contacts[j] =
                addressBook->contacts[j + 1];
        }

        addressBook->contactCount--;

        search_flag = 0;
        addressBook->ir_size = 0; }
    else
    {
        printf("No Deletion\n");

        search_flag = 0;
        addressBook->ir_size = 0;
    }
}


/*
 * Function: list_contacts
 * Purpose : Display all contacts currently stored in the address book.
 */
void list_contacts(struct AddressBook *addressBook)
{
    int i;

    for (i = 0; i < addressBook->contactCount; i++)
    {
        printf("%d. Name: %s | Mobile: %s | Gmail: %s\n",
               i + 1,
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].mail);
    }
}
