enum Buying {
    unselected,
    vhigh,
    high,
    med,
    low
};

enum Maint {
    unselected,
    vhigh,
    high,
    med,
    low
};

enum Doors {
    unselected,
    two,
    three,
    four,
    five_or_more
};

enum Persons {
    unselected,
    two,
    four,
    more
};

enum Lug_boot {
    unselected,
    small,
    med,
    big
};

enum Safety {
    unselected,
    low,
    med,
    high
};

#define BUYING_LO 0
#define BUYING_HI 2
#define MAINT_LO 3
#define MAINT_HI 5
#define DOORS_LO 6
#define DOORS_HI 8
#define PERSONS_LO 9
#define PERSONS_HI 10
#define LUG_BOOT_LO 11
#define LUG_BOOT_HI 12
#define SAFETY_LO 13
#define SAFETY_HI 14

inline set_mask(int mask, int lo, int hi, int val) {
    
} 

pair<int,int> parse_input(vector<string> &tokens) {
    assert(tokens.size() == 6); // 6 attributes

    // buying

}

void input_dataset(string filepath, ordered_set<pair<int,int>> &os) {
    
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


    }

    

    
}
