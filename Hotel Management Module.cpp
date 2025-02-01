#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <limits>
using namespace std;

struct Room
{
    string Type;
    int Price;
    int Quantity;
    string Description;
};

struct Hotel
{
    string Name;
    string Address;
    long long int Mob;
    int FP; // Food Price
    vector<Room> rooms;
};

struct User
{
    int UserID = 1000;
    std::string Name;
    long long int Adhar;
    long long int Mob;
    char Food_option;
    std::string Bed_option;
    int Days;
    std::string Date;
    double Totalprice;
};

vector<Room> rooms;
vector<Hotel> hotels;

void SaveHotelDetails(const Hotel &hotel);
void LoadHotels();
void AddHotel();
void DeleteHotel();
void ViewHotelDetails(const Hotel &hotel);
void HotelDashboard();
void UserDashboard();
int InitializeUserID();
void SaveUserID(int userID);
void Booking();
void DisplayAdditionalRoomDetails();
void SortRooms();
void FilterRoomsByType();
void DisplayAllUsers();
void ApproveRoom();
void CheckBooking();
void CancleBooking();
void SearchUser();
void CheckCancelList();
void AvailableRooms();
void ProvideFeedback();
void InitializeUserIDFile();
void UpdateUserDetails(int userID);

struct Price
{
    double DisplayPrice(const string &roomType, char foodOption, int days, int foodPrice)
    {
        double price = 0;

        // Find the room price based on room type
        for (const Hotel &hotel : hotels)
        {
            for (const Room &room : hotel.rooms)
            {
                if (room.Type == roomType)
                {
                    price += room.Price;
                    break;
                }
            }
        }

        // Calculate total price for the number of days
        price *= days;

        // Add food service price if applicable
        if (foodOption == 'y' || foodOption == 'Y')
        {
            price += foodPrice * days;
        }

        return price;
    }
};

void HotelDashboard()
{
    int option;

    do
    {
        cout << "------------------------------------------------\n";
        cout << "                Hotel Dashboard                 \n";
        cout << "------------------------------------------------\n\n";
        cout << "1. Add Hotel\n";
        cout << "2. Delete Hotel\n";
        cout << "3. View Hotel Details\n";
        cout << "4. Exit\n";
        cout << "Select an option: ";
        cin >> option;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (option)
        {
        case 1:
            AddHotel();
            break;
        case 2:
            DeleteHotel();
            break;
        case 3:
            if (hotels.empty())
            {
                cout << "No hotels available.\n";
            }
            else
            {
                cout << "Available Hotels:\n";
                for (size_t i = 0; i < hotels.size(); ++i)
                {
                    cout << i + 1 << ". " << hotels[i].Name << endl;
                }
                int hotelOption;
                cout << "Select a hotel to view details: ";
                cin >> hotelOption;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (hotelOption > 0 && hotelOption <= hotels.size())
                {
                    ViewHotelDetails(hotels[hotelOption - 1]);
                }
                else
                {
                    cout << "Invalid option.\n";
                }
            }
            break;
        case 4:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid option. Please select a valid option.\n";
        }
    } while (option != 4);
}

void SaveHotelDetails(const Hotel &hotel)
{
    ofstream file("HotelDetails.txt", ios::app);
    if (file.is_open())
    {
        file << hotel.Name << "\n";
        file << hotel.Address << "\n";
        file << hotel.Mob << "\n";
        file << hotel.FP << "\n";

        for (const Room &room : hotel.rooms)
        {
            file << room.Type << "\n"
                 << room.Price << "\n"
                 << room.Quantity << "\n";
            file << room.Description << "\n";
        }
        file << "END_HOTEL\n"; // Add delimiter to separate hotels
        file.close();
    }
    else
    {
        cout << "Error opening file to save hotel details." << endl;
    }
}

