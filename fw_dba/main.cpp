///This is fluent example how to create configuration files.
///I take users's "txt" files(examples in /fwdb directory) to create configuration files by using serialization.
///Further binary ".conf" files are created by this code. They contains Zone-based fw objects, which are using by "Rule_maker" programm.
///So this file depend from fw_dbadmin.cpp. If you want to change the management conception(for example, using sql db instead simple files),
///you have to pay attention on serialization functions in fw_dbadmin.cpp and rewrite "read_config_file" function.



#include "fw_dbadmin.h"
#include <Windows.h>
#include <string>
#include <algorithm>


//#define MK_DB_DEBUG

zone_based_fw read_config_file(ifstream &ifs, list<string> &k_list, list<fw_interactions> &interact_rules_list) // We have to put lists outside function to avoid reading to few places
{

    string tmp_r;
    char bool_char_tmp;
    getline(ifs, tmp_r, ' ');
#ifdef MK_DB_DEBUG
    cout << tmp_r << " is name of FW" << tmp_r.size() << endl;
#endif // MK_DB_DEBUG

    zone_based_fw zbfw(tmp_r);///
#ifdef MK_DB_DEBUG
    cout << "name "  << endl;
    zbfw.get_fw_name();
    cout << "-name "  << endl;
#endif // MK_DB_DEBUG

///
    while(ifs)
        {
             tmp_r.clear();
             getline(ifs, tmp_r);
             //cout << tmp_r << " " << tmp_r.find("interactions:") << "preInteractions"<<endl;
             if(tmp_r.find("interactions:") != string::npos) {break;}

        }
    list<string>::iterator k_iter;
    list<fw_interactions>::iterator ia_iter;

    fw_interactions tmp_interact;
#ifdef MK_DB_DEBUG
    cout << "Star reading interactions" << endl ;
#endif // MK_DB_DEBUG
    string str_special_comment;
    while(ifs)
        {

            tmp_r.clear();
            getline(ifs, tmp_r, ' '); ///
            if(tmp_r.find("endInteractions") != string::npos)
                {
#ifdef MK_DB_DEBUG
                    cout << "End of Interactions" << endl;
#endif // MK_DB_DEBUG
                    break;
                }

            ia_iter = interact_rules_list.end();
            ifs >> bool_char_tmp;

            if(bool_char_tmp == '0')
                {
                    tmp_interact.allow_from_zone = false;
                }
                else if(bool_char_tmp == '1')
                {
                    tmp_interact.allow_from_zone = true;

                }
            ifs >> bool_char_tmp;
                if(bool_char_tmp == '0')
                {
                    tmp_interact.allow_to_zone = false;
                }
                else if(bool_char_tmp == '1')
                {
                    tmp_interact.allow_to_zone = true;

                }
            ifs >> bool_char_tmp;
                if(bool_char_tmp == '0')
                {
                    tmp_interact.any_allow_from_zone = false;
                }
                else if(bool_char_tmp == '1')
                {
                    tmp_interact.any_allow_from_zone = true;

                }
            ifs >> bool_char_tmp;
                if(bool_char_tmp == '0')
                {
                    tmp_interact.any_allow_to_zone = false;
                }
                else if(bool_char_tmp == '1')
                {
                    tmp_interact.any_allow_to_zone = true;

                }
            str_special_comment.clear();
            getline(ifs, str_special_comment); 
            memset(tmp_interact.special_comment, 0, 50);
            for(int i =0; i < str_special_comment.size(); i++ )
            {tmp_interact.special_comment[i] = str_special_comment[i];}  


            k_list.push_back(tmp_r);
            interact_rules_list.push_back(tmp_interact);


            str_special_comment.clear();




        }

    k_iter = k_list.begin();
    ia_iter = interact_rules_list.begin();
#ifdef MK_DB_DEBUG
    cout << "Copying interaction rules to the Object..." << endl;
#endif // MK_DB_DEBUG
    while(k_iter != k_list.end())
        {
            zbfw.putInteractRule((char*)k_iter->c_str(), *ia_iter);
            k_iter++; ia_iter++;
        }

    while(ifs)
        {
            tmp_r.clear();
            getline(ifs, tmp_r);

            if(tmp_r.find("interfaces:") != string::npos) {break;}
        }

#ifdef MK_DB_DEBUG
    cout << "Start to read interfaces" << endl;
#endif // MK_DB_DEBUG
    tmp_r.clear();
    while(!ifs.eof())
        {


            string intface_name;

            while(ifs)
            {
                getline(ifs, tmp_r);

                if(tmp_r.find("interface:") != string::npos) {break;}
                tmp_r.clear();


            }

            getline(ifs, intface_name, ' '); 
            
            string tmp_ip;
            getline(ifs, tmp_ip, ' ') ;
            
            
            char bool_char_tmp2;
            char bool_char_tmp3;

            ifs >> bool_char_tmp;
            ifs >> bool_char_tmp2;
            ifs >> bool_char_tmp3;


            zbfw.AddInterface(intface_name.c_str(), ip_address(tmp_ip), (bool_char_tmp == '1') ? true : false, (bool_char_tmp2 == '1') ? true : false, (bool_char_tmp3 == '1') ? true : false);
        ///

            getline(ifs, tmp_r); 

            while(ifs)
            {
                tmp_ip.clear();
                getline(ifs, tmp_ip);
                if(tmp_ip.find("endInterface") != string::npos)
                    {
#ifdef MK_DB_DEBUG
                        cout << "End ip interface read" << endl;
#endif // MK_DB_DEBUG
                        break;
                    }
#ifdef MK_DB_DEBUG
                cout << tmp_ip << "Zoned IP" << endl;
#endif // MK_DB_DEBUG
                zbfw.AddIPaddr(intface_name, ip_address(tmp_ip));

            }
            intface_name.clear();
            tmp_ip.clear();
            tmp_r.clear();
        }
    return zbfw;
}


