#include "pets.h"
#include <iostream>
#include <iomanip>
using namespace std;

Pets::Pets()
{
    this->name = "N/A";
    this->id = id;
    this->age = 0;
    this->sex = "unknown";
}
Pets::Pets(string id, string name) //name,id
{
    this->name = name;
    this->id = id;
    this->age = 0;
    this->sex = "unknown";
}

//setters
void Pets::add_day(int days) {days_in_shelter = ++days;}
void Pets::set_age(double age) {this->age = age;}
void Pets::set_sex(string sex) {this->sex = sex;}
void Pets::set_adopter(string name, string tel) 
{
    if (name != "N/A" && name != "")
        adopter = new InterestedAdopter(name, tel);
}
void Pets::clear_adopter()
{
    delete adopter;
    adopter = nullptr;
}
///getters
string Pets::get_breed() const {return breed;}
string Pets::get_name() const {return name;}
string Pets::get_id() const {return id;}
double Pets::get_age() const {return age;}
string Pets::get_sex() const {return sex;}
int Pets::get_days_in_shelter() const {return days_in_shelter;}
InterestedAdopter* Pets::get_adopter() const {return adopter;}

void Pets::print_info(ostream& out, string mode) const
{
    if (mode == "csv")
        out << days_in_shelter << ","
            << id << ","
            << name << ","
            << age << ","
            << sex << ",";
    else if (mode == "table")
        out << setw(6) << left << days_in_shelter
            << setw(5) << left << id
            << setw(10) << left << name
            << setw(6) << left << age
            << setw(5) << left << sex;
}

Pets::~Pets()
{
    delete adopter;
}