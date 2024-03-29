/**
 * @file vehicle_tests.cpp
 * @brief Unit tests for the Vehicle class and related functions.
 */
#include "gtest/gtest.h"
#include "../../vehicleapp/header/vehicleapp.h"  // Adjust this include path based on your project structure

 /**
  * @brief VehicleTests class is derived from ::testing::Test to facilitate unit testing.
  */
class VehicleTests : public ::testing::Test {
protected:
    /**
     * @brief Set up function called before each test case.
     */
    void SetUp() override {
        initializeAppointmentMatrix(2050, 13, 30);
    }

    /**
     * @brief Tear down function called after each test case.
     */
    void TearDown() override {
        appointmentMatrix.clear();
        partHash.clear();
    }
};

/**
 * @brief Test case for the calculateTotalPrice function.
 */
TEST_F(VehicleTests, TestCalculateTotalPrice) {
    // Test data
    Part part1{ 1, "Part1", "Model1", 200.0 };
    Part part2{ 2, "Part2", "Model2", 1000.0 };
    Part part3{ 1, "Part3", "Model3", 8000.0 };
    Part part4{ 2, "Part4", "Model4", 11000.0 };
    vector<Part> selectedParts{ part1, part2, part3, part4 };

    // Test calculation
    PriceResult result = calculateTotalPrice(selectedParts);

    // Test expectations
    EXPECT_FLOAT_EQ(result.totalAmount, 32200.0);
    EXPECT_FLOAT_EQ(result.amountWithoutLabor, 20200.0);
}

/**
 * @brief Test case for the initializeAppointmentMatrix function.
 */
TEST_F(VehicleTests, TestInitializeAppointmentMatrix) {
    // Test initialization
    initializeAppointmentMatrix(2, 6, 10);

    // Test expectations
    EXPECT_EQ(appointmentMatrix.size(), 3); // 2 + 1
    EXPECT_EQ(appointmentMatrix[2].size(), 7); // 6 + 1
    EXPECT_EQ(appointmentMatrix[2][6].size(), 11); // 10 + 1
}

/**
 * @brief Test case for the addAppointment function.
 */
TEST_F(VehicleTests, TestAddAppointment) {
    // Test data
    Date date{ 2023, 11, 14 };
    Appointment appointment{ 1, "John Doe", "Repair" };

    // Test function
    addAppointment(date, appointment);

    // Test expectations
    EXPECT_EQ(appointmentMatrix[2023][11][14].front().customerName, "John Doe");
}

/**
 * @brief Test case for the listAppointments function.
 */
TEST_F(VehicleTests, TestListAppointments) {
    // Test data
    Date date{ 2023, 11, 14 };
    Appointment appointment{ 1, "John Doe", "Repair" };

    // Test function
    addAppointment(date, appointment);

    // Redirect cout to capture output
    std::stringstream outputStream;
    std::streambuf* oldCout = std::cout.rdbuf(outputStream.rdbuf());

    // Test function
    listAppointments(date);

    // Restore cout
    std::cout.rdbuf(oldCout);

    // Test expectations
    EXPECT_EQ(outputStream.str(), "Appointments 15.12.2023:\n1. John Doe\n");

}

/**
 * @brief Test case for reading and writing appointments to/from files.
 */
TEST_F(VehicleTests, TestReadAndWriteAppointments) {
    // Test data
    initializeAppointmentMatrix(2050, 13, 30);
    Appointment testAppointment;
    Date testDate;
    testAppointment.vehicleID = 1;
    testAppointment.customerName = "John";
    testAppointment.appointmentType = "Repair";
    testDate.day = 11;
    testDate.month = 10;
    testDate.year = 2023;

    // Test function
    addAppointment(testDate, testAppointment);

    // Clear the appointments file
    ofstream clearFile("appointments.dat", std::ios::trunc);
    clearFile.close();

    // Test write and read functions
    writeAppointmentsToFile("appointments.dat");
    readAppointmentsFromFile("appointments.dat");

    // Test expectations
    EXPECT_EQ(appointmentMatrix[2023][10][11].front().customerName, "John");
}

/**
 * @brief Test case for getting error from read appointments from files.
 */
