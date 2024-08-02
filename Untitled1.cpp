#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h> // Include windows.h for graphical elements

struct address {
    char city[40];
    char tole[40];
    char house_number[10];
};

struct signup {
    char name[40];
    char username[40];
    long int contact_number;
    struct address addr;
    int pin;
    int conform_pin;
};

struct login {
    char username[40];
    int pin;
};

struct account {
    char username[40];
    int pin;
    int totdepo;
    int totwidra;
    int total;
    float reward_points; // New field for reward points
};

void deposit(struct account* acc, int amt);
void withdrawl(struct account* acc, int wid);
void checkbalance(struct account* acc);
void savedata(struct account* acc);
void displaydata(struct account* acc);
void sendMoney(struct account* sender, char* receiver_name, int amt);
void setColor(int color);
void showWelcomeMessage();
void showGoodbyeMessage();
void checkRewardPoints(struct account* acc); // New function to check reward points

int getPin()
{
    char pin[5] = {0};
    int i = 0;
    char ch;


    while (i < 4)
    {
        ch = _getch();

        if (ch >= '0' && ch <= '9') {
            pin[i++] = ch;
            printf("*");
        } else if (ch == '\b' && i > 0) {
            i--;
            printf("\b \b");
        } else if (ch == '\r' && i == 4) {
            break;
        }
    }

    pin[4] = '\0';
    return atoi(pin);
}

int isUsernameTaken(char *username, FILE *p)
{
    struct signup temp;
    rewind(p);
    while (fread(&temp, sizeof(struct signup), 1, p) == 1) {
        if (strcmp(temp.username, username) == 0) {
            return 1;
        }
    }
    return 0;
}

int loadAccountData(struct account* acc)
{
    FILE* file;
    char filename[50];
    sprintf(filename, "D:\\file\\%s.txt", acc->username);

    file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }

    fread(acc, sizeof(struct account), 1, file);
    fclose(file);
    return 1;
}

