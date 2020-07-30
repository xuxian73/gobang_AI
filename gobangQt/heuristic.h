#ifndef HEURISTIC_H
#define HEURISTIC_H

#include <set>
#include <algorithm>
#include <utility>
#include "memory.h"

int ai_side;

int turnPVE;
typedef long long ll;
static int point[2][6] = {
    { 0, 10, 100, 1000, 10000, 10000000 },
    { 0, 0, 10, 100, 1000, 10000000 }
};

const int DEFAULT_DEPTH = 10;

const int SPECIFIC_DEPTH = 6;

const long long NEG_INF = -1000000000000000000;

const long long INF = 1000000000000000000;

int WIDTH[15] =  {0, 2, 3, 3, 3, 3, 3, 5, 6, 7, 9};

ll score;

class Coordinate {
public:
    int x, y, weight;
    Coordinate(int X, int Y): x(X), y(Y), weight(80) {
        if(x == 0) weight -= 60;
        if (x == 14) weight -= 60;
        if (y == 0) weight -= 60;
        if (y == 14) weight -= 60;
        weight = std::max(weight, 0);
    }
    Coordinate(int X, int Y, int W): x(X), y(Y), weight(W) {}

    Coordinate operator+(Coordinate& rhs) {
        return Coordinate(x + rhs.x, y + rhs.y);
    }

    Coordinate operator-(Coordinate& rhs) {
        return Coordinate(x - rhs.x, y - rhs.y);
    }

    Coordinate operator*(int dis) {
        return Coordinate(x * dis, y * dis);
    }

    bool operator<(const Coordinate& rhs) const {
        if (weight != rhs.weight) {
            return weight > rhs.weight;
        } if (x != rhs.x) {
        return x > rhs.x;
        }
        return y > rhs.y;
    }

    bool operator==(const Coordinate& rhs) const {
        return x == rhs.x and y == rhs.y;
    }

    bool valid() {
        return x < 15 and x >= 0 and y < 15 and y >= 0;
    }

};

Coordinate dir[4] = { Coordinate(1,0), Coordinate(0,1), Coordinate(1,1), Coordinate(1,-1) };

class Board {
public:
  int board[15][15];
  Board() {
      memset(board, -1, sizeof(board));
  }

  int operator[](const Coordinate & coor) {
      return board[coor.x][coor.y];
  }

  void modify(Coordinate coor, int color) {
      board[coor.x][coor.y] = color;
  }

};

struct Heuristic {
    int l, r;
    bool lblank, rblank;
    Heuristic(): l(0), r(0), lblank(true), rblank(true) {}

    int len() {
        return l + r + 1 > 5 ? 5 : l + r + 1;
    }

    int def_len() {
        return l + r;
    }

    int heuristic(){
        if(len() == 5) return int(1e7);
        if (!lblank and !rblank) return 0;
        return point[lblank ^ rblank][len()];
    }

    int def_heuristic(){
        if (!lblank and !rblank) return 0;
        return point[lblank ^ rblank][def_len()];
    }
};

class AI {
public:
    Board board;
    std::set<Coordinate> blank;
    Heuristic status[15][15][2][4];
    AI () {
        for (int i = 0; i < 15; ++i)
            for (int j = 0; j < 15; ++j) {
                blank.insert(Coordinate(i, j));
            }
        for (int i = 0; i < 15; ++i) {
            for (int color = 0; color < 2; ++color) {
                status[0][i][color][0].lblank = status[0][i][color][2].lblank = status[0][i][color][3].lblank = false;
                status[14][i][color][0].rblank = status[14][i][color][2].rblank = status[14][i][color][3].rblank = false;
                status[i][0][color][1].lblank = status[i][0][color][2].lblank = status[i][14][color][3].lblank = false;
                status[i][14][color][1].rblank = status[i][14][color][2].rblank = status[i][0][color][3].rblank = false;
            }
        }
    }

    int get_Heuristic(int x, int y) {
        int ret = 0;
        if (x == -1) return 0;
        for (int color = 0; color <= 1; ++color)
            for (int dir = 0; dir < 4; ++dir) {
                ret += status[x][y][color][dir].heuristic();
            }
        return ret;

    }

    int get_Heuristic(int x, int y, int color) {
        int ret = 0;
            if (x == -1) return 0;
        for (int dir = 0; dir < 4; ++dir) {
            ret += status[x][y][1 - color][dir].def_heuristic();
        }

        for (int dir = 0; dir < 4; ++dir) {
            ret += status[x][y][color][dir].heuristic();
        }
        return ret;
    }

