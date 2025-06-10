//hello vector version
#include <iostream>
#include <string>
#include <vector>
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

void greet(vector<Pets*>& roster); //UPD
bool exist_inventory(); //New UPD
int get_max(); //UPD
void load_shelter(vector<Pets*>& roster, int& max);

void print_title(); //New UPD

void print(const vector<Pets*>& roster); //UPD
void add_pet(vector<Pets*>& roster, int max); //UPD
void longest_resident(const vector<Pets*>& roster); //UPD
void daily_tasks(vector<Pets*>& roster); //UPD
void contact_adopter(vector<Pets*>& roster, vector<Pets*>& waiting_roster, vector<Pets*>& having_roster);
void save(vector<Pets*>& roster, int max); //UPD
void exit_the_program(vector<Pets*>& roster); //UPD

void contact_announce(vector<Pets*>& roster, vector<Pets*>& waiting_roster, vector<Pets*>& having_roster); //UPD
void remove_fullday(vector<Pets*>& roster); //UPD
void contact_needed_list(const vector<Pets*>& roster, vector<Pets*>& waiting); //UPD
void get_having_list(const vector<Pets*>& roster, vector<Pets*>& having); //UPD

//contact menu functions
void view_contact_needed(const vector<Pets*>& waiting_roster, const vector<Pets*>& having_roster); //UPD
void become_interested_adopter(vector<Pets*>& roster); //UPD
void adopt_pet(vector<Pets*>& roster); //UPD


int main()
{
    vector<Pets*> pets;
    greet(pets);
    int max_size = 0;
    bool have_inventory = exist_inventory();
    if (have_inventory)
        load_shelter(pets, max_size);
    else
        max_size = get_max();
    do
    {
        vector<Pets*> waiting_adopter_pets;
        vector<Pets*> having_adopter_pets;
        remove_fullday(pets);
        contact_announce(pets, waiting_adopter_pets, having_adopter_pets);
        int option = main_menu(7);
        switch (option)
        {
            case 1:
                print(pets);
                break;
            case 2:
                add_pet(pets, max_size);
                break;
            case 3:
                longest_resident(pets);
                break;
            case 4:
                daily_tasks(pets);
                break;
            case 5:
                contact_adopter(pets, waiting_adopter_pets, having_adopter_pets);
                break;
            case 6:
                save(pets, max_size);
                break;
            case 7:
                exit_the_program(pets);
        }
    } while (true);
}








/*functions*/

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

void greet(vector<Pets*>& roster)
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

bool exist_inventory()
{
    //ask for their inventory and get the max size of shelter
    cout << "Is there a current inventory? (y/n) >> ";
    string yn;
    do
    {
        cin >> yn;
        if (yn != "y" && yn != "n")
        {
            cout << "Please enter \"y\" or \"n\" >> ";
            cin >> yn;
        }
    } while (yn != "y" && yn != "n");
    return yn == "y";
}

int get_max() 
{
    int max = 0;
    do
    {
        cout << "Please enter the max size of the shelter: ";
        cin >> max;
        if (max < 0)
        {
            cout << "Max size cannot be negative!" << endl;
        }
    } while (max < 0);
    
    return max;
}


//option
void load_shelter(vector<Pets*>& roster, int& max)
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
        }
    } while (!ifs);
    
    cout << "Successfully opened." << endl;
    //first line -> shelter capacity
    string size;
    getline(ifs,size);
    max = stoi(size);

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
        for (auto r : roster)
        {
            if (r->get_id() == id)
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
            roster.push_back(new Dog(id, name, ch));
        else if (breed == "Cat")
            roster.push_back(new Cat(id, name, ch));
        roster.back()->add_day(stoi(days_str));
        roster.back()->set_age(age);
        roster.back()->set_sex(sex);
        roster.back()->set_adopter(adopter_name, tel);
    }
}

void print_title()
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
}

void print(const vector<Pets*>& roster)
{
    print_title();
    for (auto r : roster)
    {
        r->print_info(cout, "table");
    }
}

void add_pet(vector<Pets*>& roster, int max)
{
    //full capacity
    int count = roster.size();
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
        
        roster.push_back(new Dog(id, name, ch));
    }
    else if (breed == "Cat")
    {
        do
        {
            cout << "Is " << name << "'s a Long hair or a Short hair cat (Long/Short)? ";
            cin >> ch;
        } while (ch != "Long" && ch != "Short");

        roster.push_back(new Cat(id, name, ch));
    }
    roster.back()->set_age(age);
    roster.back()->set_sex(sex);
}

