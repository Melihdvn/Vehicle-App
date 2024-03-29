/**
 * @brief This directive includes the "vehicleapp.h" header file,
 *        which likely contains declarations and definitions related to the vehicle application.
 *        Including this file allows access to functionalities and structures defined in "vehicleapp.h."
 */
#include "../header/vehicleapp.h"

 /**
  * @brief Sets the cursor position on the console window.
  *
  * @param x The x-coordinate of the cursor.
  * @param y The y-coordinate of the cursor.
  */
void gotoxy(int x, int y) {
    cout << "\033[" << y + 1 << ";" << x + 1 << "H";
}

/**
 * @brief Draws the main box on the console window.
 */
void main_box() {
    int i;

    // Draw the top border
    gotoxy(1, 1);
    cout << "_";
    for (i = 1; i < 79; i++) {
        gotoxy(1 + i, 1);
        cout << "_";
    }

    // Draw the left and right borders
    gotoxy(80, 1);
    cout << "_";
    gotoxy(1, 1);
    for (i = 0; i < 25; i++) {
        gotoxy(1, 2 + i);
        cout << "|";
    }
    gotoxy(1, 27);
    cout << "|";
    for (i = 1; i < 79; i++) {
        gotoxy(1 + i, 27);
        cout << "_";
    }
    gotoxy(80, 27);
    cout << "|";
    gotoxy(80, 1);
    for (i = 0; i < 25; i++) {
        gotoxy(80, 2 + i);
        cout << "|";
    }
}

/**
 * @brief Clears the console or terminal screen.
 *        Uses the appropriate system command based on the operating system.
 */
void clearScreen() {
    std::system(CLEAR_SCREEN);
}

/**
 * @brief Displays a menu on the console window and gets user input for the selected option.
 *
 * @param title The title of the menu.
 * @param options A vector containing the menu options.
 * @return The index of the selected option.
 */
int showMenu(const string& title, const vector<string>& options) {
    int choice = 0;
    int numOptions = options.size();

    while (true) {
        clearScreen();
        main_box();
        gotoxy((windowWidth - 3), windowHeight - 4);
        //HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        //SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << ANSI_COLOR_RED << title << ANSI_COLOR_RESET << endl;
        //SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        // Display menu options with a ">" indicating the selected option
        for (int i = 0; i < numOptions; i++) {
            gotoxy(windowWidth, windowHeight + i * 3);
            cout << (choice == i ? "> " : "  ") << options[i] << endl;
        }

        char key = getch();

        // Handle user input for navigation and selection
        if (key == 'W' || key == 'w' || key == 72) {
            choice = (choice > 0) ? choice - 1 : numOptions - 1;
        }
        else if (key == 'S' || key == 's' || key == 80) {
            choice = (choice < numOptions - 1) ? choice + 1 : 0;
        }
        else if (key == 13 || key == 'O' || key == 'o') {
            return choice; // Enter key - return the selected choice
        }
        else if (key == 27) {
            return numOptions - 1; // Escape key - return the last option (exit)
        }
    }
}

/**
 * @brief Displays text on the console window.
 *
 * @param title The title of the text.
 * @param options A vector containing the lines of text.
 */
void showText(const string& title, const vector<string>& options) {
    int numOptions = options.size();

    clearScreen();
    main_box();
    gotoxy((windowWidth - 3), windowHeight - 4);
    //HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    //SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY);
    cout << ANSI_COLOR_RED << title << ANSI_COLOR_RESET << endl;
    //SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    // Display text options
    for (int i = 0; i < numOptions; i++) {
        gotoxy(windowWidth, windowHeight + i * 3);
        cout << options[i] << endl;
    }
}


/**
 * @brief Handles the vehicle registration process.
 *
 * This function displays a registration form on the console,
 * collects information from the user, creates a new vehicle,
 * and adds it to the linked list of vehicles. The information
 * is also written to a file.
 */
