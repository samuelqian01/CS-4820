#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <utility>
using namespace std;
int min_sol_horz(vector<vector<vector<vector<bool > > > > preprocess_h, vector<vector<vector<vector<bool > > > > preprocess_p, vector<vector<vector<vector<int > > > > dp_array, int h, int w, int r, int c);
int min_sol_vert(vector<vector<vector<vector<bool > > > > preprocess_h, vector<vector<vector<vector<bool > > > > preprocess_p, vector<vector<vector<vector<int > > > > dp_array, int h, int w, int r, int c);
bool cut(vector<vector<vector<vector<bool > > > > preprocess_h, vector<vector<vector<vector<bool > > > > preprocess_p, int a, int b, int c, int d);

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
    vector <vector <string> > grid (m, vector<string> (n));
    for (int i = 0; i < parsed.size(); i++) {
        for (int j = 0; j < parsed[i][0].length(); j++) {
            grid[i][j] = parsed[i][0][j];
        }
    }
    //stores whether there is a hazlenut/peanut or not in intervals (start row, end row, start column, end column), requires 1 extra row and column for each
    vector<vector<vector<vector<bool > > > > preprocess_h(m + 1, vector<vector<vector<bool> > >(m + 1, vector<vector<bool> >(n+1, vector<bool>(n+1))));
    vector<vector<vector<vector<bool > > > > preprocess_p(m + 1, vector<vector<vector<bool> > >(m + 1, vector<vector<bool> >(n+1, vector<bool>(n+1))));
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (grid[i-1][j-1] == "p") {
                preprocess_p[i-1][i][j-1][j] = true;
            }
            else if (grid[i-1][j-1] == "h") {
                preprocess_h[i-1][i][j-1][j] = true;
            }
        }
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < m - 1; j++) {
            for (int k = j + 2; k <= m; k++) {
                if (preprocess_p[j][k-1][i-1][i] == true|| preprocess_p[k-1][k][i-1][i] == true) {
                    preprocess_p[j][k][i-1][i] = true;
                }
                else {
                    preprocess_p[j][k][i-1][i] = false;
                }
                if (preprocess_h[j][k-1][i-1][i] == true|| preprocess_h[k-1][k][i-1][i] == true) {
                    preprocess_h[j][k][i-1][i] = true;
                }
                else {
                    preprocess_h[j][k][i-1][i] = false;
                }
            }
        }
    }
    for (int i = 1; i <= m; i++) {
        for (int j = 0; j < n-1; j++) {
            for (int k = j + 2; k <= n; k++) {
                preprocess_p[i-1][i][j][k] = preprocess_p[i-1][i][j][k-1] == true || preprocess_p[i-1][i][k-1][k] == true;
                preprocess_h[i-1][i][j][k] = preprocess_h[i-1][i][j][k-1] == true || preprocess_h[i-1][i][k-1][k] == true;
            }
        }
    }
    for (int i = 0; i < m-1; i++) {
        for (int j = i + 2; j <= m; j++) {
            for (int k = 0; k < n - 1; k++) {
                for (int l = k + 2; l <= n; l++) {
                    preprocess_p[i][j][k][l] = preprocess_p[i][j][k][l-1] || preprocess_p[i][j][l-1][l];
                    preprocess_h[i][j][k][l] = preprocess_h[i][j][k][l-1] || preprocess_h[i][j][l-1][l];
                }
            }
        }
    }

    for (int i = 1; i < m + 1; i++) {
        cout << preprocess_h[0][i][0][10] << endl;
    }
    //dp table
    vector<vector<vector<vector<int > > > > dp_array(m + 1, vector<vector<vector<int> > >(m + 1, vector<vector<int> >(n+1, vector<int>(n+1))));
    for (int i = 0; i < m + 1; i++) {
        for (int j = 0; j < m + 1; j++) {
            for (int k = 0; k < n + 1; k++) {
                for (int l = 0; l < n + 1; l++) {
                    dp_array[i][j][k][l] = 0;
                }
            }
        }
    }
    //dp solution
    for (int i = 1; i <= m; i++) {
        for (int j = 0; j <= m-i; j++) {
            for (int k = 1; k <= n; k++) {
                for (int l = 0; l <= n - k; l++) {
                    if (preprocess_h[j][j+i][l][l+k] == true && preprocess_h[j][j+i][l][l+k] == true) {
                        int currentBest = m * n;
                        for (int o = j + 1; o <= j + i - 1; o++) {
                            int sum = 1 + dp_array[j][o][l][l+k] + dp_array[o][j+i][l][l+k];
                            cout << currentBest << endl;
                            if (sum < currentBest) {
                                currentBest = sum;
                            }
                        }
                        for (int o = l + 1; o <= l + k - 1; o++) {
                            int sum = 1 + dp_array[j][j+i][l][o] + dp_array[j][j+i][o][l+k];
                            if (sum < currentBest) {
                                currentBest = sum;
                            }
                        }
                        dp_array[j][j+i][l][l+k] = currentBest;
                    }
                }
            }
        }
    }
    cout << dp_array[0][m][0][n] << endl;

    return 0;

}
bool cut(vector<vector<vector<vector<bool > > > > preprocess_h, vector<vector<vector<vector<bool > > > > preprocess_p, int a, int b, int c, int d) {
    if (preprocess_h[a][b][c][d] && preprocess_p[a][b][c][d]) {
        return true;
    }
    return false;
}
int min_sol_horz(vector<vector<vector<vector<bool > > > > preprocess_h, vector<vector<vector<vector<bool > > > > preprocess_p, vector<vector<vector<vector<int > > > > dp_array, int h, int w, int r, int c) {
    int minimum = 10000000;
    for (int a = r + 1; a <= h + r - 1; a++) {
        int s1 = dp_array[r][a][c][w + c];
        int s2 = dp_array[a][h + r][c][w + c];
        int calculated = 1 + s1 + s2;
        if (calculated < minimum) {
            minimum = calculated;
        }
    }
    return minimum;
}
int min_sol_vert(vector<vector<vector<vector<bool > > > > preprocess_h, vector<vector<vector<vector<bool > > > > preprocess_p, vector<vector<vector<vector<int > > > > dp_array, int h, int w, int r, int c) {
    int minimum = 10000000;
    for (int a = c + 1; a <= w + c - 1; a++) {
        int s1 = dp_array[r][h + r][c][a];
        int s2 = dp_array[r][h + r][c][w + c];
        int calculated = 1 + s1 + s2;
        cout << calculated << endl;
        if (calculated < minimum) {
            minimum = calculated;
        }
    }
    return minimum;
}



            
        
    



