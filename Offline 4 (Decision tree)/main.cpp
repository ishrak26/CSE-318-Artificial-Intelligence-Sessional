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

inline int dcmp(double x) { if (fabs(x) < EPS) return 0; else return x < 0 ? -1 : 1; }

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

struct Node {
    map<string, Node*> child; // one child node for each branch
    Node *par;
    string attr;
    bool isLeaf;
    string decision;

    Node() {
        
    }

    Node(Node *par) : par(par) {}

    ~Node() {
        if (!isLeaf) {
            for (set<string>::iterator it2 = attr_vals[attr].begin(); it2 != attr_vals[attr].end(); it2++) {
                delete child[*it2];
            }
        }   
    }
};

Node *root;

/*
    ds: dataset under consideration
    rem_attrs: set of remaining attributes to be applied now
    par: parent node
    cur: current node under consideration
*/
void build_decision_tree(ordered_set<map<string, string>> ds, set<string> rem_attrs, Node *cur, double h_par, string par_plu) {
    // find next_attribute among remaining attributes applicable for ds

    double info_gain = -1.0;
    string sel_attr; // selected attribute
    map<string, map<string, string>> plu; 
    /*
        plu["buying"]["vhigh"] = "acc" means plurality for "vhigh" branch of "buying" attribute will be "acc"
    */
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
        for (set<string>::iterator it2 = attr_vals[attr].begin(); it2 != attr_vals[attr].end(); it2++) {
            /*
                for an attribute "buying",
                *it2 can be "vhigh"
                cnts[*it2] can be a map containing ("acc",3),("unacc",4)... 
            */
            
            // calculate entropy for this branch
            double h = 0.0; 
            double tot = 0.0;
            for (map<string,int>::iterator it3 = cnts[*it2].begin(); it3 != cnts[*it2].end(); it3++) {
                tot += it3->second;
            }

            if (dcmp(tot) == 0) {
                // no example in the dataset for this branch
                // take plurality of the parent, and make this a leaf
                // entropy will be 0 for this branch
                new_hpar[attr][*it2] = 0.0;
                plu[attr][*it2] = par_plu;
                
                cur->child[*it2] = new Node(cur);
                
                cur->child[*it2]->isLeaf = true;
                cur->child[*it2]->decision = par_plu;

            }
            else {
                int plu_cnt = -1;
                string plu_val = "acc"; // default
                for (map<string,int>::iterator it3 = cnts[*it2].begin(); it3 != cnts[*it2].end(); it3++) {
                    /*
                        it3->first cam be "acc","unacc"...
                        it3->second is the no. of examples in ds whose value is it3->first
                    */
                    double p = it3->second / tot;
                    h -= p * log2(p);
                    if (it3->second > plu_cnt) {
                        plu_cnt = it3->second;
                        plu_val = it3->first;
                    }
                }
                rem += (tot / ds.size()) * h;
                new_hpar[attr][*it2] = h;
                plu[attr][*it2] = plu_val;
            }
        }

        // check if every branch leads to 0 entropy
        bool flag = true;
        for (set<string>::iterator it2 = attr_vals[attr].begin(); it2 != attr_vals[attr].end(); it2++) {
            if (dcmp(new_hpar[attr][*it2]) != 0) {
                flag = false;
                break;
            } 
        }
        if (flag) {
            // every branch leads to 0 entropy
            // no need to create new node here, rather create a leaf for every branch
            cur->attr = attr;
            cur->isLeaf = false;
            for (set<string>::iterator it2 = attr_vals[attr].begin(); it2 != attr_vals[attr].end(); it2++) {
                
                cur->child[*it2] = new Node(cur);
                
                cur->child[*it2]->isLeaf = true;
                cur->child[*it2]->decision = plu[attr][*it2];
            }
            return;
        }
        else {
            double gain = h_par - rem;
            if (gain > info_gain) {
                info_gain = gain;
                sel_attr = attr;
            }
        }
    }

    assert(!sel_attr.empty());

    cur->attr = sel_attr;

    rem_attrs.erase(sel_attr);

    if (rem_attrs.empty()) {
        // it was the last attribute
        // no need to create branch from here
        // assign decision to every branch of sel_attr
        
        
        cur->isLeaf = false;
        for (set<string>::iterator it2 = attr_vals[sel_attr].begin(); it2 != attr_vals[sel_attr].end(); it2++) {
            cur->child[*it2] = new Node(cur);
                
            cur->child[*it2]->isLeaf = true;
            cur->child[*it2]->decision = plu[sel_attr][*it2];
        }
        return;
    }

    // create new branches from here
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

    // build decision tree for each branch of sel_attr
    cur->isLeaf = false;
    for (set<string>::iterator it = attr_vals[sel_attr].begin(); it != attr_vals[sel_attr].end(); it++) {
        cur->child[*it] = new Node(cur);
        build_decision_tree(new_ds[*it], rem_attrs, cur->child[*it], new_hpar[sel_attr][*it], plu[sel_attr][*it]);
    }

    return;
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
