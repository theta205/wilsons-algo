
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <utility>
#include <stack>
#include <chrono>
#include <thread>



using namespace std;

int rows = 15;
int cols = 30;
pair<int,int> diagonals[] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

pair<int,int> dirs[] = {{0,1},{1,0},{0,-1},{-1,0}};


bool findValidPosition(vector<vector<char>> &maze, int &x, int &y);
bool validPosition(vector<vector<char>> &maze, int x, int y);
bool validCand(vector<vector<char>> &maze,vector<pair<int,int>> &path, pair<int,int> p);

int getRandom(int limit);
void genPath(vector<vector<char>> &maze, int x, int y);
pair<int,int> getLoopOrigin(vector<vector<char>> &maze, int x, int y);

void printMaze(vector<vector<char>> &maze);

int main(){
    srand (time(NULL));
    vector<vector<char>> maze(rows, vector<char>(cols, '#'));

    int x = getRandom(rows), y = getRandom(cols);
    findValidPosition(maze, x, y);
    maze[x][y] = ' ';

    x = getRandom(rows); y = getRandom(cols);
    while (findValidPosition(maze, x, y)){
        printMaze(maze);

        genPath(maze, x, y);

        x = getRandom(rows);
        y = getRandom(cols);

        printMaze(maze);
    }
    return 1;
}
bool findValidPosition(vector<vector<char>> &maze, int &x, int &y){
    if( x >= rows || y >= cols || x < 0 || y < 0) {
        return 0;
    }
    int start;
    for (int i = x; i < rows; i++){
        if (i == x) start = y;
        else start = 0;
        for (int j = start; j < cols; j++){
            if (validPosition(maze, i, j)) {
                x = i;
                y = j;
                return 1;
            }
        }
    }

    for (int i = 0; i <= x; i++){
        for (int j = 0; j < cols; j++){
            if (validPosition(maze, i, j)) {
                x = i;
                y = j;
                return 1;
            }
        }
    }
    return 0;
}

void genPath(vector<vector<char>> &maze, int x, int y) {
    if (x < 0 || x >= rows || y < 0 || y >= cols) return;

    vector<pair<int,int>> path;          
    path.push_back({x,y});
    maze[x][y] = '+';

    while (!path.empty()) {
        auto [cx, cy] = path.back();
        vector<pair<int,int>> candidates;
        vector<pair<int,int>> prev;
        
        bool found_path = false;
        for (auto dir : dirs) {
            int nx = cx + dir.first;
            int ny = cy + dir.second;
            pair<int,int> np = {nx,ny};
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols) {
                if (validCand(maze, path, np)) {
                    candidates.push_back({nx, ny});
                }
                else if(maze[nx][ny] == '+'){
                    prev.push_back({nx, ny});
                }
                else if(maze[nx][ny] == ' '){
                    found_path = true;
                }
            }
        }
        if(found_path) break;

        auto oldest = prev.begin();
        pair<int,int> target = {-1,-1};
        pair<int,int> temp = {-1,-1};
        if(prev.size() > 1){
            while(!prev.empty()){ 
                
                auto it = find(path.begin(),path.end(), prev.back());
                temp = *it;
                prev.pop_back();
                if (oldest > it) oldest = it;
                target = *oldest;
            }

            while (path.back() != target){
                maze[path.back().first][path.back().second] = '#';
                path.pop_back();
            }

        }
        else if(candidates.empty()){
            while (path.size() > 1){
                maze[path.back().first][path.back().second] = '#';
                path.pop_back();
            }
        }
        else {
            auto [nx, ny] = candidates[rand() % candidates.size()];
            maze[nx][ny] = '+';
            path.push_back({nx, ny});
        }

        printMaze(maze);
    }

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            if (maze[i][j] == '+') maze[i][j] = ' ';
}

int getRandom(int limit){
    return rand() % limit;
}

bool validPosition(vector<vector<char>> &maze, int x, int y){
    if( x >= rows || y >= cols || x < 0 || y < 0) return 0;
    if(maze[x][y] != '#') return 0;
    int path_neighbors = 0;
    int newX, newY;
    int dx, dy, count = 0;

    for (auto dir : dirs){
        newX = x + dir.first;
        newY = y + dir.second;
        if( newX >= rows || newY >= cols || newX < 0 || newY < 0) continue;
        if(maze[newX][newY] == ' ' || maze[newX][newY] == '+') path_neighbors++;
    }
    for (auto dia : diagonals) {
        dx = dia.first + x;
        dy = dia.second + y;
        if (dx >= 0 && dx < rows && dy >= 0 && dy < cols) {
            if (maze[dx][dy] == ' ')
                count++;
        }
    }
    if (count > 1) return 0;
    return path_neighbors <= 1;
}

bool validCand(vector<vector<char>> &maze, vector<pair<int,int>> &path, pair<int,int> p){
    int dx, dy, count = 0;
    if (maze[p.first][p.second] == '#' && find(path.begin(), path.end(), p) == path.end()) {
        for (auto dia : diagonals) {
            dx = dia.first + p.first;
            dy = dia.second + p.second;
            if (dx >= 0 && dx < rows && dy >= 0 && dy < cols) {
                if (maze[dx][dy] == '+')
                    count++;
            }
        }
        if (count > 1) return false;
        return true;
    }
    return false;
}

void printMaze(vector<vector<char>> &maze){

    cout << '\n';
    for (int i = 0; i < cols + 2; i++) cout << "#";
    cout << '\n';

    for (int i = 0; i < rows; i++){
        cout << '#';
        if (i >= maze.size()) { 
            cout << "[ERROR] i=" << i << " >= maze.size()=" << maze.size() << '\n'; 
            continue;
        }
        for (int j = 0; j < cols; j++){
            if (j >= maze[i].size()) { 
                cout << "[ERROR] j=" << j << " >= maze[i].size()=" << maze[i].size(); 
                break; 
            }
            cout << maze[i][j];
        }
        cout << "#\n"; 
    }
    for (int i = 0; i < cols + 2; i++) cout << "#";
    cout << endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}