void FilterHotelsByPlaceAndPrice(const string &place, int maxPrice)
{
    vector<Hotel> filteredHotels;
    for (const Hotel &hotel : hotels)
    {
        if (hotel.Address.find(place) != string::npos)
        {
            for (const Room &room : hotel.rooms)
            {
                if (room.Price <= maxPrice)
                {
                    filteredHotels.push_back(hotel);
                    break;
                }
            }
        }
    }

    if (filteredHotels.empty())
    {
        cout << "No hotels found matching the criteria.\n";
    }
    else
    {
        cout << "Filtered Hotels:\n";
        for (const Hotel &hotel : filteredHotels)
        {
            cout << "Hotel Name: " << hotel.Name << "\n";
            cout << "Address: " << hotel.Address << "\n";
            cout << "Customer Care Mob No.: " << hotel.Mob << "\n";
            cout << "Food Service Price: " << hotel.FP << "\n";
            for (const Room &room : hotel.rooms)
            {
                cout << "Room Type: " << room.Type << " | Price: " << room.Price << " | Quantity: " << room.Quantity << " | Description: " << room.Description << "\n";
            }
            cout << "------------------------------------------------\n";
        }
    }
}

void LoadHotels()
{
    ifstream file("HotelDetails.txt");
    if (!file.is_open())
    {
        cout << "Error opening HotelDetails.txt" << endl;
        return;
    }

    hotels.clear();
    while (!file.eof())
    {
        Hotel hotel;
        getline(file, hotel.Name);
        if (hotel.Name.empty())
            break;
        getline(file, hotel.Address);
        file >> hotel.Mob;
        file >> hotel.FP;
        file.ignore();

        hotel.rooms.clear();
        while (true)
        {
            Room room;
            if (!getline(file, room.Type) || room.Type.empty())
                break;
            file >> room.Price;
            file >> room.Quantity;
            file.ignore();
            getline(file, room.Description);
            hotel.rooms.push_back(room);
        }
        hotels.push_back(hotel);
        string endMarker;
        getline(file, endMarker); // Read the "END_HOTEL" marker
    }
    file.close();
}

void AddHotel()
{
    Hotel hotel;
    int roomCount;

    cout << "Enter Hotel Name: ";
    getline(cin, hotel.Name);
    cout << "Enter Address: ";
    getline(cin, hotel.Address);
    cout << "Enter Customer Care Mob No.: ";
    cin >> hotel.Mob;
    if (to_string(hotel.Mob).length() == 10)
    {
        cout << " Mob No.: " << hotel.Mob<< endl;
    }
    else
    {
        cout << " Mob No.: Invalid (not a 10-digit number)" << endl;
    }
    cin.ignore();
    cout << "Enter number of room types: ";
    cin >> roomCount;
    cin.ignore();

    while (roomCount <= 0)
    {
        cout << "Please enter a valid number of room types: ";
        cin >> roomCount;
        cin.ignore();
    }

    for (int i = 0; i < roomCount; ++i)
    {
        Room room;
        cout << "Enter room type (e.g., Deluxe, Suite): ";
        getline(cin, room.Type);
        cout << "Enter price for " << room.Type << " room: ";
        cin >> room.Price;
        while (room.Price <= 0)
        {
            cout << "Invalid price. Please re-enter the price for " << room.Type << " room: ";
            cin >> room.Price;
        }
        cout << "Enter quantity for " << room.Type << " room: ";
        cin >> room.Quantity;
        while (room.Quantity <= 0)
        {
            cout << "Invalid quantity. Please re-enter the quantity for " << room.Type << " room: ";
            cin >> room.Quantity;
        }
        cin.ignore();
        cout << "Enter description for " << room.Type << " room: ";
        getline(cin, room.Description);
        hotel.rooms.push_back(room);
    }

    cout << "Enter Food Service Price: ";
    cin >> hotel.FP;
    cin.ignore();

    hotels.push_back(hotel);
    SaveHotelDetails(hotel);

    cout << "\n\nData Saved Successfully!" << endl;
}

void DeleteHotel()
{
    if (hotels.empty())
    {
        cout << "No hotels available to delete.\n";
        return;
    }

    cout << "Available Hotels:\n";
    for (size_t i = 0; i < hotels.size(); i++)
    {
        cout << i + 1 << ". " << hotels[i].Name << endl;
    }

    int hotelOption;
    cout << "Select a hotel to delete: ";
    cin >> hotelOption;
    cin.ignore();

    if (hotelOption > 0 && hotelOption <= hotels.size())
    {
        hotels.erase(hotels.begin() + (hotelOption - 1));
        ofstream file("HotelDetails.txt", ios::trunc);
        file.close();
        for (const Hotel &hotel : hotels)
        {
            SaveHotelDetails(hotel);
        }
        cout << "Hotel deleted successfully.\n";
    }
    else
    {
        cout << "Invalid option.\n";
    }
}

