#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <string>
#include <sstream>
#include <vector>
#include <conio.h>
#include <algorithm>
#include <numeric>
#include <limits>
#include <ctime>

using namespace std;

struct EmissionData
{
    double cox;
    double nox;
    double temp;
};
struct Point
{
    double x, y;
    int cluster;
};

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
            file << cox << "-" << nox << "\n";
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
        file << name << "," << isBus << "," << "0" << "\n";
        file.close();
        logEmissions();
    }
    void logEmissions() const override
    {
        double cox = isBus ? 5.0 : 0.5;  // ppm
        double nox = isBus ? 1000 : 150; // ppb
        fstream file("emissions.txt", ios::app);
        if (file)
            file << cox << "-" << nox << "\n";
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
            file << cox << "-" << nox << "\n";
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
            file << cox << "-" << nox << "\n";
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
class DataAnalyzer
{
public:
    static vector<EmissionData> loadPollutionData()
    {
        vector<EmissionData> data;
        ifstream file("pollution_data.csv");

        if (!file)
            throw runtime_error("Data file not found");

        string line;
        while (getline(file, line))
        {
            if (line.empty())
                continue;

            stringstream ss(line);
            EmissionData entry;
            char delim;

            if (!(ss >> entry.cox >> delim >> entry.nox >> delim >> entry.temp))
            {
                cerr << "Invalid data format in line: " << line << endl;
                continue;
            }

            data.push_back(entry);
        }

        if (data.empty())
            throw runtime_error("No valid data in file");
        return data;
    }

    static vector<Point> convertToPoints(const vector<EmissionData> &data)
    {
        vector<Point> points;
        for (const auto &entry : data)
        {
            points.push_back({entry.cox, entry.nox, -1});
        }
        return points;
    }

    static void kmeansClustering(vector<Point> &points, int k, int maxIterations = 100)
    {
        if (points.empty())
            throw runtime_error("No data for clustering");
        vector<Point> centroids(k);
        srand(time(0));
        for (int i = 0; i < k; ++i)
        {
            int idx = rand() % points.size();
            centroids[i] = {points[idx].x, points[idx].y, i};
        }

        for (int iter = 0; iter < maxIterations; ++iter)
        {
            // Assign points to nearest centroid
            for (auto &p : points)
            {
                double minDist = numeric_limits<double>::max();
                for (const auto &c : centroids)
                {
                    double dist = pow(p.x - c.x, 2) + pow(p.y - c.y, 2);
                    if (dist < minDist)
                    {
                        minDist = dist;
                        p.cluster = c.cluster;
                    }
                }
            }

            // Update centroids
            vector<Point> newCentroids(k);
            vector<int> counts(k, 0);

            for (const auto &p : points)
            {
                newCentroids[p.cluster].x += p.x;
                newCentroids[p.cluster].y += p.y;
                counts[p.cluster]++;
            }

            for (int i = 0; i < k; ++i)
            {
                if (counts[i] > 0)
                {
                    centroids[i].x = newCentroids[i].x / counts[i];
                    centroids[i].y = newCentroids[i].y / counts[i];
                }
            }
        }
    }
    static void visualizeClustersASCII(const vector<Point> &points)
    {
        const int WIDTH = 60;
        const int HEIGHT = 20;
        vector<vector<char>> grid(HEIGHT, vector<char>(WIDTH, ' '));

        // Find data ranges
        auto [minX, maxX] = minmax_element(points.begin(), points.end(),
                                           [](const Point &a, const Point &b)
                                           { return a.x < b.x; });
        auto [minY, maxY] = minmax_element(points.begin(), points.end(),
                                           [](const Point &a, const Point &b)
                                           { return a.y < b.y; });

        double xRange = maxX->x - minX->x;
        double yRange = maxY->y - minY->y;

        // Plot points
        for (const auto &p : points)
        {
            int col = static_cast<int>((p.x - minX->x) / xRange * (WIDTH - 1));
            int row = static_cast<int>((p.y - minY->y) / yRange * (HEIGHT - 1));
            grid[HEIGHT - 1 - row][col] = '0' + p.cluster;
        }

        // Draw grid
        cout << "\nCluster Visualization:\n";
        for (const auto &row : grid)
        {
            for (char c : row)
            {
                cout << (c == ' ' ? '.' : c) << ' ';
            }
            cout << endl;
        }
    }
    static double calculateCorrelation(const vector<double> &x, const vector<double> &y)
    {
        double sumX = accumulate(x.begin(), x.end(), 0.0);
        double sumY = accumulate(y.begin(), y.end(), 0.0);
        double sumXY = inner_product(x.begin(), x.end(), y.begin(), 0.0);
        double sumX2 = inner_product(x.begin(), x.end(), x.begin(), 0.0);
        double sumY2 = inner_product(y.begin(), y.end(), y.begin(), 0.0);

        int n = x.size();
        double numerator = sumXY - (sumX * sumY / n);
        double denominator = sqrt((sumX2 - pow(sumX, 2) / n)) * sqrt((sumY2 - pow(sumY, 2) / n));

        return denominator == 0 ? 0 : numerator / denominator;
    }
    static pair<double, double> trainLinearRegression(const vector<EmissionData> &data)
    {
        double sumX = 0, sumY = 0, sumX2 = 0, sumXY = 0;
        int n = data.size();

        for (const auto &d : data)
        {
            sumX += d.cox;
            sumY += d.temp;
            sumX2 += d.cox * d.cox;
            sumXY += d.cox * d.temp;
        }
        double denom = n * sumX2 - sumX * sumX;
        double slope = (n * sumXY - sumX * sumY) / denom;
        double intercept = (sumY - slope * sumX) / n;

        return {slope, intercept};
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
    pair<double, double> regressionModel;
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
        bool modelTrained;
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
        {
            showHeader();
            auto data = DataAnalyzer::loadPollutionData();
            auto points = DataAnalyzer::convertToPoints(data);

            cout << "Performing K-means clustering...\n";
            DataAnalyzer::kmeansClustering(points, 3);
            DataAnalyzer::visualizeClustersASCII(points);

            vector<double> cox, nox, temp;
            for (const auto &d : data)
            {
                cox.push_back(d.cox);
                nox.push_back(d.nox);
                temp.push_back(d.temp);
            }
            cout << "\nCorrelation Analysis:\n";
            cout << "COx vs Temperature: "
                 << DataAnalyzer::calculateCorrelation(cox, temp) << endl;
            cout << "NOx vs Temperature: "
                 << DataAnalyzer::calculateCorrelation(nox, temp) << endl;

            regressionModel = DataAnalyzer::trainLinearRegression(data);
            modelTrained = true;
            cout << "\nRegression model trained!\n";
            system("pause");
            break;
        }

        case 6:
        {
            showHeader();
            if (!modelTrained)
            {
                cout << "Please train model first using option 5!\n";
                system("pause");
                break;
            }

            double cox, nox;
            cout << "Enter current COx level (ppm): ";
            cin >> cox;
            cout << "Enter current NOx level (ppb): ";
            cin >> nox;

            double slope = regressionModel.first;
            double intercept = regressionModel.second;
            double predictedTemp = slope * cox + intercept;

            cout << "\nPredicted Temperature: " << fixed << setprecision(1)
                 << predictedTemp << "°C\n";
            system("pause");
            break;
        }
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