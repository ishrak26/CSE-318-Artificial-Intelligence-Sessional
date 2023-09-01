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

void build_decision_tree(ordered_set<map<string, string>> &ds) {
    ;
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