void ViewHotelDetails(const Hotel &hotel)
{
    cout << "Hotel Name: " << hotel.Name << endl;
    cout << "Address: " << hotel.Address << endl;
    cout << "Customer Care Mob No.: " << hotel.Mob << endl;
    cout << "Food Service Price: " << hotel.FP << endl;

    for (const Room &room : hotel.rooms)
    {
        cout << "Room Type: " << room.Type << " | Available: " << room.Quantity << " | Price: " << room.Price << " | Description: " << room.Description << endl;
    }
    cout << "------------------------------------------------" << endl;
}

int InitializeUserID()
{
    ifstream file("UserID.txt");
    int userID;
    if (file.is_open())
    {
        file >> userID;
        file.close();
    }
    return userID;
}

void SaveUserID(int userID)
{
    ofstream file("UserID.txt", ios::trunc); // Open file in truncate mode to overwrite
    if (file.is_open())
    {
        file << userID << endl;
        file.close();
    }
}

void InitializeUserIDFile()
{
    ifstream file("UserID.txt");
    if (!file.is_open())
    {
        ofstream outFile("UserID.txt");
        if (outFile.is_open())
        {
            outFile << 1000 << endl;
            outFile.close();
        }
    }
}

bool isValidDate(const string &date)
{
    if (date.length() != 10)
        return false;
    if (date[4] != '-' || date[7] != '-')
        return false;
    for (size_t i = 0; i < date.length(); ++i)
    {
        if (i == 4 || i == 7)
            continue;
        if (!isdigit(date[i]))
            return false;
    }
    return true;
}

