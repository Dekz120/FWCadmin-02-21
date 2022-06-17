#include "ip_address.h"
#include <regex>

using namespace std;

    ip_address::ip_address(int mask)
    {
        addr = { 0,0,0,0,0 };
        int i = 0;
        for (; i < (mask - 1) / 8; i++)
        {
            addr[i] = 255;
        }
        addr[i] = 256 - pow(2, 8 - (mask - 8*i));
        this->putMask(mask);
        if(mask > 32) addr = { 255,255,255,255,32 };
    }


   ip_address::ip_address(string ip_addr)
 {          
       string filter("0123456789");
       string filter2("./");
       auto pos = 0;
       auto pos2 = 0;
       addr = { 0,0,0,0,0 };
       addr[4] = 32;
       for (auto& oct : addr)
       {
           pos = ip_addr.find_first_of(filter, pos2);
           pos2 = ip_addr.find_first_of(filter2, pos);
           if (pos != string::npos && pos2 != string::npos)
           {
               string tmpss(ip_addr.substr(pos, pos2 - pos));
               oct = stoi(tmpss);
           }
           else if(pos != string::npos)
           {
               string tmpss(ip_addr.substr(pos, ip_addr.size() - pos));
               oct = stoi(tmpss);
           }

       }
}

   ostream& operator<<(ostream& os, const ip_address& a)
   {
       copy(begin(a.addr), end(a.addr)-2, ostream_iterator<int>{os, "."});
       os << (int)a.addr[3];
       if (a.addr[4] != 32) os << '/' << (int)a.addr[4];

       return os;
   }


    bool ip_address::operator ==(const ip_address& adr2)
    {
        return (addr == adr2.addr);
    }

    bool ip_address::operator !=(const ip_address& adr2)
    {
        return !(addr == adr2.addr);
    }

    bool ip_address::operator < (const ip_address& adr2)
    {
        for (int i = 0; i < 5; i++)
        {
            if (addr[i] != adr2.addr[i]) return(addr[i] < adr2.addr[i]);
        }
        return false;
    }


    char ip_address::getMask(){ return addr[4];}
    ////////////
    void ip_address::putMask(int msk){ addr[4] = msk;}

//////////////

ip_address networkCalculation(const ip_address &ip_adr, int mask){
    ip_address ntwk(mask);

    for (int i = 0; i < ntwk.addr.size()-1; i++)
    {
        ntwk.addr[i] = (ntwk.addr[i] & ip_adr.addr[i]);
    }

    return ntwk;
}
///////

void UniqueNetworks(list<ip_address>& adr_lst)  // remove excess networks(like a x.x.x.34 in x.x.x.0/24)
{
    ip_address base_address;
    ip_address addr_for_compare;

    int net_mask;
    list<ip_address>::iterator iter;
    iter = adr_lst.begin();
    net_mask = iter->getMask();
    base_address = networkCalculation(*iter++, net_mask);

    while(iter != adr_lst.end()){
        addr_for_compare = networkCalculation(*iter, net_mask);

        if (base_address == addr_for_compare) {adr_lst.erase(iter++); }
        else {net_mask = iter->getMask(); base_address = networkCalculation(*iter++, net_mask);}

    }

}
////////

void ParseIPfromFile(list<ip_address>& list_of_address, ifstream &sourse_file, int end_posit)
{
    char ch;
    sourse_file >> ch;
    while (sourse_file && sourse_file.tellg() < end_posit)
    {
        while (!isdigit(ch) && sourse_file) {
            sourse_file >> ch;
        }

        string potential_address;
        int points_counter = 0;
        int oct_check = 0;

        while (points_counter < 4 && oct_check < 4 && sourse_file.tellg() < end_posit)
        {
            if (ch == '.' || ch == '/' || isdigit(ch)) {
                if (ch == '.') { points_counter++; oct_check = 0; }
                if (isdigit(ch)) { oct_check++; }
                if (ch == '/') { oct_check = 1; }
            }
            else { sourse_file >> noskipws >> ch; break; }

            potential_address.push_back(ch);

            sourse_file >> noskipws >> ch;
        }

        if (points_counter == 3) {

            list_of_address.push_back(ip_address(potential_address));
        }
        potential_address.clear();

    }
}
////

/////
void ipSumm(list<ip_address>& addr_list)
{
    addr_list.sort();
    addr_list.unique();
    
    auto prev_addr = (begin(addr_list));
    auto curr_mask(begin(addr_list)->getMask());
    auto netwkchek([&curr_mask, &prev_addr](ip_address& curr_addr)
        {
            if (curr_mask == curr_addr.getMask())
            {
                ip_address tmp = networkCalculation(curr_addr, curr_mask - 1);
                tmp = networkCalculation(*prev_addr, curr_mask - 1);
                if (networkCalculation(curr_addr, curr_mask - 1) == networkCalculation(*prev_addr, curr_mask - 1))
                {
                    *prev_addr = networkCalculation(*prev_addr, (curr_mask - 1));
                    curr_addr = ip_address("255.255.255.255/32");
                }
                else
                {
                    prev_addr++;
                    curr_mask = curr_addr.getMask();
                }
            }
            else
            {
                prev_addr++;
                curr_mask = curr_addr.getMask();
            }
        });

    auto currsize(addr_list.size());
    while (true)
    {
        for_each(++begin(addr_list), end(addr_list), netwkchek);
        prev_addr = begin(addr_list);
        curr_mask = begin(addr_list)->getMask();

        remove(begin(addr_list), end(addr_list), ip_address("255.255.255.255/32"));
        if (currsize == addr_list.size()) break;
        else currsize = addr_list.size();
    }
    
}
