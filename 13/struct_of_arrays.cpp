#include <vector>
#include <iostream>
#include <benchmark/benchmark.h>



struct Student
{
    unsigned id;
    unsigned grade;
    std::string name;
    std::string pennkey;
    int arr[1000] = {0}; // padding to ensure different cache lines
};

void f1 (const std::vector<Student>& students)
{
    int sum = 0;
    for (const auto& student : students)
    {
        sum += student.grade;
    }
}

struct Students
{
    std::vector<unsigned> ids;
    std::vector<unsigned> grades;
    std::vector<std::string> names;
    std::vector<std::string> pennkeys;
    std::vector<int [1000]> arrs;
};

void f2 (const Students& students)
{
    int sum = 0;
    for (const auto& grade : students.grades)
    {
        sum += grade;
    }
}

static void BM_F1(benchmark::State& state) {
  auto s2 = Students{ std::vector<unsigned>(1000),
                        std::vector<unsigned>(1000),
                        std::vector<std::string>(1000),
                        std::vector<std::string>(1000),
                        std::vector<int[1000]> (1000) };
  for (auto _ : state)
  {
    f2(s2);
  }
}

static void BM_F2(benchmark::State& state) {
  auto s1 = std::vector<Student>(100);
  for (auto _ : state)
  {
    f1(s1);
  }
}
BENCHMARK(BM_F1);
BENCHMARK(BM_F2);

BENCHMARK_MAIN();

