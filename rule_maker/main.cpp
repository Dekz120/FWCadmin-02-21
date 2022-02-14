///This main function creates fw_rule objects, which contains all information you need(except some describe) to create a rule in real firewall
///like a PaloAlto Cisco or ip_tables.
///I don't have any functions to Push some configuration on real firewall In this moment.


//#define RULE_MAKER_DEBUG
using namespace std;

#include "ruleMaker.h"


void CreateFwStructures(const ip_address &searching_ip, vector<zone_based_fw> &firewalls, list<context_n_ipPtr> &cnt_ptr_list) //passion through all contexts and defi
{

    context_n_ipPtr cnt_ip;
    string fw_iface;
    const ip_address *ip_ptr;

    for(int i =0; i<firewalls.size(); i++)
    {



        bool tmp_bool = firewalls[i].searchIPaddr(searching_ip, fw_iface, 1);
        if(tmp_bool == true)
        {

            auto cnt_ptr_list_iter = find_if(cnt_ptr_list.begin(), cnt_ptr_list.end(),
                    [&](context_n_ipPtr& cnt_ip1)
                        {

                            return (cnt_ip1.fw_name == firewalls[i].return_fw_name() && cnt_ip1.intface_name == fw_iface); // лень, пусть пока будет регистрочувствительным поиск

                        } );

            if(cnt_ptr_list_iter != cnt_ptr_list.end() && cnt_ptr_list_iter->fw_name == firewalls[i].return_fw_name() && cnt_ptr_list_iter->intface_name == fw_iface)
            {

                ip_ptr = &searching_ip;
                (cnt_ptr_list_iter->iface_roup_iter).push_back(ip_ptr);
            } // add to old prt-group
            else // add a new ptr-groud
            {

            cnt_ip.fw_name = firewalls[i].return_fw_name();
            cnt_ip.intface_name = fw_iface;
            cnt_ip.fbfw_ptr = &firewalls[i];

            ip_ptr = &searching_ip;

            (cnt_ip.iface_roup_iter).push_back(ip_ptr);
            ip_ptr = NULL;
            cnt_ptr_list.push_back(cnt_ip);


            }
        }


        (cnt_ip.fw_name).clear();
        (cnt_ip.intface_name).clear();
        (cnt_ip.iface_roup_iter).clear();
        fw_iface.clear();
    }


};

///
void getTicketFrFile(list<ip_address> &src_ip, list<ip_address> &dst_ip) // example of getting ticket from file. Instead file it could be anything
{
    ifstream ticket_file("ticket_file.txt", ios::binary);

        string tmpstr;
            while(tmpstr.find("dest_addr_list") == string::npos)
            {
                getline(ticket_file, tmpstr, ' ');
            }
            int pos_file;
            getline(ticket_file, tmpstr, ' ');

            pos_file = ticket_file.tellg();
            ticket_file.seekg(0);


            ParseIPfromFile(src_ip, ticket_file, pos_file);
            ticket_file.seekg(pos_file);
            ParseIPfromFile(dst_ip, ticket_file, 2000000 );

            ipSumm(src_ip);
            ipSumm(dst_ip);


}
///

void getAllInteractions(vector<zone_based_fw> &FW_list)
    {


    wstring tmp_wstr;
    WIN32_FIND_DATAW context_file;
    HANDLE const hFind2 = FindFirstFileW(L"..\\FW_DBA\\rules\\*.conf", &context_file);


    zone_based_fw zbfw;
    list<zone_based_fw>::iterator iter; // Debug
///
    int i = 0;
    if (INVALID_HANDLE_VALUE != hFind2)
    {
        do
        {
            tmp_wstr = &context_file.cFileName[0];

            ifstream read_some("..\\FW_DBA\\rules\\" + string(tmp_wstr.begin(), tmp_wstr.end()), ios::binary);



            FW_list[i].deserialize(read_some);
            read_some.close();

            i++;




        } while (NULL != FindNextFileW(hFind2, &context_file));

        FindClose(hFind2);


    }
    }
///

///
void initTicketList(const list<context_n_ipPtr>& sourse_contexts, const list<context_n_ipPtr>& dst_contexts,
                    vector<fw_rule>& src_rules, vector<fw_rule>& dst_rules)
{
    list<context_n_ipPtr>::const_iterator scIter;
    scIter = sourse_contexts.begin();

    list<context_n_ipPtr>::const_iterator dcIter;
    dcIter = dst_contexts.begin();
    int i =0;

    while(scIter != sourse_contexts.end())
    {   int j =0;
        while(dcIter != dst_contexts.end())
        {

            src_rules[i*dst_contexts.size() + j] = CreateRule(*scIter, *dcIter, true); // if "true" we'll get rules where scIter is source
            dst_rules[j*sourse_contexts.size() + i] = CreateRule(*dcIter++, *scIter, false);
            j++;
        }
        scIter++;

        dcIter = dst_contexts.begin();
        i++;
    }

}



int main()
{
    vector<zone_based_fw> all_firewalls(15);

    getAllInteractions(all_firewalls);

    list<ip_address> source_ip; //Will read to these lists
    list<ip_address> dest_ip;
    getTicketFrFile(source_ip, dest_ip);

    list<ip_address>::iterator iter_ip;
    iter_ip = dest_ip.begin();

    list<context_n_ipPtr> sourse_contexts; // elements contains pointer to ip
    list<context_n_ipPtr> dest_contexts;

    for_each(source_ip.begin(), source_ip.end(),
                [&](ip_address& IP)
                        {
                            CreateFwStructures(IP, all_firewalls, sourse_contexts);

                        });

    for_each(dest_ip.begin(), dest_ip.end(),
                [&](ip_address& IP)
                        {
                            CreateFwStructures(IP, all_firewalls, dest_contexts);

                        });


    vector<fw_rule> src_rules(sourse_contexts.size() * dest_contexts.size());
    vector<fw_rule> dst_rules(sourse_contexts.size() * dest_contexts.size());

    initTicketList(sourse_contexts, dest_contexts, src_rules, dst_rules);


    list<fw_rule> srcRules;
    list<fw_rule> dstRules;
    mergeRulesList(src_rules, srcRules);
    mergeRulesList(dst_rules, dstRules);


    cout << "Display Merged rules(SRC RULES): \n"  << endl;

    for_each(srcRules.begin(), srcRules.end(),
             [&](fw_rule& mergedRule)
             {
                 mergedRule.display();
             });


    cout << endl << "Display Merged rules(DST RULES): \n" << endl;
    for_each(dstRules.begin(), dstRules.end(),
             [&](fw_rule& mergedRule)
             {
                 mergedRule.display(); cout << endl;
             });

    return 0;
}