void Booking()
{
    if (hotels.empty())
    {
        cout << "No hotels available for booking.\n";
        return;
    }

    cout << "Available Hotels:\n";
    for (size_t i = 0; i < hotels.size(); ++i)
    {
        cout << i + 1 << ". " << hotels[i].Name << endl;
    }

    int hotelOption;
    cout << "Select a hotel to book: ";
    cin >> hotelOption;
    cin.ignore();

    if (hotelOption <= 0 || hotelOption > hotels.size())
    {
        cout << "Invalid hotel selection.\n";
        return;
    }

    Hotel &selectedHotel = hotels[hotelOption - 1];

    InitializeUserIDFile(); // Ensure the UserID file is initialized
    int userID = InitializeUserID();
    User user;
    Price price;

    user.UserID = userID;

    cout << "Enter Your Name: ";
    cin.ignore();
    getline(cin, user.Name);
    cout << "Enter Your Aadhaar No.: ";
    cin >> user.Adhar;
    while (to_string(user.Adhar).length() != 12)
    {
        cout << "Invalid Aadhaar No. Please enter a 12-digit Aadhaar number: ";
        cin >> user.Adhar;
    }
    cout << "Enter Mob No.: ";
    cin >> user.Mob;
    while (to_string(user.Mob).length() != 10)
    {
        cout << "Invalid Mob No. Please enter a 10-digit mobile number: ";
        cin >> user.Mob;
    }
    cout << "Food Service (Y/N): ";
    cin >> user.Food_option;

    while (user.Food_option != 'Y' && user.Food_option != 'y' && user.Food_option != 'N' && user.Food_option != 'n')
    {
        cout << "Invalid input. Please enter 'Y' or 'N' for Food Service: ";
        cin >> user.Food_option;
    }

    cout << "Enter Room Type (e.g., Deluxe, Suite): ";
    cin.ignore();
    getline(cin, user.Bed_option);

    bool validRoomType = false;
    for (const Room &room : selectedHotel.rooms)
    {
        if (room.Type == user.Bed_option)
        {
            validRoomType = true;
            break;
        }
    }
    while (!validRoomType)
    {
        cout << "Invalid Room Type. Please enter a valid room type (e.g., Deluxe, Suite): ";
        getline(cin, user.Bed_option);
        for (const Room &room : selectedHotel.rooms)
        {
            if (room.Type == user.Bed_option)
            {
                validRoomType = true;
                break;
            }
        }
    }

    cout << "Enter Days to Stay: ";
    cin >> user.Days;
    cout << "Enter Booking Date (YYYY-MM-DD): ";
    cin.ignore();
    getline(cin, user.Date);

    while (!isValidDate(user.Date))
    {
        cout << "Invalid date format. Please enter the date in YYYY-MM-DD format: ";
        getline(cin, user.Date);
    }

    user.Totalprice = price.DisplayPrice(user.Bed_option, user.Food_option, user.Days, selectedHotel.FP);
    cout << "Total Price: " << user.Totalprice << endl;

    bool roomUpdated = false;
    for (Room &room : selectedHotel.rooms)
    {
        if (room.Type == user.Bed_option)
        {
            if (room.Quantity > 0)
            {
                room.Quantity--;
                roomUpdated = true;
            }
            else
            {
                cout << "Sorry, no " << room.Type << " rooms available." << endl;
                return;
            }
        }
    }

    if (!roomUpdated)
    {
        cout << "Room type not found. Please try again." << endl;
        return;
    }

    ofstream roomFile("Room.txt", ios::app);
    for (const Room &room : selectedHotel.rooms)
    {
        roomFile << room.Type << "\n"
                 << room.Quantity << "\n";
    }
    roomFile.close();

    ofstream file("UserData.txt", ios::app);
    file << user.UserID << "\n";
    file << user.Name << "\n";
    file << user.Adhar << "\n";
    file << user.Mob << "\n";
    file << user.Food_option << "\n";
    file << user.Bed_option << "\n";
    file << user.Days << "\n";
    file << user.Date << "\n";
    file << user.Totalprice << "\n";
    file.close();

    cout << "Booking completed successfully!\n";
    cout << "\nBooking Summary:\n";
    cout << "Hotel: " << selectedHotel.Name << "\n";
    cout << "Room Type: " << user.Bed_option << "\n";
    cout << "Total Price: " << user.Totalprice << "\n";
    cout << "Days: " << user.Days << "\n";
    cout << "Booking Date (yyyy-mm-dd): " << user.Date << "\n";
    cout << "Your User ID: " << user.UserID << "\n\n";
    cout << "Please keep your User ID safe for future reference.\n";
    cout << "If you have any questions, feel free to contact our customer care at " << selectedHotel.Mob << ".\n";
    cout << "Additional Information: \n";
    cout << "1. Check-in time: 2 PM\n";
    cout << "2. Check-out time: 12 PM\n";
    cout << "3. Breakfast is complimentary\n";
    cout << "4. Free cancellation within 24 hours of booking\n";
    cout << "5. For any issues, contact: support@hotelbooking.com\n";
    cout << "6. Enjoy your stay at " << selectedHotel.Name << "!\n";

    SaveUserID(userID + 1); // Save the incremented UserID
}

void DisplayAllUsers()
{
    ifstream file("UserData.txt", ios::in);
    if (!file.is_open())
    {
        cout << "Error opening UserData.txt" << endl;
        return;
    }

    User user;
    int n = 1;
    while (file >> user.UserID)
    {
        file.ignore();
        getline(file, user.Name);
        file >> user.Adhar;
        file >> user.Mob;
        file >> user.Food_option;
        file.ignore();
        getline(file, user.Bed_option);
        file >> user.Days;
        file.ignore();
        getline(file, user.Date);
        file >> user.Totalprice;

        cout << "\n Customer -> " << n << "\n";
        cout << "\tID: " << user.UserID << endl;
        cout << "\tName: " << user.Name << endl;
        cout << "\tAadhaar No.: " << user.Adhar << endl;
        cout << "\tMob No.: " << user.Mob << endl;
        cout << "\tFood Option: " << user.Food_option << endl;
        cout << "\tRoom Type: " << user.Bed_option << endl;
        cout << "\tDays: " << user.Days << endl;
        cout << "\tBooking Date: " << user.Date << endl;
        cout << "\tTotal Price: " << user.Totalprice << endl;

        n++;
    }
    file.close();

    cout << "Total Users Displayed: " << n - 1 << endl;
}

