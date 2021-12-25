// Copyright 2021 Mironova Ekaterina
#include "../../../modules/task_3/mironova_e_jarvis_pass/jarvis_pass.h"

void buildStructPoint(MPI_Datatype* struct_point) {
    int count = 2;
    int array_of_blocklengths[] = { 1, 1 };
    MPI_Aint array_of_displacements[] = { offsetof(point, x), offsetof(point, y) };
    MPI_Datatype array_of_types[] = { MPI_INT, MPI_INT };
    MPI_Datatype tmp_type;
    MPI_Aint lb, extent;
    MPI_Type_create_struct(count, array_of_blocklengths, array_of_displacements, array_of_types, &tmp_type);
    MPI_Type_get_extent(tmp_type, &lb, &extent);
    MPI_Type_create_resized(tmp_type, lb, extent, struct_point);
    MPI_Type_commit(struct_point);
}

std::vector<point> fillPoints(const int size) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<point> points(size);
    for (int i = 0; i < size; i++) {
        points[i].x = dev() % 1000;
        points[i].y = dev() % 1000;
    }
    return points;
}

int rotation(const point& a, const point& b, const point& c) {
    return (b.x - a.x) * (c.y - b.y)
        - (b.y - a.y) * (c.x - b.x);
}

std::vector<point> jarvisPass(const std::vector<point>& points) {
    std::vector<int> vectorOfIndex(points.size());
    int pointsNumber = points.size();
    for (int i = 0; i < pointsNumber; i++) {
        vectorOfIndex[i] = i;
    }
    for (int i = 1; i < pointsNumber; ++i) {
        if (points[vectorOfIndex[i]].x < points[vectorOfIndex[0]].x) {
            std::swap(vectorOfIndex[i], vectorOfIndex[0]);
        }
    }
    std::vector<int> result = { vectorOfIndex[0] };
    vectorOfIndex.erase(vectorOfIndex.begin());
    vectorOfIndex.push_back(result[0]);
    while (true) {
        int right = 0;
        for (std::vector<point>::size_type i = 1; i < vectorOfIndex.size(); i++) {
            if (rotation(points[result[result.size() - 1]],
                points[vectorOfIndex[right]], points[vectorOfIndex[i]]) < 0) {
                right = i;
            }
        }
        if (vectorOfIndex[right] == result[0]) {
            break;
        } else {
            result.push_back(vectorOfIndex[right]);
            vectorOfIndex.erase(vectorOfIndex.begin() + right);
        }
    }
    std::vector<point> pass(result.size());
    for (std::vector<point>::size_type i = 0; i < result.size(); i++) {
        pass[i] = points[result[i]];
    }
    return pass;
}

std::vector<point> parallelJarvisPass(const std::vector<point>& points, const int size) {
    int procNum, procRank;
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    MPI_Datatype struct_point;
    buildStructPoint(&struct_point);
    int dataPiece = size/ procNum;
    int remainingData = size % procNum;
    std::vector<point> partOfPoints(dataPiece);
    std::vector<point> partJarvisPass, result;
    MPI_Scatter(points.data(), dataPiece, struct_point, partOfPoints.data(), dataPiece,
        struct_point, 0, MPI_COMM_WORLD);
    partJarvisPass = jarvisPass(partOfPoints);
    if (procRank != 0) {
        MPI_Send(partJarvisPass.data(), partJarvisPass.size(), struct_point, 0, 0, MPI_COMM_WORLD);
    } else {
        for (int i = 1; i < procNum; i++) {
            MPI_Status status;
            int sendElements = 0;
            MPI_Probe(i, 0, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, struct_point, &sendElements);
            std::vector<int>::size_type oldSize = partJarvisPass.size();
            partJarvisPass.resize(oldSize + sendElements);
            MPI_Recv(partJarvisPass.data() + oldSize, sendElements, struct_point, i, 0,
                MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        if ((procNum != 1) && (remainingData != 0)) {
            partOfPoints = std::vector<point>(points.begin() +
               (procNum * dataPiece), points.end());
            result = jarvisPass(partOfPoints);
            std::vector<int>::size_type oldPartJarvisPassSize = partJarvisPass.size();
            partJarvisPass.resize(oldPartJarvisPassSize + result.size());
            for (std::vector<int>::size_type i = oldPartJarvisPassSize, j = 0; i < partJarvisPass.size(); i++, j++) {
                partJarvisPass[i] = result[j];
            }
        }
        result = jarvisPass(partJarvisPass);
    }
    return result;
}
