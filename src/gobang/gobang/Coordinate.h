//
//  Coordinate.h
//  gobang
//
//  Created by 许贤 on 2020/7/21.
//  Copyright © 2020 许贤. All rights reserved.
//

#ifndef Coordinate_h
#define Coordinate_h

#include <algorithm>
class Coordinate {
public:
    int x, y, w;

    Coordinate(int X, int Y): x(X), y(Y) { w = std::min(15 - x, x + 1) * std::min(15 - y, y + 1); }

    
    Coordinate operator+(Coordinate& rhs) {
        return Coordinate(x + rhs.x, y + rhs.y);
    }
    
    Coordinate operator+(const Coordinate& rhs) const{
        return Coordinate(x + rhs.x, y + rhs.y);
    }
    
    Coordinate operator-(Coordinate& rhs) {
        return Coordinate(x - rhs.x, y - rhs.y);
    }
    
    Coordinate operator-(const Coordinate& rhs) const{
        return Coordinate(x - rhs.x, y - rhs.y);
    }
    
    Coordinate operator*(int dis) {
        return Coordinate(x * dis, y * dis);
    }
    
    bool operator<(const Coordinate& rhs) const {
        if (w != rhs.w) {
            return w < rhs.w;
        } else if (x != rhs.x) {
            return x < rhs.x;
        } else return y < rhs.y;
    }
    
    bool operator==(const Coordinate& rhs) const{
        return (x == rhs.x and y == rhs.y);
    }
    bool valid() {
        return (x < 15 and x >= 0 and y < 15 and y >= 0);
    }
};

struct compare {
    bool operator()(const Coordinate& l, const Coordinate& r) const {
        if (l.w != r.w) {
            return l.w > r.w;
        } else if (l.x != r.x) {
            return l.x < r.x;
        } else return l.y < r.y;
    }
};
            
Coordinate dir[4] = {Coordinate(1,0), Coordinate(0,1), Coordinate(1,1), Coordinate(1,-1)};

#endif /* Coordinate_h */