void Registration() {
    // Define the registration form options
    vector<string> options = {
        "Customer ID  : ",
        "Customer Name: ",
        "Vehicle Model: ",
        "Plate Number :",
    };

    // Display the registration form on the console
    showText("Registration", options);

    // Create a new vehicle
    Vehicle* newVehicle = new Vehicle;

    // Assign a unique vehicle ID
    newVehicle->vehicleID = vehicleID++;

    // Collect customer information
    gotoxy(45, 9);
    cin >> newVehicle->customerID;
    gotoxy(45, 12);
    cin >> newVehicle->customerName;

    // Collect vehicle information
    gotoxy(45, 15);
    cin >> newVehicle->model;
    gotoxy(45, 18);
    cin >> newVehicle->plateNumber;

    // Set pointers for linked list
    newVehicle->next = nullptr;
    newVehicle->prev = nullptr;

    // Add the new vehicle to the linked list
    if (!headVehicle) {
        headVehicle = newVehicle;
    }
    else {
        Vehicle* current = headVehicle;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newVehicle;
        newVehicle->prev = current;
    }

    // Write the new vehicle information to a file
    writeVehicleToFile(newVehicle, vehiclefileName);
}

/**
 * @brief Lists vehicles in a paginated format.
 *
 * This function displays a paginated list of vehicles on the console.
 * It shows details such as Vehicle ID, Customer ID, Customer Name,
 * Vehicle Model, and Plate Number. The user can navigate through
 * pages using 'A' (left) and 'D' (right) keys or press the 'esc' key
 * to return to the main menu.
 */
void ListVehicles() {
    int page = 1;
    int pageSize = 14;
    int totalVehicles = CountVehicles();
    int totalPages = (totalVehicles + pageSize - 1) / pageSize;

    while (true) {
        // Clear the console screen
        system(CLEAR_SCREEN);

        // Display the title
        showText("List of Vehicles", { " " });
        gotoxy(5, 8);
        cout << "Vehicle ID | Customer ID | Customer Name | Vehicle Model | Plate Number\n";

        int startVehicle = (page - 1) * pageSize;
        int endVehicle = min(startVehicle + pageSize, totalVehicles);

        Vehicle* current = headVehicle;
        for (int i = 0; i < endVehicle; i++) {
            if (current == nullptr) {
                break;
            }
            if (i >= startVehicle) {
                // Display vehicle details in a formatted manner
                cout << " | " << setw(12) << current->vehicleID << " | " << setw(11) << current->customerID << " | " << setw(13) << current->customerName << " | " << setw(13) << current->model << " | " << current->plateNumber << endl;
            }
            current = current->next;
        }

        // Display page information
        gotoxy(11, 24);
        cout << "Page " << page << " of " << totalPages << endl;

        // Prompt user for navigation options
        gotoxy(9, 26);
        cout << "Press A and D to change page, or esc to return: ";

        // Capture user input
        char input = getch();
        if (input == 'D' || input == 'd' || input == 77) {
            if (page < totalPages) {
                page++;
            }
        }
        else if (input == 'A' || input == 'a' || input == 75) {
            if (page > 1) {
                page--;
            }
        }
        else if (input == 27) {
            // Break the loop if the user presses the 'esc' key
            break;
        }
    }
}


/**
 * @brief Updates information for a specific vehicle.
 *
 * This function prompts the user to enter the Vehicle ID of the
 * vehicle they want to update. If the vehicle is found, it then
 * prompts the user for new information such as Customer ID,
 * Customer Name, Vehicle Model, and Plate Number. The information
 * is updated in-memory, and the corresponding file is also updated.
 */
void UpdateVehicle() {
    int vehicleID;

    // Prompt the user to enter the Vehicle ID
    showText("Update Vehicle", { "Enter Vehicle ID: " });
    gotoxy(45, 9);
    cin >> vehicleID;

    // Search for the vehicle with the specified ID
    Vehicle* current = headVehicle;
    while (current != nullptr) {
        if (current->vehicleID == vehicleID) {
            // Prompt the user for updated information
            showText("Update Vehicle", { "New Customer ID  : ","New Customer Name : ","New Vehicle Model : ", "New Plate Number : " });
            gotoxy(65, 9);
            cin >> current->customerID;
            gotoxy(65, 12);
            cin >> current->customerName;
            gotoxy(65, 15);
            cin >> current->model;
            gotoxy(65, 18);
            cin >> current->plateNumber;

            // Update the vehicle information in the file
            updateVehicleFile(tempvehiclefileName, vehiclefileName);

            // Display a confirmation message
            showText("Update Vehicle", { "Vehicle has been updated." , "Press any key to return..." });
            getch();
            return;
        }
        current = current->next;
    }

    // If the vehicle with the given ID was not found
    showText("Update Vehicle", { "Vehicle with ID " + to_string(vehicleID) + " not found!" , "Press any key to return..." });
    getch();
}



