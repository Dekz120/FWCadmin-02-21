
#include "fw_dbadmin.h"
#include "ip_address.h"
//#define FW_DBA_DEBUG

using namespace std;

///Tempalte functions for data serialization
template<typename TL>                                       
void writeList(list<TL>& some_list, ostream& os)
{
typename list<TL>::iterator iter;
iter = some_list.begin();

while(iter != some_list.end() && os) {

                os.write(reinterpret_cast<char*>(&(*iter)), sizeof(*iter));
#ifdef FW_DBA_DEBUG
                cout  << os.tellp() << " - is position of list element"  << endl;
#endif // FW_DBA_DEBUG
                iter++;}
}
////
template<typename TL>
void readList(list<TL>& some_list, ifstream& is, int& read_lengh)
{
        TL read_to;
#ifdef FW_DBA_DEBUG
        cout << "List template reading " << read_lengh << " elements" << endl;
#endif // FW_DBA_DEBUG
        for(int i=0; i < read_lengh && is; i++) {
                is.read(reinterpret_cast<char*>(&read_to), sizeof(TL));
                some_list.push_back(read_to);
                }
}
///
///                                                 }
template<typename T_KEY, typename T_ELEM>
void UMkeyElemWrite(unordered_map<T_KEY, T_ELEM>& some_map, ofstream& os, list<T_KEY>& key_list)
{ //
    typename list<T_KEY>::iterator list_iter;
    list_iter = key_list.begin();

#ifdef FW_DBA_DEBUG
    cout << "Start to write Key and Element of UM" << endl;
#endif // FW_DBA_DEBUG
    int key_size;
    while(list_iter != key_list.end() && os) {
#ifdef FW_DBA_DEBUG
        cout << os.tellp() << " write key position" << endl;
#endif // FW_DBA_DEBUG
        key_size = list_iter->size();
        os.write(reinterpret_cast<char*>(&key_size), sizeof(int));
#ifdef FW_DBA_DEBUG
        cout << key_size << " write key size" << endl;
#endif // FW_DBA_DEBUG
        for(int i=0; i < key_size && os ; i++ ) {os.write(reinterpret_cast<char*>(&(   (*list_iter)[i])    ), sizeof((*list_iter)[i])); }
#ifdef FW_DBA_DEBUG
        cout << endl << "Writing key " <<  *list_iter << ' ' << sizeof(list_iter) << " bytes size." << endl;
#endif // FW_DBA_DEBUG

        os.write(reinterpret_cast<char*>(&some_map[*list_iter]), sizeof(some_map[*list_iter]));

        list_iter++; }
}
///
 // I have to divide read-function for "key" because promblem with reading to c_str
