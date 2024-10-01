cmake_minimum_required(VERSION 3.16)

project(dynwmis C CXX)
enable_language(CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# debugging
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include_directories(${INTERFACE_INCLUDE_DIRECTORIES})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/argtable3-3.0.3)


#set(GUROBI_LIBRARY /home/cschulz/local/opt/gurobi912/lib)
#find_package(Gurobi REQUIRED)

#MESSAGE("Gurobi: include header files: ${GUROBI_INCLUDE_DIRS}")
#include_directories(${GUROBI_INCLUDE_DIRS})

#add_definitions(-g -pg)
SET(DYNWMIS_INCLUDE_DIRS
        lib
        lib/algorithms
        lib/data_structure
        lib/io
        lib/tools
        extern/sparsehash/src
        extern/argtable3-3.0.3/
        )
include_directories(${DYNWMIS_INCLUDE_DIRS})
add_definitions(-w)
        
set(KAMIS_DIR extern/wmis)
include_directories(
        ${KAMIS_DIR}/app
        ${KAMIS_DIR}/lib
        ${KAMIS_DIR}/lib/io
        ${KAMIS_DIR}/lib/mis
        ${KAMIS_DIR}/lib/mis/hils
        ${KAMIS_DIR}/lib/mis/ils
        ${KAMIS_DIR}/lib/mis/initial_mis
        ${KAMIS_DIR}/lib/mis/kernel
        ${KAMIS_DIR}/lib/partition
        ${KAMIS_DIR}/lib/partition/uncoarsening/refinement/quotient_graph_refinement/flow_refinement
        ${KAMIS_DIR}/lib/tools
        ${KAMIS_DIR}/extern
        ${KAMIS_DIR}/extern/KaHIP
        ${KAMIS_DIR}/extern/KaHIP/lib
        ${KAMIS_DIR}/extern/KaHIP/lib/io
        ${KAMIS_DIR}/extern/KaHIP/lib/partition
        ${KAMIS_DIR}/extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/flow_refinement
        ${KAMIS_DIR}/extern/KaHIP/lib/tools
        ${KAMIS_DIR}/extern/KaHIP/lib
        ${KAMIS_DIR}/extern/KaHIP/lib/data_structure
        ${KAMIS_DIR}/extern/KaHIP/lib/data_structure/matrix
        ${KAMIS_DIR}/extern/KaHIP/lib/data_structure/priority_queues)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/onlinemis)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/onlinemis/ils)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/onlinemis/initial_mis)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/onlinemis/data_structure)


# add source files of wmis
add_subdirectory(${KAMIS_DIR})



# VARS
SET(DYNWMIS dynwmis)
#SET(CMAKE_BUILD_TYPE Debug)
MESSAGE("${CMAKE_BUILD_TYPE}")

add_definitions(-O3 -DNDEBUG -fpermissive -march=native)


SET(DYNWMIS_CXX_FILES
        lib/data_structure/dyn_graph_access.cpp
        lib/io/mwis_graph_io.cpp
        lib/tools/timer.cpp
        lib/algorithms/dyn_wmis_simple.cpp
        lib/algorithms/dyn_wmis_greedy.cpp
        lib/algorithms/dyn_wmis_greedy_deg.cpp
        lib/algorithms/dyn_wmis_bfs.cpp
        lib/algorithms/dyn_wmis_static.cpp
        lib/algorithms/solve_static_wmis.cpp
        lib/algorithms/mis_branch_and_reduce_algorithm.cpp
        lib/algorithms/modified.cpp
        extern/onlinemis/ils/local_search_online.cpp
        extern/onlinemis/ils/online_ils.cpp
        extern/onlinemis/tools/mis_log.cpp
        extern/onlinemis/initial_mis/greedy_mis_online.cpp
        extern/onlinemis/data_structure/candidate_list.cpp
        extern/onlinemis/data_structure/mis_permutation_online.cpp
        extern/onlinemis/data_structure/priority_queues/bucket_array.cpp
        )

set(LIBCONVERT_SOURCE_FILES
./lib/io/mwis_graph_io.cpp
./lib/data_structure/dyn_graph_access.cpp
./lib/tools/random_functions.cpp
  extern/argtable3-3.0.3/argtable3.c)
add_library(libconvert OBJECT ${LIBCONVERT_SOURCE_FILES})

# Obejct Library
set(DYNWMIS_LIB dynwmis_lib)
add_library(${DYNWMIS_LIB} OBJECT ${DYNWMIS_CXX_FILES})


# build settings
# debugging is activated if NDEBUG is not set
# logging

# Executables
add_executable(${DYNWMIS} app/${DYNWMIS}.cpp $<TARGET_OBJECTS:libkaffpa> $<TARGET_OBJECTS:libsources> $<TARGET_OBJECTS:${DYNWMIS_LIB}>)

add_executable(convert_metis_seq app/metis_to_sequence.cpp $<TARGET_OBJECTS:libconvert> )
install(TARGETS convert_metis_seq DESTINATION ./)

# install
set(CMAKE_INSTALL_PREFIX ../deploy)
install(TARGETS ${DYNWMIS} DESTINATION ./)