void ApproveRoom()
{
    int UserID;
    cout << "\nEnter Customer ID: ";
    cin >> UserID;

    ifstream file;
    User user;
    bool found = false;

    file.open("UserData.txt", ios::in);
    if (!file.good())
    {
        cout << " No Data Available... ";
        return;
    }
    while (file >> user.UserID >> user.Name >> user.Adhar >> user.Mob >> user.Food_option >> user.Bed_option >> user.Days >> user.Date >> user.Totalprice)
    {
        file.ignore();
        if (user.UserID == UserID)
        {
            found = true;
            cout << "\n\n-----------------------------------------------------------------";
            cout << "\n                             RECORD                                ";
            std::cout << "\n\n------------------------------------------------------------------\n\n";

            cout << "Customer Information - \n\n";
            cout << " Name: " << user.Name << endl;
            cout << " Aadhaar No.: " << user.Adhar << endl;
            if (to_string(user.Mob).length() == 10)
            {
                cout << " Mob No.: " << user.Mob << endl;
            }
            else
            {
                cout << " Mob No.: Invalid (not a 10-digit number)" << endl;
            }
            cout << " Food Option: " << user.Food_option << std::endl;
            cout << " Room Type: " << user.Bed_option << endl;
            cout << " Days: " << user.Days << endl;
            cout << " Booking Date: " << user.Date << endl;
            cout << " Total Price: " << user.Totalprice << endl;

            ofstream File;
            File.open("Allotment.txt", ios::app);
            if (!File.is_open())
            {
                cout << "\n\nError opening file. " << endl;
                return;
            }

            int RoomNo;
            cout << "\nEnter Room No. to Allot: ";
            cin >> RoomNo;

            while (RoomNo <= 0)
            {
                cout << "Invalid Room No. Please enter a valid room number: ";
                cin >> RoomNo;
            }

            cout << "Room Allotted Successfully! " << endl;

            File << RoomNo << "\n"
                 << user.UserID << "\n"
                 << user.Name << "\n"
                 << user.Totalprice << "\n"
                 << user.Bed_option << "\n";
            File.close();
            break;
        }
    }

    if (!found)
    {
        cout << "\n\nRecord with User ID " << UserID << " not found." << endl;
    }

    file.close();
}

void CheckBooking()
{
    User user;
    ifstream file("UserData.txt", ios::in);

    int userID;
    cout << "Enter Your User ID : ";
    cin >> userID;

    bool found = false;
    while (file >> user.UserID)
    {
        file.ignore();
        getline(file, user.Name);
        file >> user.Adhar;
        file >> user.Mob;
        file >> user.Food_option;
        file.ignore();
        getline(file, user.Bed_option);
        file >> user.Days;
        file.ignore();
        getline(file, user.Date);
        file >> user.Totalprice;

        if (user.UserID == userID)
        {
            found = true;
            cout << "Great! Your Room is allotted.\n"
                 << endl;
            cout << "Room Type: " << user.Bed_option << endl;
            cout << "Total Price: " << user.Totalprice << endl;
            break;
        }
    }

    if (!found)
    {
        cout << "Invalid User ID or Your Application is pending.\n";
    }

    file.close();
}

void SearchUser()
{
    int UserID;
    cout << "\nEnter Customer ID: ";
    cin >> UserID;

    ifstream file("UserData.txt");
    User user;
    bool found = false;

    if (!file.is_open())
    {
        cout << " No Data Available... ";
        return;
    }

    while (file >> user.UserID >> user.Name >> user.Adhar >> user.Mob >> user.Food_option >> user.Bed_option >> user.Days >> user.Date >> user.Totalprice)
    {
        if (user.UserID == UserID)
        {
            found = true;
            cout << "\n\n-----------------------------------------------------------------------------------";
            cout << "\n                                        RECORD                                       ";
            cout << "\n\n-----------------------------------------------------------------------------------\n\n";

            cout << "Customer Information - \n\n";
            cout << " Name: " << user.Name << endl;
            cout << " Aadhaar No.: " << user.Adhar << endl;
            cout << " Mob No.: " << user.Mob << endl;
            cout << " Food Option: " << user.Food_option << endl;
            cout << " Room Type: " << user.Bed_option << endl;
            cout << " Days: " << user.Days << endl;
            cout << " Booking Date: " << user.Date << endl;
            cout << " Total Price: " << user.Totalprice << endl;
        }
    }

    if (!found)
    {
        cout << "\n\nRecord with User ID " << UserID << " not found." << endl;
    }

    file.close();
}

