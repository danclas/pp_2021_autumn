// Copyright 2021 Mironova Ekaterina
#ifndef MODULES_TASK_2_MIRONOVA_E_READERS_WRITERS_READERS_WRITERS_H_
#define MODULES_TASK_2_MIRONOVA_E_READERS_WRITERS_READERS_WRITERS_H_

#include <string>

void read();
void write();
void reader(int rank);
void writer(int rank);
void manager(int rank, int procs);

#define S_REQUEST 1
#define S_RESPONSE 2
#define S_RELEASE 3

#define RC_REQUEST 4
#define RC_RESPONSE 5
#define RC_RELEASE 6

#define ACCESS_REQUEST 7
#define ACCESS_RESPONSE 8
#define ACCESS_RELEASE 9

#define FINISH 99

#endif  // MODULES_TASK_2_MIRONOVA_E_READERS_WRITERS_READERS_WRITERS_H_
