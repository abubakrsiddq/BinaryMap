#include "main.hpp"
int main()
{
     int init[2]{0, 0};
    int goal[2]{4, 5};
    auto board = ReadBoardFile("./1.board");
    PrintBoard(board);
    cout<<"===================================";
    cout<<"\n\t solution\n";
    cout<<"===================================\n";
    auto solution = Search(board, init, goal);
    PrintBoard(solution);
   return 0;
}