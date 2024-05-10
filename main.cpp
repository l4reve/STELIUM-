#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>

using namespace std;

char t[100][100];
int counter = 0;
int m, n, k;
int x, y;
char mt[100][100];
bool bomb;
bool dead = false;
bool won = false;
const int l = 30;

void create(int r, int c, int sl)
{
    int mine = 0, i, j;
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            mt[i][j] = ' ';
        }
    }
    srand(time(NULL));
    while (mine < sl)
    {
        bomb = false;
        i = rand() % r;
        j = rand() % c;
        if (mt[i][j] == 'X')
        {
            bomb = true;
        }
        if (!bomb) {
            mt[i][j] = 'X';
            mine++;

            for (int di = -1; di <= 1; ++di) {
                for (int dj = -1; dj <= 1; ++dj) {
                    if (di == 0 && dj == 0) continue;
                    int ni = i + di;
                    int nj = j + dj;
                    if (ni >= 0 && ni < r && nj >= 0 && nj < c) {
                        if (mt[ni][nj] == ' ') mt[ni][nj] += 17;
                        else if (mt[ni][nj] != 'X') mt[ni][nj]++;
                    }
                }
            }
        }
    }
}

void print(int r, int c, char arr[100][100])
{
    cout << "    ";

    for (int i = 0; i < c; i++)
    {
        cout << setw(3) << i;
    }
    cout << '\n' << setw(7);
    for (int i = 0; i < 3 * c - 1; i++)
    {
        cout << "_";
    }
    cout << '\n';
    for (int i = 0; i < r; i++)
    {
        cout << setw(3) << i << "|";
        for (int j = 0; j < r; j++)
        {
            cout << setw(3) << arr[i][j];
        }
        cout << '\n';
    }
}

void clears()
{
    for (int i = 0; i < l; i++)
    {
        cout << '\n';
    }
}

void reveal(int i, int j) {
    if (t[i][j] == '*') {
        t[i][j] = mt[i][j];
        counter++;

        int directions[8][2] = {
            {1, 0}, {-1, 0}, {0, 1}, {0, -1},
            {-1, -1}, {1, 1}, {-1, 1}, {1, -1}
        };

        for (int d = 0; d < 8; ++d) {
            int ni = i + directions[d][0];
            int nj = j + directions[d][1];

            if (ni >= 0 && ni < m && nj >= 0 && nj < n && t[ni][nj] == '*') {
                reveal(ni, nj);
            }
        }
    }
}


void play(int i, int j, int sl)
{
    if (mt[i][j] != 'X')
    {
        reveal(i, j);
    }
    else
    {
        dead = true;
    }
    if (m * n - counter == sl)
    {
        won = true;
    }
}

void answer(int r, int c)
{
    cout << endl << endl;
    cout << "    ";
    for (int i = 0; i < c; i++)
    {
        cout << setw(3) << i;
    }
    cout << endl << setw(7);
    for (int i = 0; i < 3 * c - 1; i++)
    {
        cout << "_";
    }
    cout << '\n';
    for (int i = 0; i < r; i++)
    {
        cout << setw(3) << i << "|";
        for (int j = 0; j < r; j++)
        {
            cout << setw(3) << mt[i][j];
        }
        cout << '\n';
    }
}

int main()
{
    cout << "                    " << char(196) << char(197) << char(198) << "  WELCOME TO MINESWEEPER :) " << char(198) << char(197) << char(196) << '\n';
    //cout << '\n';
    cout << "                                 " << char(15) << "     " << char(15) << '\n';
    cout << "                                    " << char(15) << '\n';
    cout << "                                 " << char(15) << "     " << char(15) << '\n';
    //cout << '\n';
    cout << "-------------------------------------------------------------------\n";
    cout << "                        " << char(16) << " " << char(16) << " " << char(16) << " " << char(16) << " " << char(16) << " RULES " << char(17) << " " << char(17) << " " << char(17) << " " << char(17) << " " << char(17);
    
    cout << '\n';
    //cout << "-------------------------------------------------------------------\n";
    cout << "-------------------------------------------------------------------\n";
    cout << '\n';
    cout << char(15) << " Don't click on squares containing mines :)\n";
    cout << char(15) << " Each number indicates how many mines are adjacent to that square.\n";
    cout << char(15) << " Aware of squares ya suspect contain mines to avoid clicking them.\n";
    cout << char(15) << " ( x - horizontal | y - vertical )\n";

    cout << '\n';
    cout << "                        " << char(32) << " " << char(32) << " " << char(32) << " " << char(32) << " " << char(4) << " ENJOY " << char(4) << " " << char(32) << " " << char(32) << " " << char(32) << " " << char(32);
    cout << '\n';
    cout << "-------------------------------------------------------------------\n";
    //cout << "-------------------------------------------------------------------\n";
    cout << '\n';
    cout << '\n';
    cout << "Please enter numbers ya prefer ;> \n";
    cout << '\n';
    cout << char(15) << " Rows : "; cin >> m;
    cout << '\n';
    cout << char(15) << " Columns : "; cin >> n;
    cout << '\n';
    cout << char(15) << " Mines : "; cin >> k;
    cout << '\n';
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            t[i][j] = '*';
        }
    }
    create(m, n, k);
    while (dead != true && won != true)
    {
        clears();
        print(m, n, t);
        cout << "  " << char(4) << " (x, y) : ";
        cin >> x >> y;
        cout << '\n';
        play(x, y, k);
        if (dead)
        {
            clears();
            cout << "YOU LOST :(\n";
        }
        if (won)
        {
            cout << "YOU WIN :D\n";
        }
    }
    if (dead) answer(m, n);
}
