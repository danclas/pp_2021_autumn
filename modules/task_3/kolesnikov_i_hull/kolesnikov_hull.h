// Copyright 2021 Kolesnikov Ilya
#ifndef MODULES_TASK_3_KOLESNIKOV_I_HULL_KOLESNIKOV_HULL_H_
#define MODULES_TASK_3_KOLESNIKOV_I_HULL_KOLESNIKOV_HULL_H_
#include <vector>
#include <string>
#include <fstream>
#include <ctime>

MPI_Comm topology_ring(const MPI_Comm comm);

int single_find_symbol(char symbol, std::string str);
int parallel_find_symbol(char symbol, std::string str);

class Point {
 public:
    Point(int k, int l) {
        x = k; y = l;
    }
    explicit Point(Point* pt) {
        x = pt->returnX(); y = pt->returnY();
    }
    int returnX() {
        return this->x;
    }
    int returnY() {
        return this->y;
    }
    bool operator != (Point* other) {
        bool check1 = other->returnX()!= this->x && other->returnY()!= this->y;
        bool check2 = other->returnX() == this->x && other->returnY() != this->y;
        bool check3 = other->returnX() != this->x && other->returnY() == this->y;
        if (check1 && check2 && check3) {
            return true;
        } else {
            return false;
        }
    }
    bool operator == (Point* other) {
        return (other->returnX() == this->x) && (other->returnY() == this->y)?true:false;
    }
    Point operator - (Point* p) {
        return Point(this->x - p->returnX(), this->y - p->returnY());
    }

 private:
    int x, y;
};

void swap_p(Point* a, Point* b);
std::vector<Point> sort_vec(std::vector<Point> vec);
int orientation(Point p, Point q, Point r);
int get_pre_last(std::vector<Point> vec);
std::vector<int> point_to_int(std::vector<Point> vec);
std::vector<Point> int_to_point(std::vector<int> vec);
std::vector<Point> convexHull_jarvis(std::vector<Point> vec);
std::vector<Point> convexHull_jarvis_parallel(std::vector<Point> vec);

#endif  // MODULES_TASK_3_KOLESNIKOV_I_HULL_KOLESNIKOV_HULL_H_
