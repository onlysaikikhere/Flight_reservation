#include <stdio.h>          //including header files
#include <stdlib.h>
#include <string.h>

// Structure to hold information about each flight
typedef struct FlightInfo {
    char number[10];  // Unique flight identifier (e.g., AI101)
    char origin[50];  // Origin city (e.g., Delhi)
    char destination[50]; // Destination city (e.g., Mumbai)
    int capacity;     // Maximum seats available on the flight
    int available;    // How many seats are still free
    int *seats;       // Seat availability array (1: booked, 0: free)
} FlightInfo;

// Structure to hold passenger information
typedef struct PassengerInfo {
    char name[50];      // Full name of the passenger
    int age;            // Age of the passenger
    char gender;        // Gender of the passenger ('M' or 'F')
    char phone[15];     // Contact number of the passenger
    char flightNumber[10]; // The flight number that the passenger is booked on
    int seatNumber;     // Assigned seat number for the passenger
} PassengerInfo;

// Main system structure to manage flight reservations and data
typedef struct ReservationSystem {
    FlightInfo *flightList;          // Dynamic array of flights
    int totalFlights;                // Current number of flights in the system
    int maxFlights;                  // Maximum capacity for flights
    PassengerInfo *passengerList;    // Dynamic array of passengers
    int totalPassengers;             // Current number of passengers in the system
    int maxPassengers;               // Maximum capacity for passengers
} ReservationSystem;

// Function to initialize the reservation system with predefined data
void setupReservationSystem(ReservationSystem *resSys) {
    resSys->maxFlights = 15;          // Initial capacity for flights
    resSys->maxPassengers = 10;       // Initial capacity for passengers
    resSys->totalFlights = 0;         // Start with no flights
    resSys->totalPassengers = 0;      // Start with no passengers

    // Allocate memory for flights and passengers
    resSys->flightList = (FlightInfo *)malloc(resSys->maxFlights * sizeof(FlightInfo));
    resSys->passengerList = (PassengerInfo *)malloc(resSys->maxPassengers * sizeof(PassengerInfo));

    // Sample predefined flights to populate the system so that user already has some available options
    FlightInfo sampleFlights[10] = {
        {"AI101", "Delhi", "Mumbai", 150, 150, NULL},
        {"AI102", "Mumbai", "Delhi", 150, 150, NULL},
        {"AI103", "Delhi", "Paris", 200, 200, NULL},
        {"AI104", "Bangalore", "Milan", 200, 200, NULL},
        {"AI105", "Delhi", "Wellington", 180, 180, NULL},
        {"AI106", "Kolkata", "Delhi", 180, 180, NULL},
        {"AI107", "Mumbai", "Incheon", 160, 160, NULL},
        {"AI108", "Chennai", "Sydney", 160, 160, NULL},
        {"AI109", "Delhi", "Hyderabad", 170, 170, NULL},
        {"AI110", "Hyderabad", "Delhi", 170, 170, NULL},
    };

    // Initialize flight data and allocate seats for each flight
    for (int i = 0; i < 10; i++) {
        resSys->flightList[resSys->totalFlights] = sampleFlights[i]; // Copy predefined flight
        resSys->flightList[resSys->totalFlights].seats = (int *)calloc(sampleFlights[i].capacity, sizeof(int)); // Allocate memory for seat tracking
        resSys->totalFlights++; // Increase flight count after adding the flight
    }
}

// Function to increase flight storage capacity if the current limit is reached
void increaseFlightCapacity(ReservationSystem *resSys) {
    resSys->maxFlights *= 2;  // Double the maximum flight capacity
    resSys->flightList = (FlightInfo *)realloc(resSys->flightList, resSys->maxFlights * sizeof(FlightInfo)); // Reallocate memory for flight list
}

// Function to increase passenger storage capacity if the current limit is reached
void increasePassengerCapacity(ReservationSystem *resSys) {
    resSys->maxPassengers *= 2; // Double the maximum passenger capacity
    resSys->passengerList = (PassengerInfo *)realloc(resSys->passengerList, resSys->maxPassengers * sizeof(PassengerInfo)); // Reallocate memory for passenger list
}