    void update_status(const int& x, const int& y, int d) {
        for (int color = 0; color < 2; ++color) {
            status[x][y][color][d].l = status[x][y][color][d].r = 0;
            Coordinate rtmp = Coordinate(x, y) + dir[d];
            while (rtmp.valid() and board[rtmp] == color)
            { ++status[x][y][color][d].r; rtmp = rtmp + dir[d]; }
            Coordinate ltmp = Coordinate(x, y) - dir[d];
            while (ltmp.valid() and board[ltmp] == color)
            { ++status[x][y][color][d].l; ltmp = ltmp - dir[d]; }
            status[x][y][color][d].lblank = ltmp.valid() and board[ltmp] == -1;
            status[x][y][color][d].rblank = rtmp.valid() and board[rtmp] == -1;
        }
    }

    void update_status(const int& x, const int& y) {
        for (int i = std::max(0, x - 5); i <= std::min(14, x + 5); ++i) {
            for (int j = std::max(0, y - 5); j <= std::min(14, y + 5); ++j) {
                if (board.board[i][j] == -1) {
                    blank.erase(Coordinate(i, j, get_Heuristic(i, j)));
                    for (int k = 0; k < 4; ++k)
                        update_status(i, j, k);
                    blank.insert(Coordinate(i, j, get_Heuristic(i, j)));
                }
            }
        }
    }

    ll minimax(int depth, ll alpha, ll beta, int color, Coordinate& ans, int* width)
    {
        if (depth == 0) return 0;
        ll value;
        if (color == ai_side) {
            value = NEG_INF;
            auto it = blank.begin();
            for ( int i = 0; i < width[depth] and i <= int(blank.size()); ++i, ++it) {
                Coordinate cur = *it;
                ll tmp = get_Heuristic(cur.x, cur.y, color);
                if (tmp >= int(1e7)) { value = INF; ans = cur; break;}
                blank.erase(it);
                board.modify(cur, color);
                update_status(cur.x, cur.y);
                Coordinate t(-1, -1);
                tmp += minimax( depth - 1, alpha - tmp, beta - tmp, 1 - color, t, width);
                if (tmp > value) {
                    value = tmp;
                    ans = cur;
                }
                blank.insert(cur);
                board.modify(cur, -1);
                update_status(cur.x, cur.y);
                it = blank.find(cur);
                alpha = std::max(value, alpha);
                if (alpha >= beta) return INF;
            }
            return value;
        } else {
            value = INF;
            std::set<Coordinate>::iterator it = blank.begin();
            for (int i = 0; i < width[depth] and i <= int(blank.size()); ++i, ++it) {
                Coordinate cur = *it;
                Coordinate t(-1, -1);
                ll tmp = -get_Heuristic(cur.x, cur.y, color);
                if(tmp <= int(-1e7)) { value = NEG_INF; ans = cur; break;}
                blank.erase(it);
                board.modify(cur, color);
                update_status(cur.x, cur.y);
                tmp += minimax(depth - 1, alpha - tmp, beta - tmp, 1 - color, t, width);
                if (tmp < value) {
                    value = tmp;
                    ans = cur;
                }
                blank.insert(cur);
                board.modify(cur, -1);
                update_status(cur.x, cur.y);
                it = blank.find(cur);
                beta = std::min(value, beta);
                if (alpha >= beta) return NEG_INF;
            }
            return value;
        }
    }

    void flip(){
        for (int i = 0; i < 15; ++i)
            for (int j = 0; j < 15; ++j)
                if (board.board[i][j] >= 0) {
                    board.board[i][j] = 1 - board.board[i][j];
                    update_status(i, j);
                }
    }

    ll value_flip(int color) {
        ll value = 0;
        for (int i = 0; i < 15; ++i)
            for (int j = 0; j <	15; ++j) {
                if (board.board[i][j] != -1)
                {
                    for (int d = 0; d < 4; ++d)
                        update_status(i, j, d);
                    if (board.board[i][j] == color) {
                        value += get_Heuristic(i, j);
                    } else value -= get_Heuristic(i, j);
                }
            }
        if (value > 100) value -= 100;
        value <<= 1;
        return -value;
    }

