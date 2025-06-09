#ifndef CAT_H
#define CAT_H
#include "pets.h"
#include <string>
#include <iostream>

class Cat : public Pets
{
    public:
        Cat(std::string id, std::string name, std::string fur_type);
        void set_trait(std::string);
        std::string get_trait() const override;

        void print_info(std::ostream&, std::string) const override;

    private:
        std::string trait; // Long/Short fur
};

#endif