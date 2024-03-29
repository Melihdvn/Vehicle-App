/**
 * @file vehicle.h
 * 
 * @brief Provides headers
 */

#include "../../utility/header/commonTypes.h"

 /**
  * @brief The "using namespace std;" directive is employed to simplify code by allowing direct access
  *        to elements of the C++ Standard Library without explicitly specifying the "std::" prefix.
  */
using namespace std;

/**
 * @brief Path to the file storing customer vehicle information.
 *        Used in the vehicle management system.
 */
const char* vehiclefileName = "../../../files/customer_vehicle.dat";

/**
 * @brief Path to the temporary file for customer vehicle information.
 *        Used for temporary storage during data manipulation.
 */
const char* tempvehiclefileName = "../../../files/temp_customer_vehicle.dat";

/**
 * @brief Path to the file storing information about vehicle parts.
 *        Used in the vehicle management system.
 */
const char* partfileName = "../../../files/vehicle_parts.dat";

/**
 * @brief Path to the file storing appointment information.
 *        Used in the vehicle management system.
 */
const char* appointmentsfileName = "../../../files/appointments.dat";

/**
 * @brief Path to the file storing vehicle maintenance history.
 *        Used in the vehicle management system.
 */
const char* historyfileName = "../../../files/history.dat";

/**
 * @brief Path to the file storing warranty information.
 *        Used in the vehicle management system.
 */
const char* warrantyfileName = "../../../files/warranty.dat";

/**
 * @brief Path to the file storing maintenance-related information.
 *        Used in the vehicle management system.
 */
const char* maintenancefileName = "../../../files/maintenance.dat";

/**
 * @brief Path to the file storing customer feedback.
 *        Used in the vehicle management system.
 */
const char* feedbackfileName = "../../../files/feedback.txt";

/**
 * @brief Path to the file storing common issues information.
 *        Used in the vehicle management system.
 */
const char* commonissuesfileName = "../../../files/commonissues.txt";



/**
 * @struct Vehicle
 * @brief Represents a vehicle with relevant information.
 */
struct Vehicle {
    int vehicleID;                ///< Unique identifier for the vehicle.
    long long int customerID;      ///< Unique identifier for the customer associated with the vehicle.
    string customerName;           ///< Name of the customer.
    string model;                  ///< Model of the vehicle.
    string plateNumber;            ///< License plate number of the vehicle.
    Vehicle* next;                 ///< Pointer to the next vehicle in a linked list.
    Vehicle* prev;                 ///< Pointer to the previous vehicle in a linked list.
};

/**
 * @struct Appointment
 * @brief Represents an appointment for vehicle service.
 */
struct Appointment {
    int vehicleID;                 ///< Identifier linking the appointment to a specific vehicle.
    string customerName;           ///< Name of the customer associated with the appointment.
    string appointmentType;        ///< Type of appointment (e.g., Repair, Maintenance).
};

/**
 * @struct Date
 * @brief Represents a date with year, month, and day components.
 */
struct Date {
    int year;                      ///< Year component of the date.
    int month;                     ///< Month component of the date.
    int day;                       ///< Day component of the date.
};

/**
 * @brief Matrix structure for storing appointment data.
 *        The matrix is a 3D vector of queues, allowing for organized appointment storage.
 *        The structure is as follows:
 *        - Outer vector: Represents the years dimension.
 *        - Middle vector: Represents the months dimension.
 *        - Inner vector: Represents the days dimension.
 *        - Queue: Represents the container for storing Appointment objects.
 */
std::vector<std::vector<std::vector<std::queue<Appointment>>>> appointmentMatrix;

/**
 * @struct Part
 * @brief Represents a vehicle part with relevant information.
 */
struct Part {
    int partID;                    ///< Unique identifier for the part.
    string name;                   ///< Name of the part.
    string compatiblemodel;        ///< Model compatibility information for the part.
    float price;                   ///< Price of the part.
};

/**
 * @struct PriceResult
 * @brief Represents the result of a price calculation for selected parts.
 */
struct PriceResult {
    float totalAmount;             ///< Total amount including labor costs.
    float amountWithoutLabor;      ///< Total amount excluding labor costs.
};

/**
 * @var int partID
 * @brief Global variable representing the current part identifier.
 */
int partID = 1;

/**
 * @var unordered_map<string, Part> partHash
 * @brief Hash map storing parts with their names as keys.
 */
unordered_map<string, Part> partHash;

/**
 * @var int vehicleID
 * @brief Global variable representing the current vehicle identifier.
 */
int vehicleID = 1;

/**
 * @var Vehicle* headVehicle
 * @brief Pointer to the head of a linked list containing vehicle information.
 */
Vehicle* headVehicle;