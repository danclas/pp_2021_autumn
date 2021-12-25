// Copyright 2021 Shabunin Vladislav
#ifndef MODULES_TASK_2_SHABUNIN_V_SLEEPING_BARBER_SLEEPING_BARBER_H_
#define MODULES_TASK_2_SHABUNIN_V_SLEEPING_BARBER_SLEEPING_BARBER_H_

#define SEAT_REQUEST 1
#define SEAT_RESPONSE 2
#define HAIRCUT_REQUEST 3
#define HAIRCUT 4

void barber(int rank, int client_count);
void client(int rank);
void queue(int seat_count, int runs);

#endif  // MODULES_TASK_2_SHABUNIN_V_SLEEPING_BARBER_SLEEPING_BARBER_H_
