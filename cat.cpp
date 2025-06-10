#include "cat.h"
#include <string>
#include <iostream>
#include <iomanip>
using namespace std;

Cat::Cat(string id, string name, string trait)
    : Pets(id, name)
{
    breed = "Cat";
    this->trait = trait;
}
void Cat::set_trait(string trait) {this->trait = trait;}
string Cat::get_trait() const {return trait;}

void Cat::print_info(ostream& out, string mode) const
{
    if (mode == "csv")
    {
        out << breed << ",";
        Pets::print_info(out, mode);
        out << trait << ",";
        if (get_adopter() == nullptr)
        {
            out << "N/A" << ","
                << "N/A" << endl;
            return;
        }
        out << get_adopter()->get_name() << ","
            << get_adopter()->get_tel() << endl; 
    }
    else if (mode == "table")
    {
        out << setw(7) << left << breed;
        Pets::print_info(out, mode);
        out << setw(8) << left << trait;
        if (get_adopter() == nullptr)
        {
            out << endl;
            return;
        }
        out << setw(10) << left << get_adopter()->get_name()
            << get_adopter()->get_tel() << endl; 
    }
}