/**
 * @brief Deletes a vehicle with the specified Vehicle ID.
 *
 * This function prompts the user to enter the Vehicle ID of the
 * vehicle they want to delete. If the vehicle is found, it removes
 * the vehicle from the linked list. The corresponding file is also
 * updated, and a confirmation message is displayed.
 */
void DeleteVehicle() {
    int vehicleID;

    // Prompt the user to enter the Vehicle ID
    showText("Delete Vehicle", { "Enter Vehicle ID to delete: " });
    gotoxy(53, 9);
    cin >> vehicleID;

    // Search for the vehicle with the specified ID
    Vehicle* current = headVehicle;
    while (current != nullptr) {
        if (current->vehicleID == vehicleID) {
            // Remove the vehicle from the linked list
            if (current->prev && current->next) {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }
            else if (current->prev) {
                current->prev->next = nullptr;
            }
            else {
                headVehicle = current->next;
            }

            // Update the vehicle information in the file
            updateVehicleFile(tempvehiclefileName, vehiclefileName);

            // Display a confirmation message
            showText("Delete Vehicle", { "Vehicle with ID " + to_string(vehicleID) + " has been deleted." });
            //delete current; // Free the memory associated with the deleted vehicle
            getch();
            return;
        }
        current = current->next;
    }

    // If the vehicle with the given ID was not found
    showText("Delete Vehicle", { "Vehicle with ID " + to_string(vehicleID) + " not found!" , "Press any key to return..." });
    getch();
}



/**
 * @brief Displays the service history.
 *
 * This function shows the service history by displaying the content
 * of the file specified by `historyfileName`. It uses the `DisplayFileContent`
 * function to retrieve and display the file content.
 */
void Service_History() {
    showText("Service History", {});
    cout << DisplayFileContent(historyfileName);
    getch();
};

/**
 * @brief Creates a new appointment.
 *
 * This function prompts the user to enter details for a new appointment,
 * including vehicle ID, customer name, appointment type, and date. It then
 * creates a report, warranty, and maintenance file entries using the provided
 * information. The appointment is added to the appointment matrix, and the
 * appointments are written to the file.
 */
void Create_Appointment() {
    showText("Create Appointment", { "Enter the vehicle ID: " , "Enter the customer name: " ,"Enter the appointment type" , "Enter the date: " });

    // Input variables
    Appointment newAppointment;
    Date newDate;

    // User input for the appointment details
    gotoxy(50, 9);
    cin >> newAppointment.vehicleID;
    gotoxy(50, 12);
    cin >> newAppointment.customerName;
    gotoxy(50, 15);
    cin >> newAppointment.appointmentType;
    gotoxy(50, 18);
    cout << ".. / .. / ....";
    gotoxy(50, 18);
    cin >> newDate.day;
    gotoxy(55, 18);
    cin >> newDate.month;
    gotoxy(60, 18);
    cin >> newDate.year;

    // Create report, warranty, and maintenance file entries
    CreateReport(historyfileName, newAppointment, newDate);
    CreateWarranty(warrantyfileName, newAppointment, newDate);
    if (newAppointment.appointmentType == "Maintenance")
        CreateMaintenance(maintenancefileName, newAppointment, newDate);

    // Add appointment to the appointment matrix and write to file
    addAppointment(newDate, newAppointment);
    writeAppointmentsToFile(appointmentsfileName);
};

/**
 * @brief Lists appointments and displays dates and customer names.
 *
 * This function reads appointments from a file, then displays a list of
 * appointments along with their corresponding dates and customer names.
 */
void List_Appointments() {
    // Read appointments from file
    readAppointmentsFromFile(appointmentsfileName);

    // Display a list of appointments along with dates and customer names
    showText("List Appointments", { "   Date   | Customer Name " });

    for (int i = 0; i < appointmentMatrix.size(); ++i) {
        for (int j = 0; j < appointmentMatrix[i].size(); ++j) {
            for (int k = 0; k < appointmentMatrix[i][j].size(); ++k) {
                auto& appointments = appointmentMatrix[i][j][k];
                Date date{ i, j, k };
                if (!appointments.empty()) {
                    cout << " | " << setw(19) << date.day << "/" << date.month << "/" << date.year << " | " << appointments.front().customerName << "\n";
                }
            }
        }
    }

    getch();
}