TEST_F(VehicleTests, TestReadAppointmentsInvalid) {
    // ... (your existing test code)

    // Break the file by adding random text
    ofstream breakFile("appointments.dat", std::ios::app);
    if (breakFile.is_open()) {
        breakFile << "Random Text\n";
        breakFile.close();
    }

    // Test write and read functions
    writeAppointmentsToFile("appointments.dat");
    string broke = "Test";

    // Redirect cerr to a stringstream to capture the error message
    stringstream buffer;
    streambuf* oldcerr = cerr.rdbuf(buffer.rdbuf());

    readAppointmentsFromFile("appointments.dat");

    // Restore cerr
    cerr.rdbuf(oldcerr);

    // Test expectations
    // Check if the error message is printed
    EXPECT_TRUE(buffer.str().find("Incorrect file format") != string::npos);
}

/**
 * @brief Test case for creating a report file.
 */
TEST_F(VehicleTests, TestCreateReport) {
    // Test data
    Date date{ 2023, 11, 14 };
    Appointment appointment{ 1, "John Doe", "Repair" };

    // Test function
    std::string filePath = "test_report.txt";
    CreateReport(filePath, appointment, date);

    // Open the file and read its content
    std::ifstream file(filePath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    // Test expectations
    EXPECT_EQ(buffer.str(), " | The following operation: Repair, performed to the customer : \n | John Doe with vehicle ID 1, on this date: 14/11/2023\n\n");

    // Clear the file
    ofstream clearFile("test_report.txt", std::ios::trunc);
    clearFile.close();
}

/**
 * @brief Test case for creating a warranty file.
 */
TEST_F(VehicleTests, TestCreateWarranty) {
    // Test data
    Date date{ 2023, 11, 14 };
    Appointment appointment{ 1, "John Doe", "Repair" };

    // Test function
    std::string filePath = "test_warranty.txt";
    CreateWarranty(filePath, appointment, date);

    // Open the file and read its content
    std::ifstream file(filePath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    // Test expectations
    EXPECT_EQ(buffer.str(), " | Repair, performed to the customer : \n | John Doe with vehicle ID 1, on this date: 14/11/2023\n | Warranty for repair valid until: 14/12/2023\n\n");

    // Clear the file
    ofstream clearFile("test_warranty.txt", std::ios::trunc);
    clearFile.close();
}

/**
 * @brief Test case for creating a warranty file.
 */
TEST_F(VehicleTests, TestCreateWarranty2) {
    // Test data
    Date date{ 2023, 12, 14 };
    Appointment appointment{ 1, "John Doe", "Repair" };

    // Test function
    std::string filePath = "test_warranty.txt";
    CreateWarranty(filePath, appointment, date);

    // Open the file and read its content
    std::ifstream file(filePath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    // Test expectations
    EXPECT_EQ(buffer.str(), " | Repair, performed to the customer : \n | John Doe with vehicle ID 1, on this date: 14/12/2023\n | Warranty for repair valid until: 14/1/2024\n\n");

    // Clear the file
    ofstream clearFile("test_warranty.txt", std::ios::trunc);
    clearFile.close();
}

/**
 * @brief Test case for creating a maintenance file.
 */
TEST_F(VehicleTests, TestCreateMaintenance) {
    // Test data
    Date date{ 2023, 11, 14 };
    Appointment appointment{ 1, "John Doe", "Maintenance" };

    // Test function
    std::string filePath = "test_maintenance.txt";
    CreateMaintenance(filePath, appointment, date);

    // Open the file and read its content
    std::ifstream file(filePath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    // Test expectations
    EXPECT_EQ(buffer.str(), " | Maintenance appointment performed for the customer  \n | John Doe with vehicle ID 1, on this date: 14/11/2023\n | Next maintenance date is : 14/11/2024\n\n");

    // Clear the file
    ofstream clearFile("test_maintenance.txt", std::ios::trunc);
    clearFile.close();
}

/**
 * @brief Test case for creating a feedback file.
 */
TEST_F(VehicleTests, TestCreateFeedback) {
    // Test data
    std::string feedbackMessage = "Great service!";

    // Test function
    std::string filePath = "test_feedback.txt";
    CreateFeedback(filePath, feedbackMessage);

    // Open the file and read its content
    std::ifstream file(filePath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    // Test expectations
    EXPECT_EQ(buffer.str(), "Great service!\n\n");

    // Clear the file
    ofstream clearFile("test_feedback.txt", std::ios::trunc);
    clearFile.close();
}

/**
 * @brief Test case for displaying the content of a file.
 */
TEST_F(VehicleTests, TestDisplayFileContent) {
    // Test data
    std::string filePath = "test_content.txt";
    std::ofstream testFile(filePath);
    testFile << "Line 1\nLine 2\nLine 3";
    testFile.close();

    // Test function
    std::string content = DisplayFileContent(filePath);

    // Test expectations
    EXPECT_EQ(content, "Line 1\nLine 2\nLine 3\n");
}

/**
 * @brief Test case for writing and loading parts from a file.
 */

TEST_F(VehicleTests, TestWriteLoadPartsFromFile) {
    // Test data
    Part part{ 1, "TestPart", "TestModel", 500.0 };
    Part part2{ 2, "TestPart2", "TestModel2", 700.0 };
    partHash[to_string(part.partID)] = part;

    // Test functions
    writePartToFile(part, "test_parts.txt");
    partHash.clear();
    loadPartsFromFile("test_parts.txt");

    // Test expectations
    EXPECT_EQ(part.price, 500.0);
    EXPECT_EQ(part2.price, 700.0);
}

/**
 * @brief Test case for updating parts in a file.
 */

TEST_F(VehicleTests, TestUpdateVehicleFile) {
    // Assuming there are valid vehicles in the linked list
    Vehicle* vehicle1 = new Vehicle{ 1, 1001, "John Doe", "ModelX", "ABC123", nullptr, nullptr };
    Vehicle* vehicle2 = new Vehicle{ 2, 1002, "Jane Doe", "ModelY", "XYZ456", nullptr, nullptr };

    // Add vehicles to the linked list
    headVehicle = vehicle1;
    vehicle1->next = vehicle2;
    vehicle2->prev = vehicle1;

    // Write vehicles to the file
    writeVehicleToFile(vehicle1, "test_update_vehicle.dat");
    writeVehicleToFile(vehicle2, "test_update_vehicle.dat");

    // Update the file
    updateVehicleFile("test_temp_update_vehicle.dat","test_update_vehicle.dat");

    // Load vehicles from the updated file
    loadVehiclesFromFile("test_update_vehicle.dat");

    // Check if vehicles are correctly loaded
    Vehicle* readVehicle1 = headVehicle;
    Vehicle* readVehicle2 = headVehicle->next;

    EXPECT_EQ(readVehicle1->vehicleID, 1);
    EXPECT_EQ(readVehicle1->customerID, 1001);
    EXPECT_EQ(readVehicle1->model, "ModelX");
    EXPECT_EQ(readVehicle1->plateNumber, "ABC123");

    EXPECT_EQ(readVehicle2->vehicleID, 2);
    EXPECT_EQ(readVehicle2->customerID, 1002);
    EXPECT_EQ(readVehicle2->model, "ModelY");
    EXPECT_EQ(readVehicle2->plateNumber, "XYZ456");

    // Clean up
    delete vehicle1;
    delete vehicle2;
}

/**
 * @brief Test case for writing and loading vehicles from a file.
 */
TEST_F(VehicleTests, TestWriteLoadVehiclesFromFile) {
    // Test data
    Vehicle* vehicle1 = new Vehicle{ 1, 1001, "John Doe", "ModelX", "ABC123", nullptr, nullptr };
    writeVehicleToFile(vehicle1, "test_vehicle.txt");

    // Test function
    loadVehiclesFromFile("test_vehicle.txt");

    // Test expectations
    EXPECT_EQ(headVehicle->vehicleID, 1);
    EXPECT_EQ(headVehicle->customerID, 1001);
    EXPECT_EQ(headVehicle->model, "ModelX");
    EXPECT_EQ(headVehicle->plateNumber, "ABC123");

    // Clean up memory
    delete vehicle1;
}
/**
 * @brief Test case for counting vehicles in the linked list.
 */
TEST_F(VehicleTests, TestCountVehicles) {
    // Test data
    Vehicle* vehicle1 = new Vehicle{ 1, 1001, "John Doe", "ModelX", "ABC123", nullptr, nullptr };
    Vehicle* vehicle2 = new Vehicle{ 2, 1002, "Jane Doe", "ModelY", "XYZ456", nullptr, nullptr };

    // Set up linked list
    headVehicle = vehicle1;
    vehicle1->next = vehicle2;
    vehicle2->prev = vehicle1;

    // Test function
    int count = CountVehicles();

    // Test expectations
    EXPECT_EQ(count, 2);

    // Clean up memory
    delete vehicle1;
    delete vehicle2;
}

/**
 * @brief The main function of the test program.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument strings.
 * @return int The exit status of the program.
 */
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
