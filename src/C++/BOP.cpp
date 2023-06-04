#include "BOP.hpp"

void BoolOperaPolygon::create_SweepEvent(){
    for(auto &poly:Polygon_list){
        size_t last_n=0;
        for(auto &vertex:poly.vertex_list){
            size_t n = vertex.size();
            std::list<Coor>::iterator itv = vertex.begin();
            for (size_t i = 0; i < n; ++i){
                SweepEvent event(*std::next(itv, i));
                event.pid = poly.pid;
                poly.SE_list_1.push_back(event);
                poly.SE_list_2.push_back(event);
            }
            std::list<SweepEvent>::iterator it1 = poly.SE_list_1.begin();
            it1 = std::next(it1, last_n);
            std::list<SweepEvent>::iterator it2 = poly.SE_list_2.begin();
            it2 = std::next(it2, last_n);
            last_n += n;
            for (size_t i = 0; i < n; ++i) {
                SweepEvent& event_1 = *std::next(it1, i);
                SweepEvent& event_2 = *std::next(it2, i);
                const Coor& v_prev = *std::next(itv, (i - 1 + n) % n);
                const Coor& v = *std::next(itv, i);
                const Coor& v_next = *std::next(itv, (i + 1) % n);
                if(v.x==v_next.x)
                    event_1.left = (v.y<v_next.y)? true:false;
                else
                    event_1.left = (v.x<v_next.x)? true:false;
                event_1.otherEvent = &(*std::next(it2, (i + 1) % n));
                if(event_1.coor.x==event_1.otherEvent->coor.x){
                    event_1.slope = 0;
                    // event_1.y_sweepline = event_1.otherEvent->coor.y;
                }
                else
                    event_1.slope = (event_1.coor.y-event_1.otherEvent->coor.y) / (event_1.coor.x-event_1.otherEvent->coor.x);
                PQ.push(&event_1);
                if(v.x==v_prev.x)
                    event_2.left = (v.y<v_prev.y)? true:false;
                else
                    event_2.left = (v.x<v_prev.x)? true:false;
                event_2.otherEvent = &(*std::next(it1, (i - 1 + n) % n));
                if(event_2.coor.x==event_2.otherEvent->coor.x){
                    event_2.slope = 0;
                    // event_2.y_sweepline = event_2.otherEvent->coor.y;
                }
                else
                    event_2.slope = (event_2.coor.y-event_2.otherEvent->coor.y) / (event_2.coor.x-event_2.otherEvent->coor.x);
                PQ.push(&event_2);
                // std::cout << event_1.coor.x << " " << event_1.coor.y << " " << event_1.otherEvent->coor.x << " " << event_1.otherEvent->coor.y << std::endl;
            }
        }
    }
}

