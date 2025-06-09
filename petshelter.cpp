#include <iostream>
#include <string>
#include "pets.h"
#include "dog.h"
#include "cat.h"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <ctype.h>
using namespace std;

int main_menu(int options);
int contact_menu(int options);
int input(int options);

void greet(Pets** roster);
int get_max();
Pets** load_shelter(Pets** roster, int &count, int& max);

void print(Pets** roster, int count);
void add_pet(Pets** roster, int &count, int max);
void longest_resident(Pets** roster, int &count);
void daily_tasks(Pets** roster, int &count);
void save(Pets* roster[], int count, int max);
void exit_the_program(Pets* roster[]);

void remove_fullday(Pets** roster, int &count);
void contact_announce(Pets** roster, int count, int &waiting_count, int &having_count);
Pets** contact_needed_list(Pets** roster, int count, int &waiting_count);
Pets** get_having_list(Pets** roster, int count, int &having_count);
void view_contact_needed(Pets** waiting_roster, int waiting_count, Pets** having_roster, int having_count);
void contact_adopter(Pets** roster, int &count);
void become_interested_adopter(Pets** roster, int count);
void adopt_pet(Pets** roster, int &count);


int main()
{
    Pets** pets = nullptr;
    greet(pets);
    int max_size = get_max();
    int pet_count = 0;
    if (max_size == 0)
        pets = load_shelter(pets, pet_count, max_size);
    else
        pets = new Pets*[max_size];
    do
    {
        int waiting_adopter_pets = 0;
        int having_adopter_pets = 0;
        remove_fullday(pets, pet_count);
        contact_announce(pets, pet_count, waiting_adopter_pets, having_adopter_pets);
        int option = main_menu(7);
        switch (option)
        {
            case 1:
                print(pets, pet_count);
                break;
            case 2:
                add_pet(pets, pet_count, max_size);
                break;
            case 3:
                longest_resident(pets, pet_count);
                break;
            case 4:
                daily_tasks(pets, pet_count);
                break;
            case 5:
                contact_adopter(pets, pet_count);
                break;
            case 6:
                save(pets, pet_count, max_size);
                break;
            case 7:
                exit_the_program(pets);
        }
    } while (true);
}

//menu
int input(int options)
{
    cout << "Please enter a number from 1 to " << options << ": ";
    int x;
    cin >> x;
    cin.ignore(); //ignore the new line buffer
    while (cin.fail() || x < 1 || x > options)
    {
        if (cin.fail())
        {
            cin.clear();
            string garbage;
            cin >> garbage;
        }
        cout << "Invalid input. Please enter number between 1-" << options << ": ";
        cin >> x;
    }
    return x;
}

int main_menu(int options)
{
    cout << "=================Main Menu=================" << endl;
    cout << "1. View Pets" << endl
         << "2. Add Pet" << endl
         << "3. Longest Resident" << endl
         << "4. Daily Tasks List" << endl
         << "5. Contact Interested Adopter" << endl
         << "6. Save" << endl
         << "7. Exit" << endl;
    return input(options);
}

int contact_menu(int options)
{
    cout << "===============Contact Menu================" << endl;
    cout << "1. Become An Interested Adopter" << endl
         << "2. Adopt Pet" << endl
         << "3. View adopt needed pets" << endl
         << "4. Exit" << endl;
    return input(options);
}

void greet(Pets** roster)
{
    cout << "Welcome to CS Pets Shelter." << endl;
    cout << "Would you like to use this program? (y/n) >> ";
    string yn;
    cin >> yn;
    do 
    {
        if (yn == "n")
        {
            exit_the_program(roster);
        }
        else if (yn != "y")
        {
            cout << "Please enter \"y\" or \"n\" >> ";
            cin >> yn;
        }
    } while (yn != "y");
}

int get_max() 
{
    //ask for their inventory and get the max size of shelter
    int max = 0;
    cout << "Is there a current inventory? (y/n) >> ";
    string yn;
    cin >> yn;
    do 
    {
        if (yn == "n")
        {
            cout << "Please enter the max size of the shelter: ";
            cin >> max;
            if (max < 0)
            {
                cout << "Max size cannot be negative!" << endl;
                continue;
            }
            break;
        }
        else if (yn != "y")
        {
            cout << "Please enter \"y\" or \"n\" >> ";
            cin >> yn;
        }
    } while (yn != "y" || max < 0);
    return max;
}