int main(int argc, char* argv[])
{


        list<zone_based_fw> firewalls;
        list<string> files_list;
        wstring tmp_wstr;


        list<string> key_list; list<fw_interactions> interaction_rules_list;

    zone_based_fw Zbfw1;

    WIN32_FIND_DATAW context_file;

    HANDLE const hFind = FindFirstFileW(L"fwdb\\*.txt", &context_file);

    ifstream read_conf;
    string fw_name;
    if (INVALID_HANDLE_VALUE != hFind)
    {
        do
        {
            tmp_wstr = &context_file.cFileName[0];

            cout << "Reading" << endl << "fwdb\\" + string(tmp_wstr.begin(), tmp_wstr.end()) << endl;

            read_conf.open("fwdb\\" + string(tmp_wstr.begin(), tmp_wstr.end()));
            if(read_conf.good())
            {
                Zbfw1 = read_config_file(read_conf, key_list, interaction_rules_list);
#ifdef MK_DB_DEBUG
                cout << "Write to file: " << endl;
#endif // MK_DB_DEBUG

                fw_name.clear();
                fw_name = Zbfw1.return_fw_name();
#ifdef MK_DB_DEBUG
                cout << fw_name << endl << endl;
#endif // MK_DB_DEBUG
                ofstream record("rules\\" + fw_name + ".conf", ios::binary);
                Zbfw1.serialize(record);
#ifdef MK_DB_DEBUG
                cout << "Test before serialization: " << endl;
                Zbfw1.get_interact_rules("prod");
#endif // MK_DB_DEBUG

                record.close();
            }
            else{cout << "Bad File" << endl; break;}
            read_conf.close();
            key_list.clear();
            interaction_rules_list.clear();
        } while (NULL != FindNextFileW(hFind, &context_file));

        FindClose(hFind);


    }

    return 0;
}
