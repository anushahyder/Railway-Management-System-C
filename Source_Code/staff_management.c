#include "globals.h"

char currentStaffEmail[50];

int errorChk(char *str1, char *str2){ return strcmp(str1, str2) == 0 ? 1 : 0; } 

int staffLogin(void){
    char email[50], pass[21];
    printf("\n--- STAFF LOGIN ---\n");
    printf("Email: "); scanf("%49s", email);
    printf("Password: "); getHiddenPassword(pass, 20);
    
    if (strcmp(pass, "f4d19ead") != 0) { printf("Invalid Password!\n"); return 0; }
    if (strcmp(email, "anusha@gmail.com") == 0 || strcmp(email, "fatima1@gmail.com") == 0 || 
        strcmp(email, "fatima2@gmail.com") == 0 || strcmp(email, "sameer@gmail.com") == 0) {
        
        strcpy(currentStaffEmail, email);
        printf("\nLogin Successful!\n"); return 1;
    }
    printf("Invalid Email.\n"); return 0;
}

void staffLiveBoard(void) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    int currentTotalMins = (tm->tm_hour * 60) + tm->tm_min;

    printf("\n===============================================================\n");
    printf("||               STAFF LIVE TRAIN BOARD                       ||\n");
    printf("===============================================================\n");
    printf(" System Time: %02d:%02d\n", tm->tm_hour, tm->tm_min);
    printf("---------------------------------------------------------------\n");

    int times[] = {480, 630, 780, 930, 1080, 1215, 1320, 1425}; 
    char *names[] = {"Star Rail", "Rika Express", "The Blue Ray", "Magarink", "Ascend 600", "SAO Unit", "Blitz Express", "Zegabound 60"};
    char *stringTimes[] = {"08:00 AM", "10:30 AM", "01:00 PM", "03:30 PM", "06:00 PM", "08:15 PM", "10:00 PM", "11:45 PM"};

    int i;
    for(i = 0; i < 8; i++) {
        int diff = times[i] - currentTotalMins;
        printf("Train: %-15s | Time: %-10s | Status: ", names[i], stringTimes[i]);
        
        if (diff > 60) printf("[SCHEDULED] Departs in %d hrs %d mins\n", diff/60, diff%60);
        else if (diff > 0) printf("[BOARDING] Departs in %d mins\n", diff);
        else if (diff > -120) printf("[RUNNING] On track. ETA: %d mins\n", 120 + diff);
        else printf("[ARRIVED] Journey complete.\n");
    }
    printf("===============================================================\n");
}

void staffBookTicket(void) {
    printf("\n--- BOOK TICKET ---\n");
    printf("[Note: This ticket will be booked under: %s]\n", currentStaffEmail);
    
    booking(&p[passengerCount], currentStaffEmail, 1); 
}

void manageTickets(void) {
    int tId, i, found = 0;
    printf("\n--- MANAGE ACTIVE TICKETS ---\n");
    for(i=0; i<passengerCount; i++) {
        if(p[i].booked) {
            // BETTER SPACING HERE
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
                        printf("1. View All\n2. Update Email\n3. Delete Account\n4. Back\nChoice: ");
                        if (scanf("%d", &a) != 1) { clearBuffer(); continue; }
                        if(a==1) displayCustomers(p, passengerCount);
                        if(a==2) updateCustomer(p, passengerCount);
                        if(a==3) deleteCustomer(p, &passengerCount);
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