//option
Pets** load_shelter(Pets** roster, int &count, int& max)
{   
    string filename;
    ifstream ifs;
    do
    {
        cout << "Please enter the file name: ";
        cin >> filename;
        ifs.open(filename);
        if (!ifs)
        {
            cout << "File not found." << endl;
            max = get_max();
            if (max >= 0)
            {
                roster = new Pets*[max];
                return roster;
            }
        }
    } while (!ifs);
    
    cout << "Successfully opened." << endl;
    //first line -> shelter size
    string size;
    getline(ifs,size);
    max = stoi(size);
    roster = new Pets*[max];

    //format: breed,days_in_shelter,id,name,age,sex,trait,adopter_name,phone_number
    string line;
    while (getline(ifs, line)) 
    {
        istringstream info;
        info.str(line);

        //pet info
        string breed;
        string days_str;
        string id, name;
        double age;
        string age_str, sex, ch;
        getline(info, breed, ',');
        getline(info, days_str, ',');
        getline(info, id, ',');
        
        //check if ID exists (no same ID in a shelter)
        bool id_exists = false;
        for (int i = 0; i < count; i++)
        {
            if (roster[i]->get_id() == id)
            {
                id_exists = true;
                break;
            }
        }
        if (id_exists) continue;
        getline(info, name, ',');
        getline(info, age_str, ',');
        age = stod(age_str);
        getline(info, sex, ',');
        getline(info, ch, ',');

        //adopter info
        string adopter_name, tel;
        getline(info, adopter_name, ',');
        getline(info, tel);

        if (breed == "Dog")
            roster[count] = new Dog(id, name, ch);
        else if (breed == "Cat")
            roster[count] = new Cat(id, name, ch);
        roster[count]->add_day(stoi(days_str));
        roster[count]->set_age(age);
        roster[count]->set_sex(sex);
        roster[count]->set_adopter(adopter_name, tel);
        
        count++;
    }
    return roster;
}

void print(Pets** roster, int count)
{
    cout << setw(7) << left << "Breed"
            << setw(6) << left << "Days"
            << setw(5) << left << "ID"
            << setw(10) << left << "Name"
            << setw(6) << left << "Age"
            << setw(5) << left << "Sex"
            << setw(8) << left << "Trait"
            << setw(10) << left << "Adopter"
            << "Tel." << endl;
    for (int i = 0; i < count; i++)
    {
        roster[i]->print_info(cout, "table");
    }
}

void add_pet(Pets** roster, int &count, int max)
{
    //full capacity
    if (count >= max) 
    {
        cout << "Sorry! Our shelter is full." << endl;
        return;
    }

    string breed;
    string id;
    string name, sex;
    double age;
    //id
    cout << "Pet's ID: ";
    cin >> id;
        //check if ID exists
    for (int i = 0; i < count; i++)
    {
        if (roster[i]->get_id() == id)
        {
            cout << "The pet already exists" << endl;
            return;
        }
    }

    //breed
        //input Dog and Cat only
    do
    {
        cout << "Pet's Breed (Dog/Cat): ";
        cin >> breed;
        if (breed == "Dog" || breed == "Cat")
            break;
        cout << "Sorry! We only take in dog or cat." << endl;
    } while (breed != "Dog" && breed != "Cat");
    
    //name, age, sex
    cout << "Pet's Name: ";
    cin.ignore();
    getline(cin, name);
    cout << name << "'s age: ";
    cin >> age;
    //input validation for age
    while (cin.fail())
    {
        if (cin.fail())
        {
            cin.clear();
            string garbage;
            cin >> garbage;
        }
        cout << "Invalid input. Please enter a number for age: ";
        cin >> age;
    };
    
    //input validation for sex
    cout << name << "'s sex (F/M): ";
    do
    {
        cin >> sex;
        if (sex != "F" && sex != "M")
            cout << "Invalid input. Please enter \"F\" or \"M\": ";
    } while (sex != "F" && sex != "M");

    //characteristic
    string ch;
    if (breed == "Dog")
    {
        do
        {
            cout << "Is " << name << "'s a Large, Medium or Small dog (Large/Medium/Small)? ";
            cin >> ch;
        } while (ch != "Large" && ch != "Medium" && ch != "Small");
        
        roster[count] = new Dog(id, name, ch);
    }
    else if (breed == "Cat")
    {
        do
        {
            cout << "Is " << name << "'s a Long hair or a Short hair cat (Long/Short)? ";
            cin >> ch;
        } while (ch != "Long" && ch != "Short");

        roster[count] = new Cat(id, name, ch);
    }
    roster[count]->set_age(age);
    roster[count]->set_sex(sex);

    count++;
}

