#ifndef INTERESTEDADOPTER_H
#define INTERESTEDADOPTER_H
#include <string>

class InterestedAdopter
{
    public:
        InterestedAdopter(std::string name, std::string tel);

        //setters
        void set_name(std::string);
        void set_tel(std::string);

        //getters
        std::string get_name();
        std::string get_tel();
    private:
        std::string name;
        std::string tel; //phone number

};


#endif