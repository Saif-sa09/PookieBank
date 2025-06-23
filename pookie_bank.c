#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 100
#define FILE_NAME "accounts.dat"

typedef struct {
    int accountNumber;
    char name[50];
    double balance;
} Account;

Account accounts[MAX_ACCOUNTS];
int accountCount = 0;

void loadAccounts() {
    FILE *file = fopen(FILE_NAME, "rb");
    if (file == NULL) {
        // No file yet
        return;
    }
    fread(&accountCount, sizeof(int), 1, file);
    fread(accounts, sizeof(Account), accountCount, file);
    fclose(file);
}

void saveAccounts() {
    FILE *file = fopen(FILE_NAME, "wb");
    if (file == NULL) {
        printf("Error saving accounts!\n");
        return;
    }
    fwrite(&accountCount, sizeof(int), 1, file);
    fwrite(accounts, sizeof(Account), accountCount, file);
    fclose(file);
}

int findAccount(int accountNumber) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            return i;
        }
    }
    return -1;
}

void createAccount() {
    if (accountCount >= MAX_ACCOUNTS) {
        printf("Maximum accounts reached!\n");
        return;
    }

    Account newAccount;
    printf("Enter new account number: ");
    scanf("%d", &newAccount.accountNumber);

    if (findAccount(newAccount.accountNumber) != -1) {
        printf("Account number already exists!\n");
        return;
    }

    printf("Enter account holder name: ");
    getchar(); // consume newline
    fgets(newAccount.name, sizeof(newAccount.name), stdin);
    newAccount.name[strcspn(newAccount.name, "\n")] = 0; // remove newline

    newAccount.balance = 0.0;

    accounts[accountCount++] = newAccount;
    saveAccounts();
    printf("Account created successfully!\n");
}

void deposit() {
    int accNum;
    double amount;
    printf("Enter account number to deposit to: ");
    scanf("%d", &accNum);
    int index = findAccount(accNum);
    if (index == -1) {
        printf("Account not found!\n");
        return;
    }
    printf("Enter amount to deposit: ");
    scanf("%lf", &amount);
    if (amount <= 0) {
        printf("Invalid amount!\n");
        return;
    }
    accounts[index].balance += amount;
    saveAccounts();
    printf("Deposit successful! New balance: %.2lf\n", accounts[index].balance);
}

void withdraw() {
    int accNum;
    double amount;
    printf("Enter account number to withdraw from: ");
    scanf("%d", &accNum);
    int index = findAccount(accNum);
    if (index == -1) {
        printf("Account not found!\n");
        return;
    }
    printf("Enter amount to withdraw: ");
    scanf("%lf", &amount);
    if (amount <= 0) {
        printf("Invalid amount!\n");
        return;
    }
    if (accounts[index].balance < amount) {
        printf("Insufficient funds!\n");
        return;
    }
    accounts[index].balance -= amount;
    saveAccounts();
    printf("Withdrawal successful! New balance: %.2lf\n", accounts[index].balance);
}

void checkBalance() {
    int accNum;
    printf("Enter account number to check balance: ");
    scanf("%d", &accNum);
    int index = findAccount(accNum);
    if (index == -1) {
        printf("Account not found!\n");
        return;
    }
    printf("Account Holder: %s\nBalance: %.2lf\n", accounts[index].name, accounts[index].balance);
}

void listAccounts() {
    printf("\n---- All Accounts ----\n");
    if (accountCount == 0) {
        printf("No accounts available.\n");
        return;
    }
    for (int i = 0; i < accountCount; i++) {
        printf("Account Number: %d | Name: %s | Balance: %.2lf\n", 
            accounts[i].accountNumber, accounts[i].name, accounts[i].balance);
    }
    printf("----------------------\n");
}

int main() {
    loadAccounts();
    int choice;

    while (1) {
        printf("\n=== Pookie Bank ===\n");
        printf("1. Create Account\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Check Balance\n");
        printf("5. List All Accounts\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createAccount(); break;
            case 2: deposit(); break;
            case 3: withdraw(); break;
            case 4: checkBalance(); break;
            case 5: listAccounts(); break;
            case 6: 
                printf("Thank you for using Pookie Bank!\n");
                exit(0);
            default: printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}
