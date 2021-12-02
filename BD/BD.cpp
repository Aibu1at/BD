#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <iomanip>

using namespace std;

class Date
{
    int Year = 0, Month = 0, Day = 0;
public:
    Date(const string& str);
    int GetYear() const;
    int GetMonth() const;
    int GetDay() const;
};

ostream& operator<<(ostream& out, const Date& date);
bool operator<(const Date& lhs, const Date& rhs);
bool operator==(const Date& lhs, const Date& rhs);

class Database
{
    map<Date, set<string>> m;
public:
    void AddEvent(const Date& date, const string& event);
    bool DeleteEvent(const Date& date, const string& event);
    int  DeleteDate(const Date& date);

    set<string> Find(const Date& date) const;

    void Print() const;
};

int main()
{
    try
    {
        Database db;

        string command;
        while (getline(cin, command))
        {

            stringstream in(command);
            string com;
            in >> com;
            if (com == "Print")
            {
                db.Print();
            }
            else if (com == "Find")
            {
                string date_str;
                in >> date_str;
                Date date(date_str);

                set<string> finded = db.Find(date);
                for (const string& item : finded)
                {
                    cout << item << endl;
                }
            }
            else if (com == "Del")
            {
                string date_str;
                in >> date_str;
                Date date(date_str);

                if (in.eof())
                {
                    cout << "Deleted " << db.DeleteDate(date) << " events" << endl;
                }
                else
                {
                    string event;
                    in >> event;
                    if (db.DeleteEvent(date, event))
                    {
                        cout << "Deleted successfully" << endl;
                    }
                    else
                    {
                        cout << "Event not found" << endl;
                    }
                }
            }
            else if (com == "Add")
            {
                string date_str;
                in >> date_str;
                Date date(date_str);

                string event;
                in >> event;

                db.AddEvent(date, event);
            }
            else if (com.empty()) {}
            else
            {
                throw runtime_error("Unknown command : " + command);
            }
        }
    }
    catch (exception& ex)
    {
        cout << ex.what() << endl;
    }
    return 0;
}

Date::Date(const string& str)
{
    stringstream stream(str);
    char a, b;
    if (!(stream >> Year >> a >> Month >> b >> Day) || a != '-' || b != '-' || !stream.eof())
    {
        throw runtime_error("Wrong date format : " + str);
    }
    if (Month > 12 || Month < 1)
    {
        throw runtime_error("Month value is invalid : " + to_string(Month));
    }
    if (Day > 31 || Day < 1)
    {
        throw runtime_error("Day value is invalid: " + to_string(Day));
    }
}

int Date::GetYear() const
{
    return Year;
}

int Date::GetMonth() const
{
    return Month;
}

int Date::GetDay() const
{
    return Day;
}

bool operator<(const Date& lhs, const Date& rhs)
{
    if (lhs.GetYear() == rhs.GetYear())
    {
        if (lhs.GetMonth() == rhs.GetMonth())
        {
            return lhs.GetDay() < rhs.GetDay();
        }
        else
        {
            return lhs.GetMonth() < rhs.GetMonth();
        }
    }
    else
    {
        return lhs.GetYear() < rhs.GetYear();
    }
}

bool operator==(const Date& lhs, const Date& rhs)
{
    if ((lhs.GetDay() == rhs.GetDay()) && (lhs.GetMonth() == rhs.GetMonth()) && (lhs.GetYear() == rhs.GetYear()))
    {
        return true;
    }
    else
    {
        return false;
    }
}


ostream& operator<<(ostream& out, const Date& date)
{
    out << setfill('0') << setw(4) << date.GetYear() << '-' <<
        setw(2) << date.GetMonth() << '-' << setw(2) << date.GetDay();
    return out;
}

void Database::AddEvent(const Date& date, const string& event)
{
    if (!m.count(date))
    {
        set<string> ss;
        m[date] = ss;
    }
    m[date].insert(event);
}

bool Database::DeleteEvent(const Date& date, const string& event)
{
    if (m.count(date))
    {
        if (m.at(date).count(event))
        {
            m.at(date).erase(event);
            if (m.at(date).empty())
            {
                m.erase(date);
            }
            return true;
        }
    }
    else
    {
        return false;
    }
}

int Database::DeleteDate(const Date& date)
{
    int n = 0;
    if (m.count(date))
    {
        for (auto item : m.at(date))
        {
            n++;
        }
        m.erase(date);
    }
    return n;
}

set<string> Database::Find(const Date& date) const
{
    return m.at(date);
}

void Database::Print() const
{
    for (const auto& i : m)
    {
        for (const string& j : i.second)
        {
            cout << i.first << ' ' << j << endl;
        }
    }
}
