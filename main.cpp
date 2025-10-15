
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <utility>
#include <stack>
#include <chrono>
#include <thread>



using namespace std;

int rows = 10;
int cols = 10;

pair<int,int> dirs[] = {{0,1},{1,0},{0,-1},{-1,0}};
srand (time(NULL));

void findValidPosition(vector<vector<bool>> &maze, int &x, int &y);
bool validPosition(vector<vector<bool>> &maze, int x, int y);
void validDir(int &x, int &y);

int getRandom(int limit);
void genPath(vector<vector<char>> &maze, int x, int y);

void printMaze(vector<vector<char>> &maze);

int main(){
    vector<vector<char>> maze(rows, vector<int>(cols, '#'));

    int x = getRandom(rows), y = getRandom(cols);

    while (x != -1 && y != -1){
        printMaze();
        x = getRandom(rows);
        y = getRandom(cols);
        findValidPosition(maze, x, y);
        genPath(maze, x, y);
        printMaze();
    }
    return 1;
}
void findValidPosition(vector<vector<bool>> &maze, int &x, int &y){
    if( x >= rows || y >= cols || x < 0 || y < 0) {
        x = -1;
        y = -1;
        return
    }


    for (int i = x; i < rows; i++){
        for (int j = 0; j < cols; j++){
            if (i == x) j = y;
            if (validPosition(maze, i, j)) {
                x = i;
                y = j;
                return;
            }
        }
    }
    int j_limit = cols;
    for (int i = 0; i <= x; i++){
        for (int j = 0; j < cols; j++){
            if (i == x) j_limit = y + 1;
            if (validPosition(maze, i, j)) {
                x = i;
                y = j;
                return;
            }
        }
    }
    x = -1;
    y = -1;
    return;
}

void genPath(vector<vector<char>> &maze, int x, int y){
    if( x >= rows || y >= cols || x < 0 || y < 0) return;

    stack<pair<int,int>> s;
    pair<int,int> p;

    int dir_index;
    int newX = x;
    int newY = y;
    char pos = maze[newX][newY];

    while (pos != ' '){
        if (pos == '+'){
            p = {newX, newY};
            while(s.top() != {newX, newY}){
                s.pop();
            }
            s.pop();
        }
        maze[newX][newY] = '+';
        s.push({newX,newY});
        validDir(newX,newY); // set newX and newY to valid dir;
        pos = maze[newX][newY];
        printMaze();
    }
    while(s.empty()){
        p = s.top(); s.pop();
        maze[p.first][p.second] = ' ';
    }
}



int getRandom(int limit){
    return rand() % limit;
}

bool validPosition(vector<vector<bool>> &maze, int x, int y){
    if( x >= rows || y >= cols || x < 0 || y < 0) return 0;

    int paths = 0;
    int newX, newY;

    for (auto dir : dirs){
        newX = x + dir.first;
        newY = y + dir.second;
        if( newX >= rows || newY >= cols || newX < 0 || newY < 0) continue;
        if(maze[newX][newY] == ' ') paths++;
    }

    if (paths > 1) return 0;
    else return 1;
}
void validDir(int &x, int &y){
    int newX, newY;
    for (auto dir : dirs){
        newX = x + dir.first;
        newY = y + dir.second;
        if(!(newX >= rows || newY >= cols || newX < 0 || newY < 0)) break;
    }
    x = newX;
    y = newY;
}

void printMaze(vector<vector<char>> &maze){
    for (int i = 0; i < rows + 1; i++) cout << "#";
    cout << '\n';
    for (int i = 0; i < rows; i++){
        cout << '#';
        for (int j = 0; j < cols; j++){
            cout << maze[i][j];
        }
        cout << "#\n"
    }
    for (int i = 0; i < rows + 1; i++) cout << "#";
    std::this_thread::sleep_for(std::chrono::seconds(0.1));
}
