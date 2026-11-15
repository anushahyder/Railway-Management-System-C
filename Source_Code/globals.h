#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <conio.h> 
#include <windows.h>

struct passenger {
    int ticketId;
    int customerId;
    char name[80];
    char email[50];
    char password[50];
    int age;
    char phone[20];
    char trainName[40];
    char trainTime[20];
    char departure[30];
    char destination[30];
    char bookingDate[20];
    char notification[200]; 
    int booked;
    int cancelled;
    float Fare;
    float serviceCost;
    float totalBill;
    float gst;
    float finalBill;
};

struct staff {
    char email[50];
    char regpass[21]; 
};

extern struct passenger p[200];
extern int passengerCount;
extern int nextCustomerID;
extern int nextTicketID;
extern struct staff *staff_floor;
extern int pcount;

// NO SEAT MATRIX HERE ANYMORE

void staffMenu(void);
int staffLogin(void);
int errorChk(char *str1, char *str2);
void displayCustomers(struct passenger *p, int count);
void deleteCustomer(struct passenger p[], int *count);
void updateCustomer(struct passenger p[], int count);
void booking(struct passenger *accountUser, char *bookedByEmail, int isStaffBooking);
void cancelRequest(struct passenger *accountUser);
void recursiveServices(struct passenger *p, int sFood, int sPet, int sLux, int sLug, int sWifi);
void billing(struct passenger *p);
void receipt(struct passenger *p);
int phonechk(char phone[]);
int emailchk(char email[]);
void clearBuffer(void);
void getHiddenPassword(char *password, int maxLength);
int compareIgnoreCase(char *str1, char *str2);
void registerAccount(struct passenger *p, int id);
void customerInfo(struct passenger *ticketHolder, int accountId);
void checkCustomerStatus(struct passenger *accountUser);
void staffLiveBoard(void);
void manageTickets(void);
void staffBookTicket(void);

#endif
