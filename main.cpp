
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
pair<int,int> diagonals[] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

pair<int,int> dirs[] = {{0,1},{1,0},{0,-1},{-1,0}};


void findValidPosition(vector<vector<char>> &maze, int &x, int &y);
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


    while (x != -1 && y != -1){
        // cout << "Starting new path" << x << y<< endl;

        printMaze(maze);
        x = getRandom(rows);
        y = getRandom(cols);
        // cout << "x " << x << " y " << y << endl;
        findValidPosition(maze, x, y);
        // cout << "\t"<< x << " "<< y<< endl;

        genPath(maze, x, y);
        printMaze(maze);
    }
    return 1;
}
void findValidPosition(vector<vector<char>> &maze, int &x, int &y){
    if( x >= rows || y >= cols || x < 0 || y < 0) {
        x = -1;
        y = -1;
        return;
    }
    // cout << "1" << endl;
    int start;
    for (int i = x; i < rows; i++){
        if (i == x) start = y;
        else start = 0;
        for (int j = start; j < cols; j++){
            if (validPosition(maze, i, j)) {
                x = i;
                y = j;
                return;
            }
        }
    }
    // cout << "2" << endl;

    for (int i = 0; i <= x; i++){
        for (int j = 0; j < cols; j++){
            if (validPosition(maze, i, j)) {
                x = i;
                y = j;
                return;
            }
        }
    }
    // cout << "3" << endl;

    x = -1;
    y = -1;
    return;
}

void genPath(vector<vector<char>> &maze, int x, int y) {
    if (x < 0 || x >= rows || y < 0 || y >= cols) return;

    vector<pair<int,int>> path;          // acts as stack
    path.push_back({x,y});
    maze[x][y] = '+';



    while (!path.empty()) {
        auto [cx, cy] = path.back();
        // cout << "\nCurrent top of path: (" << cx << "," << cy << ") pos: " << maze[cx][cy] << "\n";
        // cout << "Rest of path:\n";
       // for(auto p : path){
            // cout << "{" << p.first << ", " << p.second << "}\n";
      //  }
        // Collect valid moves
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
            // cout << "Current place is not valid" << endl;
            while(!prev.empty()){ 
                
                auto it = find(path.begin(),path.end(), prev.back());
                //if(it == path.end()) cout << "\nnot found\n";
                temp = *it;
                // cout << temp.first << " " << temp.second << endl;
                prev.pop_back();
                if (oldest > it) oldest = it;
                target = *oldest;
            }
            // cout << endl;
            // cout << target.first << " " << target.second << endl;
            while (path.back() != target){
                // cout << path.size() << endl;
                maze[path.back().first][path.back().second] = '#';
                path.pop_back();
            }
            // cout << path.size() << endl;

        }
        else {
            // Pick a random candidate
            auto [nx, ny] = candidates[rand() % candidates.size()];
            maze[nx][ny] = '+';
            path.push_back({nx, ny});
        }

        printMaze(maze);
    }

    // Solidify the path
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            if (maze[i][j] == '+') maze[i][j] = ' ';
}








int getRandom(int limit){
    return rand() % limit;
}

bool validPosition(vector<vector<char>> &maze, int x, int y){
    // cout << "Checking: {" << x << ", "<< y << "}" << endl; 
    if( x >= rows || y >= cols || x < 0 || y < 0) return 0;
    if(maze[x][y] != '#') return 0;
    int path_neighbors = 0;
    int newX, newY;

    for (auto dir : dirs){
        newX = x + dir.first;
        newY = y + dir.second;
        if( newX >= rows || newY >= cols || newX < 0 || newY < 0) continue;
        if(maze[newX][newY] == ' ' || maze[newX][newY] == '+') path_neighbors++;
    }
    return path_neighbors <= 1;
}

bool validCand(vector<vector<char>> &maze, vector<pair<int,int>> &path, pair<int,int> p){
    int dx, dy, count = 0;
    if (maze[p.first][p.second] == '#' && find(path.begin(), path.end(), p) == path.end()) {
        // // 
        // for (auto dia : diagonals) {
        //     dx = dia.first + p.first;
        //     dy = dia.second + p.second;
        //     if (dx >= 0 && dx < rows && dy >= 0 && dy < cols) {
        //         if (maze[dx][dy] == '+')
        //             count++;
        //     }
        // }
        // if (count > 1) return false;
        // //
        return true;
    }
    return false;
}

void printMaze(vector<vector<char>> &maze){
    // cout << "\n\n";
    // cout << rows << " " << cols;
    cout << '\n';
    for (int i = 0; i < rows + 2; i++) cout << "#";
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
    for (int i = 0; i < rows + 2; i++) cout << "#";
    cout << endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}