string UMkeyRead(ifstream& is)
{ //

        char tmp[15];
        int r_size;
#ifdef FW_DBA_DEBUG
        cout << is.tellg() << " read key position" << endl;
#endif // FW_DBA_DEBUG
        is.read(reinterpret_cast<char*>(&r_size), sizeof(r_size));
#ifdef FW_DBA_DEBUG
        cout << r_size << "-keysize" << endl;
#endif // FW_DBA_DEBUG
        int i =0;
        for(i=0; i < r_size && is; i++ ) {is.read(reinterpret_cast<char*>(&tmp[i]), sizeof(tmp[i])); }
        //is.read(reinterpret_cast<char*>(&tmp), r_size);
        tmp[i] = '\0';
#ifdef FW_DBA_DEBUG
        cout << tmp << " key has been read" << endl;
#endif // FW_DBA_DEBUG
        return tmp;
}
template<typename T_ELEM>
void UMelemRead(ifstream& is, list<T_ELEM>& some_map)
{ //

    T_ELEM map_element;
        is.read(reinterpret_cast<char*>(&map_element), sizeof(T_ELEM));
#ifdef FW_DBA_DEBUG
        cout << map_element.special_comment << " UM element has been read" << sizeof(map_element) << "bytes" <<  endl;
#endif // FW_DBA_DEBUG
        some_map.push_back(map_element);
}
///

    void fw_interface::serialize(ofstream& os)
    {

        os.write(reinterpret_cast<char*>(&interface_name), sizeof(interface_name));
#ifdef FW_DBA_DEBUG
        cout << "Interface " <<  interface_name << " has been written" << endl;
#endif // FW_DBA_DEBUG
        os.write(reinterpret_cast<char*>(&interface_ip), sizeof(interface_ip)); 
#ifdef FW_DBA_DEBUG
        cout << "Management ip has been written " << endl;
#endif // FW_DBA_DEBUG

        os.write(reinterpret_cast<char*>(&direction_in), sizeof(direction_in)); 
        os.write(reinterpret_cast<char*>(&direction_out), sizeof(direction_out));
        os.write(reinterpret_cast<char*>(&zone_role), sizeof(zone_role));

#ifdef FW_DBA_DEBUG
        cout << "Interface parameters has been written " << endl;
#endif // FW_DBA_DEBUG



        int iplist_size = ntworks_behing_int.size();
        os.write(reinterpret_cast<char*>(&iplist_size), sizeof(iplist_size));
#ifdef FW_DBA_DEBUG
        cout << "Preparation to write " <<ntworks_behing_int.size() <<" ip addresses on position " << os.tellp() << endl;
        cout << "Start to write ip addresses" << endl;
#endif // FW_DBA_DEBUG
        writeList(ntworks_behing_int, os);

    }
    void fw_interface::deserialize(ifstream& is)
    {
        int read_lengh;

        is.read(reinterpret_cast<char*>(&interface_name), sizeof(interface_name));
#ifdef FW_DBA_DEBUG
        cout << "Start to read " << interface_name << " interface data" << endl;
#endif // FW_DBA_DEBUG

        is.read(reinterpret_cast<char*>(&interface_ip), sizeof(ip_address)); 
#ifdef FW_DBA_DEBUG
        cout << is.tellg() << " position" << endl;
#endif // FW_DBA_DEBUG

        is.read(reinterpret_cast<char*>(&direction_in), sizeof(bool)); 
        is.read(reinterpret_cast<char*>(&direction_out), sizeof(bool));
        is.read(reinterpret_cast<char*>(&zone_role), sizeof(bool));
#ifdef FW_DBA_DEBUG
        cout << "Interface parameters has been read " << endl;
#endif // FW_DBA_DEBUG

        is.read(reinterpret_cast<char*>(&read_lengh), sizeof(int));

        ip_address ip_read;
#ifdef FW_DBA_DEBUG
        cout << "Preparation to read " << read_lengh << " ip addresses" << endl;
#endif // FW_DBA_DEBUG
        if(read_lengh != 0) { readList(ntworks_behing_int, is, read_lengh);}
#ifdef FW_DBA_DEBUG
        cout << is.tellg() << " end read position of current interface" << endl;
#endif // FW_DBA_DEBUG
     }
     ///
     /// Функуции там
    fw_interface::fw_interface(){};
    fw_interface::~fw_interface()
    {
#ifdef FW_DBA_DEBUG
        cout << "~Destroyed interface " << interface_name << endl;
#endif // FW_DBA_DEBUG
    }

    fw_interface::fw_interface(char* name)
    {
        memset(interface_name, 0, 15);
        strcpy(interface_name, name) ;
#ifdef FW_DBA_DEBUG
        cout << interface_name << " interface has been created" << endl;
#endif // FW_DBA_DEBUG
    } //


    void fw_interface::put_int_name(char* name) { strcpy(interface_name, name);}  //

    string fw_interface::getIntfaceName(){string tmpstr = interface_name; return tmpstr;}

    void fw_interface::AddIpToIntface(ip_address&& IP){ ntworks_behing_int.push_back(IP) ;}

    void fw_interface::AddIpToIntface(ip_address& IP)    {        ntworks_behing_int.push_back(IP) ;    }
