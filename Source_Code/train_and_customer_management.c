#include "globals.h"

void clearBuffer() { int c; while ((c = getchar()) != '\n' && c != EOF); }

void getHiddenPassword(char *password, int maxLength) {
    int i = 0;
    char ch;
    while (1) {
        ch = _getch();
        
    // _getch instead of getch because it is a windows convention
    
        if (ch == 13) { 
            password[i] = '\0';
            printf("\n");
            break;
        } else if (ch == 8) { 
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else if (i < maxLength - 1 && ch >= 32 && ch <= 126) {
            password[i++] = ch;
            printf("*");
        }
    }
}

int phonechk(char phone[]) { int i; if (strlen(phone) != 11) return 0; for (i = 0; phone[i] != '\0'; i++) if (!isdigit((unsigned char)phone[i])) return 0; return 1; }
int emailchk(char email[]) { int i, at = -1, dot = -1; for (i = 0; email[i] != '\0'; i++) { if (email[i] == '@') { if (at != -1) return 0; at = i; } if (email[i] == '.') dot = i; } return (at > 0 && dot > at + 1 && email[dot + 1] != '\0'); }
int namechk(char name[]) { int i; for (i = 0; name[i] != '\0'; i++) { if (!isalpha(name[i]) && name[i] != ' ') return 0; } return 1; }

// to bypass the lower and upper case scenario so the program doesnt turn away the value just because its in caps lock
int compareIgnoreCase(char *str1, char *str2) { int i; for (i = 0; str1[i] && str2[i]; i++) { if (tolower(str1[i]) != tolower(str2[i])) return 0; } return str1[i] == '\0' && str2[i] == '\0'; }

void registerAccount(struct passenger *p, int id) {
    memset(p, 0, sizeof(struct passenger));
    p->customerId = id;
    char pass[50];
    
    printf("\n--- ACCOUNT REGISTRATION ---\n");
    do { printf("Enter Email: "); scanf("%49s", p->email); if(!emailchk(p->email)) printf("[!] Invalid Email Format!\n"); } while (!emailchk(p->email));
    do { printf("Create Password (min 4 chars): "); getHiddenPassword(pass, 49); if(strlen(pass) < 4) printf("[!] Password too short!\n"); } while (strlen(pass) < 4);
    strcpy(p->password, pass);
    
    printf("\n+-------------------------------------------------------------+\n");
    printf("| [+] ACCOUNT CREATED SUCCESSFULLY!                           |\n");
    printf("+-------------------------------------------------------------+\n");
}

void customerInfo(struct passenger *ticketHolder, int accountId) {
    memset(ticketHolder, 0, sizeof(struct passenger)); 
    ticketHolder->customerId = accountId; 
    ticketHolder->ticketId = nextTicketID++; 
    
    printf("\n--- PASSENGER DETAILS ---\n");
    do { printf("Passenger Name: "); scanf(" %79[^\n]", ticketHolder->name); if (!namechk(ticketHolder->name)) printf("[!] Invalid Name!\n"); } while (!namechk(ticketHolder->name));
    while(1) { printf("Age (1-100): "); if (scanf("%d",&ticketHolder->age) != 1) { clearBuffer(); } else if (ticketHolder->age < 1 || ticketHolder->age > 100) { printf("[!] Invalid Age!\n"); } else break; }
    do { printf("Phone (11 digits): "); scanf("%19s",ticketHolder->phone); } while (!phonechk(ticketHolder->phone));
}

void recursiveServices(struct passenger *p, int sFood, int sPet, int sLux, int sLug, int sWifi) {
    int choice;
    
    printf("\n+-------------------------------------------------------------+\n");
    printf("|                    EXTRA SERVICES                           |\n");
    printf("+-------------------------------------------------------------+\n");
    printf("\nCurrent Add-on Total: Rs. %.2f\n", p->serviceCost);
    printf("   [1] Food (Rs. 500)          %s\n", sFood ? "<- ADDED" : "");
    printf("   [2] Pet Transport (Rs. 200) %s\n", sPet ? "<- ADDED" : "");
    printf("   [3] Luxury Car (Rs. 1000)   %s\n", sLux ? "<- ADDED" : "");
    printf("   [4] Extra Luggage (Rs. 300) %s\n", sLug ? "<- ADDED" : "");
    printf("   [5] Onboard Wi-Fi (Rs. 150) %s\n", sWifi ? "<- ADDED" : "");
    printf("   [0] Done selecting\n");
    printf("Choose: ");
    
    if (scanf("%d",&choice) != 1) { clearBuffer(); recursiveServices(p, sFood, sPet, sLux, sLug, sWifi); return; }
    if (choice == 0) return; 
    
    switch(choice) {
        case 1: if(!sFood) { p->serviceCost += 500; sFood = 1; } break;
        case 2: if(!sPet) { p->serviceCost += 200; sPet = 1; } break;
        case 3: if(!sLux) { p->serviceCost += 1000; sLux = 1; } break;
        case 4: if(!sLug) { p->serviceCost += 300; sLug = 1; } break;
        case 5: if(!sWifi) { p->serviceCost += 150; sWifi = 1; } break;
    }
    
    recursiveServices(p, sFood, sPet, sLux, sLug, sWifi); 
}

void billing(struct passenger *p) {
    float discount = 0;
    p->totalBill = p->Fare + p->serviceCost;
    if(p->age >= 60) discount = p->totalBill * 0.50;
    p->discount = discount; // Save discount for receipt
    p->totalBill -= discount;
    p->gst = p->totalBill * 0.20;
    p->finalBill = p->totalBill + p->gst;
}

void receipt(struct passenger *p) {
    printf("\n");
    printf("===============================================================\n");
    printf("||                                                           ||\n");
    printf("||                 RAILWAY TICKET RECEIPT                    ||\n");
    printf("||                                                           ||\n");
    printf("===============================================================\n");
    printf(" TICKET ID      : #%d\n", p->ticketId);
    printf(" ACCOUNT EMAIL  : %s\n", p->email);
    printf("---------------------------------------------------------------\n");
    printf(" PASSENGER NAME : %s\n", p->name);
    printf(" SEAT NUMBER    : Standard\n"); 
    printf(" AGE            : %d\n", p->age);
    printf(" PHONE          : %s\n", p->phone);
    printf("---------------------------------------------------------------\n");
    printf(" TRAIN          : %s\n", p->trainName);
    printf(" DEPARTURE TIME : %s\n", p->trainTime);
    printf(" ROUTE          : %s -> %s\n", p->departure, p->destination);
    printf(" BOOKING DATE   : %s\n", p->bookingDate);
    printf("---------------------------------------------------------------\n");
    printf(" BASE FARE      : Rs. %.2f\n", p->Fare);
    printf(" ADD-ONS        : Rs. %.2f\n", p->serviceCost);
    
    // SHOW DISCOUNT IF APPLIED
    if (p->discount > 0) {
        printf(" DISCOUNT (50%%) : -Rs. %.2f\n", p->discount);
    }
    
    printf(" GST (20%%)      : Rs. %.2f\n", p->gst);
    printf("---------------------------------------------------------------\n");
    printf(" TOTAL          : Rs. %.2f\n", p->finalBill);
    printf("===============================================================\n");
    
    // SHOW IF BOOKED BY STAFF
    if (strstr(p->email, "sameer") || strstr(p->email, "anusha") || 
        strstr(p->email, "fatima1") || strstr(p->email, "fatima2")) {
        printf(" [i] NOTE: This ticket was booked by a Staff Member.\n");
        printf("===============================================================\n");
    }
}

void booking(struct passenger *accountUser, char *bookedByEmail, int isStaffBooking) {
    struct passenger ticketHolder;
    char source[30], destination[30];
    int trainNo, sIdx = 0, dIdx = 0;

    printf("\n+-------------------------------------------------------------+\n");
    printf("|                    NEW TICKET BOOKING                       |\n");
    printf("+-------------------------------------------------------------+\n");
    
    customerInfo(&ticketHolder, accountUser->customerId);
    
    printf("\n[ AVAILABLE STATIONS ]\n");
    printf("[1] Lahore   [2] Karachi   [3] Islamabad   [4] Peshawar   [5] Quetta\n");
    
    while (1) {
        printf("Select Source Station (1-5): "); scanf(" %29[^\n]", source);
        if (compareIgnoreCase(source, "Lahore") || strcmp(source, "1") == 0) { strcpy(source, "Lahore"); sIdx = 1; break; }
        if (compareIgnoreCase(source, "Karachi") || strcmp(source, "2") == 0) { strcpy(source, "Karachi"); sIdx = 2; break; }
        if (compareIgnoreCase(source, "Islamabad") || strcmp(source, "3") == 0) { strcpy(source, "Islamabad"); sIdx = 3; break; }
        if (compareIgnoreCase(source, "Peshawar") || strcmp(source, "4") == 0) { strcpy(source, "Peshawar"); sIdx = 4; break; }
        if (compareIgnoreCase(source, "Quetta") || strcmp(source, "5") == 0) { strcpy(source, "Quetta"); sIdx = 5; break; }
        printf("[!] Invalid Station. Try again.\n");
    }

    printf("\n"); 
    
    while (1) {
        printf("Select Destination Station (1-5): "); scanf(" %29[^\n]", destination);
        if (compareIgnoreCase(destination, "Lahore") || strcmp(destination, "1") == 0) { strcpy(destination, "Lahore"); dIdx = 1; break; }
        if (compareIgnoreCase(destination, "Karachi") || strcmp(destination, "2") == 0) { strcpy(destination, "Karachi"); dIdx = 2; break; }
        if (compareIgnoreCase(destination, "Islamabad") || strcmp(destination, "3") == 0) { strcpy(destination, "Islamabad"); dIdx = 3; break; }
        if (compareIgnoreCase(destination, "Peshawar") || strcmp(destination, "4") == 0) { strcpy(destination, "Peshawar"); dIdx = 4; break; }
        if (compareIgnoreCase(destination, "Quetta") || strcmp(destination, "5") == 0) { strcpy(destination, "Quetta"); dIdx = 5; break; }
        printf("[!] Invalid Station. Try again.\n");
    }

    if (sIdx == dIdx) { printf("\n[!] Source and Destination cannot be same! Booking cancelled.\n"); return; }

    printf("\n+-------------------------------------------------------------+\n");
    printf("|                   AVAILABLE TRAINS                          |\n");
    printf("+-------------------------------------------------------------+\n");
    
    int isValid = 0;
    while(!isValid) {
        if ((sIdx == 1 || sIdx == 2) && (dIdx == 1 || dIdx == 2)) {
            printf("   [101] Star Rail (08:00 AM)    [102] Rika Express (10:30 AM)\n");
            printf("Enter Train Number: "); 
            if(scanf("%d", &trainNo) != 1) { clearBuffer(); printf("[!] Invalid input.\n"); continue; }
            if(trainNo == 101) { strcpy(ticketHolder.trainName, "Star Rail"); strcpy(ticketHolder.trainTime, "08:00 AM"); isValid = 1; }
            if(trainNo == 102) { strcpy(ticketHolder.trainName, "Rika Express"); strcpy(ticketHolder.trainTime, "10:30 AM"); isValid = 1; }
        } else if (sIdx == 5 || dIdx == 5) {
            printf("   [107] Blitz Express (10:00 PM)   [108] Zegabound 60 (11:45 PM)\n");
            printf("Enter Train Number: "); 
            if(scanf("%d", &trainNo) != 1) { clearBuffer(); printf("[!] Invalid input.\n"); continue; }
            if(trainNo == 107) { strcpy(ticketHolder.trainName, "Blitz Express"); strcpy(ticketHolder.trainTime, "10:00 PM"); isValid = 1; }
            if(trainNo == 108) { strcpy(ticketHolder.trainName, "Zegabound 60"); strcpy(ticketHolder.trainTime, "11:45 PM"); isValid = 1; }
        } else {
            printf("   [103] The Blue Ray (01:00 PM)    [105] Ascend 600 (06:00 PM)\n");
            printf("Enter Train Number: "); 
            if(scanf("%d", &trainNo) != 1) { clearBuffer(); printf("[!] Invalid input.\n"); continue; }
            if(trainNo == 103) { strcpy(ticketHolder.trainName, "The Blue Ray"); strcpy(ticketHolder.trainTime, "01:00 PM"); isValid = 1; }
            if(trainNo == 105) { strcpy(ticketHolder.trainName, "Ascend 600"); strcpy(ticketHolder.trainTime, "06:00 PM"); isValid = 1; }
        }
        if(!isValid) printf("[!] Invalid Train for this route. Pick from the list.\n");
    }

    ticketHolder.Fare = 1500; 
    strcpy(ticketHolder.departure, source);
    strcpy(ticketHolder.destination, destination);
    strcpy(ticketHolder.email, bookedByEmail);
    ticketHolder.booked = 1;
    ticketHolder.cancelled = 0;
    
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    sprintf(ticketHolder.bookingDate, "%02d/%02d/%04d", tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);
    
    recursiveServices(&ticketHolder, 0, 0, 0, 0, 0); 

    billing(&ticketHolder);
    receipt(&ticketHolder); 

    p[passengerCount] = ticketHolder;
    passengerCount++;
}

void cancelRequest(struct passenger *accountUser) {
    int tId, i, found = 0;
    printf("\n--- CANCEL TICKET REQUEST ---\n");
    for(i=0; i<passengerCount; i++) {
        if(strcmp(p[i].email, accountUser->email) == 0 && p[i].booked && p[i].cancelled == 0) {
            printf("Ticket ID: %d | Train: %s | Name: %s\n", p[i].ticketId, p[i].trainName, p[i].name);
            found = 1;
        }
    }
    if(!found) { printf("You have no active tickets to cancel.\n"); return; }
    
    printf("\nEnter Ticket ID to cancel: ");
    if(scanf("%d", &tId) != 1) { clearBuffer(); return; }
    for(i=0; i<passengerCount; i++) {
        if(p[i].ticketId == tId && strcmp(p[i].email, accountUser->email) == 0) {
            p[i].cancelled = 1; 
            sprintf(p[i].notification, "Ticket #%d cancellation is pending. Staff will process it shortly.", tId);
            printf("\n[!] Request submitted. Staff has been notified and will process your refund.\n");
            return;
        }
    }
    printf("Ticket ID not found.\n");
}

void displayCustomers(struct passenger *p, int count) {
    int i, found = 0;
    printf("\n+-------------------------------------------------------------+\n");
    printf("|                 CUSTOMER ACCOUNTS                           |\n");
    printf("+-------------------------------------------------------------+\n");
    for (i = 0; i < count; i++) {
        if(p[i].customerId != 0 && strlen(p[i].email) > 0 && p[i].ticketId == 0) { 
            printf("Account ID: %d | Email: %s\n", p[i].customerId, p[i].email);
            found = 1;
        }
    }
    if (!found) printf("No accounts found.\n");
}

void updateCustomer(struct passenger p[], int count) {
    int id, i, j;
    char newEmail[50];
    printf("Enter Account ID to update: "); 
    if(scanf("%d", &id) != 1) { clearBuffer(); return; }
    
    for(i=0; i<count; i++) {
        if(p[i].customerId == id && p[i].ticketId == 0) {
            printf("Enter New Email: "); 
            scanf("%49s", newEmail);
            for(j=0; j<count; j++) {
                if(p[j].customerId == id) {
                    strcpy(p[j].email, newEmail);
                }
            }
            printf("\n[+] Email updated: the customer will be notified.\n"); 
            return;
        }
    }
    printf("Account not found.\n");
}

void deleteCustomer(struct passenger p[], int *count) {
    int id, i;
    printf("Enter Account ID to delete: "); 
    if(scanf("%d", &id) != 1) { clearBuffer(); return; }
    
    int found = 0;
    for(i=0; i<*count; i++) {
        if(p[i].customerId == id && p[i].ticketId == 0) {
            p[i].customerId = 0;
            found = 1;
        }
    }
    
    if(found) {
        printf("\n[+] Account Deleted! The customer will be notified.\n"); 
    } else {
        printf("Account not found.\n");
    }
}

void checkCustomerStatus(struct passenger *accountUser) {
    int i, found = 0, depHour = 0, depMin = 0, trainTotalMins, diffMins;
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    int currentHour = tm->tm_hour;
    int currentMin = tm->tm_min;
    int currentTotalMins = (currentHour * 60) + currentMin;
    
    char todayDate[20];
    sprintf(todayDate, "%02d/%02d/%04d", tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);

    printf("\n+-------------------------------------------------------------+\n");
    printf("|               LIVE TRAIN STATUS BOARD                       |\n");
    printf("+-------------------------------------------------------------+\n");
    printf(" System Time: %02d:%02d\n", currentHour, currentMin);
    printf("---------------------------------------------------------------\n");

    for(i = 0; i < passengerCount; i++) {
        if(strcmp(p[i].email, accountUser->email) == 0 && p[i].booked) {
            found = 1;
            depHour = 0; depMin = 0;
            if (strstr(p[i].trainTime, "08:00 AM")) { depHour = 8; depMin = 0; }
            else if (strstr(p[i].trainTime, "10:30 AM")) { depHour = 10; depMin = 30; }
            else if (strstr(p[i].trainTime, "01:00 PM")) { depHour = 13; depMin = 0; }
            else if (strstr(p[i].trainTime, "03:30 PM")) { depHour = 15; depMin = 30; }
            else if (strstr(p[i].trainTime, "06:00 PM")) { depHour = 18; depMin = 0; }
            else if (strstr(p[i].trainTime, "08:15 PM")) { depHour = 20; depMin = 15; }
            else if (strstr(p[i].trainTime, "10:00 PM")) { depHour = 22; depMin = 0; }
            else if (strstr(p[i].trainTime, "11:45 PM")) { depHour = 23; depMin = 45; }

            trainTotalMins = (depHour * 60) + depMin;
            diffMins = trainTotalMins - currentTotalMins;

            printf("\nTicket #%d | Train: %s | Route: %s -> %s\n", p[i].ticketId, p[i].trainName, p[i].departure, p[i].destination);
            printf("Booking Date: %s\n", p[i].bookingDate);
            
            if (p[i].cancelled == 1) {
                printf("Status: [CANCELLATION PENDING] Waiting for staff approval.\n");
            }
            else if (diffMins > 120) {
                printf("Status: [WAITING] Train departs in %d hours and %d minutes.\n", diffMins / 60, diffMins % 60);
            } else if (diffMins > 0) {
                printf("Status: [BOARDING SOON] Train departs in %d minutes!\n", diffMins);
            } else if (diffMins > -60) {
                printf("Status: [ON THE WAY] Train is currently traveling.\n");
                printf("ETA: Reaching destination in approximately %d minutes.\n", 60 + diffMins);
            } else {
                if (strcmp(p[i].bookingDate, todayDate) == 0) {
                    printf("Status: [JOURNEY COMPLETE] Arrived at destination TODAY.\n");
                } else {
                    printf("Status: [JOURNEY COMPLETE] Arrived on %s.\n", p[i].bookingDate);
                }
            }
            printf("---------------------------------------------------------------\n");
        }
    }
    if(!found) printf(" You have no upcoming rides or active tickets.\n");
    printf("+-------------------------------------------------------------+\n");
}
