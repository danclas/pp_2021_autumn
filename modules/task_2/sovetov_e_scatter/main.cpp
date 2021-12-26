// Copyright 2021 Sovetov Egor
#include <gtest/gtest.h>


#include <string>
#include <vector>

#include "./scatter.h"
#include <gtest-mpi-listener.hpp>

TEST(MPI_Scatter, int_to_int) {
  const int prod = 0;
  int bees;
  int razmer;
  int number = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &razmer);
  MPI_Comm_rank(MPI_COMM_WORLD, &bees);

  std::vector<int> fubber(20);
  std::vector<int> fubber1(20);
  std::vector<int> fubber2(20);
  std::vector<int> recieve;
  std::vector<int> tron;
  for (int& num : fubber) {
    num = ++number;
  }
  recieve.resize(fubber.size() / razmer);
  tron.resize(recieve.size());
  FasterScatter(fubber.data(), fubber.size() / razmer, MPI_INT, recieve.data(),
                fubber.size() / razmer, MPI_INT, prod, MPI_COMM_WORLD);
  MPI_Scatter(fubber.data(), fubber.size() / razmer, MPI_INT, tron.data(),
              fubber.size() / razmer, MPI_INT, prod, MPI_COMM_WORLD);
  MPI_Gather(recieve.data(), recieve.size(), MPI_INT, fubber1.data(),
             recieve.size(), MPI_INT, prod, MPI_COMM_WORLD);
  MPI_Gather(tron.data(), tron.size(), MPI_INT, fubber2.data(), tron.size(),
             MPI_INT, prod, MPI_COMM_WORLD);

  if (bees == prod) {
    ASSERT_EQ(fubber1, fubber2);
  }
}

TEST(MPI_Scatter, effectiv) {
  int bees;
  MPI_Comm_rank(MPI_COMM_WORLD, &bees);
  std::vector<int> fubber(100000);
  int number = 0;
  int max1, max2;
  for (int& num : fubber) num = ++number;
  max1 = MyTest(fubber, FasterScatter);
  max2 = MyTest(fubber, MPI_Scatter);
  if (bees == 0) {
    ASSERT_TRUE(max1 == max2);
  }
}

