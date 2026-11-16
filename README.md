# Railway Management System

## Project Description
A fully functional console-based Railway Management System built in C. It features secure staff and customer login systems, dynamic ticket booking, real-time train status tracking, and persistent data storage using file handling.

## Group Members
*   Anusha (Roll No: CT-26272) - Main Menu, Ticketing, Billing, File Handling
*   Syeda Fatima Kazmi (Roll No: CT-26262) - Staff Management, Error Handling
*   Fatima (Roll No: CT-26261) - Customer Management
*   Sameer (Roll No: CT-26289) - Train Management

## Features
*   **Authentication:** Secure login for Staff and Customers with hidden passwords.
*   **Ticketing:** Book, cancel, and view tickets with dynamic GST and add-on calculations.
*   **Train Status:** Live tracking system showing if trains are scheduled, running, or arrived.
*   **Staff Dashboard:** Staff can manage customer accounts, view all bookings, and authorize cancellations.
*   **File Handling:** All user and ticket data is permanently saved to `customer_db.dat`.

## Live Web Mockup
You can view the interactive frontend prototype of this project here: [Railway System Web Mockup](https://anushahyd.github.io/Railway-Management-System-C/)


## How to Compile and Run

(Alternatively, open the .dev project file in Dev-C++ and press F11 to compile and run.)
Since the project uses multiple files, it is compiled and run using GCC:
```bash
gcc Source_Code/main.c Source_Code/final_train01.c Source_Code/staff_management.c -o RailwaySystem
./RailwaySystem