// Function to display the main menu options for the user
void showMenu() {
    printf("\nWelcome to the Flight Reservation System\n");
    printf("1. Add a New Flight\n");
    printf("2. Book a Ticket\n");
    printf("3. Cancel a Ticket\n");
    printf("4. View Flights\n");
    printf("5. View Passengers\n");
    printf("6. Exit\n");
    printf("Please select an option: ");
}

// Function to register a new flight in the reservation system
void registerFlight(ReservationSystem *resSys) {
    if (resSys->totalFlights == resSys->maxFlights) {  
        increaseFlightCapacity(resSys);  // Increase capacity if needed
    }

    FlightInfo *newFlight = &resSys->flightList[resSys->totalFlights]; // Point to the new flight entry

    // Collecting flight details from user input
    printf("Enter flight number: ");
    scanf("%s", newFlight->number);
    printf("Enter origin: ");
    scanf("%s", newFlight->origin);
    printf("Enter destination: ");
    scanf("%s", newFlight->destination);
    printf("Enter maximum seating capacity: ");
    scanf("%d", &newFlight->capacity);

    newFlight->available = newFlight->capacity; // All seats are available on a new flight
    newFlight->seats = (int *)calloc(newFlight->capacity, sizeof(int)); // Initialize seat array to track availability (0 - free)

    resSys->totalFlights++; // Increase the total flight count
    printf("Flight successfully added.\n");
}

// Function to display all available flights in the system
void listFlights(ReservationSystem *resSys) {
    printf("\nCurrent Available Flights:\n");
    printf("--------------------------------------------------\n");
    printf("Flight No.  Origin       Destination  Capacity  Available\n");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < resSys->totalFlights; i++) {
        // Display the details of each flight
        printf("%-10s %-12s %-12s %-8d %-9d\n",
               resSys->flightList[i].number,
               resSys->flightList[i].origin,
               resSys->flightList[i].destination,
               resSys->flightList[i].capacity,
               resSys->flightList[i].available);
    }
    printf("--------------------------------------------------\n");
}

// Function to reserve a ticket for a passenger
void reserveTicket(ReservationSystem *resSys) {
    char flightNum[10]; // To hold the flight number entered by the user
    printf("Enter the flight number: ");
    scanf("%s", flightNum);

    // Search for the flight using the provided flight number
    int flightIdx = -1; // Initialize index to an invalid value
    for (int i = 0; i < resSys->totalFlights; i++) {
        if (strcmp(resSys->flightList[i].number, flightNum) == 0) {
            flightIdx = i; // Flight found
            break;
        }
    }

    if (flightIdx == -1) {
        printf("Requested flight not found.\n"); // Flight does not exist
        return;
    }

    FlightInfo *flight = &resSys->flightList[flightIdx]; // Retrieve the flight details
    if (flight->available == 0) {
        printf("Sorry, there are no seats available for this flight.\n"); // No available seats
        return;
    }

    // Check if there's space for a new passenger
    if (resSys->totalPassengers == resSys->maxPassengers) {
        increasePassengerCapacity(resSys); // Increase capacity if needed
    }

    // Display available seats to the user
    printf("Available seats: ");
    for (int i = 0; i < flight->capacity; i++) {
        if (flight->seats[i] == 0) { // If the seat is free
            printf("%d ", i + 1); // Print the seat number (1-based index)
        }
    }
    printf("\n");

    int selectedSeat; // Variable to hold the user's choice of seat
    printf("Please choose a seat: ");
    scanf("%d", &selectedSeat);

    // Validate the selected seat
    if (selectedSeat < 1 || selectedSeat > flight->capacity || flight->seats[selectedSeat - 1] != 0) {
        printf("Invalid seat selection, please try again.\n"); // Invalid input
        return;
    }

    flight->seats[selectedSeat - 1] = 1; // Mark the selected seat as booked

    PassengerInfo *newPassenger = &resSys->passengerList[resSys->totalPassengers]; 
    // Collect passenger information from user input
    printf("Enter passenger name: ");
    scanf("%s", newPassenger->name);
    printf("Enter passenger age: ");
    scanf("%d", &newPassenger->age);
    printf("Enter gender (M/F): ");
    scanf(" %c", &newPassenger->gender);
    printf("Enter phone number: ");
    scanf("%s", newPassenger->phone);

    newPassenger->seatNumber = selectedSeat; // Assign the seat number to the passenger
    strcpy(newPassenger->flightNumber, flight->number); // Save the flight number for the passenger record
    flight->available--; // Decrease the count of available seats
    resSys->totalPassengers++; // Increase the total passenger count
    printf("Ticket successfully booked! Your Seat: %d\n", newPassenger->seatNumber);
}