/**
 * @brief Allows the user to select parts for labor cost estimation.
 *
 * This function displays a list of parts compatible with a given vehicle model.
 * The user can add parts to the invoice, calculate the total price, or quit the selection process.
 */

void partSelection(const string& searchModel) {
    // Vector to store parts compatible with the specified vehicle model
    vector<Part> displayedParts;
    auto it = partHash.begin();

    // Iterate through partHash to find compatible parts
    while (it != partHash.end()) {
        if (it->second.compatiblemodel == searchModel) {
            displayedParts.push_back(it->second);
        }
        ++it;
    }

    // Vector to store selected parts
    vector<Part> selectedParts;
    // Vector to store the names and prices of selected parts for display
    vector<string> selectedList;
    int currentIndex = 0;

    // Loop for part selection
    while (true) {
        // Display the header and options for part selection
        showText("Labor Cost Estimation", {});
        gotoxy(7, 9);
        cout << "Press A to add to invoice, enter to calculate total price, and Q to quit.";
        gotoxy(12, 11);
        cout << "Part name | Price ";
        gotoxy(0, 12);

        // Display the list of parts with a cursor indicating the current selection
        for (int i = 0; i < displayedParts.size(); ++i) {
            if (i == currentIndex) {
                cout << " | ->";
            }
            else {
                cout << " |   ";
            }
            cout << setw(16) << displayedParts[i].name << " | " << displayedParts[i].price << endl;
        }

        // Display the list of selected parts
        gotoxy(36, 11);
        cout << "Selected parts \n";

        for (int i = 0; i < selectedList.size(); ++i) {
            gotoxy(36, 12 + i);
            cout << selectedList[i];
        }

        // Get user input
        char input = getch();

        // Process user input
        if (input == 'A' || input == 'a') {
            // Add the selected part to the invoice
            selectedParts.push_back(displayedParts[currentIndex]);
            selectedList.push_back(displayedParts[currentIndex].name + "  " + to_string(displayedParts[currentIndex].price));
        }
        else if (input == 13 || input == 'O' || input == 'o') {
            // Calculate and display the total price
            PriceResult result = calculateTotalPrice(selectedParts);

            float totalPrice = result.totalAmount;
            float partPrice = result.amountWithoutLabor;

            showText("Total Price", { "Parts       : " + to_string(partPrice) ,"Labor Fee   : " + to_string(totalPrice - partPrice), "Total Amount: " + to_string(totalPrice), "Press any key to continue..." });
            getch();
            return;
        }
        else if (input == 'Q' || input == 'q') {
            // Quit the part selection process
            break;
        }
        else if (input == 72) {
            // Move the cursor up
            if (currentIndex > 0) {
                --currentIndex;
            }
        }
        else if (input == 80) {
            // Move the cursor down
            if (currentIndex < displayedParts.size() - 1) {
                ++currentIndex;
            }
        }
    }
}

/**
 * @brief Initiates the process of estimating labor costs for a vehicle based on the compatible parts model.
 *
 * This function prompts the user to enter the model of the car and then calls the partSelection function
 * to allow the user to select compatible parts for labor cost estimation.
 */
void Labor_Cost_Estimation() {
    // Input variable to store the model of the car for part search
    string searchModel;

    // Display the prompt for entering the car model
    showText("Part Search By Model", { "Enter the model of the car: " });
    gotoxy(59, 9);

    // Get user input for the car model
    cin >> searchModel;

    // Call the partSelection function to allow the user to select compatible parts
    partSelection(searchModel);

    // Wait for user input before returning
    getch();
}



/**
 * @brief Initiates the process of collecting service feedback from the user.
 *
 * This function displays a prompt asking the user to write their feedback, reads the input, and then
 * stores the feedback in a file with a predefined title.
 */
