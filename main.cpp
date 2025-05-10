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
    virtual void updateTofile() {}
    virtual void logEmissions() const = 0;
    virtual string toString() const = 0;
    virtual ~Cityobj() = default;
};

class Building : public Cityobj
{
protected:
    int capacity;

public:
    Building(const string &n, int c) : Cityobj(n), capacity(c) {}

    void updateTofile() override
    {
        fstream file("buildings.txt", ios::app);
        if (!file)
        {
            cerr << "Failed to open file!\n";
            return;
        }
        file << name << "-" << capacity << "\n";
        file.close();
        logEmissions();
    }
    void logEmissions() const override
    {
        double cox = 2.0 * capacity / 100.0;
        double nox = 100.0 * capacity / 100.0;
        fstream file("emissions.txt", ios::app);
        if (file)
            file << name << "-" << cox << "-" << nox << "\n";
        file.close();
    }
    string toString() const override
    {
        return "Building: " + name + " with capacity " + to_string(capacity);
    }
};

class Transport : public Cityobj
{
protected:
    bool isBus;
    bool isEV;

public:
    Transport(const string &n, bool bus, bool ev) : Cityobj(n), isBus(bus), isEV(ev) {}
    virtual ~Transport() {}
};

class Combustion : public Transport
{
public:
    Combustion(const string &n, bool bus) : Transport(n, bus, false) {}
    void updateTofile() override
    {

        fstream file("Transport1.txt", ios::app);
        if (!file)
        {
            cerr << "Failed to open file!\n";
            return;
        }
        file << name << "-" << isBus << "-" << "0" << "\n";
        file.close();
        logEmissions();
    }
    void logEmissions() const override
    {
        double cox = isBus ? 5.0 : 0.5;  // ppm
        double nox = isBus ? 1000 : 150; // ppb
        fstream file("emissions.txt", ios::app);
        if (file)
            file << name << "-" << cox << "-" << nox << "\n";
        file.close();
    }

    string toString() const override
    {
        return "Combustion " + string(isBus ? "Bus" : "Car") + ": " + name;
    }
};

class EV : public Transport
{
public:
    EV(const string &n, bool bus) : Transport(n, bus, true) {}
    void updateTofile() override
    {

        fstream file("Transport1.txt", ios::app);
        if (!file)
        {
            cerr << "Failed to open file!\n";
            return;
        }
        file << name << "-" << isBus << "-" << "1" << "\n";
        file.close();
        logEmissions();
    }
    void logEmissions() const override
    {
        double cox = 0.1;
        double nox = 10;
        fstream file("emissions.txt", ios::app);
        if (file)
            file << name << "-" << cox << "-" << nox << "\n";
        file.close();
    }
    string toString() const override
    {
        return "Electric " + string(isBus ? "Bus" : "Car") + ": " + name;
    }
};
template <typename T>
class CityLog
{
    vector<T *> entries;
    string filename;

public:
    CityLog(const string &file) : filename(file) {}

    void add(T *obj)
    {
        entries.push_back(obj);
        obj->updateTofile();
        ofstream file(filename, ios::app);
        if (file)
        {
            file << obj->toString() << "\n"; // Save the object's string representation
        }
        file.close();
    }

    void displayLog() const
    {
        ifstream file(filename);
        if (!file)
        {
            cerr << "Failed to open log file: " << filename << "\n";
            return;
        }
        cout << "\n--- Log from " << filename << " ---\n";
        string line;
        while (getline(file, line))
        {
            cout << line << endl;
        }
        cout << "------------------------------\n";
        file.close();
    }

    ~CityLog()
    {
        for (auto ptr : entries)
            delete ptr;
    }
};
class EcoActivity : public Cityobj
{
private:
    double area;        // In square meters (formula parameter)
    int durationMonths; // Duration of activity

public:
    EcoActivity(const string &n, double a, int d)
        : Cityobj(n), area(a), durationMonths(d) {}

    void updateTofile() override
    {
        fstream file("eco_activities.txt", ios::app);
        if (file)
        {
            file << name << "-" << area << "-" << durationMonths << "\n";
        }
        file.close();
        logEmissions();
    }