void longest_resident(Pets** roster, int &count)
{
    int longest = roster[0]->get_days_in_shelter();
    int longest_count = 0;
    int pos = 0;
    //find longest day
    for (int i = 1; i < count; i++)
    {
        if (roster[i]->get_days_in_shelter() > longest)
        {
            longest = roster[i]->get_days_in_shelter();
            pos = i;
        }
    }

    //check the longest residents
    for (int i = 0; i < count; i++)
    {
        if (longest == roster[i]->get_days_in_shelter())
            longest_count++;
    }

    //single longest
    if (longest_count == 1)
    {
        string pronouns;
        cout << "The longest resident is " << roster[pos]->get_name() << endl;
        if (roster[pos]->get_sex() == "F")
            pronouns = "her";
        else
            pronouns = "his";
        cout << setw(7) << left << "Breed"
            << setw(6) << left << "Days"
            << setw(5) << left << "ID"
            << setw(10) << left << "Name"
            << setw(6) << left << "Age"
            << setw(5) << left << "Sex"
            << setw(8) << left << "Trait"
            << setw(10) << left << "Adopter"
            << "Tel." << endl;
        roster[pos]->print_info(cout, "table");
        cout << "Today is " << pronouns << " DAY " << roster[pos]->get_days_in_shelter() << endl;
        return;
    }

    //multiple longest
    cout << "There are " << longest_count << " longest residents." << endl;
    cout << "Today is their DAY " << longest << endl;
    cout << "Here is the list of their info: " << endl;
    cout << setw(7) << left << "Breed"
            << setw(6) << left << "Days"
            << setw(5) << left << "ID"
            << setw(10) << left << "Name"
            << setw(6) << left << "Age"
            << setw(5) << left << "Sex"
            << setw(8) << left << "Trait"
            << setw(10) << left << "Adopter"
            << "Tel." << endl;
    for (int i = 0; i < count; i++)
    {
        if (roster[i]->get_days_in_shelter() == longest)
            roster[i]->print_info(cout, "table");
    }
}

void daily_tasks(Pets** roster, int &count)
{
    int need_grooming = 0; //amount of needed grooming
    int need_checkup = 0; //amount of needed grooming
    int grooming_list[count]; //save roster index of grooming
    int checkup_list[count]; //save roster index of checkup

    //find who need to be groomed or checkup
    for (int i = 0; i < count; i++) //control roster
    {    
        int day = roster[i]->get_days_in_shelter();
        string name = roster[i]->get_name();
        if (day > 0)
        {
            string breed = roster[i]->get_breed();
            if (day % 3 == 0 || breed == "Dog")
                grooming_list[need_grooming++] = i;
            if (day % 7 == 0)
                checkup_list[need_checkup++] = i;
        }
    }

    //print the info
    cout << endl << "Grooming Needed:"<< endl;
    if (need_grooming == 0)
        cout << "*No pet needs to be groomed*" << endl;
    else
    {    
        cout << setw(5) << left << "ID" << "Name" << endl;
        for (int i = 0; i < need_grooming; i++)
        {
            cout << setw(5) << left << roster[grooming_list[i]]->get_id() 
                            << roster[grooming_list[i]]->get_name() << endl;
        }
    }
    cout << endl;
    cout << "Checkup Needed:"<< endl;
    if (need_checkup == 0)
        cout << "*No pet needs a checkup*" << endl;
    else
    {    
        cout << setw(5) << left << "ID" << "Name" << endl;
        for (int i = 0; i < need_checkup; i++)
        {
            cout << setw(5) << left << roster[checkup_list[i]]->get_id() 
                            << roster[checkup_list[i]]->get_name() << endl;
        }
    }
    
}

void save(Pets* roster[], int count, int max)
{
    string filename;
    cout << "Enter the file name you want to save in: ";
    cin >> filename;
    ofstream ofs(filename);
    if (ofs.fail())
        cout << "Fail to open the saving file." << endl;
    cout << "Saving..." << endl;
    
    ofs << max << endl;
    cout << setw(7) << left << "Breed"
            << setw(6) << left << "Days"
            << setw(5) << left << "ID"
            << setw(10) << left << "Name"
            << setw(6) << left << "Age"
            << setw(5) << left << "Sex"
            << setw(8) << left << "Trait"
            << setw(10) << left << "Adopter"
            << "Tel." << endl;
    for (int i = 0; i < count; i++)
    {
        roster[i]->print_info(cout, "table"); //print
        roster[i]->print_info(ofs, "csv"); //file
    }
    cout << "Completely Save" << endl;
}