void Service_Feedback() {
    // Feedback string to store the user's feedback
    string feedback = "Service feedback : ";
    // Title for the feedback message
    string feedbackTitle = "Service Feedback: ";

    // Display the prompt for writing feedback
    showText("Service Feedback", { "Please write your feedback: \n " });
    gotoxy(10, 10);

    // Read user input for feedback
    getline(cin, feedback);

    // Prepend the title to the feedback
    feedback = feedbackTitle + feedback;

    // Create feedback file with the user's feedback
    CreateFeedback(feedbackfileName, feedback);
}

/**
 * @brief Displays warranty expiration alerts to the user.
 *
 * This function shows a screen with information about warranty expirations by reading the content
 * from a file and printing it on the screen.
 */
void Warranty_Expiration_Alerts() {
    // Display screen for warranty expirations
    showText("Warranty Expirations", {});

    // Print content of the warranty file on the screen
    cout << DisplayFileContent(warrantyfileName);

    // Wait for user input before returning
    getch();
}

/**
 * @brief Displays preventive maintenance reminders.
 *
 * This function shows the next maintenance dates and displays the content
 * of the maintenance file. It waits for user input before returning.
 */
void Preventive_Maintenance_Reminders() {
    showText("Next Maintenance Dates", {}); // Display a heading for maintenance dates
    cout << DisplayFileContent(maintenancefileName); // Display content from the maintenance file
    getch(); // Wait for user input
};

/**
 * @brief Displays common issues.
 *
 * This function shows common issues and displays the content
 * of the common issues file. It waits for user input before returning.
 */
void Common_issues() {
    showText("Common issues", {}); // Display a heading for common issues
    cout << DisplayFileContent(commonissuesfileName); // Display content from the common issues file
    getch(); // Wait for user input
};

/**
 * @brief Handles customer feedback.
 *
 * This function prompts the user to provide feedback, collects the input,
 * and appends a feedback title before storing it using CreateFeedback().
 */
void Customer_feedback() {
    string feedback;
    string feedbackTitle = "Customer Feedback: ";
    showText("Customer Feedback", { "Please write your feedback: \n " }); // Display a prompt for feedback
    gotoxy(10, 10); // Move the cursor to a specific position
    getline(cin, feedback); // Collect feedback from the user
    feedback = feedbackTitle + feedback; // Add feedback title
    CreateFeedback(feedbackfileName, feedback); // Store the feedback in a file
};


/**
 * @brief Displays monthly service statistics.
 *
 * This function outputs a message indicating monthly service statistics.
 */
void Monthly_service_stats() { cout << "Monthly Service Stats" << endl; };

/**
 * @brief Adds a new part to the inventory.
 *
 * This function collects information about a new part, assigns a unique ID,
 * and adds it to the partHash and a file. It then outputs a success message.
 */
void AddParts() {
    Part newPart;
    newPart.partID = partID++; // Assign a unique part ID

    showText("Add Parts", { "Enter the part name: ", "Enter the car model: ", "Enter the price of the part:" });

    gotoxy(50, 9);
    cin >> newPart.name;
    gotoxy(50, 12);
    cin >> newPart.compatiblemodel;
    gotoxy(50, 15);
    cin >> newPart.price;

    partHash[to_string(newPart.partID)] = newPart; // Add part to the inventory
    writePartToFile(newPart, partfileName); // Write part information to a file

    cout << "Part successfully added." << endl;
}

/**
 * @brief Lists parts with pagination and allows searching by model.
 *
 * This function displays a paginated list of parts, allowing the user to navigate
 * through pages, search for parts by model, or return to the main menu.
 */
