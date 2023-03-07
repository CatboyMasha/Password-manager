#include <bits/stdc++.h>
#include "pwget.hpp"

class Password {
    public:
    std::vector < std::string > encrypted;
    Password ( int pw_length ) {
        this->encrypted = Cryptography().convrt_encrypt( Pwget().mkpw( pw_length ) );
    }
};

void generate_new_pass ( std::string password_name ) {
    Password pw = Password (20);
    std::string decrypted = Cryptography().convrt_decrypt(pw.encrypted);
    std::cout << "Password for " << password_name << ": " << decrypted << std::endl;
    Save().writeToFile("passwords/" + password_name + ".csv", pw.encrypted);
};

bool file_exists ( std::string name ) {
    std::ifstream f(name.c_str());
    return f.good();
}

int main () {
    while ( true ) {    // Event-loop
        std::string usr_input;
        std::cout << "> ";
        std::getline(std::cin, usr_input);
        if ( usr_input == "exit" ) { break; }
        else if ( usr_input.substr(0, 5) == "mkpw " ) {
            std::string password_name = usr_input.substr(5, usr_input.size() - 5);
            if ( file_exists ( "passwords/" + password_name + ".csv" ) ) {
                std::cout << "This password already exists, do you wish to override it? (Y/n) > " << std::endl;
                std::string answer;
                std::cin >> answer;
                switch ( tolower ( answer[0] ) ) {
                    case 'y':
                        generate_new_pass ( password_name );
                        break;
                    case 'n':
                        std::cout << "Keeping the old password." << std::endl;
                        break;
                }
            } else { generate_new_pass ( password_name ); }
        }
        else if ( usr_input.substr(0, 6) == "getpw " ) {
            std::string password_name = usr_input.substr(6, usr_input.size() - 6);
            if ( file_exists ( "passwords/" + password_name + ".csv" ) ) {
                std::string password_value = Save().ReadFromFile( "passwords/" + password_name + ".csv" );
                std::cout << "Password for " << password_name << ": " << password_value << std::endl;
            }
        }
        else if ( usr_input == "pwlist" || usr_input == "listpw" ) {
            std::string path("\"passwords\"");
            std::string command("dir /a-d ");
            command.append(path);
            const char* final_command = command.c_str();
            system(final_command);
        }
        else if ( usr_input.substr(0, 5) == "rmpw " ) {
            std::string password_name = usr_input.substr(5, usr_input.size() - 5);
            std::string areyousure;
            std::cout << "Are you sure you want to delete the password \"" << password_name << "\"? (Y/n) > " << std::endl;
            std::cin >> areyousure;
            
            if ( tolower ( areyousure[0] ) ) {
                if ( file_exists ( "passwords/" + password_name + ".csv" ) ) {
                    std::string temp_pw = "passwords/" + password_name + ".csv";

                    char* char_array = new char[temp_pw.size() + 1];
                    std::strcpy(char_array, temp_pw.c_str());
                    int status = remove ( char_array );

                    if ( status == 0 ) {
                        std::cout << "Password deleted succesfully." << std::endl;
                    } else { std::cout << "En error occurred, please try again later." << std::endl; }
                }
            }
        }
    }
    return 0;
}