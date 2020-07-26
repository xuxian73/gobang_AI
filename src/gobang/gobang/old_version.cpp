//
//  old_version.cpp
//  gobang
//
//  Created by 许贤 on 2020/7/21.
//  Copyright © 2020 许贤. All rights reserved.

//
//#include <cstdio>
//#include <string>
//#include <set>
//#include <algorithm>
//#include "AIController.h"
//#include <utility>
//#include "memory.h"
//
//using namespace std;
//
//typedef long long ll;
//
//extern int ai_side; //0: black, 1: white
//std::string ai_name = "myGobang";
//
//int turn;
//
//static int point[2][5] = {
//    { 10, 100, 1000, 10000, 100000000 },
//    { 10, 100, 1000, 10000 }
//};
//
//class Coordinate {
//public:
//    int x, y, w;
//
//    Coordinate(int X, int Y): x(X), y(Y) { w = min(15 - x, x + 1) * min(15 - y, y + 1); }
//
//    Coordinate operator+(Coordinate& rhs) {
//        return Coordinate(x + rhs.x, y + rhs.y);
//    }
//
//    Coordinate operator-(Coordinate& rhs) {
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
//Coordinate dir[4] = { Coordinate(1,0), Coordinate(0,1), Coordinate(1,1), Coordinate(1,-1) };
//
//
//
//struct Heuristic {
//    int l, r;
//    bool lblank, rblank;
//
//    Heuristic(): l(0), r(0), lblank(true), rblank(true) {}
//
//    int len() {
//        return l + r + 1;
//    }
//
//    int heuristic(){
//        if (!lblank and !rblank) return 0;
//        return point[lblank ^ rblank][len()];
//    }
//};
//
//
//class Board {
//    int board[15][15];
//    Heuristic value[15][15][2][4];
//    std::set<Coordinate, compare> blank;
//
//
//    bool valid(int x, int y) {
//        return (x >=0 and x <= 14 and y >= 0 and y <= 14);
//    }
//
//    bool isBlack(int x, int y) {
//        return board[x][y] == 0;
//    }
//
//    bool isWrite(int x, int y) {
//        return board[x][y] == 1;
//    }
//
//    void debug(){
//        for (int i = 0; i < 15; ++i) {
//            for (int j = 0; j < 15; ++j) {
//                if (board[i][j] == -1)
//                    printf("_ ");
//                else printf("%d ",board[i][j]);
//            }
//            printf("\n");
//        }
//        for (int k = 0; k < 2; ++k) {
//            printf("side: %d\n", k);
//            for (int d = 0; d < 4; ++d) {
//                printf("dir: %d\n", d);
//                for (int i = 0; i < 15; ++i) {
//                    for (int j = 0; j < 15; ++j) {
//                        printf("%d ",value[i][j][k][d].len());
//                    }
//                    printf("\n");
//                }
//            }
//        }
//    }
//public:
//    Board() {
//        memset(board, -1, sizeof(board));
//        for (int i = 0; i < 15; ++i)
//            for (int j = 0; j < 15; ++j)
//                blank.insert(Coordinate(i, j));
//    }
//
//    int get_hueristic(Coordinate pos, int color) {
//        return value[pos.x][pos.y][color][0].heuristic() + value[pos.x][pos.y][color][1].heuristic() + value[pos.x][pos.y][color][2].heuristic() + value[pos.x][pos.y][color][3].heuristic();
//    }
//
//    void flip(){
//        for (int i = 0; i < 15; ++i) {
//            for (int j = 0; j < 15; ++j) {
//                if (board[i][j] >= 0) board[i][j] ^= 1;
//            }
//        }
//    }
//
//    int negmax(Coordinate pos, unsigned depth, int alpha, int beta, int color, Coordinate& ans) {
//        /* try to put color chess on pos, depth remained to search, alpha-beta used to prun */
//        if(depth == 0) {
//            return get_hueristic(pos, color);
//        }
//
//        std::set<Coordinate, compare>::iterator it = blank.begin();
//        int value = -1e18;
//        for(int i = 0; i < int(blank.size()); ++it, ++i) {
//            printf("%d %d\n",(*it).x,(*it).y);
//            Coordinate tmp = *it;
//            blank.erase(it);
//            modify(tmp, color);
//            debug();
//            int n = -negmax(*it, depth - 1, -beta, -alpha, 1 - color, ans);
//            if (value < n) {
//                value = n;
//                ans = tmp;
//            }
//            resume(tmp, color);
//            it = blank.find(tmp);
//            alpha = max(alpha, value);
//            if (alpha >= beta) break;
//        }
//        return value ;
//    }
//
//    void modify(Coordinate pos, int color) {
//        board[pos.x][pos.y] = color;
//        for (int i = 0; i < 4; ++i) {
//            Coordinate tmp = pos + dir[i];
//            int lmax = 0; int rmax = 0;
//
//            while ( valid(tmp.x, tmp.y) and board[tmp.x][tmp.y] == color) {
//                ++rmax; tmp = tmp + dir[i];
//            }
//            tmp = pos - dir[i];
//            while ( valid(tmp.x, tmp.y) and board[tmp.x][tmp.y] == color) {
//                ++lmax; tmp = tmp - dir[i];
//            }
//
//            int len = lmax + rmax + 1;
//            bool lb = ( valid(tmp.x, tmp.y) and board[tmp.x][tmp.y] == -1);
//            bool rb = ( valid(tmp.x + dir[i].x * (len + 1), tmp.y + dir[i].y * (len + 1)) and board[tmp.x + dir[i].x *(len + 1)][tmp.y + dir[i].y*(len + 1)] == -1);
//            if (lmax == 0 and valid(tmp.x, tmp.y)) {
//                value[tmp.x][tmp.y][color][i].rblank = value[tmp.x][tmp.y][1 - color][i].rblank = false;
//            }
//            for (int l = 1; l <= len; ++l) {
//                tmp = tmp + dir[i];
//                value[tmp.x][tmp.y][color][i].l = l - 1;
//                value[tmp.x][tmp.y][color][i].r = len - l;
//                value[tmp.x][tmp.y][color][i].lblank = lb;
//                value[tmp.x][tmp.y][color][i].rblank = rb;
//            }
//            tmp = tmp + dir[i];
//            if (rmax == 0 and valid(tmp.x, tmp.y)) {
//                value[tmp.x][tmp.y][color][i].lblank = value[tmp.x][tmp.y][1 - color][i].lblank = false;
//            }
//        }
//    }
//
//    void resume(Coordinate pos, int color) {
//        blank.insert(pos);
//        board[pos.x][pos.y] = -1;
//
//        for (int i = 0; i < 3; ++i) {
//            value[pos.x][pos.y][color][i] = value[pos.x][pos.y][1-color][i] = Heuristic();
//            Coordinate tmp = pos + dir[i];
//            if (valid(tmp.x, tmp.y)) {
//                switch (board[tmp.x][tmp.y]) {
//                    case -1:
//                        value[tmp.x][tmp.y][color][i].lblank = value[tmp.x][tmp.y][1 - color][i].lblank = false;
//                        break;
//                    default:
//                        int cur = 0;
//                        value[pos.x][pos.y][color][i].rblank = value[pos.x][pos.y][1-color][i].rblank = false;
//                        if (board[tmp.x][tmp.y] == color) {
//                            do {
//                                value[tmp.x][tmp.y][color][i].l = cur++;
//                                value[tmp.x][tmp.y][color][i].lblank = true;
//                                tmp = tmp + dir[i];
//                            } while (valid(tmp.x, tmp.y) and board[tmp.x][tmp.y] == color);
//                        } else {
//                            while (valid(tmp.x, tmp.y) and board[tmp.x][tmp.y] == 1 - color) {
//                                value[tmp.x][tmp.y][1 - color][i].lblank = true;
//                                tmp = tmp + dir[i];
//                            }
//                        }
//                        break;
//                }
//            }
//
//            tmp = pos - dir[i];
//            if (valid(tmp.x, tmp.y)) {
//                switch (board[tmp.x][tmp.y]) {
//                    case -1:
//                        value[tmp.x][tmp.y][color][i].rblank = value[tmp.x][tmp.y][1 - color][i].rblank = false;
//                        break;
//                    default:
//                        int cur = 0;
//                        value[pos.x][pos.y][color][i].lblank = value[pos.x][pos.y][1-color][i].lblank = false;
//                        if (board[tmp.x][tmp.y] == color) {
//                            do {
//                                value[tmp.x][tmp.y][color][i].r = cur++;
//                                value[tmp.x][tmp.y][color][i].rblank = true;
//                                tmp = tmp - dir[i];
//                            } while (valid(tmp.x, tmp.y) and board[tmp.x][tmp.y] == color);
//                        } else {
//                            while (valid(tmp.x, tmp.y) and board[tmp.x][tmp.y] == 1 - color) {
//                                value[tmp.x][tmp.y][1 - color][i].rblank = true;
//                                tmp = tmp - dir[i];
//                            }
//                        }
//                        break;
//                }
//            }
//        }
//    }
//
//    std::pair<int, int> solve_flip() {
//        return make_pair(-1, -1);
//    }
//};
//
//Board board;
//
//void init() {
//    turn = 0;
//}
//
//
//std::pair<int, int> action(std::pair<int, int> loc) {
//    ++turn;
//    if (turn == 1 and ai_side == 0) {
//        board.modify(Coordinate(7, 7), 0);
//        return make_pair(7, 7);
//    }
//    if (loc.first == -1)
//        board.flip();
//    else board.modify(Coordinate(loc.first, loc.second), 1 - ai_side);
//    if (turn == 1 and ai_side == 1)
//    {
//        board.modify(Coordinate(loc.first-1, loc.first-1), 1);
//        return make_pair(loc.first - 1, loc.second - 1);
//    }
//    if (turn == 2 and ai_side == 1) {
//        Coordinate ans_flip(0,0);
//        Coordinate ans_not_flip(0,0);
//        if (board.negmax(Coordinate(0,0), 7, 1e18, -1e18, 1, ans_flip) <= 0) {
//            board.flip();
//            return make_pair(-1, -1);
//        }
//    }
//    Coordinate ans(0,0);
//    for (int i = 1; i < 6; ++i) {
//        if ( board.negmax(Coordinate(0, 0), i, -1e18, 1e18, ai_side, ans) >= 1e8 ) {
//            board.modify(ans, ai_side);
//            return make_pair(ans.x, ans.y);
//        }
//    }
//    board.modify(ans, ai_side);
//    return make_pair(ans.x, ans.y);
//}
