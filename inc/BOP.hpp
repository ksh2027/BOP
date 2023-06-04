#pragma once
#include "data_struct.hpp"
#include <queue>
#include <set>
#include <algorithm>
#include <cstring>
#include <cmath>

class BoolOperaPolygon{
public:
    BoolOperaPolygon(){};
    BoolOperaPolygon(std::vector<Polygon> polys, const char* _type){
        if(std::strcmp(_type, "Union") == 0)
            type = 0;
        else if(std::strcmp(_type, "Intersection") == 0)
            type = 1;
        else if(std::strcmp(_type, "XOR") == 0)
            type = 2;
        else if(std::strcmp(_type, "Difference") == 0)
            type = 3;

        for(size_t i=0; i<polys.size(); i++){
            polys[i].pid = i;
            Polygon_list.push_back(polys[i]);
        }
    };
    ~BoolOperaPolygon(){};
    void create_SweepEvent();
    void subdividing_edge();
    void joining_edge();

    std::vector<std::vector<pdd>> resultP;
    std::vector<bool> result_INOUT;

private:
    double direction(double x1, double y1, double x2, double y2, double x3, double y3) {
        return (x3 - x1) * (y2 - y1) - (x2 - x1) * (y3 - y1);
    }
    void possibleInter(SweepEvent* a, SweepEvent* b);
    void update_PQandS(SweepEvent* a, SweepEvent* b, double interX, double interY, size_t type);
    void setInsideFlag(SweepEvent* le, SweepEvent* ple);
    void add_result(SweepEvent* event);
    bool compute_contour(size_t pos, std::vector<pdd>& Contour, std::set<pdd>& contour_vertex);
    void check_child_contour(pdd v);

    struct PQcmp {
        bool operator()(const SweepEvent* a, const SweepEvent* b) {
            if(a->coor.x == b->coor.x){
                if(a->coor.y == b->coor.y){
                    if(a->left==b->left){
                        if(a->slope == b->slope){
                            if(a->otherEvent->coor.y == b->otherEvent->coor.y)
                                return a->otherEvent->coor.x > b->otherEvent->coor.x;
                            return a->otherEvent->coor.y > b->otherEvent->coor.y;
                        }
                        return a->slope > b->slope;
                    }
                    return a->left;
                }
                return a->coor.y > b->coor.y;
            }
            return a->coor.x > b->coor.x;
        }
    };
    struct SEcmp {
        bool operator()(const SweepEvent* a, const SweepEvent* b) const {
            if(a->y_sweepline == b->y_sweepline){
                // if(a->coor.y == b->coor.y){
                //     if(a->coor.x == b->coor.x){
                        if(a->slope == b->slope){
                            if(a->otherEvent->coor.y == b->otherEvent->coor.y)
                                return a->otherEvent->coor.x < b->otherEvent->coor.x;
                            return a->otherEvent->coor.y < b->otherEvent->coor.y;
                        }
                        return a->slope < b->slope;
                //     }
                //     return a->coor.x < b->coor.x;
                // }
                // return a->coor.y < b->coor.y;
            }
            return a->y_sweepline < b->y_sweepline;
        }
    };
    std::priority_queue <SweepEvent*, std::vector<SweepEvent*>, PQcmp> PQ;
    std::set<SweepEvent*, SEcmp> S;
    std::vector<Polygon> Polygon_list;
    std::vector<SweepEvent*> result_event;
    size_t type=0;  //0:union   1:intersection  2:XOR   3:difference
    size_t target; //for compute_contour()
};