void BoolOperaPolygon::subdividing_edge(){
    while(!PQ.empty()){
        SweepEvent* event = PQ.top();
        PQ.pop();
        std::cout << event->coor.x << " " << event->coor.y << " " << event->otherEvent->coor.x << " " << event->otherEvent->coor.y << "   " << event->left << " " << event->y_sweepline << std::endl;
        std::set<SweepEvent*, SEcmp> S_buffer;
        for(auto element:S){
            if(element->slope)
                element->y_sweepline = element->coor.y+element->slope*(event->coor.x-element->coor.x);
            S_buffer.insert(element);
        }
        S = S_buffer;
        for(auto& s:S)
            std::cout<<"("<<s->coor.x<<","<<s->coor.y<<"   "<<s->otherEvent->coor.x<<","<<s->otherEvent->coor.y<<")   ";
        std::cout<<std::endl;
        if(event->left){
            auto ret = S.insert(event);
            auto it = ret.first;
            if (it == S.begin()){   //ple == NULL
                (*it)->inOut = false;
                (*it)->inside = false;
            }
            else{
                setInsideFlag(*it, *std::prev(it));
                // SweepEvent* ple = *std::prev(it);
                // std::cout<< ple->coor.x<<" "<<ple->coor.y<< std::endl;
                // for(auto& s:S)
                //     std::cout<<"("<<s->coor.x<<","<<s->coor.y<<"   "<<s->otherEvent->coor.x<<","<<s->otherEvent->coor.y<<") "<<s->y_sweepline;
                // std::cout<<std::endl;
            }
            std::cout <<"inOut: "<< event->inOut <<"   inside: "<< event->inside<<std::endl;
            if (std::next(it) != S.end())
                possibleInter(*it, *std::next(it));
            if (it != S.begin())
                possibleInter(*std::prev(it), *it);
            add_result(event);
        }
        else{
            auto it = S.find(event->otherEvent);
            if(it==S.end()){
                // for(auto& s:S)
                //     std::cout<<"("<<s->coor.x<<","<<s->coor.y<<"   "<<s->otherEvent->coor.x<<","<<s->otherEvent->coor.y<<") "<<"("<<s->otherEvent->coor.x<<","<<s->otherEvent->coor.y<<"   "<<s->otherEvent->otherEvent->coor.x<<","<<s->otherEvent->otherEvent->coor.y<<")   ";
                // std::cout<<std::endl;
                std::cout<<"error"<<std::endl;
                exit(1);
            }
            SweepEvent* next_event;
            SweepEvent* prev_event;
            bool check = false;
            if(std::next(it) != S.end() && it != S.begin()){
                next_event = *std::next(it);
                prev_event = *std::prev(it);
                check = true;
            }
            S.erase(it);
            if(check)
                possibleInter(prev_event, next_event);
        }
        for(auto& s:S)
            std::cout<<"("<<s->coor.x<<","<<s->coor.y<<"   "<<s->otherEvent->coor.x<<","<<s->otherEvent->coor.y<<")   ";
        std::cout<<std::endl;
    }
    size_t n = result_event.size();
    for(size_t i=0; i<n;i++){
        if(type == 3 && result_event[i]->pid == 1)
            result_event[i]->inside = !result_event[i]->inside;
        result_event[i]->otherEvent->inside = result_event[i]->inside;
        result_event.push_back(result_event[i]->otherEvent);
    }

    std::sort(result_event.begin(), result_event.end(), [](SweepEvent* a, SweepEvent* b){
        if(a->coor.x == b->coor.x)
            return a->coor.y < b->coor.y;
        return a->coor.x < b->coor.x;
    });
    std::cout<<std::endl;
    std::cout<<result_event.size()<<std::endl;
    for(auto s:result_event)
        std::cout<<"("<<s->coor.x<<","<<s->coor.y<<"   "<<s->otherEvent->coor.x<<","<<s->otherEvent->coor.y<<")   " <<s->inside<<std::endl;;
    return;
}

