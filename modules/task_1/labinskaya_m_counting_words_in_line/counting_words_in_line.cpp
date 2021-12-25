// Copyright 2021 Labinskaya Marina
#include <mpi.h>
#include "../../../modules/task_1/labinskaya_m_counting_words_in_line/counting_words_in_line.h"

int sequential_counting_words(std::string str) {
    int creating_word = 0;
    int count = 0;

    str += ' ';

    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] != ' ' && str[i] != '-') {
            ++creating_word;
        } else {
             if (creating_word > 0) {
                 ++count;
                 creating_word = 0;
             }
        }
    }
    return count;
}

int parallel_counting_words(std::string str) {
    int ProcNum, ProcRank;
    int len_proc_str;

    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if (ProcRank == 0) {
        for (size_t i = 0; i < str.size() % ProcNum; ++i) {
            str += " ";
        }

        len_proc_str = str.size() / ProcNum;
    }

    MPI_Bcast(&len_proc_str, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (len_proc_str != 0) {
        int res_count = 0;

        std::string temp_str;
        temp_str.resize(len_proc_str);

        MPI_Scatter(str.c_str(), len_proc_str, MPI_CHAR, const_cast<char *>(temp_str.data()),
        len_proc_str, MPI_CHAR, 0, MPI_COMM_WORLD);

        int count = sequential_counting_words(temp_str);

        MPI_Reduce(&count, &res_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        if (ProcRank == 0) {
            int extra_count = 0;
            for (int i = 1; i < ProcNum; ++i)
                if (str[len_proc_str * i - 1] != ' ' && str[len_proc_str * i] != ' ')
                    ++extra_count;

            res_count -= extra_count;
            return res_count;
        }
        return 0;
    } else {
        return 0;
    }
}
