// Copyright 2021 Lazarev Alexey

#include "../../../modules/task_1/lazarev_numbers_of_words_in_line/word_count.h"

int GetCountString(std::string st) {
  int word = 0;
  int size = st.size();
  if (size > 0) {
    word = std::count(st.begin(), st.end(), ' ');
    word++;
    if (st[0] == ' ') {
      word--;
    }
    if (st[size - 1] == ' ') {
      word--;
    }
  }
  return word;
}

int Count_pp(std::string st) {
  int ProcNum, ProcRank;
  int size_string = st.size();
  int local_count = 0;
  int result = 0;
  int step = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  int* counts = new int[ProcNum];
  for (int i = 0; i < ProcNum; i++) {
    if (i < (size_string % ProcNum))
      counts[i] = size_string / ProcNum + 1;
    else
      counts[i] = size_string / ProcNum;
  }

  std::string local_string("", counts[ProcRank]);

  if (ProcRank == 0) {
    local_string = std::string(st.begin(), st.begin() + counts[ProcRank]);
    for (int i = 1; i < ProcNum; i++) {
      step += counts[i - 1];
      MPI_Send(&st[step], counts[i], MPI_CHAR, i, 0, MPI_COMM_WORLD);
    }
    local_count = GetCountString(std::string(
        local_string.begin(), local_string.begin() + counts[ProcRank]));
  } else {
    MPI_Status status;
    MPI_Recv(&local_string[0], counts[ProcRank], MPI_CHAR, 0, 0, MPI_COMM_WORLD,
             &status);
    local_count = GetCountString(std::string(
        local_string.begin(), local_string.begin() + counts[ProcRank]));
  }
  MPI_Reduce(&local_count, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (ProcRank == 0) {
    int stp = 0;
    for (int i = 1; i < ProcNum; i++) {
      stp += counts[i - 1];
      if (st[stp - 1] >= '!' && st[stp - 1] <= '~' && st[stp] >= '!' &&
          st[stp] <= '~') {
        result--;
      }
    }
  }
  MPI_Bcast(&result, 1, MPI_INT, 0, MPI_COMM_WORLD);
  return result;
}