TEST(MPI_Scatter, double_to_float) {
  const int prod = 0;
  int bees, razmer;
  double number = 1.2;
  MPI_Comm_size(MPI_COMM_WORLD, &razmer);
  MPI_Comm_rank(MPI_COMM_WORLD, &bees);

  std::vector<double> fubber(12);
  std::vector<float> fubber1(12 * 2);
  std::vector<float> fubber2(12 * 2);
  std::vector<float> recieve;
  std::vector<float> tron;

  for (double& num : fubber) {
    num = ++number;
  }

  recieve.resize((fubber.size() / razmer) * (sizeof(double) / sizeof(float)));
  tron.resize(recieve.size());

  FasterScatter(fubber.data(), fubber.size() / razmer, MPI_DOUBLE,
                recieve.data(), recieve.size(), MPI_FLOAT, prod,
                MPI_COMM_WORLD);
  MPI_Scatter(fubber.data(), fubber.size() / razmer, MPI_DOUBLE, tron.data(),
              recieve.size(), MPI_FLOAT, prod, MPI_COMM_WORLD);

  MPI_Gather(recieve.data(), recieve.size(), MPI_FLOAT, fubber1.data(),
             recieve.size(), MPI_FLOAT, prod, MPI_COMM_WORLD);
  MPI_Gather(tron.data(), tron.size(), MPI_FLOAT, fubber2.data(), tron.size(),
             MPI_FLOAT, prod, MPI_COMM_WORLD);
  if (bees == 0) {
    ASSERT_EQ(fubber1, fubber2);
  }
}
TEST(MPI_Scatter, throw_when_error_count_and_root) {
  const int prod = -1;

  std::vector<int> fubber(1000);
  std::vector<int> fubber1(1000);
  std::vector<int> fubber2(1000);
  std::vector<int> recieve;
  std::vector<int> tron;

  ASSERT_ANY_THROW(FasterScatter(fubber.data(), fubber.size() / 3, MPI_INT,
                                 recieve.data(), fubber.size() / 3, MPI_INT,
                                 prod, MPI_COMM_WORLD));
  ASSERT_ANY_THROW(FasterScatter(fubber.data(), 0, MPI_INT, recieve.data(), 0,
                                 MPI_INT, 0, MPI_COMM_WORLD));
}
TEST(MPI_Scatter, char_to_int) {
  const int prod = 0;
  int bees, razmer;
  char number = 'a';
  MPI_Comm_size(MPI_COMM_WORLD, &razmer);
  MPI_Comm_rank(MPI_COMM_WORLD, &bees);

  std::vector<char> fubber(4 * razmer * razmer);
  std::vector<int> fubber1(4 * razmer * razmer / 4);
  std::vector<int> fubber2(4 * razmer * razmer / 4);
  std::vector<int> recieve;
  std::vector<int> tron;

  for (char& num : fubber) {
    num = ++number;
  }

  recieve.resize((fubber.size() / razmer) / 4);
  tron.resize(recieve.size());

  FasterScatter(fubber.data(), fubber.size() / razmer, MPI_CHAR, recieve.data(),
                recieve.size(), MPI_INT, prod, MPI_COMM_WORLD);
  MPI_Scatter(fubber.data(), fubber.size() / razmer, MPI_CHAR, tron.data(),
              recieve.size(), MPI_INT, prod, MPI_COMM_WORLD);

  MPI_Gather(recieve.data(), recieve.size(), MPI_INT, fubber1.data(),
             recieve.size(), MPI_INT, prod, MPI_COMM_WORLD);
  MPI_Gather(tron.data(), tron.size(), MPI_INT, fubber2.data(), tron.size(),
             MPI_INT, prod, MPI_COMM_WORLD);
  if (bees == 0) {
    ASSERT_EQ(fubber1, fubber2);
  }
}
TEST(MPI_Scatter, double_to_int) {
  const int prod = 0;
  int bees, razmer;
  double number = 1.2;
  MPI_Comm_size(MPI_COMM_WORLD, &razmer);
  MPI_Comm_rank(MPI_COMM_WORLD, &bees);

  std::vector<double> fubber(12);
  std::vector<int> fubber1(12 * 2);
  std::vector<int> fubber2(12 * 2);
  std::vector<int> recieve;
  std::vector<int> tron;

  for (double& num : fubber) {
    num = ++number;
  }

  recieve.resize((fubber.size() / razmer) * (sizeof(double) / sizeof(int)));
  tron.resize(recieve.size());

  FasterScatter(fubber.data(), fubber.size() / razmer, MPI_DOUBLE,
                recieve.data(), recieve.size(), MPI_INT, prod, MPI_COMM_WORLD);
  MPI_Scatter(fubber.data(), fubber.size() / razmer, MPI_DOUBLE, tron.data(),
              recieve.size(), MPI_INT, prod, MPI_COMM_WORLD);

  MPI_Gather(recieve.data(), recieve.size(), MPI_INT, fubber1.data(),
             recieve.size(), MPI_INT, prod, MPI_COMM_WORLD);
  MPI_Gather(tron.data(), tron.size(), MPI_INT, fubber2.data(), tron.size(),
             MPI_INT, prod, MPI_COMM_WORLD);
  if (bees == 0) {
    ASSERT_EQ(fubber1, fubber2);
  }
}
TEST(MPI_Scatter, int_to_char) {
  const int prod = 0;
  int bees, razmer;
  int number = 1;
  MPI_Comm_size(MPI_COMM_WORLD, &razmer);
  MPI_Comm_rank(MPI_COMM_WORLD, &bees);

  std::vector<int> fubber(12);
  std::vector<char> fubber1(12 * 4);
  std::vector<char> fubber2(12 * 4);
  std::vector<char> recieve;
  std::vector<char> tron;

  for (int& num : fubber) {
    num = ++number;
  }

  recieve.resize((fubber.size() / razmer) * (sizeof(int) / sizeof(char)));
  tron.resize(recieve.size());

  FasterScatter(fubber.data(), fubber.size() / razmer, MPI_INT, recieve.data(),
                recieve.size(), MPI_CHAR, prod, MPI_COMM_WORLD);
  MPI_Scatter(fubber.data(), fubber.size() / razmer, MPI_INT, tron.data(),
              recieve.size(), MPI_CHAR, prod, MPI_COMM_WORLD);

  MPI_Gather(recieve.data(), recieve.size(), MPI_CHAR, fubber1.data(),
             recieve.size(), MPI_CHAR, prod, MPI_COMM_WORLD);
  MPI_Gather(tron.data(), tron.size(), MPI_CHAR, fubber2.data(), tron.size(),
             MPI_CHAR, prod, MPI_COMM_WORLD);
  if (bees == 0) {
    ASSERT_EQ(fubber1, fubber2);
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