// Function to cancel an existing ticket
void revokeTicket(ReservationSystem *resSys) {
    char passengerName[50], flightNum[10]; // Hold inputs for passenger name and flight number
    printf("Enter passenger name: ");
    scanf("%s", passengerName);
    printf("Enter flight number: ");
    scanf("%s", flightNum);

    // Search for the passenger in the records
    int passengerIdx = -1; // Initialize to an invalid value
    for (int i = 0; i < resSys->totalPassengers; i++) {
        if (strcmp(resSys->passengerList[i].name, passengerName) == 0 && strcmp(resSys->passengerList[i].flightNumber, flightNum) == 0) {
            passengerIdx = i; // Passenger found
            break;
        }
    }

    if (passengerIdx == -1) {
        printf("Passenger not found in the records.\n"); // Passenger does not exist
        return;
    }

    // Locate the appropriate flight using the flight number
    int flightIdx = -1; // Initialize index to an invalid value
    for (int i = 0; i < resSys->totalFlights; i++) {
        if (strcmp(resSys->flightList[i].number, flightNum) == 0) {
            flightIdx = i; // Flight found
            break;
        }
    }

    // Free the booked seat and update passenger information
    FlightInfo *flight = &resSys->flightList[flightIdx];
    flight->seats[resSys->passengerList[passengerIdx].seatNumber - 1] = 0; // Mark the seat as available again
    flight->available++; // Increment the count of available seats

    // Shift passenger details to remove the canceled passenger from the array
    for (int i = passengerIdx; i < resSys->totalPassengers - 1; i++) {
        resSys->passengerList[i] = resSys->passengerList[i + 1]; // Move next passenger forward in the array
    }
    resSys->totalPassengers--; // Decrease total passenger count

    printf("Ticket cancellation successful.\n");
}

// Function to display all passengers currently booked in the system
void listPassengers(ReservationSystem *resSys) {
    printf("\nList of Passengers:\n");
    printf("-----------------------------------------------------------\n");
    printf("Name            Age Gender Phone           Flight No.  Seat\n");
    printf("-----------------------------------------------------------\n");
    for (int i = 0; i < resSys->totalPassengers; i++) {
        // Print details of each passenger
        printf("%-15s %-3d %-6c %-15s %-10s %-4d\n",
               resSys->passengerList[i].name,
               resSys->passengerList[i].age,
               resSys->passengerList[i].gender,
               resSys->passengerList[i].phone,
               resSys->passengerList[i].flightNumber,
               resSys->passengerList[i].seatNumber);
    }
    printf("-----------------------------------------------------------\n");
}

// Function to save flight data to a binary file for persistence
void saveFlightsToFile(ReservationSystem *resSys) {
    FILE *file = fopen("flights.dat", "wb");  // Opening the file in write-binary mode
    if (file == NULL) {
        printf("Could not open file for writing.\n"); // Error handling for file opening
        return;
    }

    // Write the number of flights and flight data to the file
    fwrite(&resSys->totalFlights, sizeof(int), 1, file); // Save flight count
    fwrite(resSys->flightList, sizeof(FlightInfo), resSys->totalFlights, file); // Save flight details

    fclose(file);  // Close the file after writing
    printf("Flight data saved successfully.\n");
}