void CheckCancelList()
{
    ifstream file("CancelData.txt", ios::in);
    if (!file.good())
    {
        cout << "No Data Available... ";
        return;
    }

    User user;
    int RoomNo;
    char Cancel[7];
    string reason;
    double cancelFee;

    int n = 1;

    while (!file.eof())
    {
        file >> RoomNo;
        file >> user.UserID;
        file >> user.Name;
        file >> Cancel;
        file >> reason;
        file >> cancelFee;

        cout << "\nCustomer -> " << n << "\n";
        cout << "\tRoom No.: " << RoomNo << endl;
        cout << "\tID: " << user.UserID << endl;
        cout << "\tName: " << user.Name << endl;
        cout << "\tCancelled: " << Cancel << endl;
        cout << "\tReason: " << reason << endl;
        cout << "\tCancellation Fee: " << cancelFee << endl;

        // Additional details
        cout << "\tCancellation Date: " << __DATE__ << endl; // Adding current date as cancellation date
        cout << "\tCancellation Time: " << __TIME__ << endl; // Adding current time as cancellation time
        cout << "\tProcessed by: System Admin" << endl;      // Adding a placeholder for processed by

        n++;
    }
    file.close();

    cout << "\nTotal number of cancellations: " << n - 1 << endl;
    cout << "End of cancellation list.\n"; // Adding end of list message
}

void AvailableRooms()
{
    ifstream file;
    file.open("Room.txt", ios::in);

    if (!file.is_open())
    {
        cout << "No Data Available..." << endl;
        return;
    }
    else
    {
        rooms.clear();

        while (!file.eof())
        {
            Room room;
            file >> room.Type >> room.Quantity;
            rooms.push_back(room);
        }
        file.close();

        if (rooms.empty())
        {
            cout << "No rooms available at the moment." << endl;
            return;
        }

        cout << "Available Rooms:\n\n";
        for (const Room &room : rooms)
        {
            cout << "Room Type: " << room.Type << " | Available: " << room.Quantity << endl;
        }

        DisplayAdditionalRoomDetails();
        SortRooms();

        cout << "\nSorted Available Rooms:\n\n";
        for (const Room &room : rooms)
        {
            cout << "Room Type: " << room.Type << " | Available: " << room.Quantity << endl;
        }

        FilterRoomsByType();
    }
}

void DisplayAdditionalRoomDetails()
{
    cout << "\nRoom Details:\n";
    for (const Room &room : rooms)
    {
        cout << "Type: " << room.Type << endl;
        cout << "Quantity: " << room.Quantity << endl;
        cout << "---------------------------------" << endl;
    }
}

void SortRooms()
{
    sort(rooms.begin(), rooms.end(), [](const Room &a, const Room &b)
         {
        if (a.Type != b.Type)
        {
            return a.Type < b.Type;
        }
        return a.Quantity < b.Quantity; });
}

void FilterRoomsByType()
{
    string filterType;
    cout << "\nEnter room type to filter (or 'all' to show all rooms): ";
    getline(cin, filterType);

    if (filterType == "all")
    {
        cout << "\nAll Available Rooms:\n\n";
        for (const Room &room : rooms)
        {
            cout << "Room Type: " << room.Type << " | Available: " << room.Quantity << endl;
        }
    }
    else
    {
        cout << "\nFiltered Available Rooms for type '" << filterType << "':\n\n";
        for (const Room &room : rooms)
        {
            if (room.Type == filterType)
            {
                cout << "Room Type: " << room.Type << " | Available: " << room.Quantity << endl;
            }
        }
    }
}

