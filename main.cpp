#include <iostream>
#include <iomanip>
#include <time.h>
#include <stdlib.h>
#include <limits> 
#include <ios>

using namespace std;

char t[100][100], mt[100][100];
int m, n, k, x, y, counter = 0;
bool bomb, dead = false, won = false;
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

            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (dx == 0 && dy == 0) continue;
                    int ni = i + dx;
                    int nj = j + dy;
                    if (mt[ni][nj] == ' ') mt[ni][nj] += 17;
                    else if (mt[ni][nj] != 'X') mt[ni][nj]++;
                }
            }
        }
    }
}

void print(int r, int c, char arr[100][100])
{
    cout << "    ";
    for (int i = 0; i < c; i++) cout << setw(3) << i;
    cout << '\n' << setw(7);
    for (int i = 0; i < 3 * c - 1; i++) cout << "_";
    cout << '\n';
    for (int i = 0; i < r; i++)
    {
        cout << setw(3) << i << "|";
        for (int j = 0; j < r; j++) cout << setw(3) << arr[i][j];
        cout << '\n';
    }
}

void clears()
{
    for (int i = 0; i < l; i++) cout << '\n';
}

void rev(int i, int j)
{
    if (t[i][j] == '*')
    {
        t[i][j] = mt[i][j];
        counter++;
        if (mt[i][j] == ' ')
        {
            rev(i+1, j);
            rev(i-1, j);
            rev(i, j+1);
            rev(i, j-1);
            rev(i-1, j-1);
            rev(i+1, j+1);
            rev(i-1, j+1);
            rev(i+1, j-1);
        }
    }
}


void play(int i, int j, int sl)
{
    if (mt[i][j] != 'X') rev(i, j);
    else dead = true;
    if (m * n - counter == sl) won = true;
}

void answer(int r, int c)
{
    cout << '\n';
    cout << '\n';
    cout << "    ";
    for (int i = 0; i < c; i++) cout << setw(3) << i;
    cout << endl << setw(7);
    for (int i = 0; i < 3 * c - 1; i++) cout << "_";
    cout << '\n';
    for (int i = 0; i < r; i++)
    {
        cout << setw(3) << i << "|";
        for (int j = 0; j < r; j++) cout << setw(3) << mt[i][j];
        cout << '\n';
    }
}
int main()
{
    cout << "                    " << char(196) << char(197) << char(198) << "  WELCOME TO MINESWEEPER :) " << char(198) << char(197) << char(196) << '\n';
    cout << "                                 " << char(15) << "     " << char(15) << '\n';
    cout << "                                    " << char(15) << '\n';
    cout << "                                 " << char(15) << "     " << char(15) << '\n';
    cout << "-------------------------------------------------------------------\n";
    cout << "                        " << char(16) << " " << char(16) << " " << char(16) << " " << char(16) << " " << char(16) << " RULES " << char(17) << " " << char(17) << " " << char(17) << " " << char(17) << " " << char(17);
    cout << '\n';
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
    cout << '\n';
    cout << '\n';
    cout << "Please enter numbers ya prefer :> \n";
    cout << '\n';
    cout << char(15) << " Rows : ";
    while (!(cin >> m) || m <= 0)
    {
        cout << " ... oops, again pls :< " << '\n' << " " << char(15) << " Rows : ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << '\n';
    cout << char(15) << " Columns : ";
    while (!(cin >> n) || n <= 0)
    {
        cout << " ... oops, again pls :< " << '\n';
        cout << " " << char(15) << " Columns : ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << '\n';
    cout << char(15) << " Mines : ";
    while (!(cin >> k) || k <= 0 || k >= m * n)
    {
        cout << " ... oops, again pls :< " << '\n';
        cout << " " << char(15) << " Mines : ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << '\n';
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            t[i][j] = '*';
        }
    }
    create(m, n, k);
    do
    {
        while (dead != true && won != true)
        {
            clears();
            print(m, n, t);
            cout << "  " << char(4) << " (x, y) : ";
            while (!(cin >> x >> y) || x < 0 || x >= m || y < 0 || y >= n)
            {
                cout << "  " << "oops, again pls :< " << '\n' << char(4) << " (x, y) : ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
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
        cout << "\nDo you want to play again? (Y/N): ";
        char choice;
        cin >> choice;
        if (choice != 'Y' && choice != 'y') break;
        counter = 0;
        dead = false;
        won = false;
    } while (true);
    return 0;
}