// Function to load flight data from a binary file into the system
void loadFlightsFromFile(ReservationSystem *resSys) {
    FILE *file = fopen("flights.dat", "rb");  // Open the file in read-binary mode
    if (file == NULL) {
        printf("No previous flight data available.\n"); // Error handling
        return;
    }

    fread(&resSys->totalFlights, sizeof(int), 1, file); // Read flight count
    resSys->flightList = (FlightInfo *)realloc(resSys->flightList, resSys->totalFlights * sizeof(FlightInfo)); // Resize flight list
    fread(resSys->flightList, sizeof(FlightInfo), resSys->totalFlights, file); // Read flight data

    fclose(file);  // Close the file after reading
    printf("Flight data loaded successfully.\n");
}

// Function to save passenger data to a binary file
void savePassengersToFile(ReservationSystem *resSys) {
    FILE *file = fopen("passengers.dat", "wb");  // Open file in write-binary mode
    if (file == NULL) {
        printf("Could not open file for writing.\n"); // Error handling for file opening
        return;
    }

    // Write the number of passengers and passenger data to the file
    fwrite(&resSys->totalPassengers, sizeof(int), 1, file); // Save passenger count
    fwrite(resSys->passengerList, sizeof(PassengerInfo), resSys->totalPassengers, file); // Save passenger details

    fclose(file);  // Close the file after writing
    printf("Passenger data saved successfully.\n");
}

// Function to load passenger data from a binary file into the system
void loadPassengersFromFile(ReservationSystem *resSys) {
    FILE *file = fopen("passengers.dat", "rb");  // Open the file in read-binary mode
    if (file == NULL) {
        printf("No previous passenger data available.\n"); // Error handling
        return;
    }

    fread(&resSys->totalPassengers, sizeof(int), 1, file); // Read passenger count
    resSys->passengerList = (PassengerInfo *)realloc(resSys->passengerList, resSys->totalPassengers * sizeof(PassengerInfo)); // Resize passenger list
    fread(resSys->passengerList, sizeof(PassengerInfo), resSys->totalPassengers, file); // Read passenger data

    fclose(file);  // Close the file after reading
    printf("Passenger data loaded successfully.\n");
}

// Function to release allocated resources during cleanup
void cleanupReservationSystem(ReservationSystem *resSys) {
    for (int i = 0; i < resSys->totalFlights; i++) {
        free(resSys->flightList[i].seats); // Free each flight's seat allocation
    }
    free(resSys->flightList);  // Free the flight list
    free(resSys->passengerList); // Free the passenger list
}

// Main function that serves as the entry point for the reservation system
int main() {
    ReservationSystem resSys;  // Declare the main reservation system structure
    setupReservationSystem(&resSys);  // Initialize the system with sample flights

    // Load any previously saved flight and passenger data from files
    loadFlightsFromFile(&resSys);
    loadPassengersFromFile(&resSys);

    int userChoice; // Variable to store menu selection from the user
    while (1) { // Infinite loop until user decides to exit
        showMenu(); // Display the menu
        scanf("%d", &userChoice); // Accept user input

        // Based on user input, call the appropriate function
        switch (userChoice) {
            case 1:
                registerFlight(&resSys); // Option to add a new flight
                break;
            case 2:
                reserveTicket(&resSys); // Option to book a ticket
                break;
            case 3:
                revokeTicket(&resSys); // Option to cancel a ticket
                break;
            case 4:
                listFlights(&resSys); // View available flights
                break;
            case 5:
                listPassengers(&resSys); // View list of passengers
                break;
            case 6:
                cleanupReservationSystem(&resSys); // Release resources before exit
                printf("Thank you for using the system. Goodbye!\n");
                return 0; // Exit the program
            default:
                printf("Invalid selection, please try again.\n"); // Handle invalid options
        }
    }

    return 0; // Exit the main function
}