void BoolOperaPolygon::possibleInter(SweepEvent* a, SweepEvent* b){
    if(a->pid == b->pid)
        return;
    const Coor& p1 = a->coor;
    const Coor& p2 = a->otherEvent->coor;
    const Coor& q1 = b->coor;
    const Coor& q2 = b->otherEvent->coor;
    // Line segment a: p1-p2
    double x1 = p1.x;
    double y1 = p1.y;
    double x2 = p2.x;
    double y2 = p2.y;
    // Line segment b: q1-q2
    double x3 = q1.x;
    double y3 = q1.y;
    double x4 = q2.x;
    double y4 = q2.y;
    // Calculate the direction of the lines
    double d1 = direction(x3, y3, x4, y4, x1, y1);
    double d2 = direction(x3, y3, x4, y4, x2, y2);
    double d3 = direction(x1, y1, x2, y2, x3, y3);
    double d4 = direction(x1, y1, x2, y2, x4, y4);
    
    // Check if the line segments intersect
    if (d1*d2<0 && d3*d4<0) {   //Type1
        double interX = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
        double interY = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
        std::cout<<"Type 1: intersect at "<<interX<<" "<<interY<<std::endl;
        update_PQandS(a, b, interX, interY, 1);
    }
    else if(d1*d2==0 && d3*d4<0){   //Type2
        double interX = (!d1)? x1:x2;
        double interY = (!d1)? y1:y2;
        // std::cout<<"Type 2: intersect at "<<interX<<" "<<interY<<std::endl;
        update_PQandS(a, b, interX, interY, 2);
    }
    else if(d1*d2<0 && d3*d4==0){   //Type3
        double interX = (!d3)? x3:x4;
        double interY = (!d3)? y3:y4;
        // std::cout << x1 << " " << y1 << " " << x2 << " " << y2 << " " << x3 << " " << y3 << " " << x4 << " " << y4 << std::endl;
        // std::cout<<"Type 3: intersect at "<<interX<<" "<<interY<<std::endl;
        update_PQandS(a, b, interX, interY, 3);
    }
    else if(!d1 && !d2 && !d3 && !d4){   //Type4
        // std::cout << x1 << " " << y1 << " " << x2 << " " << y2 << " " << x3 << " " << y3 << " " << x4 << " " << y4 << std::endl;
        std::set<pdd> collinear;
        collinear.insert({x1, y1});
        collinear.insert({x2, y2});
        collinear.insert({x3, y3});
        collinear.insert({x4, y4});
        // std::cout<<collinear.size()<<std::endl;
        size_t i = -1;
        for(auto coor:collinear){
            i++;
            if(i==0 || i==collinear.size()-1) continue;
            double interX = coor.first;
            double interY = coor.second;
            // std::cout<<"Type 4: intersect at "<<interX<<" "<<interY<<std::endl;
            if((interX==x1 && interY==y1) || (interX==x2 && interY==y2))    //intersect at a-terminal
                update_PQandS(a, b, interX, interY, 2);
            else    //intersect at b-terminal
                update_PQandS(a, b, interX, interY, 3);
        }
    }

    return;
}

void BoolOperaPolygon::update_PQandS(SweepEvent* a, SweepEvent* b, double interX, double interY, size_t type){
        Polygon* poly1 = &Polygon_list[a->pid];
        Polygon* poly2 = &Polygon_list[b->pid];

        if(type == 1 || type == 3){                             //subdivide a
            poly1->vertex_list.back().push_back(Coor(interX, interY));
            SweepEvent new_event1(poly1->vertex_list.back().back());
            new_event1.pid = poly1->pid;
            new_event1.slope = a->slope;
            poly1->SE_list_1.push_back(new_event1);
            poly1->SE_list_2.push_back(new_event1);
            a->otherEvent->otherEvent = &poly1->SE_list_1.back();
            poly1->SE_list_1.back().otherEvent = a->otherEvent;
            poly1->SE_list_1.back().left = 1;
            a->otherEvent = &poly1->SE_list_2.back();
            poly1->SE_list_2.back().otherEvent = a;
            poly1->SE_list_2.back().left = 0;
            // if(poly1->SE_list_1.back().coor.x==poly1->SE_list_1.back().otherEvent->coor.x)
            //     poly1->SE_list_1.back().y_sweepline = poly1->SE_list_1.back().otherEvent->coor.y;
            // if(poly1->SE_list_2.back().coor.x==poly1->SE_list_2.back().otherEvent->coor.x)
            //     poly1->SE_list_2.back().y_sweepline = poly1->SE_list_2.back().otherEvent->coor.y;
            PQ.push(&poly1->SE_list_1.back());
            PQ.push(&poly1->SE_list_2.back());
        }
        if(type == 1 || type == 2){                             //subdivide b
            poly2->vertex_list.back().push_back(Coor(interX, interY));
            SweepEvent new_event2(poly2->vertex_list.back().back());
            new_event2.pid = poly2->pid;
            new_event2.slope = b->slope;
            poly2->SE_list_1.push_back(new_event2);
            poly2->SE_list_2.push_back(new_event2);
            b->otherEvent->otherEvent = &poly2->SE_list_1.back();
            poly2->SE_list_1.back().otherEvent = b->otherEvent;
            poly2->SE_list_1.back().left = 1;
            b->otherEvent = &poly2->SE_list_2.back();
            poly2->SE_list_2.back().otherEvent = b;
            poly2->SE_list_2.back().left = 0;
            // if(poly2->SE_list_1.back().coor.x==poly2->SE_list_1.back().otherEvent->coor.x)
            //     poly2->SE_list_1.back().y_sweepline = poly2->SE_list_1.back().otherEvent->coor.y;
            // if(poly2->SE_list_2.back().coor.x==poly2->SE_list_2.back().otherEvent->coor.x)
            //     poly2->SE_list_2.back().y_sweepline = poly2->SE_list_2.back().otherEvent->coor.y;
            PQ.push(&poly2->SE_list_1.back());
            PQ.push(&poly2->SE_list_2.back());
        }
}

