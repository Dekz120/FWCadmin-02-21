/*File describes two classes, firewall and interface.



*/

    #define PLATFORM_WINDOWS  1
    #define PLATFORM_MAC      2
    #define PLATFORM_UNIX     3

    #if defined(_WIN32)
    #define PLATFORM PLATFORM_WINDOWS
    #elif defined(__APPLE__)
    #define PLATFORM PLATFORM_MAC
    #else
    #define PLATFORM PLATFORM_UNIX
    #endif
    ///////////
    #if PLATFORM == PLATFORM_WINDOWS

        #include <winsock2.h>

    #elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

        #include <sys/socket.h>
        #include <netinet/in.h>
        #include <fcntl.h>

    #endif
    ////
     #if PLATFORM == PLATFORM_WINDOWS
    #pragma comment( lib, "wsock32.lib" )
    #endif


#include <iostream>
#include <fstream>
#include <map>
#include <list>
#include <algorithm>
#include <unordered_map>

#include "ip_address.h"

using namespace std;
const ip_address zero_ip("255.255.255.255/32");

template<typename _T>                                       
void writeList(list<_T>& some_list, ostream& os);
////
template<typename _T>
void readList(list<_T>& some_list, ifstream& is, int& read_lengh);
///
///                                                 }
template<typename _T_key, typename _T_elem>
void UMkeyElemWrite(unordered_map<_T_key, _T_elem>& some_map, ofstream& os, list<_T_key>& key_list);
///

string UMkeyRead(ifstream& is);
template<typename _T_elem>
void UMelemRead(ifstream& is, list<_T_elem>& some_map);
///
///

class fw_interface{
private:
    char interface_name[15];
    ip_address interface_ip;
    list<ip_address> ntworks_behing_int; 

    bool direction_in;//
    bool direction_out;//
    bool zone_role; //false means where is route to 0.0.0.0/0
public:
    
    friend void writeList();
    friend void read_list();
    void serialize(ofstream& os);
     void deserialize(ifstream& is);
     ///
     /// Ôóíêóöèè òàì
    fw_interface();
    ~fw_interface();

    fw_interface(char* name);

    fw_interface(const char* name, const bool &in, const bool &out, const bool &is_zone): direction_in(in), direction_out(out), zone_role(is_zone){strcpy(interface_name, name);};
    fw_interface(const char* name, ip_address &m_ip, const bool &is_zone, const bool &in, const bool &out): interface_ip(m_ip),direction_in(in), direction_out(out), zone_role(is_zone){strcpy(interface_name, name);};

    void put_int_name(char* name);//
    string getIntfaceName();

    void AddIpToIntface(ip_address&& IP);
    void AddIpToIntface(ip_address& IP);
    bool ipAddrSearch(const ip_address& IP);
    //////
    /// Ñåðèàëû)0
    //////
    void Display();
    bool isZoned();
    bool isActiveDirection(bool in) //if true. get IN_DIR request, else out direction
    {
        if(in){return direction_in;}
        else {return direction_out;}
    }

};
////////
struct fw_interactions{
	bool any_allow_to_zone;
	bool allow_to_zone; //
	bool any_allow_from_zone;
	bool allow_from_zone; //
	char special_comment[50];

};

class zone_based_fw{
private:
    //////////
    char fw_name[15];
    list<fw_interface> interface_list;

    list<string> key_list; //
    unordered_map<string , fw_interactions> Interact_rules;

public:
    zone_based_fw();
    ~zone_based_fw();
    zone_based_fw(string& cont_name);
       /// putterû âñÿêèå
    void AddInterface(const char* intface_name, const bool in, const bool out, const bool is_zone); //
    void AddInterface(const char* intface_name, ip_address &&m_ip,const bool in, const bool out, const bool is_zone);

    void AddIPaddr( string intface_name, ip_address &ip);
    void AddIPaddr( string intface_name, ip_address &&ip);
    void getIfacesInfo(); // something for debug
//////////////////
/////////////////
    void putInteractRule(char other_context[15]);
    void putInteractRule(string other_context, fw_interactions interaction);
/// getterû
    void getInteractRules(char other_context[15]);
    void getFWname();
    char* return_fw_name();
    void getInterfacesInfo();
    bool getFwDirection(const string& intface_name, bool dir_in); //if true. get IN_DIR request, else out direction

    bool searchIPaddr(const ip_address& ip, string &iface_name, const bool zone_consider_search);

    fw_interactions getInteractRule(string other_context);

    void clearFWdata();

    void serialize(ofstream& ofs);

    void deserialize(ifstream& ifs);
};



