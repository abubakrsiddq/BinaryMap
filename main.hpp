#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

enum class State
{
    kEmpty,kObstacle,kClosed,kPath,kStart,kFinish
};
vector<vector<State>> ReadBoardFile(string path);
void PrintBoard(vector<vector<State>> v);
vector<State> ParseLine(string s);
string CellString(State cell);
int Heuristic(int x1,int y1,int x2,int y2);
void AddToOpen(int x,int y,int g,int h,std::vector<std::vector<int>> &openNode,std::vector<std::vector<State>> &grid);
bool Compare(vector<int> node1,vector<int> node2);
void CellSort(vector<vector<int>> *v);
bool CheckValidCell(int x,int y,vector<vector<State>> &grid);
void ExpandNeighbours(const vector<int> &current,int goal[2],vector<vector<int>> &open,vector<vector<State>> &grid);
const int delta[4][2]{{-1,0},{0,-1},{1,0},{0,1}};
vector<vector<State>> ReadBoardFile(string path) {
    ifstream myfile(path);
    string line;
    vector<State> row;
    vector<vector<State>> board;
    
    if (myfile) {
        while (getline(myfile, line)) {
            row = ParseLine(line);
            board.push_back(row);
        }
    }
    
    return board;
}

void PrintBoard(vector<vector<State>> v) {
    for(int i=0; i < v.size(); i++) {
        for(int j=0; j < v[i].size(); j++) {
            cout << CellString(v[i][j]);
        }
        cout << "\n";
    }
}

vector<State> ParseLine(string s) {
    istringstream str(s);
    int i;
    char c;
    vector<State> v;
    
    while (str >> i >> c && c == ',') {
        if (i == 0) {
            v.push_back(State::kEmpty);
        }
        else {
            v.push_back(State::kObstacle);
        }
    }
    
    return v;
}

string CellString(State cell) {
    switch(cell) {
        case State::kObstacle : return "⛰   ";
        case State::kPath:return "🚗   ";
        case State::kStart: return "🚦   ";
    case State::kFinish:return "🏁   ";
        default : return "0   ";
    }
}

vector<vector<State>> Search(vector<vector<State>> grid,int init[2],int goal[2])
{
  vector<vector<int>> open {};
  int x=init[0];
  int y=init[1];
  int g=0;
  int h=Heuristic(x,y,goal[0],goal[1]);
  AddToOpen(x,y,g,h,open,grid);
  
  while (open.size()>0)
  {
      CellSort(&open);
      auto current=open.back();
      open.pop_back();
      x=current[0];
      y=current[1];
      grid[x][y]=State::kPath;
      if(x==goal[0]&&y==goal[1])
      {
        grid[init[0]][init[1]]=State::kStart;
        grid[goal[0]][goal[1]]=State::kFinish;
        return grid;
      }
      ExpandNeighbours(current,goal,open,grid);

  }
  


  cout<<"no path found";
  return vector<vector<State>> {};  
}

int Heuristic(int x1,int y1,int x2,int y2){
    return(std::abs(x2-x1)+std::abs(y2-y1));
}
void AddToOpen(int x,int y,int g,int h,std::vector<std::vector<int>> &openNode,std::vector<std::vector<State>> &grid)
{
    std::vector<int> node{x,y,g,h};
    openNode.push_back(node);
    grid[x][y]=State::kClosed;

}
void PrintVectorOfVectors(vector<vector<int>> v) {
  for (auto row : v) {
    cout << "{ ";
    for (auto col : row) {
      cout << col << " ";
    }
    cout << "}" << "\n";
  }
}

void PrintVectorOfVectors(vector<vector<State>> v) {

  for (auto row : v) {
    cout << "{ ";
    for (auto col : row) {
      cout << CellString(col) << " ";
    }
    cout << "}" << "\n";
  }
}

bool Compare(vector<int> node1,vector<int> node2)
{
    /**
     * compare 2 nodes g+h=f value
     * true if f(node1)>fnode(2)*/
    if((node1[2]+node1[3])>(node2[2]+node2[3]))
        return true;
    return false;
}

void CellSort(vector<vector<int>> *v){

    /**
     * sort the vector in descendin order of f value
     * */
    sort(v->begin(),v->end(),Compare);
}

bool CheckValidCell(int x,int y,vector<vector<State>> &grid)
{
    bool on_grid;
    on_grid=(x>=0&&x<grid.size())&&(y>=0&&x<grid[0].size());
    if(on_grid&&(grid[x][y]==State::kEmpty))
        return true;
    return false;
}


void ExpandNeighbours(const vector<int> &current,int goal[2],vector<vector<int>> &open,vector<vector<State>> &grid)
{
    int x=current[0];
    int y=current[1];
    int g=current[2];
    int h=Heuristic(x,y,goal[0],goal[1]);
    for(auto i:delta)
    {
        int x2=x+i[0];
        int y2=y+i[1];
        if(CheckValidCell(x2,y2,grid))
        {
            int g2=g+1;
            int h2=Heuristic(x2,y2,goal[0],goal[1]);
            AddToOpen(x2,y2,g2,h2,open,grid);

        }
    }
}