void longest_resident(const vector<Pets*>& roster)
{
    int longest = roster[0]->get_days_in_shelter();
    int longest_count = 0;
    int pos = 0;
    //find longest day
    for (size_t i = 1; i < roster.size(); i++)
    {
        if (roster[i]->get_days_in_shelter() > longest)
        {
            longest = roster[i]->get_days_in_shelter();
            pos = i;
        }
    }

    //check the longest residents' amount
    for (size_t i = 0; i < roster.size(); i++)
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
        print_title();
        roster[pos]->print_info(cout, "table");
        cout << "Today is " << pronouns << " DAY " << roster[pos]->get_days_in_shelter() << endl;
        return;
    }

    //multiple longest
    cout << "There are " << longest_count << " longest residents." << endl;
    cout << "Today is their DAY " << longest << endl;
    cout << "Here is the list of their info: " << endl;
    print_title();
    for (auto r : roster)
    {
        if (r->get_days_in_shelter() == longest)
            r->print_info(cout, "table");
    }
}

void daily_tasks(vector<Pets*>& roster)
{
    vector<int> grooming_list; //save roster index of grooming
    vector<int> checkup_list; //save roster index of checkup
    int count = roster.size(); //current roster size

    //find who need to be groomed or checkup
    for (int i = 0; i < count; i++)
    {    
        int day = roster[i]->get_days_in_shelter();
        string name = roster[i]->get_name();
        if (day > 0)
        {
            string breed = roster[i]->get_breed();
            if (day % 3 == 0 || breed == "Dog")
                grooming_list.push_back(i);
            if (day % 7 == 0)
                checkup_list.push_back(i);
        }
    }

    //print the info
    cout << endl;
    cout << "Grooming Needed:"<< endl;
    if (grooming_list.size() == 0)
        cout << "*No pet needs to be groomed*" << endl;
    else
    {    
        cout << setw(5) << left << "ID" << "Name" << endl;
        for (size_t i = 0; i < grooming_list.size(); i++)
        {
            cout << setw(5) << left << roster[grooming_list[i]]->get_id() 
                            << roster[grooming_list[i]]->get_name() << endl;
        }
    }

    cout << endl;
    cout << "Checkup Needed:"<< endl;
    if (checkup_list.size() == 0)
        cout << "*No pet needs a checkup*" << endl;
    else
    {    
        cout << setw(5) << left << "ID" << "Name" << endl;
        for (size_t i = 0; i < checkup_list.size(); i++)
        {
            cout << setw(5) << left << roster[checkup_list[i]]->get_id() 
                            << roster[checkup_list[i]]->get_name() << endl;
        }
    }
    
}

void save(vector<Pets*>& roster, int max)
{
    string filename;
    cout << "Enter the file name you want to save in: ";
    cin >> filename;
    ofstream ofs(filename);
    if (ofs.fail())
        cout << "Fail to open the saving file." << endl;
    cout << "Saving..." << endl;
    
    ofs << max << endl;
    print_title();
    for (size_t i = 0; i < roster.size(); i++)
    {
        roster[i]->print_info(cout, "table"); //print
        roster[i]->print_info(ofs, "csv"); //file
        if (i != roster.size() - 1)
        {
            cout << endl;
            ofs << endl;
        }
    }
    cout << "Completely Save" << endl;
}

void exit_the_program(vector<Pets*>& roster)
{
    cout << "See you next time!" << endl;
    for (auto i : roster)
        delete i;
    exit(0);
}


