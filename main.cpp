#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <string>
#include <sstream>
#include <vector>
#include <conio.h>

using namespace std;

// template <typename T>

class Cityobj
{
protected:
    string name;

public:
    Cityobj(const string &n) : name(n) {}
    virtual ~Cityobj() = default;
};

class Building : public Cityobj
{
protected:
    int capacity;

public:
    Building(const string &n, int c) : Cityobj(n), capacity(c) {}
    virtual void upgrade() {}
};

class ResidentialBuilding : public Building
{
public:
    ResidentialBuilding(const string &n, int c) : Building(n, c) {}
    void upgrade() override
    {
        int temp;
        cout << " what is the new capaciity? :";
        cin >> temp;
        capacity += temp;
        cout << "Upgraded Residential Building: " << name << ", New capacity: " << capacity << endl;
    }
};

class Transport : public Cityobj
{
protected:
    double emissions;

public:
    Transport(const string &n, double e) : Cityobj(n), emissions(e) {}
};

class Bus : public Transport
{
public:
    Bus(const string &n, double e) : Transport(n, e) {}
};

class EV : public Transport
{
public:
    EV(const string &n) : Transport(n, 0.0) {}
};

void showHeader()
{
    system("cls");
    cout << "\033[1;1H " << "\033[1;32m"; // moving the cursor to the top-left corner so thhat the header can be written in place
    cout << R"(
                                                                   CCCCCCCCCCCCC IIIIIIIIIIII TTTTTTTTTTTTTTTTTTTTTTT YYYYYYY       YYYYYYY
                                                                CCC::::::::::::C I::::::::::I T:::::::::::::::::::::T Y:::::Y       Y:::::Y
                                                              CC:::::::::::::::C I::::::::::I T:::::::::::::::::::::T Y:::::Y       Y:::::Y
                                                             C:::::CCCCCCCC::::C II::::::::II T:::::TT:::::::TT:::::T Y::::::Y     Y::::::Y
    eeeeeeeeeeee         cccccccccccccccc    ooooooooooo     C:::::C       CCCCC     I::::I   TTTTTT  T:::::T   TTTTT  Y:::::Y    Y:::::YY
  ee::::::::::::ee     cc:::::::::::::::c  oo:::::::::::oo   C:::::C                 I::::I           T:::::T            Y:::::Y Y:::::Y   
 e::::::eeeee:::::ee  c:::::::::::::::::co :::::::::::::::o  C:::::C                 I::::I           T:::::T             Y:::::Y:::::Y    
e::::::e     e:::::e c:::::::cccccc:::::co :::::ooooo:::::o  C:::::C                 I::::I           T:::::T              Y:::::::::Y     
e:::::::eeeee::::::e c::::::c     ccccccco ::::o     o::::o  C:::::C                 I::::I           T:::::T               Y:::::::Y      
e:::::::::::::::::e  c:::::c             o ::::o     o::::o  C:::::C                 I::::I           T:::::T                Y:::::Y       
e::::::eeeeeeeeeee   c:::::c             o ::::o     o::::o  C:::::C                 I::::I           T:::::T                Y:::::Y       
e:::::::e            c::::::c     ccccccco ::::o     o::::o   C:::::C       CCCCC    I::::I           T:::::T                Y:::::Y       
e::::::::e           c:::::::cccccc:::::co :::::ooooo:::::o    C:::::CCCCCCCC:::: CII:::::III       TT:::::::TT              Y:::::Y       
 e::::::::eeeeeeee    c:::::::::::::::::co :::::::::::::::o     CC::::::::::::::: CI:::::::II       T:::::::::T           YYYY:::::YYYY    
  ee:::::::::::::e     cc:::::::::::::::c  oo:::::::::::oo        CCC:::::::::::: CI:::::::II       T:::::::::T           Y:::::::::::Y    
    eeeeeeeeeeeeee       cccccccccccccccc    ooooooooooo             CCCCCCCCCCCC CIIIIIIIIII       TTTTTTTTTTT           YYYYYYYYYYYYY                                                                                                                    
)";

    cout << "\033[28;1H\033[J" << "\033[0;0m"; // Clears content below the header only.// Sets cursor position to row 4, column 1, below the header.
    cout << endl;
}

void displaymenu() // displaying menu and control the flow of the program
{
    while (true)
    {
        int choice = 1, *choice_ptr = &choice;
        char a;
        do
        {
            showHeader();
            cout << "\nPlease select what operation you want to do:\n";
            cout << "-----------------------------------------\n";
            cout << (choice == 1 ? "\033[1;34m-->\033[0;0m" : "") << " Add a building\n";
            cout << (choice == 2 ? "\033[1;34m-->\033[0;0m" : "") << " Register a transport vehicle\n";
            cout << (choice == 3 ? "\033[1;34m-->\033[0;0m" : "") << " Register an ecoActivity\n";
            cout << (choice == 4 ? "\033[1;34m-->\033[0;0m" : "") << " Watch data analytics\n";
            cout << (choice == 5 ? "\033[1;34m-->\033[0;0m" : "") << " Watch predicted weather of the city\n";
            cout << (choice == 6 ? "\033[1;34m-->\033[0;0m" : "") << " 'Koi marks nahi dun gi' mode (exit)\n";
            cout << "-----------------------------------------\n";
            char a = getch();
            if (a == char(-32))
            {
                a = getch();
                if (a == 'H')
                {
                    if (choice == 1)
                    {
                        choice = 6;
                    }
                    else
                    {
                        choice--;
                    }
                }
                else if (a == 'P')
                {
                    if (choice == 6)
                    {
                        choice = 1;
                    }
                    else
                    {
                        choice++;
                    }
                }
            }
            if (a == char(13))
            {
                break;
            }
        } while (true);

        switch (choice)
        {
        case 1:
            // managerpassword();
            break;
        case 2:
            showHeader();
            // operatorpassword();
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            showHeader();
            cout << "\nThank you for using the ecoCity. Goodbye!\n";
            return;
        default:
            cout << "\nInvalid choice. Please try again.\n";
        }
    }
}

main()
{
    displaymenu();
    return 0;
}