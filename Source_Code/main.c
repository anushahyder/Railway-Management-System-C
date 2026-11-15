#include "globals.h"

struct passenger p[200];
int passengerCount = 0;
int nextCustomerID = 1001;
int nextTicketID = 5001;
struct staff *staff_floor = NULL;
int pcount = 0;

void lockoutTimer() {
    printf("\n[!] LOCKED OUT. Too many failed attempts.\n");
    printf("Please wait 30 seconds...\n\n");
    int i;
    for (i = 30; i > 0; i--) {
        printf("\rTime remaining: %02d seconds ", i);
        fflush(stdout);
        Sleep(1000); 
    }
    printf("\rTime remaining: 00 seconds\n\n");
    printf("[+] You may try logging in again.\n");
}

int main() {
    int choice, custChoice;
    int i, idx, action;
    char loginInput[50], pass[50];
    
    FILE *fp = fopen("customer_db.dat", "rb");
    if (fp != NULL) {
        fread(&passengerCount, sizeof(int), 1, fp);
        fread(&nextCustomerID, sizeof(int), 1, fp);
        fread(&nextTicketID, sizeof(int), 1, fp);
        fread(p, sizeof(struct passenger), passengerCount, fp);
        fclose(fp);
    }

    while (1) {
        printf("\n");
        printf("+-------------------------------------------------------------+\n");
        printf("|                                                             |\n");
        printf("|               RAILWAY MANAGEMENT SYSTEM                     |\n");
        printf("|                      MAIN TERMINAL                          |\n");
        printf("|                                                             |\n");
        printf("+-------------------------------------------------------------+\n");
        printf("[1] Customer Portal\n");
        printf("[2] Staff Portal\n");
        printf("[3] View All Trains\n");
        printf("[4] Save & Exit System\n");
        printf("---------------------------------------------------------------\n");
        printf("Select an option: ");
        
        if (scanf("%d", &choice) != 1) { clearBuffer(); continue; }

        if (choice == 1) {
            printf("\n+-------------------------------------------------------------+\n");
            printf("|                   CUSTOMER PORTAL                           |\n");
            printf("+-------------------------------------------------------------+\n");
            printf("[1] Register New Account\n");
            printf("[2] Login to Existing Account\n");
            printf("[3] Back to Main Menu\n");
            printf("Select an option: ");
            if (scanf("%d", &custChoice) != 1) { clearBuffer(); continue; }
            
            if (custChoice == 1) {
                registerAccount(&p[passengerCount], nextCustomerID);
                passengerCount++;
                nextCustomerID++;
            } 
            else if (custChoice == 2) {
                int attempts = 0;
                idx = -1;
                
                while (attempts < 3) {
                    printf("\n--- SECURE LOGIN ---\n");
                    printf("Enter '0' in Email to cancel and go back.\n");
                    printf("Email: "); scanf("%49s", loginInput);
                    
                    if (strcmp(loginInput, "0") == 0) {
                        printf("\n[+] Login cancelled.\n");
                        break; 
                    }
                    
                    printf("Password: "); getHiddenPassword(pass, 49);
                    
                    for (i = 0; i < passengerCount; i++) { 
                        if (strcmp(p[i].email, loginInput) == 0 && strcmp(p[i].password, pass) == 0) {
                            idx = i; break;
                        }
                    }
                    
                    if (idx != -1) {
                        printf("\n[+] Login Successful. Welcome back!\n");
                        break;
                    } else {
                        attempts++;
                        printf("\n[!] Invalid Email or Password. Attempt %d of 3.\n", attempts);
                    }
                }
                
                if (attempts == 3) {
                    lockoutTimer(); 
                }
                else if (idx != -1) {
                    while(1) {
                        printf("\n+-------------------------------------------------------------+\n");
                        printf("|                   CUSTOMER DASHBOARD                        |\n");
                        printf("+-------------------------------------------------------------+\n");
                        printf("[1] Book a New Ticket\n");
                        printf("[2] Check Live Train Status\n");
                        printf("[3] Request Ticket Cancellation\n");
                        printf("[4] Logout\n");
                        printf("Select an option: ");
                        
                        if (scanf("%d", &action) != 1) { clearBuffer(); continue; }
                        
                        if (action == 1) booking(&p[idx], p[idx].email, 0);
                        else if (action == 2) checkCustomerStatus(&p[idx]);
                        else if (action == 3) cancelRequest(&p[idx]);
                        else if (action == 4) break;
                    }
                }
            }
        } 
        else if (choice == 2) {
            staffMenu();
        } 
        else if (choice == 3) {
            printf("\n+-------------------------------------------------------------+\n");
            printf("|                 ALL TRAINS AND TIMINGS                      |\n");
            printf("+-------------------------------------------------------------+\n");
            printf("[101] Star Rail        [08:00 AM]    [102] Rika Express     [10:30 AM]\n");
            printf("[103] The Blue Ray     [01:00 PM]    [104] Magarink         [03:30 PM]\n");
            printf("[105] Ascend 600       [06:00 PM]    [106] SAO Unit         [08:15 PM]\n");
            printf("[107] Blitz Express    [10:00 PM]    [108] Zegabound 60     [11:45 PM]\n");
            printf("+-------------------------------------------------------------+\n");
        }
        else if (choice == 4) {
            fp = fopen("customer_db.dat", "wb");
            if (fp != NULL) {
                fwrite(&passengerCount, sizeof(int), 1, fp);
                fwrite(&nextCustomerID, sizeof(int), 1, fp);
                fwrite(&nextTicketID, sizeof(int), 1, fp);
                fwrite(p, sizeof(struct passenger), passengerCount, fp);
                fclose(fp); 
            }
            
            if (staff_floor != NULL) {
                free(staff_floor);
                staff_floor = NULL; 
            }
            
            printf("\n[+] Data saved securely. Goodbye!\n");
            return 0;
        }
    }
    return 0;
}