void exit_the_program(Pets* roster[])
{
    cout << "See you next time!" << endl;
    delete[] roster;
    exit(0);
}


//contact adopter
void remove_fullday(Pets** roster, int &count)
{
    //get removed pets' count
    int remove_count = 0;
    for (int i = 0; i < count; i++)
    {
        if (roster[i]->get_days_in_shelter() >= 15)
        {
            delete roster[i];
            //shift left
            for (int j = i; j < count; j++)
            {
                roster[j] = roster[j+1];
            }
            count--;
            remove_count++;
            //check the index member again
            i--;
        }
    }
    //display
    if (remove_count > 0)
    {
        cout << "[Due to Days Limit Policy]" << endl << "["
             << remove_count 
             << " pet(s) were adopted by another shelter.]" << endl;
    }
}

void contact_announce(Pets** roster, int count, int &waiting_count, int &having_count)
{
    //reset
    waiting_count = 0;
    having_count = 0;
    Pets* waiting[count];
    Pets* having[count];
    for (int i = 0; i < count; i++)
    {
        int days = roster[i]->get_days_in_shelter();
        //no interested adopter
        if (days >= 10 && roster[i]->get_adopter() == nullptr)
            waiting[waiting_count++] = roster[i];
        //have interested adopter
        else if (days >= 10 && roster[i]->get_adopter() != nullptr)
            having[having_count++] = roster[i];
    }
    //display
    if (waiting_count > 0)
        cout << "[Announcement: " << waiting_count << " pet(s) are waiting for their adopter.]" << endl;

    if (having_count > 0)
        cout << "[Announcement: Time to adopt " << having_count << " pet(s).]" << endl;
}

Pets** contact_needed_list(Pets** roster, int count, int &waiting_count)
{
    //list for pets waiting for interested adopter
    Pets** waiting = new Pets*[count];
    for (int i = 0; i < count; i++)
    {
        int days = roster[i]->get_days_in_shelter();
        if (days >= 10 && roster[i]->get_adopter() == nullptr)
            waiting[waiting_count++] = roster[i];
    }
    return waiting;
}

Pets** get_having_list(Pets** roster, int count, int &having_count)
{
    //list for pets waiting for being adopted (already have an intersted adopter)
    Pets** having = new Pets*[count];
    for (int i = 0; i < count; i++)
    {
        int days = roster[i]->get_days_in_shelter();
        if (days >= 10 && roster[i]->get_adopter() != nullptr)
            having[having_count++] = roster[i];
    }
    return having;
}

void view_contact_needed(Pets** waiting_roster, int waiting_count, Pets** having_roster, int having_count)
{
    if (waiting_count > 0)
    {
        cout << "Pet(s) waiting for interested adopter:" << endl;
        cout << setw(7) << left << "Breed"
            << setw(6) << left << "Days"
            << setw(5) << left << "ID"
            << setw(10) << left << "Name"
            << setw(6) << left << "Age"
            << setw(5) << left << "Sex"
            << setw(8) << left << "Trait"
            << setw(10) << left << "Adopter"
            << "Tel." << endl;
        for (int i = 0; i < waiting_count; i++)
        {
            waiting_roster[i]->print_info(cout, "table");
        }
    }
    if (having_count > 0)
    {
        cout << endl;
        cout << "Pet(s) can be adopted:" << endl;
        cout << setw(7) << left << "Breed"
            << setw(6) << left << "Days"
            << setw(5) << left << "ID"
            << setw(10) << left << "Name"
            << setw(6) << left << "Age"
            << setw(5) << left << "Sex"
            << setw(8) << left << "Trait"
            << setw(10) << left << "Adopter"
            << "Tel." << endl;
        for (int i = 0; i < having_count; i++)
        {
            having_roster[i]->print_info(cout, "table");
        }
    }
}

void contact_adopter(Pets** roster, int &count)
{
    bool exit = false;
    do
    {
        //reset
        int waiting_count = 0;
        int having_count = 0;
        Pets** waiting_list = contact_needed_list(roster, count, waiting_count);
        Pets** having_list = get_having_list(roster, count, having_count);
        contact_announce(roster, count, waiting_count, having_count);
        int option = contact_menu(4);
        switch(option)
        {
            case 1:
                become_interested_adopter(roster, count);
                break;
            case 2:
                adopt_pet(roster, count);
                break;
            case 3:
                view_contact_needed(waiting_list, waiting_count, having_list, having_count);
                break;
            case 4:
                exit = true;
                break;
        }
    } while (!exit);
    
}