void CancleBooking()
{
    int userID;
    cout << "Enter your User ID to cancel booking: ";
    cin >> userID;

    ifstream userFile("UserData.txt");
    ofstream tempUserFile("TempUserData.txt");
    bool found = false;
    User user;

    // Remove booking from UserData.txt
    while (userFile >> user.UserID >> user.Name >> user.Adhar >> user.Mob >> user.Food_option >> user.Bed_option >> user.Days >> user.Date >> user.Totalprice)
    {
        if (user.UserID == userID)
        {
            found = true;
            cout << "Booking found and cancelled for User ID: " << userID << endl;
        }
        else
        {
            tempUserFile << user.UserID << "\n"
                         << user.Name << "\n"
                         << user.Adhar << "\n"
                         << user.Mob << "\n"
                         << user.Food_option << "\n"
                         << user.Bed_option << "\n"
                         << user.Days << "\n"
                         << user.Date << "\n"
                         << user.Totalprice << "\n";
        }
    }

    userFile.close();
    tempUserFile.close();

    remove("UserData.txt");
    rename("TempUserData.txt", "UserData.txt");

    if (!found)
    {
        cout << "No booking found for User ID: " << userID << endl;
        return;
    }

    // Remove booking from Allotment.txt
    ifstream allotmentFile("Allotment.txt");
    ofstream tempAllotmentFile("TempAllotment.txt");
    int roomNo;
    while (allotmentFile >> roomNo >> user.UserID >> user.Name >> user.Totalprice >> user.Bed_option)
    {
        if (user.UserID != userID)
        {
            tempAllotmentFile << roomNo << "\n"
                              << user.UserID << "\n"
                              << user.Name << "\n"
                              << user.Totalprice << "\n"
                              << user.Bed_option << "\n";
        }
    }

    allotmentFile.close();
    tempAllotmentFile.close();

    remove("Allotment.txt");
    rename("TempAllotment.txt", "Allotment.txt");

    // Add cancelled booking to CancelData.txt
    ofstream cancelFile("CancelData.txt", ios::app);
    if (cancelFile.is_open())
    {
        cancelFile << roomNo << "\n"
                   << user.UserID << "\n"
                   << user.Name << "\n"
                   << "Cancelled" << "\n"
                   << "User requested cancellation" << "\n"
                   << 0.0 << "\n";
        cancelFile.close();
    }
    else
    {
        cout << "Unable to open file to save cancellation details." << endl;
    }

    cout << "Booking successfully cancelled and updated" << endl;
}

void UserDashboard()
{
    int option1;
    do
    {
        cout << "------------------------------------------------\n";
        cout << "                USER DASHBOARD                  \n";
        cout << "------------------------------------------------\n\n";
        cout << "1. View Hotel Details\n";
        cout << "2. Available Rooms\n";
        cout << "3. Book Here!\n";
        cout << "4. Check Status of Booking\n";
        cout << "5. Cancel Your Booking\n";
        cout << "6. Provide Feedback\n";
        cout << "7. Search Hotel by Place or Price\n";
        cout << "8. Exit\n";
        cout << "Select an option: ";
        cin >> option1;
        cin.ignore();

        switch (option1)
        {
        case 1:
            if (hotels.empty())
            {
                cout << "No hotels available.\n";
            }
            else
            {
                cout << "Available Hotels:\n";
                for (size_t i = 0; i < hotels.size(); i++)
                {
                    cout << i + 1 << ". " << hotels[i].Name << endl;
                }
                int hotelOption;
                cout << "Select a hotel to view details: ";
                cin >> hotelOption;
                cin.ignore();
                if (hotelOption > 0 && hotelOption <= hotels.size())
                {
                    ViewHotelDetails(hotels[hotelOption - 1]);
                }
                else
                {
                    cout << "Invalid option.\n";
                }
            }
            break;
        case 2:
            AvailableRooms();
            break;
        case 3:
            Booking();
            break;
        case 4:
            CheckBooking();
            break;
        case 5:
            CancleBooking();
            break;
        case 6:
            ProvideFeedback();
            break;
        case 7:
        {
            string place;
            int maxPrice;
            cout << "Enter place to search hotels: ";
            getline(cin, place);
            cout << "Enter maximum price: ";
            cin >> maxPrice;
            cin.ignore();
            FilterHotelsByPlaceAndPrice(place, maxPrice);
            break;
        }
        case 8:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid option. Please select a valid option.\n";
        }
    } while (option1 != 8);
}