void BoolOperaPolygon::setInsideFlag(SweepEvent* le, SweepEvent* ple){
    if(le->pid == ple->pid){    //same poly
        le->inside = ple->inside;
        le->inOut = !ple->inOut;
    }
    else{
        le->inside = !ple->inOut;
        le->inOut = ple->inside;       
    }
    return;
}

void BoolOperaPolygon::add_result(SweepEvent* event){
    switch (type)
    {
    case 0:
        if(!event->inside){
            result_event.push_back(event);
        }
        break;
    case 1:
        if(event->inside){
            result_event.push_back(event);
        }
        break;
    case 2:
        result_event.push_back(event);
        break;
    case 3:
        if((!event->inside && event->pid==0) || (event->inside && event->pid==1)){
            result_event.push_back(event);
        }
        break;
    
    default:
        break;
    }
}

void BoolOperaPolygon::joining_edge(){
    for(size_t i=0; i<result_event.size(); i++)
        result_event[i]->pos = i;
    // size_t currentid = 0;

    for(size_t i=0; i<result_event.size(); i++){
        SweepEvent* event = result_event[i];
        if(event->processed)    continue;
        std::vector<pdd> Contour;
        std::set<pdd> contour_vertex;
        target = i;
        Contour.push_back({result_event[i]->coor.x, result_event[i]->coor.y});
        compute_contour(result_event[i]->otherEvent->pos, Contour, contour_vertex);
        event->processed = true;
        event->otherEvent->processed = true;
        if(result_INOUT.empty())
            result_INOUT.push_back(1);
        else{
            pdd left_bottom = {event->coor.x, event->coor.y};
            double dis = (event->coor.x)*(event->coor.x) + (event->coor.y)*(event->coor.y);
            for(auto vertex:Contour){
                double dis_cmp = (vertex.first)*(vertex.first) + (vertex.second)*(vertex.second);
                if(dis_cmp < dis){
                    dis = dis_cmp;
                    left_bottom = vertex;
                }
            }
            check_child_contour(left_bottom);
        }
        std::cout<<std::endl;
        if(direction(Contour[0].first, Contour[0].second, Contour[1].first, Contour[1].second, Contour.back().first, Contour.back().second)>0)    //clockwise
            reverse(Contour.begin(), Contour.end());
        resultP.push_back(Contour);
    }
}

bool BoolOperaPolygon::compute_contour(size_t pos, std::vector<pdd>& Contour, std::set<pdd>& contour_vertex){
    std::cout<<result_event[pos]->coor.x<<" "<<result_event[pos]->coor.y<<"   "<<pos<<std::endl;
    pdd coor = {result_event[pos]->coor.x, result_event[pos]->coor.y};
    if(contour_vertex.count(coor) || result_event[pos]->processed)
        return 1;
    if(result_event[pos]->coor == result_event[target]->coor && pos!=target){
        result_event[pos]->processed = true;
        result_event[pos]->otherEvent->processed = true;
        return 0;
    }
    contour_vertex.insert(coor);
    SweepEvent* curr_event = result_event[pos];
    pos = curr_event->pos;
    size_t prev_pos = pos-1, next_pos = pos+1;
    bool chance = true;
    while(1){
        if(prev_pos>=0 && result_event[prev_pos]->coor==curr_event->coor){
            if(result_event[prev_pos]->inside==curr_event->inside){
                if(!compute_contour(result_event[prev_pos]->otherEvent->pos, Contour, contour_vertex)){
                    result_event[pos]->processed = true;
                    result_event[pos]->otherEvent->processed = true;
                    Contour.push_back({result_event[pos]->coor.x, result_event[pos]->coor.y});
                    return 0;
                }
                else
                    contour_vertex.erase(coor);
            }
            chance = true;
        }
        if(next_pos<result_event.size() && result_event[next_pos]->coor==curr_event->coor){
            if(result_event[next_pos]->inside==curr_event->inside){
                if(!compute_contour(result_event[next_pos]->otherEvent->pos, Contour, contour_vertex)){  
                    result_event[pos]->processed = true;
                    result_event[pos]->otherEvent->processed = true;
                    Contour.push_back({result_event[pos]->coor.x, result_event[pos]->coor.y});
                    return 0;
                }
                else
                    contour_vertex.erase(coor);
            }
            chance = true;
        }
        if(!chance) break;
        prev_pos--;
        next_pos++;
        chance = false;
    }
    return 1;
}

