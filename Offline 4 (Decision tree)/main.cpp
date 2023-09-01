#include <bits/stdc++.h>
// ordered_set
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

#define EXP_CNT 20
#define TEST_SZ 20

using namespace std;
using namespace __gnu_pbds;

template<class T> using ordered_set =tree<T, null_type, less<T>, rb_tree_tag,tree_order_statistics_node_update>;

const string FILEPATH = "./car evaluation dataset/car.data";

const double EPS = 1e-8;

ordered_set<map<string, string>> original_dataset, train_data, test_data;
vector<string> attrs;

map<string, set<string>> attr_vals;

void prepare_attr_vals() {
    attrs = vector<string>{"buying","maint","doors","persons","lug_boot","safety","value"};
    
    attr_vals["buying"] = set<string>{"vhigh","high","med","low"};
    attr_vals["maint"] = set<string>{"vhigh","high","med","low"};
    attr_vals["doors"] = set<string>{"2","3","4","5more"};
    attr_vals["persons"] = set<string>{"2","4","more"};
    attr_vals["lug_boot"] = set<string>{"small","med","big"};
    attr_vals["safety"] = set<string>{"low","med","high"};
    attr_vals["value"] = set<string>{"unacc","acc","good","vgood"};
}

void parse_input(vector<string> &tokens, map<string, string> &mp) {
    assert(tokens.size() == attrs.size()); // 6 attributes + 1 value

    for (int i = 0; i < attrs.size(); i++) {
        assert(attr_vals[attrs[i]].find(tokens[i]) != attr_vals[attrs[i]].end());
        mp[attrs[i]] = tokens[i];
    }
}

void input_dataset(string filepath) {
    
    ifstream fin;
    fin.open(filepath, ios::in);

    string input;

    while (fin >> input) {
        char delimiter = ',';
        istringstream ss(input);
        string token;
        vector<string> tokens;

        while (getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }

        map<string, string> mp;
        parse_input(tokens, mp);
        original_dataset.insert(mp);
    }
}

string root_attr;
map<string, pair<bool,string>> decision_mp; // (isLeaf, next_attr)

string build_decision_tree(ordered_set<map<string, string>> ds, set<string> rem_attrs, double h_par) {
    // find next_attribute among remaining attributes applicable for ds

    double info_gain = -1.0;
    string sel_attr; // selected attribute
    map<string, map<string, double>> new_hpar; // h_par to be passed after an attribute is selected
    /*
        say, new_hpar["buying"]["vhigh"] = 0.46 means entropy for the branch "vhigh" for the attribute "buying"
        is 0.46
    */
    
    // iterate over all remaining attributes to find which one should be selected now
    for (set<string>::iterator it = rem_attrs.begin(); it != rem_attrs.end(); it++) {
        string attr = *it; // e.g. buying, maint, doors, etc.
        map<string,map<string,int>> cnts;
        /*
            say, cnts["vhigh"]["acc"] = 3, means for the current attribute (say, "buying"), under the given ds,
            there are 3 examples where value is "acc"
        */

        // iterate over the given dataset to find count of results
        for (ordered_set<map<string, string>>::iterator it2 = ds.begin(); it2 != ds.end(); it2++) {
            map<string, string> mp = *it2; // mp is a data, as given in dataset ds
            string val = mp[attr];
            cnts[val][mp["value"]]++;
        }

        // calculate remainder of this attr
        double rem = 0.0;
        for (map<string,map<string,int>>::iterator it2 = cnts.begin(); it2 != cnts.end(); it2++) {
            /*
                for an attribute "buying",
                it2->first can be "vhigh"
                it2->second can be a map containing ("acc",3),("unacc",4)... 
            */
            
            // calculate entropy for this branch
            double h = 0.0; 
            double tot = 0.0;
            for (map<string,int>::iterator it3 = it2->second.begin(); it3 != it2->second.end(); it3++) {
                tot += it3->second;
            }
            for (map<string,int>::iterator it3 = it2->second.begin(); it3 != it2->second.end(); it3++) {
                /*
                    it3->first cam be "acc","unacc"...
                    it3->second is the no. of examples in ds whose value is it3->first
                */
                double p = it3->second / tot;
                h -= p * log2(p);
            }
            rem += (tot / ds.size()) * h;
            new_hpar[attr][it2->first] = h;
        }
        double gain = h_par - rem;
        if (gain > info_gain) {
            info_gain = gain;
            sel_attr = attr;
        }
    }

    assert(!sel_attr.empty());

    rem_attrs.erase(sel_attr);

    if (rem_attrs.empty()) {
        return sel_attr;
    }

    map<string, ordered_set<map<string, string>>> new_ds;
    /*
        say, sel_attr is "buying"
        new_ds["vhigh"] will have the subset of ds, whose "buying" value is "vhigh"
    */

    for (ordered_set<map<string, string>>::iterator it = ds.begin(); it != ds.end(); it++) {
        map<string, string> mp = *it;
        string val = mp[sel_attr]; // val can be "vhigh" for sel_attr "buying"
        new_ds[val].insert(mp);
    }

    // build decision for each branch of sel_attr
    for (set<string>::iterator it = attr_vals[sel_attr].begin(); it != attr_vals[sel_attr].end(); it++) {
        if (abs(new_hpar[sel_attr][*it]) < EPS) {
            // entropy 0
            // leaf
            // decision_mp[sel_attr] = make_pair(true, new_ds[*it].begin());
            //TODO
        }
        
        string ret = build_decision_tree(new_ds[*it], rem_attrs, new_hpar[sel_attr][*it]);
        decision_mp[sel_attr] = make_pair(false, ret);
    }

    return sel_attr;
}

int main() {
    prepare_attr_vals();
    
    input_dataset(FILEPATH);

    srand(1);
    
    for (int i = 1; i <= EXP_CNT; i++) {
        train_data = original_dataset;
        test_data.clear();
        // prepare test dataset
        int sz = (original_dataset.size() * TEST_SZ) / 100;
        for (int j = 0; j < sz; j++) {
            int r = rand() % train_data.size();
            ordered_set<map<string, string>>::iterator it = train_data.find_by_order(r);
            test_data.insert(*it);
            train_data.erase(it);
        }
    }

    return 0;
}
