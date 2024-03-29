/**
 * @brief This directive includes the "vehicle.h" header file,
 *        which likely contains declarations and definitions related to the vehicle entity.
 *        Including this file allows access to functionalities and structures defined in "vehicle.h."
 */
#include "../header/vehicle.h"

/**
 * @brief Calculate the total price for a list of selected parts.
 *
 * This function calculates the total price and amount without labor for a list of selected parts.
 *
 * @param selectedParts Vector of Part structures representing the selected parts.
 * @return PriceResult structure containing the total amount and amount without labor.
 */
PriceResult calculateTotalPrice(const vector<Part>& selectedParts) {
    float totalAmount = 0;         //!< Total amount including labor.
    float amountWithoutLabor = 0;  //!< Amount without labor cost.

    // Iterate through each selected part and calculate the total amount and amount without labor.
    for (const auto& part : selectedParts) {
        amountWithoutLabor += part.price;

        // Calculate total amount based on part price ranges.
        if (part.price < 500)
            totalAmount += part.price;
        else if (part.price < 2000)
            totalAmount += part.price + 1000;
        else if (part.price < 10000)
            totalAmount += part.price + 3000;
        else
            totalAmount += part.price + 8000;
    }

    return { totalAmount, amountWithoutLabor };
}

/**
 * @brief Initialize the appointment matrix with given dimensions.
 *
 * This function initializes a 3D vector (appointmentMatrix) to store appointments based on date.
 *
 * @param years Number of years.
 * @param months Number of months.
 * @param days Number of days.
 */
void initializeAppointmentMatrix(int years, int months, int days) {
    // Resize the appointmentMatrix to the specified dimensions.
    appointmentMatrix.resize(years + 1);
    for (int i = 0; i <= years; ++i) {
        appointmentMatrix[i].resize(months + 1);
        for (int j = 0; j <= months; ++j) {
            appointmentMatrix[i][j].resize(days + 1);
        }
    }
}

/**
 * @brief Writes the appointments to a file.
 *
 * This function opens the file in append mode and writes the appointments from the
 * appointmentMatrix to the file. Each appointment includes the date and customer name.
 */
void writeAppointmentsToFile(const char* fileName) {
    // Open the file in append mode
    ofstream file(fileName, ios::app);

    // Loop through the appointment matrix
    for (int i = 0; i < appointmentMatrix.size(); ++i) {
        for (int j = 0; j < appointmentMatrix[i].size(); ++j) {
            for (int k = 0; k < appointmentMatrix[i][j].size(); ++k) {
                auto& appointments = appointmentMatrix[i][j][k];
                Date date{ i, j, k };

                // Write each appointment to the file
                while (!appointments.empty()) {
                    file << date.day << ' ' << date.month << ' ' << date.year << ' ' << appointments.front().customerName << '\n';
                    appointments.pop();
                }
            }
        }
    }

    // Close the file
    file.close();
}

/**
 * @brief Adds an appointment to the matrix.
 *
 * Checks if the date is within the matrix bounds and adds the appointment
 * to the appropriate location in the matrix.
 *
 * @param date The date of the appointment.
 * @param appointment The appointment to be added.
 */
void addAppointment(const Date& date, const Appointment& appointment) {
    appointmentMatrix[date.year][date.month][date.day].push(appointment);

}

/**
 * @brief Lists appointments for a specific date.
 *
 * Prints the date for which appointments are listed and checks if
 * appointments exist for the given date. If appointments exist, it
 * prints each appointment for the date.
 *
 * @param date The date for which appointments are listed.
 */
void listAppointments(const Date& date) {
    // Print the date for which appointments are listed
    cout << "Appointments " << date.day + 1 << "." << date.month + 1 << "." << date.year << ":\n";

        int count = 1;
        // Print each appointment for the date
        while (!appointmentMatrix[date.year][date.month][date.day].empty()) {
            cout << count << ". " << appointmentMatrix[date.year][date.month][date.day].front().customerName << "\n";
            appointmentMatrix[date.year][date.month][date.day].pop();
            count++;
        }
}

/**
 * @brief Reads appointments from a file and adds them to the matrix.
 *
 * This function reads each line from the appointments file, extracts date
 * and customer name information, and adds the appointment to the matrix using
 * the addAppointment function. If the file cannot be opened, an error message
 * is printed to the standard error stream. If the file format is incorrect,
 * an error message is printed for the specific line with incorrect format.
 */