void BoolOperaPolygon::check_child_contour(pdd v){
    std::pair<Coor, Coor> inter_edge={{-1,-1},{-1,-1}};
    // vertical ray
    double x1 = v.first;
    double y1 = v.second;
    double x2 = v.first;
    double y2 = -1;
    std::cout<<x1<<","<<y1<<"   "<<x2<<","<<y2<<std::endl;
    double dis_min = -1;
    size_t inter_cid;

    for(size_t j=0; j<resultP.size(); j++){
        auto contour = resultP[j];
        size_t n = contour.size();
        for(size_t i=0; i<n; i++){
            // contour line
            double x3 = contour[i].first;
            double y3 = contour[i].second;
            double x4 = contour[(i+1)%n].first;
            double y4 = contour[(i+1)%n].second;
            double ymin = (y3<=y4)? y3:y4;
            double ymax = (ymin==y3)? y4:y3;
            // Calculate the direction of the lines
            double d1 = direction(x3, y3, x4, y4, x1, y1);
            double d2 = direction(x3, y3, x4, y4, x2, y2);
            double d3 = direction(x1, y1, x2, y2, x3, y3);
            double d4 = direction(x1, y1, x2, y2, x4, y4);

            if (d1*d2>0 || d3*d4>0) continue;
            else if(d1*d2==0 && d3*d4==0)
                if((y1<ymin && y2<ymin) || (y3<y2 && y4<y2))
                    continue;
            double dis_y;
            if (d1*d2<0 && d3*d4<0)
                dis_y = y1-((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
            else if(!d1 && !d2 && !d3 && !d4)
                dis_y = y1-ymax;
            else if(d1==0)
                dis_y = 0;
            else if(d3==0)
                dis_y = y1-y3;
            else
                dis_y = y1-y4;
            std::cout<<x3<<","<<y3<<"   "<<x4<<","<<y4<<" "<<dis_y<<std::endl;
            if(dis_min == -1 || dis_y < dis_min){
                std::cout<<"  "<<x3<<","<<y3<<"   "<<x4<<","<<y4<<"   "<<dis_y<<std::endl;
                inter_edge = {{x3,y3},{x4,y4}};
                dis_min = dis_y;
                inter_cid = j;
            }
            else if(dis_y == dis_min){
                double y_same = (d3==0)? y3:y4;
                double y_cmp = (y3==y_same)? y4:y3;
                double y_now = (inter_edge.first.y==y_same)? inter_edge.second.y:inter_edge.first.y;
                if(y_cmp>y_now){
                    std::cout<<"  "<<x3<<","<<y3<<"   "<<x4<<","<<y4<<"   "<<dis_y<<std::endl;
                    inter_edge = {{x3,y3},{x4,y4}};
                    inter_cid = j;
                }
            }
        }
    }
    if(inter_edge.first.x != -1){
        std::cout<<"  "<<inter_edge.first.x<<","<<inter_edge.first.y<<"   "<<inter_edge.second.x<<","<<inter_edge.second.y<<std::endl;
        if(inter_edge.first.x<inter_edge.second.x)
            result_INOUT.push_back(!result_INOUT[inter_cid]);
        else
            result_INOUT.push_back(result_INOUT[inter_cid]);
    }
    else
        result_INOUT.push_back(1);
}