#include "InterestedAdopter.h"
#include <string>
InterestedAdopter::InterestedAdopter(std::string name, std::string tel)
{
    this->name = name;
    this->tel = tel;
}

//setters
void InterestedAdopter::set_name(std::string name) {this->name = name;}
void InterestedAdopter::set_tel(std::string tel) {this->tel = tel;}

//getters
std::string InterestedAdopter::get_name() {return name;}
std::string InterestedAdopter::get_tel() {return tel;}