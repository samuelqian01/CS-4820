#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <utility>
using namespace std;

//Check if any free proposers and whether they've proposed to everyone
int checkFreeProp (vector <int>& prop_matching, vector <vector <int> >& proposed_to, int& n) {
    for (int i = 0; i < n; i++) {
        if (prop_matching[i] == -1 && proposed_to[i].size() < n) {
            return i;
        }
    }
    return -1;
}

bool checkpref (vector <int> sorted_prefs, int p, int p_prime) {
    if (sorted_prefs[p] < sorted_prefs[p_prime]) {
        return true;
    }
    return false;
}

int main() {
    std::ios::sync_with_stdio(false);
    string input_received;
    vector< vector<int> > parsed;
    //add each input to the vector
    while (getline(cin, input_received)) {
        vector <int> parsed_inside;
        stringstream ss(input_received);
        string num;
        while (ss >> num) {
            parsed_inside.push_back(stoi(num));
        }
        parsed.push_back(parsed_inside);
    }
    //n proposers and n responders
    int n = parsed[0][0]; 
    //m forbidden pairs of form (prop, resp)
    int m = parsed[0][1];
    //proposer matching vector, -1 means free
    vector <int> prop_matching (n);
    //Respondant matching vector, -1 means free
    vector <int> resp_match (n);
    //fill vectors initially
    fill(prop_matching.begin(), prop_matching.end(), -1);
    fill(resp_match.begin(), resp_match.end(), -1);
    vector <vector <int> > proposed_to (n);
    //Flip the respondant preferences vectors in the parse vector in order to allow for O(1) preference checking for respondant
    vector <vector <int> > sorted_prefs (n, vector<int> (n));
    for (int i = 1 + n; i < 1 + (2 * n); i++) {
        for (int a = 0; a < n; a++) {
            sorted_prefs[i - 1 - n][parsed[i][a]] = a;
        }
    } 

    //2D matrix storing forbidden pairs
    vector <vector <int> > arr2d_forbid (n, vector<int> (n));
    //Adding forbidden pairs to the matrix, -2 indicates the forbidden pair
    for (int i = parsed.size() - m; i < parsed.size(); i++) {
        arr2d_forbid[parsed[i][0]][parsed[i][1]] = -2;
    }

    //Gale-Shapley Algorithm with Forbidden Pairs in O(n^2) efficiency
    while (true) {
        int p = checkFreeProp (prop_matching, proposed_to, n);
        if (p == -1) {  
            break;
        } 

        //Condition where you should not propose to someone you proposed to before
        //taken care by the natural loop progression
        for (int i = 0; i < n && prop_matching[p] == -1; i++) {
            int r = parsed[p + 1][i];
            proposed_to[p].push_back(r);
            if (arr2d_forbid[p][r] != -2){
                if (resp_match[r] == -1) {
                    prop_matching[p] = r;
                    resp_match[r] = p;
                }
                else if (resp_match[r] != -1) {
                    if (checkpref(sorted_prefs[r], p, resp_match[r])) {
                        prop_matching[resp_match[r]] = -1;
                        prop_matching[p] = r;
                        resp_match[r] = p;
                    }
                }
            }
        }
    }
    //Check if stable
    for (int i = 0; i < prop_matching.size(); i++) {
        if (prop_matching[i] == -1) {
            cout << "No" << endl;
            return 0;
        }
    }
    cout << "Yes" << endl;
    for (int i = 0; i < resp_match.size(); i++) {
        cout << resp_match[i] << endl;
    }
    return 0;
}