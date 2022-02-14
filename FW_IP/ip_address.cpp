#include "ip_address.h"

using namespace std;

//////
    ip_address::ip_address(){};

    ip_address::ip_address(int& o1, int& o2, int& o3, int& o4)
    {     
        oct1 = o1; oct2 = o2; oct3 = o3; oct4 = o4;
    }

    ip_address::ip_address(int& o1, int& o2, int& o3, int& o4, int mask)
    {    
        oct1 = o1; oct2 = o2; oct3 = o3; oct4 = o4; net_mask = mask;
    }

   ip_address::ip_address(string ip_addr2)
 {          

		int i = 0;


            string tmp;
			while (ip_addr2[i] != '.') {  
				 tmp.push_back(ip_addr2[i++]);
            } i++;
			oct1 = stoi(tmp);  tmp.clear();


			while (ip_addr2[i] != '.') {

				tmp.push_back(ip_addr2[i++]);
			} i++;
			oct2 = stoi(tmp);   tmp.clear();


			while (ip_addr2[i] != '.') {
				tmp.push_back(ip_addr2[i++]);
			} i++;

			oct3 = stoi(tmp);  tmp.clear();


			while (ip_addr2[i] != '/' && ip_addr2[i] != '\0') {
				tmp.push_back(ip_addr2[i++]);
			}

			oct4 = stoi(tmp);  tmp.clear();

            if ( ip_addr2[i] == '/' )
            {         ////
                    i++;

			while (i < ip_addr2.size()){
                    if(isdigit(ip_addr2[i]))
                    {
                        tmp.push_back(ip_addr2[i++]);
                    }
                    else{ cout << "IP pasing error" << endl;}
			}

			net_mask = stoi(tmp);
			}
			else net_mask = 32;

}

	void ip_address::display()
	{
	    cout << (int)oct1 << '.' << (int)oct2 << '.' << (int)oct3 << '.' << (int)oct4;
        if(net_mask < 32 ){ cout << '/' << (int)net_mask;}
        cout << endl;
    }

	int ip_address::display() const
	{
	    cout << (int)oct1 << '.' << (int)oct2 << '.' << (int)oct3 << '.' << (int)oct4;
        if(net_mask < 32 ){ cout << '/' << (int)net_mask;}
        cout << endl;
        return 0;
    }
/////
    bool ip_address::operator ==(const ip_address& adr2)
    {
    if (oct1 == adr2.oct1 && oct2 == adr2.oct2 && oct3 == adr2.oct3 && oct4 == adr2.oct4 && net_mask == adr2.net_mask) {return true;}
     else return false;    }
    ////////
    bool ip_address::operator !=(const ip_address& adr2)
    {
    if (oct1 == adr2.oct1 && oct2 == adr2.oct2 && oct3 == adr2.oct3 && oct4 == adr2.oct4 && net_mask == adr2.net_mask) {return false;}
    else return true;    }
    /////////
    bool ip_address::operator < (const ip_address& adr2)
    {
    if (oct1 < adr2.oct1) {return true;}
    else if( oct1 != adr2.oct1) return false;

    if (oct2 < adr2.oct2)  { return true;}
    else if( oct2 != adr2.oct2) return false;

    if (oct3 < adr2.oct3)  { return true;}
    else if( oct3 != adr2.oct3) return false;

    if (oct4 < adr2.oct4)  { return true;}
    else if( oct4 != adr2.oct4) return false;

    if (net_mask < adr2.net_mask)  { return true;}
    else return false;

    }
/////////////////
    char ip_address::getMask(){ return net_mask;}
    ////////////
    void ip_address::putMask(int msk){ net_mask = msk;}