///
    bool fw_interface::ipAddrSearch(const ip_address& IP) // ip searching behind fw-context
    {
        auto ip_list_iter = find_if(ntworks_behing_int.begin(), --ntworks_behing_int.end(), // duplicate check;
                            [&](ip_address& ip_adr){
                                ip_address tmp_ip = networkCalculation(IP, ip_adr.getMask());
#ifdef FW_DBA_DEBUG
                                IP.display(); cout << " IP CURR"<<endl;
                                ip_adr.display(); cout << " FOUNED IP"<<endl;
                                tmp_ip.display(); cout << " BOOL_AND IP"<<endl;
#endif // FW_DBA_DEBUG

                                return tmp_ip == ip_adr; //
                            });
#ifdef FW_DBA_DEBUG
        ip_list_iter->display();
#endif // FW_DBA_DEBUG
        if(*ip_list_iter != zero_ip)
        {
#ifdef FW_DBA_DEBUG
                cout << "Found IP" << endl;
#endif // FW_DBA_DEBUG
                return true;
        }
        else
        {
#ifdef FW_DBA_DEBUG
                cout << "Didnt found IP" << endl;
#endif // FW_DBA_DEBUG
                return false;
        }

    }

    bool fw_interface::isZoned()
    {
        if(zone_role){return true;}
        else return false;
    }
    void fw_interface::Display()
    {

        cout << "Interface: " << interface_name << endl;
        list<ip_address>::iterator iter;
        iter = ntworks_behing_int.begin();
        while(iter != ntworks_behing_int.end() )
        {
            cout << "Network " << endl;
            iter->display(); iter++; }
        if(this->isZoned()) {cout << "ZonedIface" << endl;}
        else {cout << "NoTZonedIface" << endl;}
    }


    zone_based_fw::zone_based_fw()
    {
#ifdef FW_DBA_DEBUG
         cout << "Not named FW was created" << endl;
#endif // FW_DBA_DEBUG
    }
    zone_based_fw::~zone_based_fw() {
#ifdef FW_DBA_DEBUG
        cout << "~Destroyed Firewall obj " << endl;
#endif // FW_DBA_DEBUG
         };

    zone_based_fw::zone_based_fw(string& cont_name){
        memset(fw_name, 0 , 15);
        for(int i =0; i < cont_name.size(); i++ )
        {fw_name[i] = cont_name[i];} } //
       /// putters
    void zone_based_fw::AddInterface(const char* intface_name, const bool in, const bool out, const bool is_zone)
    { //

        interface_list.push_back(move(fw_interface(intface_name, in, out, is_zone))); //
#ifdef FW_DBA_DEBUG
        cout << "Add interface " << intface_name << endl;
#endif // FW_DBA_DEBUG
        }
    ////
    void zone_based_fw::AddInterface(const char* intface_name, ip_address &&m_ip ,const bool in, const bool out, const bool is_zone) // Добавление интерфейса с r-value адресом ip
    { // Создаем интерфейс

        interface_list.push_back(move(fw_interface(intface_name, m_ip, in, out, is_zone))); //
#ifdef FW_DBA_DEBUG
        cout << "Add interface " << intface_name << endl;
#endif // FW_DBA_DEBUG
        }
    ///
    void zone_based_fw::AddIPaddr( string intface_name, ip_address& ip)
    { //

        auto intf_list_iter = find_if(interface_list.begin(), interface_list.end(),
                            [&](fw_interface& intf){

                                return intf.getIntfaceName() == intface_name; 

                                } );

        if(intface_name == intf_list_iter->getIntfaceName()) { intf_list_iter->AddIpToIntface(ip);}
        else cout << "Interface " << intface_name << " isnt found" << endl;
    }
    ///
    void zone_based_fw::AddIPaddr( string intface_name, ip_address &&ip)
    { //

        auto intf_list_iter = find_if(interface_list.begin(), interface_list.end(),
                            [&](fw_interface& intf){

                                return intf.getIntfaceName() == intface_name;

                                } );

        if(intface_name == intf_list_iter->getIntfaceName()) { intf_list_iter->AddIpToIntface(ip);}
        else cout << "Interface " << intface_name << " isnt found" << endl;
    }
    void zone_based_fw::getIfacesInfo(){
        list<fw_interface>::iterator iter;
        iter = interface_list.begin();
        while(iter != interface_list.end()){iter->Display(); iter++;}


    }