void readAppointmentsFromFile(const char* fileName) {
    // Open the file for reading
    ifstream file(fileName);

    string line;
    // Read each line from the file
    while (getline(file, line)) {
        istringstream iss(line);
        int day, month, year;
        string customerName;

        // Extract date and customer name information from the line
        if (iss >> day >> month >> year >> customerName) {
            // Create Date and Appointment objects
            Date date{ year, month, day };
            Appointment appointment;
            appointment.customerName = customerName;

            // Add the appointment to the matrix
            addAppointment(date, appointment);
        }
        else {
            // Print an error message for incorrect file format
            cerr << "Incorrect file format: " << line << endl;
        }
    }

    // Close the file
    file.close();
}

/**
 * @brief Creates a report and appends it to a file.
 *
 * This function creates a report with information about the appointment,
 * such as appointment type, customer name, vehicle ID, and date. It appends
 * the report to the specified file.
 *
 * @param filePath The path to the file where the report will be appended.
 * @param appointment The appointment information.
 * @param date The date of the appointment.
 */
void CreateReport(const string& filePath, const Appointment& appointment, const Date& date) {
    ofstream outputFile(filePath, ios::app);

    stringstream report;

    report << " | The following operation: " << appointment.appointmentType << ", performed to the customer : \n | " << appointment.customerName
        << " with vehicle ID " << appointment.vehicleID << ", on this date: "
        << date.day << "/" << date.month << "/" << date.year << "\n";

    // Write the report to the specified file
    outputFile << report.str() << endl;

    // Close the file
    outputFile.close();
}

/**
 * @brief Creates a warranty document and appends it to a file.
 *
 * This function creates a warranty document with information about the appointment,
 * such as appointment type, customer name, vehicle ID, date, and expiration date.
 * It calculates the expiration date (1 month later) and appends the warranty document
 * to the specified file.
 *
 * @param filePath The path to the file where the warranty document will be appended.
 * @param appointment The appointment information.
 * @param date The date of the appointment.
 */
void CreateWarranty(const string& filePath, const Appointment& appointment, const Date& date) {
    ofstream outputFile(filePath, ios::app);

    stringstream warranty;

    // Calculate the expiration date (1 month later)
    Date expirationDate = date;
    expirationDate.month += 1;
    if (expirationDate.month > 12) {
        expirationDate.month = 1;
        expirationDate.year += 1;
    }

    warranty << " | " << appointment.appointmentType << ", performed to the customer : \n | " << appointment.customerName
        << " with vehicle ID " << appointment.vehicleID << ", on this date: "
        << date.day << "/" << date.month << "/" << date.year << "\n";

    warranty << " | Warranty for repair valid until: " << expirationDate.day << "/" << expirationDate.month << "/"
        << expirationDate.year << "\n";

    // Write the warranty document to the specified file
    outputFile << warranty.str() << endl;

    // Close the file
    outputFile.close();
}

/**
 * @brief Creates a maintenance document and appends it to a file.
 *
 * This function creates a maintenance document with information about the appointment,
 * such as appointment type, customer name, vehicle ID, date, and expiration date.
 * It calculates the expiration date (1 year later) and appends the maintenance document
 * to the specified file.
 *
 * @param filePath The path to the file where the maintenance document will be appended.
 * @param appointment The appointment information.
 * @param date The date of the appointment.
 */
void CreateMaintenance(const string& filePath, const Appointment& appointment, const Date& date) {
    ofstream outputFile(filePath, ios::app);

    stringstream maintenance;

    Date expirationDate = date;
    expirationDate.year += 1;

    maintenance << " | Maintenance appointment performed for the customer  \n | " << appointment.customerName
        << " with vehicle ID " << appointment.vehicleID << ", on this date: "
        << date.day << "/" << date.month << "/" << date.year << "\n";

    maintenance << " | Next maintenance date is : " << expirationDate.day << "/" << expirationDate.month << "/"
        << expirationDate.year << "\n";

    // Write the maintenance document to the specified file
    outputFile << maintenance.str() << endl;

    // Close the file
    outputFile.close();
}

/**
 * @brief Creates a feedback entry and appends it to a file.
 *
 * This function creates a feedback entry with the provided message and appends
 * it to the specified file.
 *
 * @param filePath The path to the file where the feedback entry will be appended.
 * @param message The feedback message.
 */
void CreateFeedback(const string& filePath, string message) {
    ofstream outputFile(filePath, ios::app);
    stringstream feedback;

    feedback << message << "\n";

    // Write the feedback entry to the specified file
    outputFile << feedback.str() << endl;

    // Close the file
    outputFile.close();
}