//////////////
ip_address networkCalculation(const ip_address &ip_adr, int mask){
int mask_okt1;int mask_okt2;int mask_okt3;int mask_okt4;

switch(mask){
case 8: {mask_okt1 = 255; mask_okt2 = 0; mask_okt3 = 0; mask_okt4 = 0; break;}
case 16: {mask_okt1 = 255; mask_okt2 = 255; mask_okt3 = 0; mask_okt4 = 0; break;}
case 24: {mask_okt1 = 255; mask_okt2 = 255; mask_okt3 = 255; mask_okt4 = 0; break; }
case 32: {mask_okt1 = 255; mask_okt2 = 255; mask_okt3 = 255; mask_okt4 = 255;break;}
default: {
  if (mask < 8) { mask_okt1 = 256 - (int)pow(2, (8 - mask)); mask_okt2 = 0; mask_okt3 = 0; mask_okt4 = 0; }

  else  if (mask > 8 && mask < 16) { mask_okt1 = 255; mask_okt2 = 256 - (int)pow(2, (16 - mask)); mask_okt3 = 0; mask_okt4 = 0; }

   else if (mask > 16 && mask < 24) { mask_okt1 = 255; mask_okt2 = 255; mask_okt3 = 256 - (int)pow(2, (24 - mask)); mask_okt4 = 0; }

   else if (mask > 24 && mask < 32) { mask_okt1 = 255; mask_okt2 = 255; mask_okt3 = 255; mask_okt4 = 256 - (int)pow(2, (32 - mask)); } break;}}


int new_oct1, new_oct2, new_oct3, new_oct4;

new_oct1 = ip_adr.oct1 & mask_okt1 ;
new_oct2 = ip_adr.oct2 & mask_okt2;
new_oct3 = ip_adr.oct3 & mask_okt3;
new_oct4 = ip_adr.oct4 & mask_okt4;


ip_address temp(new_oct1, new_oct2, new_oct3, new_oct4, mask);
return temp;

}
///////
void UniqueNetworks(list<ip_address>& adr_lst) 
{
    ip_address base_address;
    ip_address addr_for_compare;

    int net_mask;
    int addr_counter=0;
    list<ip_address>::iterator iter2;
    iter2 = adr_lst.begin();
    net_mask = iter2->getMask();
    base_address = networkCalculation(*iter2++, net_mask);

    while(iter2 != adr_lst.end()){
        addr_for_compare = networkCalculation(*iter2, net_mask);

        if (base_address == addr_for_compare) {adr_lst.erase(iter2++); }
        else {net_mask = iter2->getMask(); base_address = networkCalculation(*iter2++, net_mask);}

    }

}
////////

void ParseIPfromFile(list<ip_address>& list_of_address1, ifstream &sourse_file, int end_posit)
{
    char ch;
    sourse_file >> ch;
        while( sourse_file && sourse_file.tellg() < end_posit)
        {
            while(!isdigit(ch) && sourse_file){
               sourse_file >> ch;
            }

            string potential_address;
            int points_counter=0;
            int oct_check=0;

            while(points_counter < 4 && oct_check < 4 && sourse_file.tellg() < end_posit )
            {
                if(ch == '.' || ch == '/' || isdigit(ch)){
                    if (ch == '.' ) { points_counter++; oct_check=0;}
                    if(isdigit(ch)) {oct_check++; }
                    if (ch == '/') {oct_check = 1; }
                    }
                else {sourse_file >>noskipws >> ch; break;}

                    potential_address.push_back(ch);

                sourse_file>>noskipws  >> ch;
            }

             if (points_counter == 3) { 

                list_of_address1.push_back(ip_address(potential_address));
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

    list<ip_address>::iterator iter;        
    iter = addr_list.begin();
    ip_address addr_for_compare;
    char tmp_mask;
    while(iter != addr_list.end())
    {
        tmp_mask = iter->getMask();
        iter++;
        if(tmp_mask == iter->getMask())
        {
            iter--;
            addr_for_compare = networkCalculation(*iter, tmp_mask-1);
            if(addr_for_compare == networkCalculation(*++iter, tmp_mask-1))
            {
                *iter = addr_for_compare;
                iter--;
                addr_list.erase(iter++);
                if(iter != addr_list.begin())
                {
                    iter--;
                }
            }
        }
        else {if(iter != addr_list.end()){iter++;} }
    }
}
