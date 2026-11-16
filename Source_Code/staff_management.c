#include "globals.h"

char currentStaffEmail[50];

int errorChk(char *str1, char *str2){ return strcmp(str1, str2) == 0 ? 1 : 0; } 

void lockoutTimerStaff() {
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

int staffLogin(void){
    char email[50], pass[21];
    int attempts = 0;
    
    while (attempts < 3) {
        printf("\n--- STAFF LOGIN ---\n");
        printf("Email: "); scanf("%49s", email);
        printf("Password: "); getHiddenPassword(pass, 20);
        
        int validEmail = (strcmp(email, "anusha@gmail.com") == 0 || 
                          strcmp(email, "fatima1@gmail.com") == 0 || 
                          strcmp(email, "fatima2@gmail.com") == 0 || 
                          strcmp(email, "sameer@gmail.com") == 0);
        
        if (strcmp(pass, "f4d19ead") == 0 && validEmail) {
            strcpy(currentStaffEmail, email);
            printf("\nLogin Successful!\n"); 
            return 1;
        } else {
            attempts++;
            printf("\n[!] Invalid Credentials. Attempt %d of 3.\n", attempts);
        }
    }
    
    lockoutTimerStaff();
    return 0;
}

// --- 2D ARRAY IN ACTION: TRAIN STATUS MATRIX ---
void staffLiveBoard(void) {
    // 2D Array: 8 trains, 3 statuses (Scheduled, Running, Arrived)
    // 1 means true, 0 means false
    int trainStatus[8][3] = {
        {1, 0, 0}, // Star Rail: Scheduled
        {1, 0, 0}, // Rika Express: Scheduled
        {0, 1, 0}, // The Blue Ray: Running
        {1, 0, 0}, // Magarink: Scheduled
        {1, 0, 0}, // Ascend 600: Scheduled
        {1, 0, 0}, // SAO Unit: Scheduled
        {1, 0, 0}, // Blitz Express: Scheduled
        {1, 0, 0}  // Zegabound 60: Scheduled
    };
    
    char *names[] = {"Star Rail", "Rika Express", "The Blue Ray", "Magarink", "Ascend 600", "SAO Unit", "Blitz Express", "Zegabound 60"};
    char *times[] = {"08:00 AM", "10:30 AM", "01:00 PM", "03:30 PM", "06:00 PM", "08:15 PM", "10:00 PM", "11:45 PM"};

    printf("\n===============================================================\n");
    printf("||               STAFF LIVE TRAIN BOARD                       ||\n");
    printf("===============================================================\n");
    printf(" Legend: [Sch] = Scheduled | [Run] = Running | [Arr] = Arrived\n");
    printf("---------------------------------------------------------------\n");
    
    int i;
    for(i = 0; i < 8; i++) {
        printf(" %-15s | %-10s | Status Matrix: [%d][%d][%d] ", names[i], times[i], 
               trainStatus[i][0], trainStatus[i][1], trainStatus[i][2]);
        
        if (trainStatus[i][0] == 1) printf("-> [SCHEDULED]\n");
        else if (trainStatus[i][1] == 1) printf("-> [RUNNING]\n");
        else if (trainStatus[i][2] == 1) printf("-> [ARRIVED]\n");
    }
    printf("===============================================================\n");
}
// ---------------------------------------------

void staffBookTicket(void) {
    printf("\n--- BOOK TICKET ---\n");
    printf("[Note: This ticket will be booked under: %s]\n", currentStaffEmail);
    
    booking(&p[passengerCount], currentStaffEmail, 1); 
}

void manageTickets(void) {
    int tId, i, j, found = 0;
    
    // Bubble Sort (only works with full count minus one)
    for (i = 0; i < passengerCount - 1; i++) {
        for (j = 0; j < passengerCount - i - 1; j++) {
            if (p[j].ticketId > p[j+1].ticketId) {
                struct passenger temp = p[j];
                p[j] = p[j+1];
                p[j+1] = temp;
            }
        }
    }

    printf("\n--- MANAGE ACTIVE TICKETS (SORTED BY ID) ---\n");
    for(i=0; i<passengerCount; i++) {
        if(p[i].booked) {
            printf("\n===============================================================\n");
            printf("Ticket #%d | Passenger: %s | Train: %s\n", p[i].ticketId, p[i].name, p[i].trainName);
            printf("Booked By (Email): %s\n", p[i].email);
            printf("Route: %s -> %s | Time: %s | Date: %s\n", p[i].departure, p[i].destination, p[i].trainTime, p[i].bookingDate);
            if (p[i].cancelled == 1) printf("[!!!] CANCELLATION REQUESTED [!!!]\n");
            printf("===============================================================\n\n");
            found = 1;
        }
    }
    if(!found) { printf("No active tickets found.\n"); return; }

    printf("\nEnter Ticket ID to DELETE (0 to go back): ");
    if(scanf("%d", &tId) != 1) { clearBuffer(); return; }
    if(tId == 0) return;

    for(i=0; i<passengerCount; i++) {
        if(p[i].ticketId == tId && p[i].booked) {
            p[i].booked = 0;
            sprintf(p[i].notification, "Your ticket #%d has been cancelled by staff. Please contact management at 0800-RAILWAY for refunds.", tId);
            printf("\n[+] Ticket %d has been deleted permanently.\n", tId);
            return;
        }
    }
    printf("Ticket ID not found.\n");
}

void staffMenu(void) { 
    int menu, dashChoice, i, a;
    if (staff_floor == NULL) staff_floor = (struct staff *)malloc(50 * sizeof(struct staff));
    
    while(1){
        printf("\n--- STAFF PORTAL ---\n1. Login\n2. Back\nChoice: ");
        if (scanf("%d",&menu) != 1) { clearBuffer(); continue; }
        
        if(menu == 1) {
            if (staffLogin()) {
                while(1) {
                    int pendingCancels = 0;
                    for(i=0; i<passengerCount; i++) {
                        if(p[i].booked && p[i].cancelled == 1) pendingCancels++;
                    }
                    if(pendingCancels > 0) {
                        printf("\n[!!!] NOTICE: You have %d pending cancellation request(s). Check 'Manage Tickets'. [!!!]\n", pendingCancels);
                    }

                    printf("\n=== STAFF DASHBOARD ===\n");
                    printf("Logged in as: %s\n", currentStaffEmail);
                    printf("1. Manage Customer Accounts\n"); 
                    printf("2. Manage Tickets (View/Delete)\n");
                    printf("3. Book Ticket for Customer\n");
                    printf("4. View Live Train Board\n"); 
                    printf("5. Logout\nChoice: ");
                    if (scanf("%d", &dashChoice) != 1) { clearBuffer(); continue; }
                    
                    if (dashChoice == 1) {
                        printf("\n--- CUSTOMER ACCOUNTS ---\n");
                        printf("1. View All\n2. Delete Account\n3. Back\nChoice: ");
                        if (scanf("%d", &a) != 1) { clearBuffer(); continue; }
                        if(a==1) displayCustomers(p, passengerCount);
                        if(a==2) deleteCustomer(p, &passengerCount);
                    } else if (dashChoice == 2) {
                        manageTickets();
                    } else if (dashChoice == 3) {
                        staffBookTicket();
                    } else if (dashChoice == 4) {
                        staffLiveBoard();
                    } else if (dashChoice == 5) {
                        break;
                    }
                }
            }
        }
        else if(menu == 2) return;
    }
}