void become_interested_adopter(Pets** roster, int count)
{
    cout << "Enter your preferences to find your ideal pet." << endl;
    string breed;
    string trait;
    //input validation of breed
    do
    {
        cout << "Breed (Dog or Cat): ";
        cin >> breed;
        if (breed != "Dog" && breed != "Cat")
            cout << "Enter \"Dog\" or \"Cat\". Please check if there is a typo." << endl;
    } while (breed != "Dog" && breed != "Cat");
    
    //input validation of trait
    if (breed == "Dog")
    {
        do
        {
            cout << "Size (Large, Medium or Small): ";
            cin >> trait;
            if (trait != "Large" && trait != "Medium" && trait != "Small")
                cout << "Enter \"Large\" , \"Medium\" or \"Small\". Please check if there is a typo." << endl;
        } while (trait != "Large" && trait != "Medium" && trait != "Small");
    }
    else
    {
        do
        {
            cout << "Fur (Long or Short): ";
            cin >> trait;
            if (trait != "Long" && trait != "Short")
                cout << "Enter \"Long\" or \"Short\". Please check if there is a typo." << endl;
        } while (trait != "Long" && trait != "Short");
    }

    //preferences match pets' count
    int match_count = 0;
    int match_list[count]; //save index of matching pets
    for (int i = 0; i < count; i++)
    {
        if (breed == roster[i]->get_breed() && trait == roster[i]->get_trait())
            match_list[match_count++] = i;
    }

    //not match
    if (match_count == 0)
    {
        cout << "No pet fits your prefrences currently.\nPlease check it later." << endl;
        return;
    }

    //match
    cout << "Here match " << match_count << " pet(s):" << endl;
    cout << setw(7) << left << "Breed"
            << setw(6) << left << "Days"
            << setw(5) << left << "ID"
            << setw(10) << left << "Name"
            << setw(6) << left << "Age"
            << setw(5) << left << "Sex"
            << "Trait" << endl;
    for (int i = 0; i < match_count; i++)
        roster[match_list[i]]->print_info(cout, "table");
    
    string id;
    bool valid_id = false;
    int interested = -1;
    //ask for willing
    do
    {
        cout << "Enter ID you are interested in (one only; \"N\" if none): ";
        cin >> id;
        if (id == "N")
        {
            cout << "No Worries! Feel free to check back anytime." << endl;
            return;
        }
        for (int i = 0; i < match_count; i++)
        {
            if (id == roster[match_list[i]]->get_id())
            {
                valid_id = true;
                interested = match_list[i];
            }
        }
        if (valid_id == false)
            cout << "Invalid ID." << endl;
    } while (!valid_id && id != "N");

    //get adopter info
    string name;
    string tel;
    cout << "Your Name: ";
    cin >> name;
    cout << "Your Phone Number: ";
    cin >> tel;
    roster[interested]->set_adopter(name, tel);
}

void adopt_pet(Pets** roster, int &count)
{
    //get person info
    string name;
    string tel;
    cout << "Please enter your name: ";
    cin >> name;
    cout << "Please enter your phone number: ";
    cin >> tel;

    //find matching info
    for (int i = 0; i < count; i++)
    {
        //check pet's adopter existence
        if (roster[i]->get_adopter() == nullptr)
            continue;

        //matching info found
        if (roster[i]->get_adopter()->get_name() == name && roster[i]->get_adopter()->get_tel() == tel)
        {
            char yn;
            //ask for willing
            cout << "Do you want to adopt you interested pet now (y/n)? ";
            do
            {   
                cin >> yn;
                //adopt now
                if (yn != 'n' && yn != 'y')
                {
                    cout << "Please enter \'y\' or \'n\'>> ";
                }
                //adopt later or give up the chance
                else if (yn == 'n')
                {
                    do
                    {
                        //give up
                        cout << "Are you still interested in adopting your pet (y/n)? ";
                        cin >> yn;
                        if (yn == 'n')
                        {
                            roster[i]->clear_adopter();
                            cout << "We understand. Thank you for your interest!" << endl;
                            return;
                        }
                    } while (yn != 'y');
                    //adopt later
                    cout << "Great! See you soon..." << endl;
                    return;
                }
            } while (yn != 'y');
            
            cout << "Thank you for adopting a lovely pet!" << endl;
        
            //clear pet
            delete roster[i];
            //shift left
            for (int j = i; j < count; j++)
            {
                roster[j] = roster[j+1];
            }
            count--;
            return;
        }
    }
    cout << "Sorry, we couldn't find your information." << endl;
}