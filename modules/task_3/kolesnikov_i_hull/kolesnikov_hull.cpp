// Copyright 2021 Kolesnikov Ilya
#include <mpi.h>
#include "../../../modules/task_3/kolesnikov_i_hull/kolesnikov_hull.h"

void swap_p(Point* a, Point* b) {
    Point a_tmp = *a;
    *a = *b;
    *b = a_tmp;
}

std::vector<Point> sort_vec(std::vector<Point> vec) {
    int vec_size = vec.size();
    for (int i(0); i < vec_size; ++i) {
        for (int j(0); j < vec_size - 1; ++j) {
            if ((vec[j].returnX() > vec[j + 1].returnX())) {
                swap_p(&vec[j], &vec[j + 1]);
            }
        }
    }
    for (int i(0); i < vec_size; ++i) {
        for (int j(0); j < vec_size - 1; ++j) {
            if ((vec[j].returnX() == vec[j + 1].returnX()) && (vec[j].returnY() > vec[j + 1].returnY())) {
                swap_p(&vec[j], &vec[j + 1]);
            }
        }
    }
    return vec;
}

int orientation(Point p, Point q, Point r) {
    int res = (q.returnY() - p.returnY()) *
    (r.returnX() - q.returnX()) - (r.returnY() - q.returnY()) * (q.returnX() - p.returnX());
    return res;
}

int get_pre_last(std::vector<Point> vec) {
    int res = 0;
    bool check = false;
    int x = vec[1].returnX() - vec[0].returnX();
    int y = vec[1].returnY() - vec[0].returnY();
    int vec_size = vec.size();
    for (int i(2); i < vec_size; ++i) {
        if (vec[i].returnX() - vec[i - 1].returnX() == x && vec[i].returnY() - vec[i - 1].returnY() == y) {
            check = true;
        } else {
            check = false;
            break;
          }
    }
    if (check) {
        return vec.size() - 1;
    }
    for (int i(1); i < vec_size; ++i) {
        if (vec[0].returnX() == vec[i].returnX()) {
            return i;
        }
    }
    Point pt(0, 0);
    if (orientation(vec[0], vec[1], vec[2]) <= 0) {
        if (vec[1].returnY() >= vec[0].returnY()) {
            return 1;
        }
    }

    return res;
}

std::vector<int> point_to_int(std::vector<Point> vec) {
    std::vector<int> res;
    for (auto pt : vec) {
        res.push_back(pt.returnX());
        res.push_back(pt.returnY());
    }

    return res;
}

std::vector<Point> int_to_point(std::vector<int> vec) {
    std::vector<Point> res;
    int vec_size = vec.size()-1;
    for (int i(0); i < vec_size; i+=2) {
         res.push_back(Point(vec[i], vec[i + 1]));
    }

    return res;
}

std::vector<Point> convexHull_jarvis_parallel(std::vector<Point> vec) {
    std::vector<Point> steps;
    int size = 0, rank = 0;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status status;
    int l = 0;
    int n = vec.size();
    for (int i = 0; i < n; i++) {
        if (vec[i].returnX() < vec[l].returnX()) {
            l = i;
        }
    }
    int x = l, y;
    int range = 6;
    if (rank != 0) {
        range = 6;
        MPI_Recv(&x, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
    } else if (size == 1) {
        range = vec.size();
    }
    int cnt = 0;
    int pre_last = get_pre_last(vec);
    int x_tmp = 0;

    if (rank != size - 1) {
        do {
            if (x == pre_last) {
                break;
            }
            if (rank != size - 1 && &vec[x] == &vec[0] && cnt > 0) {
                break;
            }
            if (cnt == range - 1) {
                x_tmp = x;
            }
            if (cnt <= range) {
                if (cnt + 1 < range) {
                    steps.push_back(vec[x]);
                }
            }
            if (cnt > range) {
                break;
            }
            y = (x + 1) % n;
            for (auto i = 0; i < n; i++) {
                if (i + 1 < n) {
                    if (&vec[i] == &vec[i + 1]) {
                        continue;
                    }
                }
                if (orientation(vec[x], vec[i], vec[y]) < 0) {
                    y = i;
                }
            }
            x = y;
            cnt++;
        } while (x != l);
    } else {
        do {
            steps.push_back(vec[x]);
            y = (x + 1) % n;
            for (auto i = 0; i < n; i++) {
                if (i + 1 < n) {
                    if (&vec[i] == &vec[i + 1]) {
                        continue;
                    }
                }
                if (orientation(vec[x], vec[i], vec[y]) < 0) {
                    y = i;
                }
            }
            x = y;
            cnt++;
            if (x == pre_last) {
                steps.push_back(vec[x]);
                break;
            }
        } while (x != l);
    }
    if (rank != size - 1) {
        MPI_Send(&x_tmp, 1, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
    } else {
        if (&steps[0] != &steps[steps.size() - 1]) {
            steps.push_back(vec[0]);
        }
    }

    std::vector<int> real_points = point_to_int(steps);
    std::vector<int> send_counts(size);
    int size_to_send = real_points.size();
    MPI_Gather(&size_to_send, 1, MPI_INT, send_counts.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(send_counts.data(), size, MPI_INT, 0, MPI_COMM_WORLD);
    std::vector <int> dspls;
    int sum_p = 0;
    for (int i(0); i < size; ++i) {
        dspls.push_back(sum_p);
        sum_p += send_counts[i];
    }
    std::vector<int> steps_to_send(sum_p);
    MPI_Gatherv(real_points.data(), real_points.size(), MPI_INT, steps_to_send.data(),
    send_counts.data(), dspls.data(), MPI_INT, 0, MPI_COMM_WORLD);
    std::vector<Point>steps_to_send2 = int_to_point(steps_to_send);
    std::vector<Point>steps_to_send3;
    steps_to_send3.push_back(steps_to_send2[0]);
    int steps_size_t = steps_to_send2.size();
    for (int i(1); i < steps_size_t; ++i) {
        if (&steps_to_send2[i] == &steps_to_send2[0]) {
            steps_to_send3.push_back(steps_to_send2[i]);
            break;
        }
        steps_to_send3.push_back(steps_to_send2[i]);
    }
    return steps_to_send3;
}

std::vector<Point> convexHull_jarvis(std::vector<Point> vec) {
    std::ofstream points, hull;
    points.open("points.txt");
    hull.open("hull.txt");
    std::vector<Point> steps;
    int l = 0;
    int n = vec.size();
    for (auto i = 0; i < n; i++) {
        if (vec[i].returnX() < vec[l].returnX()) {
            l = i;
        }
    }
    int x = l, y;
    int pre_last = get_pre_last(vec);
    do {
        steps.push_back(vec[x]);
        y = (x + 1) % n;
        for (auto i = 0; i < n; i++) {
            if (i + 1 < n) {
                if (&vec[i] == &vec[i + 1]) {
                    continue;
                }
            }
            if (orientation(vec[x], vec[i], vec[y]) < 0) {
                y = i;
            }
        }
        x = y;
        if (x == pre_last) {
            steps.push_back(vec[x]);
            break;
        }
    } while (x != l);
    if (&steps[0] != &steps[steps.size() - 1]) {
        steps.push_back(vec[0]);
    }
    int vec_size = vec.size();
    for (int i(0); i < vec_size; ++i) {
        points << vec[i].returnX() << " " << vec[i].returnY() << "\n";
    }
    int steps_size = steps.size();
    for (int i(0); i < steps_size; ++i) {
        hull << steps[i].returnX() << " " << steps[i].returnY() << "\n";
    }
    points.close();
    hull.close();
    return steps;
}
