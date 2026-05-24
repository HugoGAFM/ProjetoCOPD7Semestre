cmake_minimum_required(VERSION 3.10)
project(ParallelImageProcessing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# OpenCV
set(OpenCV_DIR "C:/Users/Hugog/Downloads/opencv/build")
find_package(OpenCV REQUIRED)

# OpenMP
find_package(OpenMP REQUIRED)

# MPI
find_package(MPI REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} ${MPI_INCLUDE_PATH} src)

set(SOURCES
    src/main.cpp
    src/image_processor.cpp
    src/mpi_worker.cpp
    src/openmp_filters.cpp
)

add_executable(ParallelImageProcessing ${SOURCES})
target_link_libraries(ParallelImageProcessing ${OpenCV_LIBS} ${MPI_LIBRARIES} OpenMP::OpenMP_CXX)