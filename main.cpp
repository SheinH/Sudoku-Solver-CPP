#include <iostream>
#include <cctype>
#include <vector>
#include <array>

using namespace std;

class Solution {
    struct PossibleSet {
        bool arr[9];
        int count;
        bool valid;

        void init() {
            count = 9;
            for (bool &b : arr)
                b = true;
            valid = true;
        }

        void add(char c) {
            if (!isdigit(c))
                return;
            if (arr[c - '1'])
                return;
            else {
                arr[c - '1'] = true;
                count++;
            }
        }

        void remove(char c) {
            if (!isdigit(c))
                return;
            if (!arr[c - '1'])
                return;
            arr[c - '1'] = false;
            count--;
        }

        vector<char> toArray() {
            vector<char> v(count);
            int i = 0;
            for (int j = 0; j < 9; j++) {
                if (arr[j]) {
                    v[i++] = (char) ('1' + j);
                }
            }
            return v;
        }

        bool operator[](char c) {
            return arr[c - '1'];
        }
    };

    struct Point {
        int y, x;

        friend ostream &operator<<(ostream &os, const Point &point) {
            os << "y: " << point.y << " x: " << point.x;
            return os;
        }
    };

    static Point get_square(Point p) {
        int y = (p.y / 3) * 3;
        int x = (p.x / 3) * 3;
        return {y, x};
    }

    static void print_grid(vector<vector<char>> grid) {
        for (const auto &c : grid) {
            for (auto c2 : c) {
                cout << c2 << ' ';
            }
            cout << endl;
        }
        cout << endl;
        cout << endl;
    }

    static array<Point, 20> neighbor_set(Point p) {
        array<Point, 20> arr;
        int i = 0;
        for (int y = 0; y < 9; y++) {
            if (y != p.y)
                arr[i++] = {y, p.x};
        }
        for (int x = 0; x < 9; x++) {
            if (x != p.x)
                arr[i++] = {p.y, x};
        }
        Point p2 = get_square(p);
        for (int dy = 0; dy < 3; dy++) {
            for (int dx = 0; dx < 3; dx++) {
                int y = p2.y + dy;
                int x = p2.x + dx;
                if (y != p.y && x != p.x)
                    arr[i++] = {y, x};
            }
        }
        return arr;
    }

    static PossibleSet get_possible(vector<vector<char>> &board, int y, int x) {
        PossibleSet ps;
        ps.init();
        if (y == 3 && x == 7) {
            auto s = "DEBUG TIME";
        }

        array<Point, 20> neighbors = neighbor_set({y, x});
        for (Point p : neighbors) {
            ps.remove(board[p.y][p.x]);
        }
        return ps;
    }

    static bool change_square(vector<vector<char>> &grid, array<array<PossibleSet, 9>, 9> &psg, Point p, char t) {
        char old = grid[p.y][p.x];
        bool returnval = true;
        if (t == '.') {
            grid[p.y][p.x] = t;
            auto neighbors = neighbor_set(p);
            for (Point n : neighbors) {
                if (!psg[n.y][n.x].valid)
                    continue;
                auto neighbors2 = neighbor_set(n);
                bool found_match = false;
                for (Point n2 : neighbors2) {
                    if (grid[n2.y][n2.x] == old) {
                        found_match = true;
                        break;
                    }
                }
                if (!found_match)
                    psg[n.y][n.x].add(old);
            }
            psg[p.y][p.x] = get_possible(grid, p.y, p.x);
            return true;
        } else {
            auto neighbors = neighbor_set(p);
            for (Point n : neighbors) {
                if (psg[n.y][n.x].valid) {
                    psg[n.y][n.x].remove(t);
                    if (psg[n.y][n.x].count == 0)
                        returnval = false;
                }
            }
            grid[p.y][p.x] = t;
            psg[p.y][p.x].valid = false;
            return returnval;
        }
    }

    static bool solve_rec(vector<vector<char>> &grid, array<array<PossibleSet, 9>, 9> &psg, int count) {
        if (count == 81)
            return true;
        Point p;
        int min_found = 10;
        for (int y = 0; y < 9; y++) {
            for (int x = 0; x < 9; x++) {
                Point point = {y, x};
                if (grid[y][x] != '.')
                    continue;
                PossibleSet &pos = psg[y][x];
                if (pos.count == 1) {
                    p = {y, x};
                    min_found = 1;
                    break;
                } else if (psg[y][x].count < min_found) {
                    p = {y, x};
                    min_found = psg[y][x].count;
                }
            }
            if (min_found == 1)
                break;
        }
        auto choices = psg[p.y][p.x].toArray();
        int i = 0;
        while (i < choices.size()) {
            bool result;
            if (!change_square(grid, psg, p, choices[i])) {
                result = false;
            } else {
                result = solve_rec(grid, psg, count + 1);
            }
            if (result)
                return true;
            else {
                i++;
                change_square(grid, psg, p, '.');
            }
        }
        return false;
    }

public:
    void solveSudoku(vector<vector<char>> &g) {
        array<array<PossibleSet, 9>, 9> psg;
        int count = 0;
        for (int y = 0; y < 9; y++) {
            for (int x = 0; x < 9; x++) {
                if (g[y][x] == '.')
                    psg[y][x] = get_possible(g, y, x);
                else {
                    count++;
                    psg[y][x].valid = false;
                }
            }
        }
        solve_rec(g, psg, count);
    }
};

void pretty_print(vector<vector<char>> grid){
    for (int y = 0; y < 9; ++y) {
        if(y == 3 || y == 6){
            cout << "──────┼───────┼──────\n";
        }
        for (int x = 0; x < 9; ++x) {
            if(x == 3 || x == 6)
                cout << "│ ";
            cout << grid[y][x] << ' ';
        }
        cout << '\n';
    }
}
int main() {
    cout << "\n\nUNSOLVED:\n\n";
    string str = "..9748...7.........2.1.9.....7...24..64.1.59..98...3.....8.3.2.........6...2759..";
    auto grid = vector<vector<char>>(9, vector<char>(9));
    {
       int i = 0;
       for(auto c : str){
           int y = i / 9;
           int x = i % 9;
           grid[y][x] = c;
           i++;
       }
    }
    pretty_print(grid);
    Solution().solveSudoku(grid);
    cout << "\n\nSOLVED:\n\n";
    pretty_print(grid);
}
