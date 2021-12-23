// Copyright 2021 Cheremushkin Kirill
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include "./reshetka_tor.h"

TEST(Cor_Tor, Creat_Tor) {  // Create tor topology
  int ProcRank, ProcNum;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);  // Poluchenie ranka
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);  // Poluchenie kol-vo razmerov 

  int rSize[2] = {0, 0};
  MPI_Dims_create(ProcNum, 2, rSize);
  MPI_Comm tor_comm = CreateTor(rSize[0], rSize[1]);  // Poluchenie CommTor

  if (ProcRank == 0) {
        ASSERT_TRUE(ExaminationTor(tor_comm, 2, rSize));  // Proverka pravilnogo sozdanie reshetki
  }
}

TEST(Cor_Tor, Cor_P) {
  int ProcRank, ProcNum;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  int rSize[2] = {0, 0};
  MPI_Dims_create(ProcNum, 2, rSize);  // Sozdanie dekartovoi reshetki
  MPI_Comm tor_comm = CreateTor(rSize[0], rSize[1]);

  int t_periods[2] = {ProcRank / rSize[1],
                    ProcRank - (ProcRank / rSize[1]) * rSize[1]};  // Rashet poziciy

  int p_coords[2];

  MPI_Cart_coords(tor_comm, ProcRank, 2, p_coords);  // Poluchenie coordinat
  //std::cout << "RANK" << ProcRank << t_periods[0] << "  " << p_coords[0] << std::endl;
  //std::cout << "RANK" << ProcRank << t_periods[1] << "  " << p_coords[1] << std::endl;
  ASSERT_EQ(t_periods[0], p_coords[0]);
  ASSERT_EQ(t_periods[1], p_coords[1]);
}

TEST(Cor_Tor, Cor_S) {  // Proverka razmera 
  int ProcRank, ProcNum;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  int rSize[2] = {0, 0};
  MPI_Dims_create(ProcNum, 2, rSize);  // Sozdanie decart
  MPI_Comm tor_comm = CreateTor(rSize[0] + 1, rSize[1] + 1); // Poluchenie TorComm

  if (ProcRank == 0) {
    ASSERT_EQ(tor_comm, MPI_COMM_NULL);
  }
}

TEST(Cor_Mail, two_points_of_departure) {
  int ProcRank, ProcNum;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  int rSize[2] = {0, 0};
  MPI_Dims_create(ProcNum, 2, rSize);  // Dekart
  MPI_Comm tor_comm = CreateTor(rSize[0], rSize[1]);  // Poluchenie TorComm

  if (ProcNum == 1) {  // Proverka kol-vo proccesov
    ASSERT_TRUE(true);
  } else {
    int count         = 0;  // Otpravliemoe
    int control_count = 25;  // Control value
    int direction[2] = {ProcNum - 1, ProcNum / 2};  // Kuda voevat
    if (ProcRank == 0) {
      count = 25;
      MPI_Send(&count, 1, MPI_INT, direction[0], 1, tor_comm);  // Otpravka soobshenia
      if (direction[0] != direction[1]) {  // Sravnenia napravlenia
        MPI_Send(&count, 1, MPI_INT, direction[1], 1, tor_comm);
      }
    } else if ((ProcRank == direction[0]) || (ProcRank == direction[1])) {
      MPI_Status status;
      MPI_Recv(&count, 1, MPI_INT, 0, 1, tor_comm, &status);  // poluchenie
      ASSERT_EQ(count, control_count);
    }
  }
}

TEST(Cor_Mail, All_MES) {  // Otpravka vsemi processami
  int ProcRank, ProcNum;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  int rSize[2] = {0, 0};
  MPI_Dims_create(ProcNum, 2, rSize);  // Sozdanie dekart
  MPI_Comm tor_comm = CreateTor(rSize[0], rSize[1]);

  if (ProcNum == 1) {  // Proverka kol-vo proccesov
    ASSERT_TRUE(true);
  } else {
    int value = 0;  // Otprovliemoe znachenie 
    if (ProcRank == 0) {
        value = 22;  // Zadaem znachenie, if zero Rank
    }
    MPI_Bcast(&value, 1, MPI_INT, 0, tor_comm);  // Rassilka vsem prozecam
    int control = value * (ProcRank + 1);
    int res = 0;
    MPI_Reduce(&control, &res, 1, MPI_INT, MPI_SUM, 0, tor_comm);  // Poluchenie podshetov s drugih processov
    if (ProcRank == 0) {
      int exp = 0;
      for (int i = 0; i < ProcNum; i++) {
        exp += value * (i + 1);
      }
      ASSERT_EQ(res, exp);
    }
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();
  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