void ProvideFeedback()
{
    string feedback;
    cout << "Please provide your feedback: ";
    cin.ignore();
    getline(cin, feedback);
    ofstream feedbackFile("feedback.txt", ios::app);
    if (feedbackFile.is_open())
    {
        feedbackFile << feedback << endl;
        feedbackFile.close();
        cout << "Thank you for your feedback!" << endl;
    }
    else
    {
        cout << "Unable to open file to save feedback." << endl;
    }
}

void UserManagement()
{
    int option;
    do
    {
        cout << "------------------------------------------------\n";
        cout << "                     Admin                       \n";
        cout << "------------------------------------------------\n\n";
        cout << "1. Display All Users\n";
        cout << "2. Search User\n";
        cout << "3. Update User Details\n";
        cout << "4. Exit\n";
        cout << "Select an option: ";
        cin >> option;
        cin.ignore();

        switch (option)
        {
        case 1:
            DisplayAllUsers();
            break;
        case 2:
            SearchUser();
            break;
        case 3:
        {
            int userID;
            cout << "Enter User ID to update details: ";
            cin >> userID;
            UpdateUserDetails(userID);
            break;
        }
        case 4:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid option. Please select a valid option.\n";
        }
    } while (option != 4);
}

void UpdateUserDetails(int userID)
{
    ifstream file("UserData.txt");
    ofstream tempFile("TempUserData.txt");
    bool found = false;
    User user;

    while (file >> user.UserID)
    {
        file.ignore();
        getline(file, user.Name);
        file >> user.Adhar;
        file >> user.Mob;
        file >> user.Food_option;
        file.ignore();
        getline(file, user.Bed_option);
        file >> user.Days;
        file.ignore();
        getline(file, user.Date);
        file >> user.Totalprice;

        if (user.UserID == userID)
        {
            found = true;
            cout << "Enter new details -\n";
            cout << "Name: ";
            cin.ignore();
            getline(cin, user.Name);
            cout << "Aadhaar No.: ";
            cin >> user.Adhar;
            cout << "Mob No.: ";
            cin >> user.Mob;
            cout << "Food Option (Y/N): ";
            cin >> user.Food_option;
            cout << "Room Type: ";
            cin.ignore();
            getline(cin, user.Bed_option);
            cout << "Days: ";
            cin >> user.Days;
            cout << "Booking Date (YYYY-MM-DD): ";
            cin.ignore();
            getline(cin, user.Date);
            cout << "Total Price: ";
            cin >> user.Totalprice;
        }

        tempFile << user.UserID << "\n";
        tempFile << user.Name << "\n";
        tempFile << user.Adhar << "\n";
        tempFile << user.Mob << "\n";
        tempFile << user.Food_option << "\n";
        tempFile << user.Bed_option << "\n";
        tempFile << user.Days << "\n";
        tempFile << user.Date << "\n";
        tempFile << user.Totalprice << "\n";
    }

    file.close();
    tempFile.close();

    remove("UserData.txt");
    rename("TempUserData.txt", "UserData.txt");

    if (found)
    {
        cout << "User details updated successfully!\n";
    }
    else
    {
        cout << "User ID not found.\n";
    }
}

int main()
{
    InitializeUserIDFile();
    LoadHotels();
    int option;

    do
    {
        cout << "------------------------------------------------\n";
        cout << "            Hotel Booking Application           \n";
        cout << "------------------------------------------------\n\n";
        cout << "1. Hotel\n";
        cout << "2. User\n";
        cout << "3. Admin\n";
        cout << "4. Exit\n";
        cout << "Select an option: ";
        cin >> option;
        cin.ignore();

        switch (option)
        {
        case 1:
            HotelDashboard();
            break;
        case 2:
            UserDashboard();
            break;
        case 3:
            UserManagement();
            break;
        case 4:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid option. Please select a valid option.\n";
        }
    } while (option != 4);

    return 0;
}
