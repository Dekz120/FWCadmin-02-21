#ifndef RULEMAKER
#define RULEMAKER

#include <iostream>
#include <list>
#include <vector>
#include "fw_dbadmin.h"

#include <typeinfo>

struct context_n_ipPtr{
    string fw_name; //
    string intface_name; //
    list<const ip_address*> iface_roup_iter; //One pointer to group
    zone_based_fw *fbfw_ptr;
    //bool role; // 0-sourse 1-dest

};
///

class fw_rule{  ///obj suppose to fit for many diffirent fw types. I use paloAlto cisco iptables
private:
    string fw_name;

    string comment;

    string intface_src;
    bool src_in_direction; // true. if there is active direction on config

    string intface_dst;
    bool dst_out_direction; //some ngfw doesnt pay attention to interface traffic direction, but they demands both interfaces for one rule
                            // i'll describe this case in README
    bool push_rule; //false, if interaction is illegal, or one zone, or already opened

    list<ip_address> src_list;
    list<ip_address> dst_list;
public:
    fw_rule(){};
    fw_rule(const string& ZBFW_name, const string& src_iface, const string& dst_iface, string spec_comment,
            bool src_iface_in_dir, bool dst_iface_out_dir ,list<const ip_address*> src, list<const ip_address*> dst, bool push_r);

    void display();

    bool operator ==(const fw_rule compRule);
    //friend fw_rule merge_rules(fw_rule& rule1, fw_rule& rule2);
    friend void mergeRules(fw_rule& rule1, fw_rule& rule2);
};
///
void mergeRulesList(vector<fw_rule>& fwFulesList, list<fw_rule>& newRulesList);
///
fw_rule CreateRule(const context_n_ipPtr &current_fw, const context_n_ipPtr &other_fw, bool role);
#endif
