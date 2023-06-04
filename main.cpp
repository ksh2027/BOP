#include "BOP.hpp"

int main(int argc, char** argv){
    std::cout<<"KKK"<<std::endl;
    Polygon p1({{{3.0, 0.0}, {7.0, 3.0}, {4.0, 5.0}, {1.0, 2.0}}});
    Polygon p2({{{4.0, 3.0}, {9.0, 7.0}, {0.0, 6.0}}});
    // Polygon p1({{1.0, 1.0}, {3.0, 1.0}, {3.0, 3.0}, {1.0, 3.0}});
    // Polygon p2({{2.0, 0.0}, {4.0, 0.0}, {4.0, 1.0}, {2.0, 1.0}});
    // Polygon p1({{{2.0, 3.0}, {8.0, 3.0}, {8.0, 7.0}, {2.0, 7.0}}, {{3.0, 4.0}, {5.0, 4.0}, {5.0, 6.0}, {3.0, 6.0}}});
    // Polygon p2({{{4.0, 1.0}, {7.0, 1.0}, {7.0, 5.0}, {4.0, 5.0}}});
    BoolOperaPolygon BOP({p1, p2}, 1);
    BOP.create_SweepEvent();
    BOP.subdividing_edge();
    BOP.joining_edge();
    for(size_t i=0; i<BOP.resultP.size(); i++){
        auto &contour=BOP.resultP[i];
        for(auto &coor:contour){
            std::cout<<coor.first<<" "<<coor.second<<std::endl;
        }
        std::cout<<BOP.result_INOUT[i]<<std::endl;
        std::cout<<std::endl;
    }

    return 0;
}