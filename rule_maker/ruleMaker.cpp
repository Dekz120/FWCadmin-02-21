#include "ruleMaker.h"

//#define RM_CLASS_DEBUG

    fw_rule::fw_rule(const string& ZBFW_name, const string& src_iface, const string& dst_iface, string spec_comment,
            bool src_iface_in_dir, bool dst_iface_out_dir ,list<const ip_address*> src, list<const ip_address*> dst, bool push_r)
    {
        fw_name = ZBFW_name;
        intface_src = src_iface; src_in_direction = src_iface_in_dir;
        intface_dst = dst_iface; dst_out_direction = dst_iface_out_dir;
        comment = spec_comment;
        push_rule = push_r;
        ip_address tmp_ip;
        for_each(src.begin(), src.end(),
                 [&](const ip_address* ip_ptr)
                 {
                     tmp_ip = *ip_ptr;
                     src_list.push_back(tmp_ip);
                 });
        for_each(dst.begin(), dst.end(),
                 [&](const ip_address* ip_ptr)
                 {
                     tmp_ip = *ip_ptr;
                     dst_list.push_back(tmp_ip);
                 });



    }
    void fw_rule::display()
    {
        cout << fw_name << endl;
        cout << intface_src << endl;
        cout << (push_rule) << " Push status" << endl;;
        for_each(src_list.begin(), src_list.end(),
                 [&](ip_address IP)
                 {
                     IP.display();
                 });
        cout << intface_dst << endl;
        for_each(dst_list.begin(), dst_list.end(),
                 [&](ip_address IP)
                 {
                     IP.display();
                 });
    }

    bool fw_rule::operator ==(const fw_rule compRule)
    {
        if(fw_name == compRule.fw_name && intface_src == compRule.intface_src &&
           intface_dst == compRule.intface_dst && push_rule == compRule.push_rule)
        {
        return true;
        }
        else {return false;}
    }



///
void mergeRules(fw_rule& rule1, fw_rule& rule2)
{
        rule1.src_list.merge(rule2.src_list);
        rule1.src_list.unique();
        rule1.dst_list.merge(rule2.dst_list);
        rule1.dst_list.unique();
}
///
void mergeRulesList(vector<fw_rule>& fwFulesList, list<fw_rule>& newRulesList)
{
    int p = 0;
    int j=1;
    bool isAppend = false;
    while(j < fwFulesList.size() )
    {

        if(j < fwFulesList.size() && fwFulesList[p] == fwFulesList[j])
        {
                mergeRules(fwFulesList[p], fwFulesList[j++]);
                isAppend = false;
        }
        else
        {

            newRulesList.push_back(fwFulesList[p]);
            p = j ; j++;
            isAppend = true;
        }
        
    }
    if (!isAppend) { newRulesList.push_back(fwFulesList[p]); }
}
///
fw_rule CreateRule(const context_n_ipPtr &current_fw, const context_n_ipPtr &other_fw, bool role) //current fw is an object, which we make a rule for
{                                                                                      // objects also has to know theirs role(src/dst)
    fw_rule users_rule;
    fw_interactions cur_intact;
    cur_intact = (current_fw.fbfw_ptr)->getInteractRule(other_fw.fw_name);
    string interact_iface_name;
    bool push_rule = true;
    /// create rule for Source context
    if(role)
    {
        if(cur_intact.any_allow_to_zone) {push_rule = false;}
        else
        {
            if(!cur_intact.allow_to_zone) {push_rule = false;}
        }

        (current_fw.fbfw_ptr)->searchIPaddr(**other_fw.iface_roup_iter.begin(), interact_iface_name , false);// get the first address in dst list(one is enough to define fw-interface)
        ///here we know interfaces direction. for exm. inside => outside
        return fw_rule(current_fw.fw_name, current_fw.intface_name, interact_iface_name,
                            cur_intact.special_comment,
                            (current_fw.fbfw_ptr)->getFwDirection(current_fw.intface_name, true),
                            (current_fw.fbfw_ptr)->getFwDirection(interact_iface_name, false),
                            current_fw.iface_roup_iter,
                            other_fw.iface_roup_iter,
                            push_rule                             );
    }
    else     ///Create for destination context case
    {
        if(cur_intact.any_allow_from_zone) {push_rule = false;}
        else
        {
            if(!cur_intact.allow_from_zone) {push_rule = false;}
        }
        (current_fw.fbfw_ptr)->searchIPaddr(**other_fw.iface_roup_iter.begin(), interact_iface_name , false);// get the first address in dst list(one is enough to define fw-interface)
        ///here we know interfaces direction. for exm. inside -> outside
        return fw_rule(current_fw.fw_name,
                       interact_iface_name,
                       current_fw.intface_name,
                            cur_intact.special_comment,
                            (current_fw.fbfw_ptr)->getFwDirection(interact_iface_name, true),
                            (current_fw.fbfw_ptr)->getFwDirection(current_fw.intface_name, false),
                            other_fw.iface_roup_iter,
                            current_fw.iface_roup_iter,
                            push_rule                             );
    }
}
