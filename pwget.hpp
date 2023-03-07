const std::string alpha = "0123456789!@#$%&*abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

class Pwget {
    public:
    std::string mkpw (int length) {
        std::string passwrd;
        srand(time(0));
        for ( int i = 0; i < length; i++ ) {
            passwrd += alpha [ rand() % alpha.size() ];
        }
        return passwrd;
    }
};

class Cryptography {
    public:
    std::vector < std::string > convrt_encrypt ( std::string msg ) {
        std::vector < std::string > msg_hex;
        for ( int i = 0; i < msg.size(); i ++ ) {
            for ( int j = 0; j < alpha.size(); j ++ ) {
                if ( alpha[j] == msg[i] ) {
                    int random = rand() % 128;
                    int val = j + random, key = val - j;
                    std::ostringstream sstream_val, sstream_key;
                    sstream_val << std::hex << val;
                    sstream_key << std::hex << key;
                    std::string hex_val = sstream_val.str();
                    std::string hex_key = sstream_key.str();
                    msg_hex.push_back ( hex_val );
                    msg_hex.push_back ( hex_key );
                }
            }
        }
        return msg_hex;
    };
    std::string convrt_decrypt ( std::vector < std::string > msg ) {
        std::vector < int > msg_int, msg_combined_int;
        std::string msg_char;
        for ( int i = 0; i < msg.size(); i ++ ) {
            msg_int.push_back ( std::stoi ( msg[i], 0, 16 ) );
        }
        for ( int i = 0; i < msg_int.size(); i += 2 ) {
            msg_combined_int.push_back ( msg_int [i] - msg_int [ i + 1 ] );
        }
        for ( int i = 0; i < msg_combined_int.size(); i ++ ) {
            msg_char.push_back ( alpha [ msg_combined_int [i] ] );
        }
        return msg_char;
    };
};

class Save {
    public:
    std::vector < std::string > split ( std::string text, std::string delim, bool compress ) {
        std::vector < std::string > vec;
        size_t pos = 0, prevPos = 0;
        while ( true ) {
            pos = text.find_first_of(delim, prevPos);
            while ( compress ) {
                if ( prevPos == pos ) {
                    prevPos ++;
                } else { break; }
                pos = text.find_first_of ( delim, prevPos );
            }
            if ( pos == std::string::npos ) {
                if ( prevPos != text.size() )
                    vec.push_back ( text.substr ( prevPos ) );
                return vec;
            }
            vec.push_back ( text.substr ( prevPos, pos - prevPos ) );
            prevPos = pos + 1;
        }
    }
    void writeToFile ( std::string file_location, std::vector < std::string > hex_values) {
        std::ofstream file ( file_location );
        for ( int i = 0; i < hex_values.size() - 1; i ++ ) {
            file << hex_values[i] << ",";
        }
        file << hex_values [ hex_values.size() - 1 ];
        file.close();
    };
    std::string ReadFromFile ( std::string file_location ) {
        std::ifstream file( file_location );
        file.seekg(0, std::ios::end);
        size_t size = file.tellg();
        std::string buffer(size, ' ');
        file.seekg(0);
        file.read(&buffer[0], size);
        std::vector < std::string > pw_encrypted = split(buffer, ",", true);
        return Cryptography().convrt_decrypt ( pw_encrypted );      // Cryptography is defined in the main.cpp file
    };
};