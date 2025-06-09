#ifndef PETS_H
#define PETS_H
#include "InterestedAdopter.h"
#include <string>
#include <iostream>

class Pets
{
    public:
        Pets();
        Pets(std::string id, std::string name); //name,id

        //setters
        void add_day(int);
        void set_age(double);
        void set_sex(std::string);
        void set_adopter(std::string, std::string);
        void clear_adopter();
        ///getters
        
        std::string get_breed() const;
        std::string get_id() const;
        std::string get_name() const;
        double get_age() const;
        std::string get_sex() const;
        InterestedAdopter* get_adopter() const;
        virtual std::string get_trait() const = 0;
        
        int get_days_in_shelter() const;
        virtual void print_info(std::ostream&, std::string) const;

        ~Pets();

    protected:
        std::string breed;

    private:
        std::string id;
        std::string name;
        double age;
        std::string sex;

        int days_in_shelter = 0;
        InterestedAdopter* adopter = nullptr;

};


#endif