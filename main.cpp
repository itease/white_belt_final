#include <iostream>
#include <set>
#include <map>
#include <sstream>
#include <iomanip>

using namespace std;

class Date {
public:
     Date() = default;
    explicit Date(const int new_year, const int new_month, const int new_day) {
        if (new_month < 1 || new_month > 12) {
            string string_error;
            string_error = "Month value is invalid: " + to_string(new_month);
            throw runtime_error(string_error);
        }
        else if (new_day < 1 || new_day > 31) {
            string string_error;
            string_error = "Day value is invalid: " + to_string(new_day);
            throw runtime_error(string_error);
        }
        else {
            year = new_year;
            month = new_month;
            day = new_day;
        }
    }
    int GetYear() const {
        return year;
    }
    int GetMonth() const {
        return month;
    }
    int GetDay() const {
        return day;
    }

private:
    int year;
    int month;
    int day;
};

bool operator < (const Date& lhs, const Date& rhs) {
    if ((lhs.GetYear() == rhs.GetYear()) && (lhs.GetMonth() == rhs.GetMonth())) {
        return lhs.GetDay() < rhs.GetDay();
    }
    else if ((lhs.GetYear() == rhs.GetYear()) && (lhs.GetMonth() != rhs.GetMonth())) {
        return lhs.GetMonth() < rhs.GetMonth();
    }
    else {
        return lhs.GetYear() < rhs.GetYear();
    }
}

void CheckSymbol(stringstream& stream, const string& date_string) {
    if (stream.peek() != '-') {
        stringstream string_error;
        string_error << "Wrong date format: " + date_string;
        throw runtime_error(string_error.str());
    }
    stream.ignore(1);
}

ostream& operator << (ostream& output, const Date& date) {
    output << setw(4) << setfill('0') << date.GetYear() << '-';
    output << setw(2) << setfill('0') << date.GetMonth() << '-';
    output << setw(2) << setfill('0') << date.GetDay();

    return output;
}

istream& operator >> (istream& input, Date& date) {
    int year = 10000;
    int month = 10000;
    int day = 10000;
    string date_string;
    string endError;

    if (input >> date_string) {
        stringstream ss(date_string);
        ss >> year;
        CheckSymbol(ss, date_string);
        ss >> month;
        CheckSymbol(ss, date_string);
        ss >> day;
        ss >> endError;
        if (month < 10000 && day < 10000 && endError.empty()) {
            date = Date(year, month, day);
            return input;
        }
        else {
            stringstream string_error;
            string_error << "Wrong date format: " + date_string;
            throw runtime_error(string_error.str());
        }
    }
    else {
        stringstream string_error;
        string_error << "Wrong date format: " + date_string;
        throw runtime_error(string_error.str());
    }
}

class Database {
public:
    void AddEvent(const Date& date, const string& event_name) {
        data[date].insert(event_name);
    }
    bool DeleteEvent(const Date& date, const string& event) {
        if (data[date].find(event) != data[date].end()) {
            data[date].erase(event);
            return true;
        }
        else {
            return false;
        }
    }
    int DeleteDate(const Date& date) {
        int n = data[date].size();
        data.erase(date);

        return n;
    }

    void Find(const Date& date) const {
        if (data.count(date)) {
            for (const auto& element : data.at(date)) {
                cout << element << endl;
            }
        }
    }

    void Print() const {
        for (const auto& item : data) {
            for (const auto& element : item.second) {
                cout << item.first << ' ' << element << endl;
            }
        }
    }

private:
    map<Date, set<string>> data;
};

void CommandChoice(const string& full_command, Database& database_name) {
    stringstream ss1(full_command);
    string command;
    Date date{};
    string event_name;

    if (ss1 >> command) {
        if (command == "Add") {
            if (ss1 >> date) {
                if (ss1 >> event_name) {
                    database_name.AddEvent(date, event_name);
                } else {
                    stringstream string_error;
                    string_error << "Wrong date format: " << date;
                    throw runtime_error(string_error.str());
                }
            }
        } else if (command == "Del") {
            if (ss1 >> date) {
                if (ss1 >> event_name) {
                    if (database_name.DeleteEvent(date, event_name)) {
                        cout << "Deleted successfully" << endl;
                    } else {
                        cout << "Event not found" << endl;
                    }
                } else {
                    cout << "Deleted " << database_name.DeleteDate(date) << " events" << endl;
                }
            }
        } else if (command == "Find") {
            if (ss1 >> date) {
                database_name.Find(date);
            }
        } else if (command == "Print") {
            database_name.Print();
        } else {
            stringstream string_error;
            string_error << "Unknown command: " << command;
            throw runtime_error(string_error.str());
        }
    }
}

int main() {
    Database db;
    string command;

    try {
        while (getline(cin, command)) {
            CommandChoice(command, db);
        }
    }
    catch (exception& exception) {
        cout << exception.what();
    }

    return 0;
}