// Copyright 2021 Uglinskii Bogdan
#include <mpi.h>
#include <random>
#include "../../../modules/task_1/uglinskii_b_word_count/word_count.h"

int ParallelWordCount(std::string input_str) {
  int ProcNum, ProcRank;

  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  int str_size = input_str.size();

  int chunck_size = 0;

  if (str_size % ProcNum != 0) {
    int spaces_len = ProcNum - str_size % ProcNum;
    for (int i = 0; i < spaces_len; i++) {
      input_str += " ";
    }

    str_size = input_str.size();
  }

  int index = 0, count = 0, substr_word_count = 0;

  if (ProcRank == 0) {
    int size_to_send = input_str.size();
    if (ProcNum > 1) {
      for (int i = 1; i < ProcNum; i++) {
        MPI_Send(&size_to_send, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
        MPI_Send(input_str.c_str(), size_to_send, MPI_CHAR, i, 2,
                 MPI_COMM_WORLD);
      }
    }
    chunck_size = (str_size) / ProcNum;

    substr_word_count = CountWordsSubstr(input_str.substr(0, chunck_size));

  } else {
    if (ProcNum > 1) {
      MPI_Status status;

      MPI_Recv(&str_size, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

      input_str.resize(str_size + 1);
      MPI_Recv(const_cast<char*>(input_str.data()), str_size, MPI_CHAR, 0, 2,
               MPI_COMM_WORLD, &status);

      chunck_size = str_size / ProcNum;
      index = chunck_size * ProcRank;
      substr_word_count = CountWordsSubstr(input_str.substr(index, chunck_size));
    }
  }

  MPI_Reduce(&substr_word_count, &count, 1, MPI_INT, MPI_SUM, 0,
             MPI_COMM_WORLD);

  if (ProcRank == 0) {
    index = 0;
    int double_counted_words = 0, cur_char = 0, prev_char = 0;

    for (int i = 1; i < std::min(str_size, ProcNum); i++) {
      index = i * chunck_size;

      cur_char = input_str[index];
      prev_char = input_str[index - 1];

      if ((prev_char != ' ') && (cur_char != ' ')) {
        double_counted_words++;
      }
    }
    count -= double_counted_words;

    return count;
  }

  return 0;
}

int CountWordsSubstr(std::string substr) {
  int count = 0;
  int i = 0;
  int size = substr.size();
  while ((substr[i] == '-' || substr[i] == ' ') && i < size) {
    i++;
  }
  int word = 0;

  while (i < size) {
    if ((substr[i] != ' ' && substr[i] != '-') && word == 0) {
      word = 1;
      count++;
    } else if (substr[i] == ' ') {
      word = 0;
    }
    i++;
  }

  return count;
}

std::string GenStr(int length, int max_size) {
  std::string str = "";
  int size = 1;

  std::random_device dev;
  std::mt19937 gen(dev());

  for (int i = 0; i < length; i++) {
    size = 1 + gen() % max_size;
    for (int j = 0; j < size; j++) {
      str += static_cast<char>(97 + gen() % (122 - 97));
    }
    str += ' ';
  }
  str.pop_back();
  return str;
}
