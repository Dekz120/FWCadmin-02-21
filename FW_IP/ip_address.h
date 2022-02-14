#ifndef IP_ADDRESS_H_INCLUDED
#define IP_ADDRESS_H_INCLUDED


#include <iostream>
#include <string>
#include <Windows.h>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <list>


using namespace std;


//////////////////////
class ip_address
{
private:

	unsigned char oct1, oct2, oct3, oct4;  // 0-255
	unsigned char net_mask;					// 0-32
public:
    ip_address();
    ip_address(int& o1, int& o2, int& o3, int& o4);
    ip_address(int& o1, int& o2, int& o3, int& o4, int mask);
    ip_address(string ip_addr2);

    void display();
    int display() const;

    friend ip_address networkCalculation(const ip_address &ip_adr, int mask); //

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



#endif // IP_ADDRESS_H_INCLUDED
