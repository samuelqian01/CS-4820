#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <utility>
using namespace std;

int main() {
    std::ios::sync_with_stdio(false);
    string input_received;
    vector <vector <string> > parsed;
    int count = 0;
    int m;
    int n;
    while (getline(cin, input_received)) {
        vector <string> parsed_inside;
        stringstream ss(input_received);
        string character;
        while (ss >> character) {
            parsed_inside.push_back(character);
        }
        if (count == 0) {
            m = stoi(parsed_inside[0]);
            n = stoi(parsed_inside[1]); 
        }
        else{
            parsed.push_back(parsed_inside);
        }
        count += 1;
    }
    //cout << row_del_counter << endl;
    //m = m - row_del_counter;
    vector <vector <string> > grid (m, vector<string> (n));
    vector <string> empty (n);
    for (int i = 0; i < parsed.size(); i++) {
        for (int j = 0; j < parsed[i][0].length(); j++) {
            grid[i][j] = parsed[i][0][j];
        }
    }
    vector<vector<vector<vector<bool > > > > preprocess_h(m + 1, vector<vector<vector<bool> > >(m + 1, vector<vector<bool> >(n+1, vector<bool>(n+1))));
    vector<vector<vector<vector<bool > > > > preprocess_p(m + 1, vector<vector<vector<bool> > >(m + 1, vector<vector<bool> >(n+1, vector<bool>(n+1))));
    //preprocessing columns first, iterate through each columns, iterating fully through the rows on each column iteration.
    for (int sc = 0; sc < n; sc++) {
        for (int sr = 0; sr < m; sr++) { //goes up to index m - 1
            for (int er = sr + 1; er < m+1; er++) {
                if (er == sr + 1) { //square cases
                    if (grid[sr][sc] == "h") {
                        preprocess_h[sr][er][sc][sc + 1] = true;
                    }
                    else {
                        preprocess_h[sr][er][sc][sc + 1] = false;
                    }
                    if (grid[sr][sc] == "p") {
                        preprocess_p[sr][er][sc][sc + 1] = true;
                    }
                    else {
                        preprocess_p[sr][er][sc][sc + 1] = false;          
                    }
                }
                else {
                    if (preprocess_h[sr][er - 1][sc][sc + 1] == true || grid[er - 1][sc] == "h") {
                        preprocess_h[sr][er][sc][sc + 1] = true;
                    }
                    else {
                        preprocess_h[sr][er][sc][sc + 1] = false;
                    }
                    if (preprocess_p[sr][er - 1][sc][sc+1] == true || grid[er - 1][sc] == "p") {
                        preprocess_p[sr][er][sc][sc + 1] = true;
                    }
                    else {
                        preprocess_p[sr][er][sc][sc + 1] = false;
                    }
                }
            }
        }
    }
    //Now we iterate through each row, doing each horizontal strip.
    for (int sr = 0; sr < m; sr++) {
        for (int sc = 0; sc < n; sc++) { //goes up to index n - 1
            for (int ec = sc + 1; ec < n+1; ec++) {
                if (ec != sc + 1) { //non-square cases, squares have all been processed already
                    if (preprocess_h[sr][sr + 1][sc][ec - 1] == true || grid[sr][ec - 1] == "h") {
                        preprocess_h[sr][sr + 1][sc][ec] = true;
                    }
                    else {
                        preprocess_h[sr][sr + 1][sc][ec] = false;
                    }
                    if (preprocess_p[sr][sr + 1][sc][ec - 1] == true || grid[sr][ec - 1] == "p") {
                        preprocess_p[sr][sr + 1][sc][ec] = true;
                    }
                    else {
                        preprocess_p[sr][sr + 1][sc][ec] = false;
                    }
                }
            }
        }
    }
    //Now we use recurrence to build the columns into subrectangles
    for (int sr = 0; sr < m; sr++) {
        for (int er = sr + 1; er < m + 1; er++) {
            for (int sc = 0; sc < n; sc++) {
                for (int ec = sc + 1; ec < n + 1; ec++) {
                    if (ec != sc + 1) { //if not one column, recurrence relation of newest column and all prior sols.
                        if (preprocess_h[sr][er][sc][ec - 1] == true || preprocess_h[sr][er][ec - 1][ec] == true) {
                            preprocess_h[sr][er][sc][ec] = true;
                        }
                        else {
                            preprocess_h[sr][er][sc][ec] = false;
                        }
                        if (preprocess_p[sr][er][sc][ec - 1] == true || preprocess_p[sr][er][ec - 1][ec] == true) {
                            preprocess_p[sr][er][sc][ec] = true;
                        }
                        else {
                            preprocess_p[sr][er][sc][ec] = false;
                        }
                    }
                }
            }

        }
    }
    //dp array for tabulation
    vector<vector<vector<vector<int > > > > dp_array(m + 1, vector<vector<vector<int> > >(m + 1, vector<vector<int> >(n + 1, vector<int>(n + 1))));
    //All values pre appended to 0
    //dp solution
    for (int h = 0; h < m; h++) { //possible height added to the start row
        for (int r = 1; r <= m - h; r++) {  //start row
            for (int w = 0; w < n; w++) {  //possible width added to the start col
                for (int c = 1; c <= n - w; c++) { //starting col
                    if (preprocess_h[r - 1][h + r][c - 1][w + c] == true && preprocess_p[r - 1][h + r][c - 1][w + c] == true) {
                        //if there are cuts to be made, check the vertical and horizontal cuts, recurrence relation is 1 + min(both)
                        int minimum = 100000000;
                        for (int a = c; a <= w + c - 1; a++) {
                            int s1 = dp_array[r - 1][h + r][c - 1][a]; 
                            int s2 = dp_array[r - 1][h + r][a][w + c];
                            int calculated = s1 + s2;
                            if (calculated < minimum) {
                                minimum = calculated;
                            }
                        }
                        for (int a = r; a <= h + r - 1; a++) {
                            int s1 = dp_array[r - 1][a][c - 1][w + c];
                            int s2 = dp_array[a][h + r][c - 1][w + c];
                            int calculated = s1 + s2;
                            if (calculated < minimum) {
                                minimum = calculated;
                            }
                        }
                        dp_array[r - 1][h + r][c - 1][w + c] = 1 + minimum; //recurrence relation
                    }
                }
            }
        }
    }
    cout << dp_array[0][m][0][n] << endl;
    return 0;
}


