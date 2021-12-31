// Copyright 2021 Belyaev Ilya

#ifndef MODULES_TASK_2_BELYAEV_I_READERS_WRITERS_READERS_WRITERS_H_
#define MODULES_TASK_2_BELYAEV_I_READERS_WRITERS_READERS_WRITERS_H_
#include <string>




#define S_Query 1
#define S_Answer 2
#define S_Free 3


#define R_Query 4
#define R_Answer 5
#define R_Free 6



#define A_Query 7
#define A_Answer 8
#define A_Free 9
#define THE_END 10



#define RCSENDING 11



void write_s();
void read_s();
void writers(int rank);
void readers(int rank);
void storage(int rank, int procs);




#endif  // MODULES_TASK_2_BELYAEV_I_READERS_WRITERS_READERS_WRITERS_H_
