#ifndef IP_ADDRESS_H_INCLUDED
#define IP_ADDRESS_H_INCLUDED


#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <list>
#include <array>
#include <sstream>


using namespace std;


//////////////////////
class ip_address
{
private:

    
public:
    using ip_array = array <unsigned char, 5>;
    ip_array addr;
    ip_address(string ip_addr);
    ip_address(int mask);
    ip_address() {};

    friend ip_address networkCalculation(const ip_address &ip_adr, int mask); //

    //ip_address(const ip_address& adr2) : addr(adr2.addr) {};
    //ip_address operator =(ip_address adr2) { addr = adr2.addr;  return *this; };

    bool operator ==(const ip_address& adr2);
    bool operator !=(const ip_address& adr2);
    bool operator < (const ip_address& adr2);
    char getMask();

    void putMask(int msk);
};

void UniqueNetworks(list<ip_address>& adr_lst);
////////
void ParseIPfromFile(list<ip_address>& list_of_address1,  ifstream &sourse_file, int end_posit);

void ipSumm(list<ip_address>& addr_list);

ostream& operator<<(ostream& os, const ip_address& a);

#endif // IP_ADDRESS_H_INCLUDED