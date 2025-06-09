#ifndef DOG_H
#define DOG_H
#include "pets.h"
#include <string>
#include <iostream>

class Dog : public Pets
{
    public:
        Dog(std::string id, std::string name, std::string size);
        void set_trait(std::string);
        std::string get_trait() const override;

        void print_info(std::ostream&, std::string) const override;
    private:
        std::string trait; // Large/Medium/Small size
};

#endif