void ListParts() {
    int page = 1;
    int pageSize = 14;
    int totalParts = partHash.size();
    int totalPages = (totalParts + pageSize - 1) / pageSize;

    while (true) {
        system(CLEAR_SCREEN);
        showText("List of Parts", { " " });
        gotoxy(5, 8);
        cout << "          Part Name | Vehicle Model | Price\n";

        int startPart = (page - 1) * pageSize;
        int endPart = min(startPart + pageSize, totalParts);

        auto it = partHash.begin();
        advance(it, startPart);

        for (int i = startPart; i < endPart; i++) {
            if (it == partHash.end()) {
                break;
            }
            cout << " | " << setw(21) << it->second.name << " | " << setw(13) << it->second.compatiblemodel << " | " << it->second.price << endl;
            ++it;
        }

        gotoxy(11, 24);
        cout << "Page " << page << " of " << totalPages << endl;

        gotoxy(9, 26);
        cout << "Press A and D to change page, S to search by model or esc to return: ";

        char input = getch();
        if (input == 'D' || input == 'd' || input == 77) {
            if (page < totalPages) {
                page++;
            }
        }
        else if (input == 'A' || input == 'a' || input == 75) {
            if (page > 1) {
                page--;
            }
        }
        else if (input == 'S' || input == 's') {
            // Search by model
            system(CLEAR_SCREEN);
            string searchModel;
            showText("Part Search By Model", { "Enter the compatible model to search: " });
            gotoxy(59, 9);
            cin >> searchModel;

            gotoxy(9, 12);
            it = partHash.begin();
            int count = 1;
            gotoxy(21, 11);
            cout << "Part name | Price ";
            while (it != partHash.end()) {
                if (it->second.compatiblemodel == searchModel) {
                    gotoxy(9, 11 + count);
                    cout << setw(21) << it->second.name << " | " << it->second.price;
                    count++;
                }
                ++it;
            }

            gotoxy(11, 26);
            cout << "Press any key to return to the list...";
            getch();
        }
        else if (input == 27) {
            break; // Escape key to return to the main menu
        }
    }
}


/**
 * @brief Manages the Parts Inventory System.
 *
 * This function presents a menu with options to add a part, list parts, or return to the main menu.
 * It utilizes the showMenu function to display the menu options and calls corresponding functions based on the user's choice.
 */
void Parts_Inventory_Systems() {
    vector<string> options = {
        "Add Part",
        "List Parts",
        "Main Menu"
    };

    while (true) {
        int choice = showMenu("Part Inventory System", options);

        switch (choice) {
        case 0:
            AddParts(); // Call function to add a part
            break;
        case 1:
            ListParts(); // Call function to list parts
            break;
        case 2:
            return; // Return to the main menu
        }
    }
};

/**
 * @brief Displays a message about Customer Communication Platforms.
 *
 * This function outputs a message indicating Customer Communication Platforms.
 */
void Customer_Communication_Platforms() { cout << "Customer Communication Platforms" << endl; };

/**
 * @brief Manages customer and vehicle information.
 *
 * This function presents a menu with options for registration, updating, deleting, viewing service history,
 * listing current vehicles, or returning to the main menu. It utilizes the showMenu function to display the menu options
 * and calls corresponding functions based on the user's choice.
 */
void Customer_and_vehicle_management() {
    vector<string> options = {
        "Registration",
        "Update",
        "Deleting",
        "Service History",
        "List Current Vehicles",
        "Main Menu"
    };

    while (true) {
        int choice = showMenu("Customer and Vehicle Management", options);

        switch (choice) {
        case 0:
            Registration(); // Call function for customer registration
            break;
        case 1:
            UpdateVehicle(); // Call function to update vehicle information
            break;
        case 2:
            DeleteVehicle(); // Call function to delete a vehicle
            break;
        case 3:
            Service_History(); // Call function to view service history
            break;
        case 4:
            ListVehicles(); // Call function to list current vehicles
            break;
        case 5:
            return; // Return to the main menu
        }
    }
}

/**
 * @brief Manages service appointment scheduling.
 *
 * This function presents a menu with options to create an appointment, list appointments,
 * estimate labor costs, provide service feedback, or return to the main menu.
 * It utilizes the showMenu function to display the menu options and calls corresponding functions
 * based on the user's choice.
 */
void Service_appointment_scheduling() {
    vector<string> options = {
        "Create a appointment",
        "List appointments",
        "Labor Cost Estimation",
        "Service Feedback",
        "Main Menu"
    };

    while (true) {
        int choice = showMenu("Service Appointment Scheduling", options);

        switch (choice) {
        case 0:
            Create_Appointment(); // Call function to create a service appointment
            break;
        case 1:
            List_Appointments(); // Call function to list service appointments
            break;
        case 2:
            Labor_Cost_Estimation(); // Call function to estimate labor costs
            break;
        case 3:
            Service_Feedback(); // Call function to provide service feedback
            break;
        case 4:
            return; // Return to the main menu
            break;
        }
    }
}

