#pragma once
#include <iostream>
#include <vector>
#include <list>
typedef std::pair<double, double> pdd;

class Coor{
public:
	double x, y;
	Coor(){}
	Coor(double _x, double  _y) : x(_x), y(_y){}
    bool operator==(const Coor & other) const{
		return this->x == other.x && this->y == other.y;
	}
};

class SweepEvent{
public:
    int pid;
    Coor coor;
    bool left;
    SweepEvent* otherEvent;
    double slope;
    double y_sweepline;
    bool inOut;
    bool inside;
    //for result event
    int pos;
    bool processed = false;
    // int Cid;
    // int Parid;
    // int depth;
    SweepEvent(){};
    SweepEvent(Coor _coor):coor(_coor), y_sweepline(_coor.y){};
};

class Polygon{
public:
    int pid;
    std::vector<std::list<Coor>> vertex_list;
    std::list<SweepEvent> SE_list_1;
    std::list<SweepEvent> SE_list_2;
    Polygon(){};
    Polygon(std::vector<std::vector<pdd>> poly){
        for(auto _poly:poly){
            std::list<Coor> _vertex;
            for(auto v:_poly)
                _vertex.push_back(Coor(v.first, v.second));
            vertex_list.push_back(_vertex);
        }
        std::list<Coor> _vertex={};
        vertex_list.push_back(_vertex);
    };
};

