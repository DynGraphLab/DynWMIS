cmake_minimum_required(VERSION 2.8.9)
project(components)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release) 
endif (NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fno-omit-frame-pointer -march=native -DNDEBUG -Wall -funroll-loops -fno-stack-limit -fPIC -O3 -g")

add_subdirectory(extern/KaHIP)

include_directories(
        app
        lib
        lib/data_structure
        lib/data_structure/priority_queues
        lib/mis
        lib/mis/ils
        lib/mis/hils
        lib/mis/initial_mis
        lib/mis/kernel
        lib/tools
        extern/KaHIP
        extern/KaHIP/app
        extern/KaHIP/interface
        extern/KaHIP/lib
        extern/KaHIP/lib/algorithms
        extern/KaHIP/lib/data_structure
        extern/KaHIP/lib/data_structure/matrix
        extern/KaHIP/lib/data_structure/priority_queues
        extern/KaHIP/lib/io
        extern/KaHIP/lib/parallel_mh
        extern/KaHIP/lib/parallel_mh/exchange
        extern/KaHIP/lib/parallel_mh/galinier_combine
        extern/KaHIP/lib/partition
        extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/flow_refinement
        extern/KaHIP/lib/tools
        extern/argtable-2.10/include
)


set(SOURCES
        lib/tools/mis_log.cpp
        lib/mis/hils/Solution.cpp
        lib/mis/ils/ils.cpp
        lib/mis/ils/local_search.cpp
        lib/mis/initial_mis/greedy_mis.cpp
        lib/mis/initial_mis/greedy_vertex.cpp
        lib/mis/initial_mis/random_mis.cpp
        lib/mis/initial_mis/initial_mis.cpp
        lib/data_structure/mis_permutation.cpp
        lib/data_structure/candidate_list.cpp
        lib/data_structure/operation_log.cpp
        lib/data_structure/priority_queues/bucket_array.cpp
        lib/mis/kernel/branch_and_reduce_algorithm.cpp
        lib/mis/kernel/reductions.cpp
        lib/mis/kernel/mwis_finder.cpp
        lib/mis/kernel/original_struction.cpp
        lib/mis/kernel/extended_struction.cpp
        lib/mis/kernel/key_functions.cpp
        #$<TARGET_OBJECTS:libkaffpa>
        )

add_library(libsources OBJECT ${SOURCES})
