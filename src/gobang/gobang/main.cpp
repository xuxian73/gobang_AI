

//#include <cstdio>
//#include <string>
//#include <set>
//#include <algorithm>
//#include "AIController.h"
//#include <utility>
//#include "memory.h"
//
//extern int ai_side; //0: black; 1: white
//std::string ai_name = "myGobang";
//int turn;
//
//static int point[2][6] = {
//    { 0, 10, 100, 1000, 10000, 100000000 },
//    { 0, 0, 10, 100, 1000, 100000000 }
//};
//const long long NEG_INF = -1000000000000000000;
//const long long INF = 1000000000000000000;
//
//class Coordinate {
//public:
//    int x, y, w;
//
//    Coordinate(int X, int Y): x(X), y(Y) { w = std::min(15 - x, x + 1) * std::min(15 - y, y + 1); }
//
//
//    Coordinate operator+(Coordinate& rhs) {
//        return Coordinate(x + rhs.x, y + rhs.y);
//    }
//
//    Coordinate operator+(const Coordinate& rhs) const{
//        return Coordinate(x + rhs.x, y + rhs.y);
//    }
//
//    Coordinate operator-(Coordinate& rhs) {
//        return Coordinate(x - rhs.x, y - rhs.y);
//    }
//
//    Coordinate operator-(const Coordinate& rhs) const{
//        return Coordinate(x - rhs.x, y - rhs.y);
//    }
//
//    Coordinate operator*(int dis) {
//        return Coordinate(x * dis, y * dis);
//    }
//
//    bool operator<(const Coordinate& rhs) const {
//        if (w != rhs.w) {
//            return w < rhs.w;
//        } else if (x != rhs.x) {
//            return x < rhs.x;
//        } else return y < rhs.y;
//    }
//
//    bool operator==(const Coordinate& rhs) const{
//        return (x == rhs.x and y == rhs.y);
//    }
//    bool valid() {
//        return (x < 15 and x >= 0 and y < 15 and y >= 0);
//    }
//};
//
//struct compare {
//    bool operator()(const Coordinate& l, const Coordinate& r) const {
//        if (l.w != r.w) {
//            return l.w > r.w;
//        } else if (l.x != r.x) {
//            return l.x < r.x;
//        } else return l.y < r.y;
//    }
//};
//
//Coordinate dir[4] = {Coordinate(1,0), Coordinate(0,1), Coordinate(1,1), Coordinate(1,-1)};
//
//class AI;
//
//class Board{
//public:
//    int board[15][15];
//    AI* ai;
//    Board(AI *p): ai(p){
//        memset(board, -1, sizeof(board));
//    }
//
//    int operator[](const Coordinate & coor) {
//        return board[coor.x][coor.y];
//    }
//
//    void modify(Coordinate coor, int color) {
//        board[coor.x][coor.y] = color;
//    }
//};
//
//
//class AI {
//public:
//    Board board;
//
//    std::set<Coordinate, compare> blank;
//
//    AI():board(this) {
//        for (int i = 0; i < 15; ++i)
//            for (int j = 0; j < 15; ++j)
//                blank.insert(Coordinate(i, j));
//    }
//
//    int value_color(const Coordinate& coor, const Coordinate& dir, int color){
//        int l = 0; int r = 0;
//        Coordinate tmp = coor + dir;
//        while (tmp.valid() and board[tmp] == color) {
//            ++r; tmp = tmp + dir;
//        }
//        bool rb = tmp.valid() and (board[tmp] == -1);
//        tmp = coor - dir;
//        while (tmp.valid() and board[tmp] == color) {
//            ++l; tmp = tmp - dir;
//        }
//        bool lb = tmp.valid() and (board[tmp] == -1);
//        if (!rb and !lb) return 0;
//        else return point[lb ^ rb][l + r + 1];
//    }
//
//    int value_modify(const Coordinate& coor, const Coordinate& dir, int color) {
//        int opponent = 1 - color;
//        return value_color(coor, dir, color) +
//        value_color(coor, dir, opponent) / 10;
//    }
//
//    int value_modify(const Coordinate& coor, int color) {
//        if (coor.x == -1) return 0;
//        return value_modify(coor, dir[0], color) + value_modify(coor, dir[1], color) + value_modify(coor, dir[2], color) + value_modify(coor, dir[3], color);
//    }
//
//
//    long long minimax(const Coordinate& coor, int depth, long long alpha, long long beta, int color, Coordinate& ans) {
//        if (depth == 0) return (color == ai_side ? -1 : 1) * value_modify(coor, 1- color);
//        long long value;
//        if (color == ai_side) {
//            value = NEG_INF;
//            std::set<Coordinate, compare>::iterator it;
//            for (it = blank.begin(); it != blank.end(); ++it) {
//                Coordinate cur = *it;
//                blank.erase(it); board.modify(cur, color);
//                long long tmp = minimax(cur, depth - 1, alpha, beta, 1 - color, ans);
//                blank.insert(cur); board.modify(cur, -1);
//                it = blank.find(cur);
//                if (tmp > value) {
//                    value = tmp;
//                    ans = cur;
//                }
//                alpha = std::max(alpha, value);
//                if (alpha >= beta) break;
//            }
//            return -value_modify(coor, color) + value;
//        } else {
//            value = INF;
//            std::set<Coordinate, compare>::iterator it;
//            for (it = blank.begin(); it != blank.end(); ++it) {
//                Coordinate cur = *it;
//                blank.erase(it); board.modify(cur, color);
//                long long tmp = minimax(cur, depth - 1, alpha, beta, 1 - color, ans);
//                blank.insert(cur); board.modify(cur, -1);
//                it = blank.find(cur);
//                if (tmp < value) {
//                    value = tmp;
//                    ans = cur;
//                }
//                beta = std::min(beta, value);
//                if (alpha >= beta) break;
//            }
//            return value_modify(coor, color) + value;
//        }
//    }
//
//    void add_blank(Coordinate coor) {
//
//    }
//
//    void flip() {
//        for (int i = 0; i < 15; ++i)
//            for (int j = 0; j < 15; ++j){
//                if (board.board[i][j] >= 0)
//                    board.board[i][j] ^= 0;
//            }
//    }
//
//
//};
//
//void init(){
//    turn = 0;
//}
//AI myAI;
//std::pair<int, int> action(std::pair<int, int> loc) {
//    ++turn;
//    if (loc.first == -1) myAI.flip();
//    if (turn == 1 and ai_side == 0) {
//        myAI.board.modify(Coordinate(7,7), ai_side);
//        myAI.blank.erase(Coordinate(7,7));
//        return std::make_pair(7, 7);
//    }
//
//    myAI.board.modify(Coordinate(loc.first, loc.second), 1 - ai_side);
//    myAI.blank.erase(Coordinate(loc.first, loc.second));
//
//    if (turn == 1 and ai_side == 1) {
//        myAI.blank.erase(Coordinate(loc.first + 1, loc.second + 1));
//        myAI.board.modify(Coordinate(loc.first+1, loc.second + 1), ai_side);
//        return std::make_pair(loc.first + 1, loc.second + 1);
//    }
//
//    Coordinate ans(0,0);
//    if (turn == 2 and ai_side == 1) {
//        if (myAI.minimax(Coordinate(-1, -1), 7, NEG_INF, INF, ai_side, ans) > 0) {
//            myAI.blank.erase(Coordinate(ans.x, ans.y));
//            myAI.board.modify(ans, ai_side);
//            return std::make_pair(ans.x, ans.y);
//        }
//        else { myAI.flip(); return std::make_pair(-1, -1); }
//    }
//
//    for (int i = 2; i <= 6; i += 2) {
//        if ( myAI.minimax(Coordinate(-1, -1), i, NEG_INF, INF, ai_side, ans) > 1000000) {
//            {
//                myAI.board.modify(ans, ai_side);
//                myAI.blank.erase(Coordinate(ans.x, ans.y));
//                return std::make_pair(ans.x, ans.y);
//            }
//        }
//    }
//    myAI.board.modify(ans, ai_side);
//    myAI.blank.erase(Coordinate(ans.x, ans.y));
//    return std::make_pair(ans.x, ans.y);
//}