    std::pair<int, int> solve_change_white() {
        Coordinate t(-1, -1);
        if (value_flip(ai_side) - minimax( DEFAULT_DEPTH - 1, INF, NEG_INF, ai_side, t, WIDTH)
                < minimax( DEFAULT_DEPTH, INF, NEG_INF, ai_side, t, WIDTH)) {
            board.modify(t, ai_side);
            blank.erase(Coordinate(t.x, t.y, get_Heuristic(t.x, t.y)));
            update_status(t.x, t.y);
            return std::make_pair(t.x, t.y);
        } else {
            flip();
            return std::make_pair(-1, -1);
        }
    }

    std::pair<int, int> solve_change_black() {
//		ll min = INF;
//		Coordinate ans(-1, -1);
//		Coordinate t(-1, -1);
//		ll tmp = abs(minimax( 8, INF, NEG_INF, ai_side, t, BLACK_WIDTH));
//		if (tmp < min) {
//			tmp = min;
//			ans = t;
//		}
//
//		board.modify(ans, ai_side);
//		blank.erase(Coordinate(ans.x, ans.y, get_Heuristic(ans.x, ans.y)));
//		update_status(ans.x, ans.y);
//		return std::make_pair(ans.x, ans.y);
        board.modify(Coordinate(0, 1), ai_side);
        blank.erase(Coordinate(0, 1, get_Heuristic(0, 1)));
        update_status(0, 1);
        return std::make_pair(0, 1);
    }
};

AI* myAI = NULL;

void init(){
    turnPVE = 0;
//​	if (ai_side == 0) {
//​		myAI.me = &myAI.black_blank;
//​		myAI.other = &myAI.white_blank;
//​	} else {
//​		myAI.me = &myAI.white_blank;
//​		myAI.other = &myAI.black_blank;
//​	}
}

std::pair<int, int> action(std::pair<int, int> loc) {
    ++turnPVE;
    if (loc.first == -1)
        myAI->flip();
    else {
        myAI->board.modify(Coordinate(loc.first, loc.second), 1 - ai_side);
        myAI->blank.erase(Coordinate(loc.first, loc.second,myAI->get_Heuristic(loc.first, loc.second)));
        myAI->update_status(loc.first, loc.second);
    }
    if (turnPVE == 1 and ai_side == 0) {
//		return myAI.solve_change_black();
        myAI->board.modify(Coordinate(7,7), ai_side);
        myAI->blank.erase(Coordinate(7,7));
        myAI->update_status(7, 7);
        return std::make_pair(7, 7);
  }

    if (turnPVE == 1 and ai_side == 1) {
//    myAI.blank.erase(Coordinate(loc.first + 1, loc.second + 1, myAI.get_Heuristic(loc.first + 1, loc.second + 1)));
//    myAI.board.modify(Coordinate(loc.first+1, loc.second + 1), ai_side);
//    myAI.update_status(loc.first + 1, loc.second + 1);
//    return std::make_pair(loc.first + 1, loc.second + 1);
        myAI->board.modify(Coordinate(0, 0), ai_side);
        myAI->blank.erase(Coordinate(0, 0));
        myAI->update_status(0, 0);
        return std::make_pair(0, 0);
    }
    if (turnPVE == 2 and ai_side == 1) {
        return myAI->solve_change_white();
}

    Coordinate ans(0,0);
    if (turnPVE == 2 and ai_side == 0) {
        return myAI->solve_change_black();
    }

    if (turnPVE == 3 and ai_side == 0) {
        myAI->board.modify(Coordinate(8, 6), ai_side);
        myAI->blank.erase(Coordinate(8, 6, myAI->get_Heuristic(8, 6)));
        myAI->update_status(8, 6);
        return std::make_pair(8, 6);
    }

    for (int i = 6; i <= DEFAULT_DEPTH; i += 2) {
        if ( (score = myAI->minimax( i, NEG_INF, INF, ai_side, ans, WIDTH)) > 1000000) {
            {
                myAI->board.modify(ans, ai_side);
                myAI->blank.erase(Coordinate(ans.x, ans.y, myAI->get_Heuristic(ans.x, ans.y)));
                myAI->update_status(ans.x, ans.y);
                return std::make_pair(ans.x, ans.y);
            }
        }
    }
    myAI->board.modify(ans, ai_side);
    myAI->blank.erase(Coordinate(ans.x, ans.y, myAI->get_Heuristic(ans.x, ans.y)));
    myAI->update_status(ans.x, ans.y);
    return std::make_pair(ans.x, ans.y);

}


#endif // HEURISTIC_H
