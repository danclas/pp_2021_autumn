// copyright 2021 Javier Galindo
#ifndef MODULES_TASK_3_GALINDO_SHELL_SHORT_BATCHER_GALINDO_SHELL_SHORT_BATCHER_H_
#define MODULES_TASK_3_GALINDO_SHELL_SHORT_BATCHER_GALINDO_SHELL_SHORT_BATCHER_H_
#include <vector>

using Vector = std::vector<int>;

Vector createRandomVector(int size);

Vector shellSort(Vector arr);

namespace BatcherMerge {
	Vector parallelSort(Vector arr,std::function<Vector(Vector)> sort_func);
}
#endif  //  MODULES_TASK_3_GALINDO_SHELL_SHORT_BATCHER_GALINDO_SHELL_SHORT_BATCHER_H_
