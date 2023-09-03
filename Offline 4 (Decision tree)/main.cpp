#include <bits/stdc++.h>

#define EXP_CNT 20
#define TEST_SZ 20

using namespace std;

const string FILEPATH = "./car evaluation dataset/car.data";

const double EPS = 1e-8;

vector<map<string, string>> original_dataset, train_data, test_data;
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
        original_dataset.push_back(mp);
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

int node_cnt;
int leaf_cnt;

/*
    ds: dataset under consideration
    rem_attrs: set of remaining attributes to be applied now
    cur: current node under consideration
    h_par: entropy of parent
    par_plu: plurality of parent
*/
void build_decision_tree(vector<map<string, string>> ds, set<string> rem_attrs, Node *cur, double h_par, string par_plu) {
    // find next_attribute among remaining attributes applicable for ds

    double info_gain = -1.0;
    string sel_attr; // selected attribute
    map<string, map<string, string>> plu; 
    /*
        plu["buying"]["vhigh"] = "acc" means plurality for "vhigh" branch of "buying" attribute will be "acc"
    */
    map<string, map<string, double>> new_hpar; // h_par to be passed after an attribute is selected
    /*
        say, new_hpar["buying"]["vhigh"] = 0.46 means parent entropy for the branch "vhigh" for the attribute "buying"
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
        for (map<string, string> mp : ds) {
            // mp is a data, as given in dataset ds
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

                node_cnt++;
                leaf_cnt++;
                
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
                
                if (cur->child.find(*it2) != cur->child.end()) {
                    // node already created
                    continue;
                }

                node_cnt++;
                leaf_cnt++;
                
                cur->child[*it2] = new Node(cur);
                
                cur->child[*it2]->isLeaf = true;
                cur->child[*it2]->decision = plu[attr][*it2];
            }

            // if decision for every branch is same, make cur a leaf
            bool flag = true;
            bool fi = true;
            string dec;
            for (set<string>::iterator it2 = attr_vals[attr].begin(); it2 != attr_vals[attr].end(); it2++) {
                if (fi) {
                    dec = cur->child[*it2]->decision;
                    fi = false;
                }
                else {
                    if (cur->child[*it2]->decision != dec) {
                        flag = false;
                        break;
                    }
                }
            }
            if (flag) {
                // decision for all branches are same
                for (set<string>::iterator it2 = attr_vals[attr].begin(); it2 != attr_vals[attr].end(); it2++) {
                    delete cur->child[*it2];
                    node_cnt--;
                    leaf_cnt--;
                }
                cur->isLeaf = true;
                cur->decision = dec;
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
            node_cnt++;
            
            cur->child[*it2] = new Node(cur);
                
            cur->child[*it2]->isLeaf = true;
            cur->child[*it2]->decision = plu[sel_attr][*it2];
        }
        return;
    }

    // create new branches from here
    map<string, vector<map<string, string>>> new_ds;
    /*
        say, sel_attr is "buying"
        new_ds["vhigh"] will have the subset of ds, whose "buying" value is "vhigh"
    */

    for (map<string, string> mp : ds) {
        string val = mp[sel_attr]; // val can be "vhigh" for sel_attr "buying"
        new_ds[val].push_back(mp);
    }

    // build decision tree for each branch of sel_attr
    cur->isLeaf = false;
    for (set<string>::iterator it = attr_vals[sel_attr].begin(); it != attr_vals[sel_attr].end(); it++) {
        
        node_cnt++;
        cur->child[*it] = new Node(cur);
        build_decision_tree(new_ds[*it], rem_attrs, cur->child[*it], new_hpar[sel_attr][*it], plu[sel_attr][*it]);
    }

    return;
}

void build_decision_tree() {
    // calculate initial entropy
    
    // calculate frequency of each decision value
    map<string, int> cnt;
    for (map<string, string> mp : train_data) {
        cnt[mp["value"]]++;
    }
    double h = 0.0;
    int max_cnt = -1;
    string plu = "acc";
    for (set<string>::iterator it = attr_vals["value"].begin(); it != attr_vals["value"].end(); it++) {
        if (cnt[*it] > max_cnt) {
            max_cnt = cnt[*it];
            plu = *it;
        }
        // cerr << *it << " " << cnt[*it] << endl;
        double p = ((double)(cnt[*it])) / train_data.size();
        h -= p * log2(p);
    }
    // cout << "plu is " << plu << endl;
    
    set<string> rem_attrs;
    for (int i = 0; i < 6; i++) { 
        rem_attrs.insert(attrs[i]);
    }
    node_cnt++;
    root = new Node;
    // cerr << "Initial entropy: " << h << endl;
    build_decision_tree(train_data, rem_attrs, root, h, plu);
}

string test_by_decision_tree(map<string,string> &mp) {
    Node *cur = root;
    while (!cur->isLeaf) {
        cur = cur->child[mp[cur->attr]];
    }
    return cur->decision;
}

void print_decision_tree(Node *cur) {
    if (cur->isLeaf) {
        cout << cur->decision << endl;
        return;
    }
    cout << cur->attr << endl;
    for (set<string>::iterator it = attr_vals[cur->attr].begin(); it != attr_vals[cur->attr].end(); it++) {
        cout << "taking the branch " << *it << " for attr " << cur->attr << endl;
        print_decision_tree(cur->child[*it]);
    }
}

int main() {
    prepare_attr_vals();
    
    input_dataset(FILEPATH);

    srand(time(nullptr));

    int num = 0, den = 0;
    int sum_nodes = 0, sum_leaves = 0;

    vector<double> res(EXP_CNT);

    // cout << fixed << setprecision(2);
    
    for (int i = 1; i <= EXP_CNT; i++) {
        random_shuffle(original_dataset.begin(), original_dataset.end());
        train_data.clear();
        test_data.clear();
        // prepare test dataset
        int sz = (original_dataset.size() * TEST_SZ) / 100;
        set<int> indices;
        while (indices.size() < sz) {
            int r = rand() % original_dataset.size();
            indices.insert(r);
        }
        vector<bool> mark(original_dataset.size());
        for (int idx : indices) {
            mark[idx] = 1;
        }
        for (int i = 0; i < original_dataset.size(); i++) {
            if (mark[i]) {
                test_data.push_back(original_dataset[i]);
            }
            else {
                train_data.push_back(original_dataset[i]);
            }
        }

        // cout << train_data.size() << " " << test_data.size() << endl;

        node_cnt = leaf_cnt = 0;

        build_decision_tree();

        sum_nodes += node_cnt;
        sum_leaves += leaf_cnt;;

        // print_decision_tree(root);

        int cnt = 0;
        for (map<string, string> mp : test_data) {
            string ret = test_by_decision_tree(mp);
            if (ret == mp["value"]) {
                cnt++;
            }
        }
        num += cnt;
        den += test_data.size();

        res[i-1] = (cnt / (double)(test_data.size()));

        cout << "Accuracy for experiment no. " << i << ": " << res[i-1]*100.0 << "%" << endl;

        delete root;
    }

    double mean = (num / (double)(den));

    cout << "Mean accuracy: " << mean*100 << "%" << endl;

    // calculate std deviation
    double sum = 0.0;
    for (int i = 0; i < EXP_CNT; i++) {
        sum += (mean - res[i]) * (mean - res[i]);
    }
    double deviation = sqrt(sum / EXP_CNT);
    cout << "Standard Deviation: " << deviation*100 << "%" << endl;

    cout << "Averaage no. of nodes in decision tree is " << sum_nodes/EXP_CNT << ", where no. of leaf nodes is " << sum_leaves / EXP_CNT << endl;

    return 0;
}