/**
 * @brief Displays the content of a file.
 *
 * This function reads the content of the specified file and returns it as a string.
 * If the file can't be opened, an error message is returned.
 *
 * @param filePath The path to the file whose content will be displayed.
 * @return A string containing the content of the file or an error message.
 */
string DisplayFileContent(const string& filePath) {
    ifstream inputFile(filePath);

    string line;
    string output;

        // Read and concatenate each line in the file
        while (getline(inputFile, line)) {
            output += line + "\n";
        }

        // Close the file
        inputFile.close();

    return output;
}

/**
 * @brief Loads parts information from a file into a hash map.
 *
 * This function reads parts information from the specified file and populates
 * a hash map with part names as keys and corresponding Part structures as values.
 */
void loadPartsFromFile(const char* fileName) {
    ifstream file(partfileName);

    Part part;
    while (file >> part.name >> part.compatiblemodel >> part.price) {
        // Assign part information to the hash map using the part name as the key
        partHash[to_string(part.partID)] = part;
    }

    file.close();
}

/**
 * @brief Writes part information to a file.
 *
 * This function appends part information to the specified file.
 *
 * @param part The part information to be written to the file.
 */
void writePartToFile(const Part& part, const char* fileName) {
    ofstream file(fileName, ios::app);

    // Write part information to the file
    file << part.name << " " << part.compatiblemodel << " " << part.price << endl;

    // Close the file
    file.close();
}

/**
 * @brief Writes vehicle information to a binary file.
 *
 * This function appends binary vehicle information to the specified file.
 *
 * @param vehicle Pointer to the Vehicle object to be written to the file.
 * @throw runtime_error if the file cannot be opened for writing.
 */
void writeVehicleToFile(Vehicle* vehicle, const char* fileName) {
    ofstream file(fileName, ios::binary | ios::app);

    // Write the binary representation of the vehicle object to the file
    file.write((char*)vehicle, sizeof(Vehicle));

    // Close the file
    file.close();
}

/**
 * @brief Updates the vehicle file by writing the linked list of vehicles to a temporary file.
 *
 * This function iterates through the linked list of vehicles, writes each vehicle to a temporary file,
 * removes the original vehicle file, and renames the temporary file to the original file.
 */
void updateVehicleFile(const char* tempvehiclefileName, const char* vehiclefileName) {
    // Create a temporary file.
    fstream tempFile(tempvehiclefileName, ios::binary | ios::out);

    // Write all of the vehicles in the linked list to the temporary file.
    Vehicle* current = headVehicle;
    while (current != nullptr) {
        tempFile.write((char*)current, sizeof(Vehicle));
        current = current->next;
    }

    // Close the temporary file.
    tempFile.close();

    // Remove the save file.
    remove(vehiclefileName);

    // Rename the temporary file to the save file.
    rename(tempvehiclefileName, vehiclefileName);
}

/**
 * @brief Loads vehicles from a binary file into a linked list.
 *
 * This function reads vehicles from the specified file and creates a linked list
 * of vehicles with the information loaded from the file.
 */
void loadVehiclesFromFile(const char* fileName) {
    ifstream file(fileName, ios::binary);

    if (!file.is_open()) {
        return;
    }

    // Initialize vehicleID to 0
    vehicleID = 0;

    Vehicle* current = nullptr; // Initialize the current pointer to nullptr

    while (true) {
        Vehicle* newVehicle = new Vehicle; // Allocate memory for a new vehicle node
        newVehicle->next = nullptr;

        // Attempt to read a Vehicle from the file
        if (!file.read((char*)newVehicle, sizeof(Vehicle))) {
            // If we can't read a Vehicle, we've reached the end of the file
            delete newVehicle; // Free the memory allocated for the last newVehicle
            break;
        }

        newVehicle->prev = current;

        if (current) {
            current->next = newVehicle;
        }
        else {
            // If current is nullptr, this is the first node
            headVehicle = newVehicle;
        }

        current = newVehicle;
        vehicleID = newVehicle->vehicleID + 1; // Update the vehicle ID from the loaded vehicle
    }

    file.close();
}

/**
 * @brief Counts the number of vehicles in the linked list.
 *
 * This function traverses the linked list and counts the number of vehicles.
 *
 * @return The number of vehicles in the linked list.
 */
int CountVehicles() {
    int count = 0;
    Vehicle* current = headVehicle;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}