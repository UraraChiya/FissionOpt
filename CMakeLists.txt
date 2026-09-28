cmake_minimum_required(VERSION 3.14)
project(FissionOpt)
set(CMAKE_CXX_STANDARD 20)

add_executable(FissionOpt
  Benchmark.cpp
  OverhaulFission.h
  OverhaulFission.cpp
  OptOverhaulFission.h
  OptOverhaulFission.cpp
  OverhaulFissionNet.h
  OverhaulFissionNet.cpp
)

target_include_directories(FissionOpt PRIVATE
  ./xtensor/include
  ./xtl/include
)
