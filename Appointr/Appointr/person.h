#include <iostream>
#include <string>

#ifndef PERSON_H
#define PERSON_H

class Person{

    protected:
        std::string first_name;
        std::string last_name;
        std::string address;
        std::string phone;
        int id;

    public:
        //Constructor
        Person();
        Person(std::string fname, std::string lname, std::string addr, std::string phoneNum, int idNum);

        //Accessors
        std::string get_first_name();
        std::string get_last_name();
        std::string get_address();
        std::string get_phone();
        int get_id();

        //Mutators
        void set_first_name(std::string name);
        void set_last_name(std::string name);
        void set_address(std::string addr);
        void set_phone(std::string phoneNum);
        void set_id(int id_num);

};

#endif