int main()
{
    int choice = 0, login_successful = 0, found = 0, pin_match = 0, choice2 = 0, balance_pin = 0;
    struct signup sig;
    struct login log;
    struct account acc;
    struct account sender;
    struct account receiver_name;
    FILE *p;

    p = fopen("D:\\file\\abc.txt", "a+");
    if (p == NULL) {
        printf("File not found or could not be opened.\n");
        exit(0);
    }

    showWelcomeMessage();

    while (1) {
        printf("\nMENU\n");
        printf(" 1. SIGN-UP\n");
        printf(" 2. LOG-IN\n");
        printf(" 3. FORGOT PIN\n");
        printf(" 4. EXIT \n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        fflush(stdin);

        switch (choice)
        {
            case 1: {
            system("cls");
            setColor(3);
                printf("\n--- SIGN-UP ---\n\n");
                setColor(7);

                do {
                    printf("Enter a username: ");
                    gets(sig.username);
                    fflush(stdin);

                    if (isUsernameTaken(sig.username, p))
                    {
                        setColor(4); // Red for error
                        printf("Username is already taken. Please choose a different username.\n");
                        setColor(7); // Reset to default
                    }
                } while (isUsernameTaken(sig.username, p));

                printf("Enter your name: ");
                gets(sig.name);
                fflush(stdin);

                do {
                    printf("Enter contact number (10 digits): ");
                    scanf("%ld", &sig.contact_number);
                    fflush(stdin);

                    if (sig.contact_number < 1000000000 || sig.contact_number > 9999999999)
                    {
                        setColor(4); // Red for error
                        printf("Invalid contact number. Please enter exactly 10 digits.\n");
                        setColor(7); // Reset to default
                    }
                } while (sig.contact_number < 1000000000 || sig.contact_number > 9999999999);

                printf("Enter city: ");
                gets(sig.addr.city);
                fflush(stdin);

                printf("Enter tole: ");
                gets(sig.addr.tole);
                fflush(stdin);

                printf("Enter house number: ");
                gets(sig.addr.house_number);
                fflush(stdin);

                do {
                    printf("Enter pin number (4 digits): ");
                    sig.pin = getPin();
                    printf("\n");
                    fflush(stdin);

                    printf("Confirm pin number (4 digits): ");
                    sig.conform_pin = getPin();
                    printf("\n");
                    fflush(stdin);

                    if (sig.pin != sig.conform_pin)
                    {
                        setColor(4); // Red for error
                        printf("Pin numbers do not match. Please try again.\n");
                        setColor(7); // Reset to default
                        pin_match = 1;
                    } else {
                        pin_match = 2;
                    }
                } while (pin_match != 2);

                fwrite(&sig, sizeof(struct signup), 1, p);
                system("cls");
                setColor(2); // Green for success
                printf("Signup information saved successfully.\n");
                setColor(7); // Reset to default
                fclose(p);
           
                break;
            }

            case 2:
            {
               system("cls");
                p = fopen("D:\\file\\abc.txt", "r");
                setColor(3);
                printf("\n--- LOG-IN ---\n");
                setColor(7);
                printf("\nEnter your username: ");
                gets(log.username);
                fflush(stdin);

                printf("Enter your pin number: ");
                log.pin = getPin();
                printf("\n");
                fflush(stdin);

                rewind(p);

                while (fread(&sig, sizeof(struct signup), 1, p) == 1) {
                    if (strcmp(sig.username, log.username) == 0 && sig.pin == log.pin) {
                        setColor(2); // Green for success
                        printf("Login successful. Hi %s!\n", sig.name);
                        setColor(7); // Reset to default
                        login_successful = 1;
                        strcpy(acc.username, sig.username);
                        acc.pin = sig.pin;

                        if (!loadAccountData(&acc)) 
						{
                            acc.totdepo = 0;
                            acc.totwidra = 0;
                            acc.total = 0;
                            acc.reward_points = 0; // Initialize reward points
                            savedata(&acc);
                        }

                        break;
                    }
                }

                if (!login_successful)
                {
                    setColor(4); // Red for error
                    printf("Invalid username or pin number. Please try again.\n");
                    setColor(7); // Reset to default
                }

                //system("cls");
                while (login_successful)
                {
                main:
                setColor(3);
                    printf("\n------BANKING MENU----------\n");
                    setColor(7);
                    printf(" 1. Deposit your money\n");
                    printf(" 2. Withdraw your money\n");
                    printf(" 3. Check balance\n");
                    printf(" 4. Transfer Balance\n");
                    printf(" 5. Check Reward Points\n");
                    printf(" 6. LOG-OUT\n");
                    printf("Enter your choice: ");
                    scanf("%d", &choice2);
                    fflush(stdin);

                    switch (choice2)
                    {
                        case 1:
                        {
                        system("cls");
                            printf("\nENTER YOUR DEPOSIT AMOUNT: ");
                            scanf("%d", &acc.totdepo);
                            fflush(stdin);
                            int deposit_pin;
                            do {
                            wow:
                                printf("\nENTER YOUR PIN: ");
                                deposit_pin = getPin();
                                if (deposit_pin == log.pin)
                                {
                                    deposit(&acc, acc.totdepo);
                                    break;
                                }
                                else
                                {
                                    setColor(4); // Red for error
                                    printf("\n Invalid PIN. ");
                                    setColor(7); // Reset to default
                                     
                                        char p;
                                        printf("\n Do you want to RE ENTER PASSWORD? \n choose 'y' for YES and 'n' for NO  (y/n): ");
                                        scanf(" %c", &p);

                                        if (p == 'y' || p == 'Y')
{
                                        goto  wow;
                                        }
                                        else//(x == 'n' || x == 'N')
{
                                        goto main;
                                        }

                                   
                                }
                            } while (1);

                            break;
                        }

                        case 2:
                        {
                            system("cls");
                            printf("\nENTER YOUR WITHDRAWAL AMOUNT: ");
                            scanf("%d", &acc.totwidra);
                            fflush(stdin);
                            int withdraw_pin;
                            do {
                            hi:
                                printf("\nENTER YOUR PIN: ");
                                withdraw_pin = getPin();
                                if (withdraw_pin == log.pin)
                                {
                                    withdrawl(&acc, acc.totwidra);
                                    break;
                                } else
                                {
                                    setColor(4); // Red for error
                                    printf("\n Invalid PIN. ");
                                    setColor(7); // Reset to default
                                     
                                         char o;
                                        printf(" \n Do you want to RE ENTER PASSWORD? \n choose 'y' for YES and 'n' for NO  (y/n): ");
                                        scanf(" %c", &o);

                                        if (o == 'y' || o == 'Y')
{
                                        goto  hi;
                                        }
                                        else//(x == 'n' || x == 'N')
                                        
{
                                        goto main;
                                        }

                                   
                                }
                            } while (1);

                            break;
                        }

                        case 3:
                        {
                            system("cls");
                           
                            do {
                            down:
                                printf("\n TO CHECK BALANCE \n ENTER YOUR PIN: ");
                                balance_pin = getPin();
                                if (balance_pin == log.pin)
                                {
                                    checkbalance(&acc);
                                    break;
                                } else
                                {
                                    setColor(4); // Red for error
                                    printf("\n Invalid PIN. \n ");
                                    setColor(7); // Reset to default
                                     
                                    char n;
                                        printf("\n Do you want to RE ENTER PASSWORD? \n choose 'y' for YES and 'n' for NO  (y/n): ");
                                        scanf(" %c", &n);

                                        if (n == 'y' || n == 'Y')
{
                                        goto  down;
                                        }
                                        else//(x == 'n' || x == 'N')
{
                                        goto main;
                                        }

                                }
                            } while (1);

                            break;
                        }

                        case 4:
                        {
                            system("cls");
                            printf("\n--- TRANSFER FUND ---\n");
                            char receiver_name[40];
                            int transfer_amount;
                            printf("Enter the receiver's username: \n");
                            gets(receiver_name);
                            fflush(stdin);
                            printf("Enter the amount to transfer: \n");
                            scanf("%d", &transfer_amount);
                            fflush(stdin);
                            int transfer_pin;
                            up:
                            do {
                                printf("\nENTER YOUR PIN: ");
                                transfer_pin = getPin();
                                if (transfer_pin == log.pin)
                                {
                                    sendMoney(&acc, receiver_name, transfer_amount);
                                    break;
                                } else
                                {
                                    setColor(4); // Red for error
                                    printf("\n Invalid PIN.\n ");
                                    setColor(7); // Reset to default
                                     
                                      char x;
                                        printf("\n DO YOU WANT TO RE ENTER PASSWORD? \n choose 'y' for YES and 'n' for NO  (y/n): ");
                                        scanf(" %c", &x);

                                        if (x == 'y' || x == 'Y')
                                        {
                                        goto  up;
                                        }
                                        else//(x == 'n' || x == 'N')
                                        {
                                        goto main;
                                        }
                                     
                                }
                            } while (1);

                            break;
                        }

                        case 5:
                        {
                           // system("cls");
                            checkRewardPoints(&acc); // Call the function to check reward points
                            break;
                        }

                        case 6:
                        {
                            savedata(&acc);
                            login_successful = 0;
                            showGoodbyeMessage();
                            system("cls");
                            break;
                        }
                    }
                }
                break;
            }

            case 3:
            {
                 system("cls");
                 setColor(3);
                printf("\n--- FORGOT PIN ---\n");
                setColor(7);
                p = fopen("D:\\file\\abc.txt", "r+");
                struct signup temp;
                int found = 0;
                printf("Enter your username: ");
                gets(temp.username);
                fflush(stdin);

                while (fread(&sig, sizeof(struct signup), 1, p) == 1) {
                    if (strcmp(sig.username, temp.username) == 0) {
                        found = 1;
                        break;
                    }
                }

                if (found) {
                    printf("Enter your registered contact number (10 digits): ");
                    scanf("%ld", &temp.contact_number);
                    fflush(stdin);
                    
                    if (temp.contact_number == sig.contact_number) {
                        do {
                        	printf("The OTP pin has ben send in your phone \n");
                            printf("Enter new pin number (4 digits): ");
                            temp.pin = getPin();
                            printf("\n");
                            fflush(stdin);

                            printf("Confirm new pin number (4 digits): ");
                            temp.conform_pin = getPin();
                            printf("\n");
                            fflush(stdin);

                            if (temp.pin != temp.conform_pin) {
                                setColor(4); // Red for error
                                printf("Pin numbers do not match. Please try again.\n");
                                setColor(7); // Reset to default
                                pin_match = 1;
                            } else {
                                pin_match = 2;
                            }
                        } while (pin_match != 2);

                        sig.pin = temp.pin;
                        sig.conform_pin = temp.conform_pin;

                        fseek(p, -sizeof(struct signup), SEEK_CUR);
                        fwrite(&sig, sizeof(struct signup), 1, p);
                        fflush(p);
                        system("cls");
                        setColor(2); // Green for success
                        printf("\n \t\t\t Pin updated successfully.\n");
                        setColor(7); // Reset to default
                    } else {
                    system("cls");
                        setColor(4); // Red for error
                        printf("Incorrect contact number. Please try again.\n");
                        setColor(7); // Reset to default
                    }
                } else {
                    setColor(4); // Red for error
                    printf("Username not found.\n");
                    setColor(7); // Reset to default
                }

                fclose(p);
                break;
            }

            case 4:
            {
                showGoodbyeMessage();
                exit(0);
            }
        }
    }

    fclose(p);
    return 0;
}

void deposit(struct account* acc, int amt)
{
    acc->total += amt;
    acc->reward_points = acc->total / 1000000; // Increase reward points based on the deposit amount
    savedata(acc);
    system("cls");
    setColor(2); // Green for success
    printf("\n Amount deposited successfully \n", acc->total);
    setColor(7); // Reset to default
    //system("cls");
}


void withdrawl(struct account* acc, int wid)
{
    if (acc->total >= wid) {
        acc->total -= wid;
        savedata(acc);
        system("cls");
        setColor(2); // Green for success
        printf("\n Amount withdrawn successfully.\n", acc->total);
        setColor(7); // Reset to default
      //  system("cls");
    } else {
    system("cls");
        setColor(4); // Red for error
        printf("Insufficient balance.\n");
        setColor(7); // Reset to default
    }
}

void checkbalance(struct account* acc)
{
     system("cls");
     setColor(2);
    printf("\n Current Balance: %d\n", acc->total); // Display current balance
    setColor(7);
   

}

void savedata(struct account* acc)
{
    FILE* file;
    char filename[50];
    sprintf(filename, "D:\\file\\%s.txt", acc->username);

    file = fopen(filename, "w");
    if (file == NULL) {
        setColor(4); // Red for error
        printf("Error saving data.\n");
        setColor(7); // Reset to default
        return;
    }

    fwrite(acc, sizeof(struct account), 1, file);
    fclose(file);
}

void sendMoney(struct account* sender, char* receiver_name, int amt)
{
    struct account receiver;
    strcpy(receiver.username, receiver_name);

    if (!loadAccountData(&receiver)) {
        setColor(4); // Red for error
        printf("\n Receiver's account not found.\n");
        setColor(7); // Reset to default
        return;
    }

    if (sender->total < amt) {
        setColor(4); // Red for error
        printf("\n Insufficient balance.\n");
        setColor(7); // Reset to default
        return;
    }

    sender->total -= amt;
    receiver.total += amt;

    savedata(sender);
    savedata(&receiver);
    system("cls");
    setColor(2); // Green for success
    printf("Amount transferred successfully.\n\n");
    setColor(7); // Reset to default
   
}

void showWelcomeMessage()
{
    setColor(3); // Cyan for welcome message
    printf("\t\t\t\t\t\t\t\tWELCOME TO RBC ONLINE BANKING SYSTEM\n");
    setColor(7); // Reset to default
}

void showGoodbyeMessage()
{
system("cls");
    setColor(3); // Cyan for goodbye message
    printf("\n\n\n\t\t\t\t\tThank you for using our Online Banking System. Goodbye!\n");
    setColor(7); // Reset to default
}

void setColor(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void checkRewardPoints(struct account* acc)
{
system("cls");
    setColor(2); // Cyan for info
    printf("\n\n Your current reward points: %f\n", acc->reward_points);

    if (acc->reward_points >= 10000) {
        printf("Congratulations! You have won a Car \n");
    } else if (acc->reward_points >= 5000) {
        printf("Congratulations! You have won a Bike \n");
    } else if (acc->reward_points >= 500) {
    	
        printf("Congratulations! You have won a RS1000 voucher\n");
    } else {
        printf("Keep earning more points to win exciting prizes!\n");
    }

    setColor(7); // Reset to default
   
}