/**
 * @brief Manages warranty and maintenance tracking.
 *
 * This function presents a menu with options for warranty expiration alerts,
 * preventive maintenance reminders, or returning to the main menu.
 * It utilizes the showMenu function to display the menu options and calls corresponding functions
 * based on the user's choice.
 */
void Warranty_and_maintenance_tracking() {
    vector<string> options = {
        "Warranty Expiration Alerts",
        "Preventive Maintenance Reminders",
        "Main Menu"
    };

    while (true) {
        int choice = showMenu("Warranty and Maintenance Tracking", options);

        switch (choice) {
        case 0:
            Warranty_Expiration_Alerts(); // Call function for warranty expiration alerts
            break;
        case 1:
            Preventive_Maintenance_Reminders(); // Call function for preventive maintenance reminders
            break;
        case 2:
            return; // Return to the main menu
            break;
        }
    }
}

/**
 * @brief Manages reporting functions.
 *
 * This function presents a menu with options for common issues, customer feedback,
 * monthly service statistics, or returning to the main menu.
 * It utilizes the showMenu function to display the menu options and calls corresponding functions
 * based on the user's choice.
 */
void Reporting() {
    vector<string> options = {
        "Common Issues",
        "Customer Feedback",
        "Monthly Service Stats",
        "Main Menu"
    };

    while (true) {
        int choice = showMenu("Reporting", options);

        switch (choice) {
        case 0:
            Common_issues(); // Call function to display common issues
            break;
        case 1:
            Customer_feedback(); // Call function to manage customer feedback
            break;
        case 2:
            Monthly_service_stats(); // Call function to display monthly service statistics
            break;
        case 3:
            return; // Return to the main menu
            break;
        }
    }
}


/**
 * @brief Manages integrations with external systems.
 *
 * This function presents a menu with options to integrate with parts inventory systems,
 * customer communication platforms, or return to the main menu.
 * It utilizes the showMenu function to display the menu options and calls corresponding functions
 * based on the user's choice.
 */
void Integrations() {
    vector<string> options = {
        "Parts Inventory Systems",
        "Customer Communication Platforms",
        "Main Menu"
    };

    while (true) {
        int choice = showMenu("Integrations", options);

        switch (choice) {
        case 0:
            Parts_Inventory_Systems(); // Call function for parts inventory systems integration
            break;
        case 1:
            Customer_Communication_Platforms(); // Call function for customer communication platforms integration
            break;
        case 2:
            return; // Return to the main menu
            break;
        }
    }
}

/**
 * @brief Displays the main menu and handles user choices.
 *
 * This function presents the main menu with options for customer and vehicle management,
 * service appointment scheduling, warranty and maintenance tracking, reporting, integrations, or exiting the program.
 * It utilizes the showMenu function to display the menu options and calls corresponding functions
 * based on the user's choice.
 *
 * @return An integer indicating the exit status (0 for normal exit).
 */
int mainMenu() {
    vector<string> mainMenuOptions = {
        "Customer and Vehicle Management",
        "Service Appointment Scheduling",
        "Warranty and Maintenance Tracking",
        "Reporting",
        "Integrations",
        "Exit"
    };

    while (true) {
        int choice = showMenu("Vehicle Service and Maintenance Automation", mainMenuOptions);

        switch (choice) {
        case 0:
            Customer_and_vehicle_management(); // Call function for customer and vehicle management
            break;
        case 1:
            Service_appointment_scheduling(); // Call function for service appointment scheduling
            break;
        case 2:
            Warranty_and_maintenance_tracking(); // Call function for warranty and maintenance tracking
            break;
        case 3:
            Reporting(); // Call function for reporting
            break;
        case 4:
            Integrations(); // Call function for integrations
            break;
        case 5:
            exit(0); // Exit the program
        }
    }

    return 0; // This return statement is unreachable but included for completeness
}

/**
 * @brief Main entry point of the program.
 *
 * This function loads vehicle and part information from files, initializes appointment matrices,
 * and calls the main menu function to start the program.
 */
int main() {
    loadVehiclesFromFile(vehiclefileName); // Load vehicle information from a file
    initializeAppointmentMatrix(2050, 13, 30); // Initialize appointment matrices
    loadPartsFromFile(partfileName); // Load part information from a file
    mainMenu(); // Call the main menu function to start the program
}