    void logEmissions() const override
    {
        // Formula: COx reduction = area * 0.1 * duration
        //          NOx reduction = area * 0.05 * duration
        double cox = -0.1 * area * durationMonths; // Negative for reduction
        double nox = -0.05 * area * durationMonths;

        fstream file("emissions.txt", ios::app);
        if (file)
        {
            file << name << "-" << cox << "-" << nox << "\n";
        }
        file.close();
    }

    string toString() const override
    {
        return "EcoActivity: " + name + " (Area: " + to_string(area) + " m², Duration: " + to_string(durationMonths) + " months)";
    }
};

class CalcEmission
{
public:
    static pair<double, double> calculateTotalEmissions(const string &filename = "emissions.txt")
    {
        ifstream file(filename);
        if (!file)
        {
            cerr << "Failed to open emissions file!\n";
            return {0.0, 0.0};
        }
        string line;
        double totalCOx = 0.0, totalNOx = 0.0;
        while (getline(file, line))
        {
            stringstream ss(line);
            string name, coxStr, noxStr;
            getline(ss, name, '-');
            getline(ss, coxStr, '-');
            getline(ss, noxStr, '-');
            totalCOx += stod(coxStr);
            totalNOx += stod(noxStr);
        }
        file.close();
        cout << "\nTotal COx (ppm): " << totalCOx << "\nTotal NOx (ppb): " << totalNOx << endl;
        return {totalCOx, totalNOx};
    }
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
    CityLog<Cityobj> citylog("city_log.txt");
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
            cout << (choice == 4 ? "\033[1;34m-->\033[0;0m" : "") << " Watch all the logs\n";
            cout << (choice == 5 ? "\033[1;34m-->\033[0;0m" : "") << " Watch data analytics\n";
            cout << (choice == 6 ? "\033[1;34m-->\033[0;0m" : "") << " Watch predicted weather of the city\n";
            cout << (choice == 7 ? "\033[1;34m-->\033[0;0m" : "") << " 'Koi marks nahi dun gi' mode (exit)\n";
            cout << "-----------------------------------------\n";
            char a = getch();
            if (a == char(-32))
            {
                a = getch();
                if (a == 'H')
                {
                    if (choice == 1)
                    {
                        choice = 7;
                    }
                    else
                    {
                        choice--;
                    }
                }
                else if (a == 'P')
                {
                    if (choice == 7)
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
        string name;
        switch (choice)

        {
        case 1:

            int capacity;
            showHeader();
            cout << "\nEnter building name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter building capacity: ";
            cin >> capacity;
            citylog.add(new Building(name, capacity));
            break;
        case 2:
            char type;
            char resp;
            showHeader();
            cout << "\nEnter transport name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Is it a combusyion vehicle (b) or EV (e)? ";
            cin >> type;
            if (type == 'b')
            {

                cout << "is it a bus (b) or a car/ltv (c)?";
                cin >> resp;
                if (resp == 'c')
                    citylog.add(new Combustion(name, false));

                else if (resp == 'b')
                    citylog.add(new Combustion(name, true));
                else
                    cout << "wrong choice";
            }
            else if (type == 'e')
            {
                cout << "is it a bus (b) or a car/ltv (c)?";
                cin >> resp;
                if (resp == 'c')
                    citylog.add(new EV(name, false));

                else if (resp == 'b')
                    citylog.add(new EV(name, true));
                else
                    cout << "wrong choice";
            }
            else
                cout << "Invalid transport type.\n";
            break;
        case 3:
        {
            double area;
            int duration;
            showHeader();
            cout << "\nEnter eco-activity name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter area covered (m²): ";
            cin >> area;
            cout << "Enter duration (months): ";
            cin >> duration;
            citylog.add(new EcoActivity(name, area, duration)); // Formula applied automatically
            break;
        }
        case 4:
            showHeader();
            citylog.displayLog();
            system("pause");
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            showHeader();
            cout << "\nThank you for using the ecoCity. Goodbye!\n";
            return;
        default:
            cout << "\nInvalid choice. Please try again.\n";
        }
    }
}

int main()
{
    displaymenu();
    return 0;
}