//contact adopter
void remove_fullday(vector<Pets*>& roster)
{
    //get removed pets' count
    int count = roster.size();
    int remove_count = 0;
    for (int i = 0; i < count; i++)
    {
        if (roster[i]->get_days_in_shelter() >= 15)
        {
            delete roster[i];
            roster.erase(roster.begin() + i);
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

void contact_announce(vector<Pets*>& roster, vector<Pets*>& waiting_roster, vector<Pets*>& having_roster)
{
    //reset
    waiting_roster.clear();
    having_roster.clear();
    for (size_t i = 0; i < roster.size(); i++)
    {
        int days = roster[i]->get_days_in_shelter();
        //no interested adopter
        if (days >= 10 && roster[i]->get_adopter() == nullptr)
            waiting_roster.push_back(roster[i]);
        //have interested adopter
        else if (days >= 10 && roster[i]->get_adopter() != nullptr)
            having_roster.push_back(roster[i]);
    }
    //display
    if (waiting_roster.size() > 0)
        cout << "[Announcement: " << waiting_roster.size() << " pet(s) are waiting for their adopter.]" << endl;

    if (having_roster.size() > 0)
        cout << "[Announcement: Time to adopt " << having_roster.size() << " pet(s).]" << endl;
}

//list for pets waiting for interested adopter
void contact_needed_list(const vector<Pets*>& roster, vector<Pets*>& waiting)
{
    int count = roster.size();
    for (int i = 0; i < count; i++)
    {
        int days = roster[i]->get_days_in_shelter();
        if (days >= 10 && roster[i]->get_adopter() == nullptr)
            waiting.push_back(roster[i]);
    }
}

//list for pets waiting for being adopted (already have an intersted adopter)
void get_having_list(const vector<Pets*>& roster, vector<Pets*>& having)
{
    int count = roster.size();
    for (int i = 0; i < count; i++)
    {
        int days = roster[i]->get_days_in_shelter();
        if (days >= 10 && roster[i]->get_adopter() != nullptr)
            having.push_back(roster[i]);
    }
}

void view_contact_needed(const vector<Pets*>& waiting_roster, const vector<Pets*>& having_roster)
{
    if (waiting_roster.size() > 0)
    {
        cout << "Pet(s) waiting for interested adopter:" << endl;
        print_title();
        for (size_t i = 0; i < waiting_roster.size(); i++)
        {
            waiting_roster[i]->print_info(cout, "table");
        }
    }
    if (having_roster.size() > 0)
    {
        cout << endl;
        cout << "Pet(s) can be adopted:" << endl;
        print_title();
        for (size_t i = 0; i < having_roster.size(); i++)
        {
            having_roster[i]->print_info(cout, "table");
        }
    }
}

void contact_adopter(vector<Pets*>& roster, vector<Pets*>& waiting_roster, vector<Pets*>& having_roster)
{
    bool exit = false;
    do
    {
        //reset
        contact_needed_list(roster, waiting_roster);
        get_having_list(roster, having_roster);
        contact_announce(roster, waiting_roster, having_roster);
        int option = contact_menu(4);
        switch(option)
        {
            case 1:
                become_interested_adopter(roster);
                break;
            case 2:
                adopt_pet(having_roster);
                break;
            case 3:
                view_contact_needed(waiting_roster, having_roster);
                break;
            case 4:
                exit = true;
                break;
        }
    } while (!exit);
    
}

void become_interested_adopter(vector<Pets*>& roster)
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
    vector<int> match_list; //save index of matching pets
    for (size_t i = 0; i < roster.size(); i++)
    {
        if (breed == roster[i]->get_breed() && trait == roster[i]->get_trait())
            match_list.push_back(i);
    }

    //not match
    if (match_list.size() == 0)
    {
        cout << "No pet fits your prefrences currently.\nPlease check it later." << endl;
        return;
    }

    //match
    cout << "Here match " << match_list.size() << " pet(s):" << endl;
    cout << setw(7) << left << "Breed"
            << setw(6) << left << "Days"
            << setw(5) << left << "ID"
            << setw(10) << left << "Name"
            << setw(6) << left << "Age"
            << setw(5) << left << "Sex"
            << "Trait" << endl;
    for (size_t i = 0; i < match_list.size(); i++)
        roster[match_list[i]]->print_info(cout, "table");
    
    //ask for adopter's preferred pet
    string id;
    bool valid_id = false;
    int interested = -1;
    do
    {
        cout << "Enter ID you are interested in (one only; \"N\" if none): ";
        cin >> id;
        if (id == "N")
        {
            cout << "No Worries! Feel free to check back anytime." << endl;
            return;
        }
        //get interested pet's index
        for (size_t i = 0; i < match_list.size(); i++)
        {
            if (id == roster[match_list[i]]->get_id())
            {
                valid_id = true;
                interested = match_list[i];
            }
        }
        //check if ID is valid
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

void adopt_pet(vector<Pets*>& roster)
{
    //get the person info
    string name;
    string tel;
    cout << "Please enter your name: ";
    cin >> name;
    cout << "Please enter your phone number: ";
    cin >> tel;

    //find matching info
    for (size_t i = 0; i < roster.size(); i++)
    {
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
                if (yn != 'n' && yn != 'y') //invalid input
                {
                    cout << "Please enter \'y\' or \'n\'>> ";
                    continue;
                }
                
                if (yn == 'n') //adopt later or give up the chance
                {
                    cout << "Are you still interested in adopting your pet (y/n)? ";
                    do
                    {
                        cin >> yn;
                        //invalid input
                        if (yn != 'y' && yn != 'n')
                        {
                            cout << "Please enter \'y\' or \'n\'>> ";
                            continue;
                        }

                        //give up
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
            roster.erase(roster.begin() + i);

            return;
        }
    }
    cout << "Sorry, we couldn't find your information." << endl;
}