/////////////////
    void zone_based_fw::putInteractRule(string other_context, fw_interactions interaction)
    {


#ifdef FW_DBA_DEBUG
        cout << "Push " << other_context << endl;
#endif // FW_DBA_DEBUG
        key_list.push_back(other_context);
        Interact_rules[other_context] = interaction;

    };
//////

/// getterы
    void zone_based_fw::getInteractRules(char other_context[15])
    {
         list<string>::iterator iter;
         iter = key_list.begin() ;
#ifdef FW_DBA_DEBUG
         cout << key_list.size() << "Number of keys" << endl;
         cout << Interact_rules.size() << "Number of elems" << endl;
#endif // FW_DBA_DEBUG
         while(iter != key_list.end()){ cout<< (*iter) << ' ' ;
            cout << Interact_rules[*iter].special_comment << endl;
                iter++;}


    }
    ///
    bool zone_based_fw::getFwDirection(const string& intface_name, bool dir_in) //if true. get IN_DIR request, else out direction
    { //

        auto intf_list_iter = find_if(interface_list.begin(), interface_list.end(),
                            [&](fw_interface& intf){
                                return intf.getIntfaceName() == intface_name; //
                                } );

        if(intface_name == intf_list_iter->getIntfaceName()) { return intf_list_iter->isActiveDirection(dir_in);}
        else {cout << "Interface " << intface_name << " isnt found" << endl; return false;}
    }
    ///

    fw_interactions zone_based_fw::getInteractRule(string other_context)
    {
        return Interact_rules[(char*)other_context.c_str()];
    }
    ///
    void zone_based_fw::getInterfacesInfo()
    {   list<fw_interface>::iterator iter;
        iter = interface_list.begin();
        while(iter != interface_list.end())
        {
            iter->Display();iter++;
        }
    }
    ////
    bool zone_based_fw::searchIPaddr(const ip_address& ip, string &iface_name,  const bool zone_consider_search)
    { //
#ifdef FW_DBA_DEBUG
        cout << "IP searching in " << this->return_fw_name() << endl;
        if(zone_consider_search) {cout << "Bool check true" << endl;}
        else {cout << "Bool check false" << endl;}

#endif // FW_DBA_DEBUG

        bool is_found = false;
        list<fw_interface>::iterator intf = interface_list.begin();
        while(intf != interface_list.end())
                                {

#ifdef FW_DBA_DEBUG
                                    cout << "Interface: " << intf->getIntfaceName() << endl;
#endif // FW_DBA_DEBUG
                                    if(intf->isZoned() || !zone_consider_search)
                                    {
                                        bool tmp_bool2 = intf->ipAddrSearch(ip);
                                        if(tmp_bool2)
                                        {
#ifdef FW_DBA_DEBUG
                                            cout <<"Found context____________________________" << endl;
#endif // FW_DBA_DEBUG
                                            iface_name = intf->getIntfaceName();
                                            is_found = true;
                                            return true;
                                        }
                                    }

                                    intf++;

                                }
        if(!is_found)
        {
#ifdef FW_DBA_DEBUG
                cout << "Not found IP in whole context" << endl;
#endif // FW_DBA_DEBUG
        }
        return is_found;
    }


    ///
    void zone_based_fw::getFWname(){ cout << fw_name << endl;}
    char* zone_based_fw::return_fw_name(){  return fw_name;}

    void zone_based_fw::serialize(ofstream& ofs)
    {
#ifdef FW_DBA_DEBUG
        cout << "Writing " << fw_name << " to position: " << ofs.tellp()<< endl;
#endif // FW_DBA_DEBUG
        ofs.write(reinterpret_cast<char*>(&fw_name), sizeof(fw_name));
#ifdef FW_DBA_DEBUG
        cout << "Writing number of interfaces to position: " << ofs.tellp()<< endl;
#endif // FW_DBA_DEBUG
        int size_of_interface_list = interface_list.size(); 
        ofs.write(reinterpret_cast<char*>(&size_of_interface_list), sizeof(size_of_interface_list));
#ifdef FW_DBA_DEBUG
        cout << interface_list.size() << " interfaces should be written" << endl;
#endif

        list<fw_interface>::iterator iter;
        iter  = interface_list.begin();    
        while(iter != interface_list.end()){
#ifdef FW_DBA_DEBUG
                cout << "Writing interface to position: " << ofs.tellp()<< endl;
#endif // FW_DBA_DEBUG
            iter->serialize(ofs); iter++;}
#ifdef FW_DBA_DEBUG
        cout << "Writing number of interaction rules to position: " << ofs.tellp()<< endl;
#endif // FW_DBA_DEBUG
        ofs << Interact_rules.size();
#ifdef FW_DBA_DEBUG
        cout << Interact_rules.size() << " map elements should be written" << endl;
#endif // FW_DBA_DEBUG
        UMkeyElemWrite(Interact_rules, ofs, key_list);
#ifdef FW_DBA_DEBUG
        cout << endl << "End Of Serialization" << endl;
#endif // FW_DBA_DEBUG
    }

     void zone_based_fw::deserialize(ifstream& ifs)
     {
#ifdef FW_DBA_DEBUG
        cout << ifs.tellg() << "Position to star read" << endl;
#endif // FW_DBA_DEBUG
        ifs.read(reinterpret_cast<char*>(&fw_name), sizeof(fw_name));
#ifdef FW_DBA_DEBUG
        cout << "Context name "<< fw_name << " has been read" << endl;
#endif // FW_DBA_DEBUG

        int read_len;
        fw_interface* tmp_int;
#ifdef FW_DBA_DEBUG
        cout << ifs.tellg() << "Position to read number of interfaces" << endl;
#endif // FW_DBA_DEBUG
        ifs.read(reinterpret_cast<char*>(&read_len), sizeof(int));
#ifdef FW_DBA_DEBUG
        cout << "Preparation to read "<< read_len << " interfaces of firewall" << endl;
#endif // FW_DBA_DEBUG
        for (int i=0; i < read_len; i++) { 
        tmp_int = new fw_interface;
        tmp_int->deserialize(ifs);
        interface_list.push_back(*tmp_int);
        delete tmp_int;}
#ifdef FW_DBA_DEBUG
        cout << ifs.tellg() << "Position to read number of interactions" << endl;
#endif // FW_DBA_DEBUG
        ifs >> read_len;
#ifdef FW_DBA_DEBUG
        cout << "Preparation to read " << read_len << " interactions of firewall" << endl;
#endif // FW_DBA_DEBUG


        list<string> list_for_keyRead; //
        string tmp_str;
        char* tmp_char;

        list<fw_interactions> list_for_elem_read;

        for(int i =0; i < read_len; i++){
            tmp_str = UMkeyRead(ifs);

            list_for_keyRead.push_back(tmp_str);
            tmp_char = (char*)tmp_str.c_str();

            UMelemRead(ifs, list_for_elem_read);
            }

        list<string>::iterator l_iter;
        l_iter = list_for_keyRead.begin();

        list<fw_interactions>::iterator e_iter;
        e_iter = list_for_elem_read.begin();
#ifdef FW_DBA_DEBUG
        cout << "Copying interaction rules to the Object..." << endl;
#endif // FW_DBA_DEBUG
        while( l_iter != list_for_keyRead.end() ) {
                key_list.push_back((char*)l_iter->c_str()) ;
                Interact_rules[(char*)l_iter->c_str()] = *e_iter;
                l_iter++; e_iter++;}
    }

void zone_based_fw::clearFWdata() 
    {
    interface_list.clear();
    Interact_rules.clear();
    memset(fw_name, 0,sizeof(fw_name) );
    key_list.clear();

    }




