# Dynamic (Weighted) Independent Set [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

A dynamic graph algorithm is a data structure that supports edge insertions, deletions, and problem specific queries. While extensive research exists on dynamic algorithms for graph problems solvable in polynomial time, most of these algorithms have not been implemented or empirically evaluated.  The open source framework  addresses the NP-complete maximum weight as well as the maximum cardinality independent set problem in a dynamic setting, applicable to areas like dynamic map-labeling and vehicle routing. Specifically we introduce a novel local search technique called optimal neighborhood exploration. This technique creates independent subproblems that are solved to optimality, leading to improved overall solutions. Through numerous experiments, we assess the effectiveness of our approach and compare it with other state-of-the-art dynamic solvers. Our algorithm features a parameter, the subproblem size, that balances running time and solution quality. 

Installation Notes
=====
## Downloading: 
You can download the project with the following command line:

```console
git clone https://github.com/DynGraphLab/DynWMIS
```

## Compiling: 
Just type in the main directory of the project: 
```console
./compile_withcmake.sh 
```
In this case, all binaries, libraries and headers are in the folder ./deploy/ 

Note that this script detects the amount of available cores on your machine and uses all of them for the compilation process. If you don't want that, set the variable NCORES to the number of cores that you would like to use for compilation. 

Alternatively use the standard cmake build process:
```console 
mkdir build
cd build 
cmake ../ -DCMAKE_BUILD_TYPE=Release     
make 
cd ..
```
In this case, the binaries, libraries and headers are in the folder ./build



Graph Format
=====
The graph format used is a simple edge list. The first line is a # followed by two numbers `n` and `m` which specify the number of nodes of your input `n` and the number of edges/updates your perform. If there is a third number that equals `1` in this line, then the graph is weighted. If the graph is weighted, then the next `n` lines will specify vertex weights (one line per vertex). In both cases the `m` lines that follow, each line specifies an update. An update is either an edge insertion or deletion. An edge insertion is specified by a `1` followed by two numbers `u` and `v` which specify the edge `{u,v}` to be inserted. An edge deletion is specified by a `0` followed by two numbers `u` and `v` which specify the edge `{u,v}` to be deleted. In general, vertex IDs start at 1. Updates of vertex weights are in principle also possible, but not supported by the current implementation.

To convert a METIS graph file (like the ones available here https://dimacs10.github.io/) into the graph sequence format used by our program, you can use the following command:
```console
./deploy/convert_metis_seq nameofgraphfile.graph 
```
The output will be store `nameofgraphfile.graph.seq`; For example, 
```console
./deploy/convert_metis_seq examples/3elt.graph
```
Creates the file `examples/3elt.graph.seq`; Note, however, that this results in insertion only sequences.


Running Programs
=====
Running the algorithm is done as follows: 

Running the algorithm on a graph file `3elt.graph.seq` with the algorithm `DynamicOneFast`:
```console
./deploy/dynwmis examples/3elt.graph.seq --algorithm=DynamicOneFast
```

Running the algorithm on a graph file `3elt.graph.seq` with the algorithm `DynamicOneFast`:
```console
./deploy/dynwmis examples/4elt.graph.seq --algorithm=DynamicOneFast
```

The following commands should reproduce the values of the `4elt` instance in Table 1 of our paper:

```console
for g in `seq 1 10`; do ./deploy/dynwmis examples/4elt.graph.seq --algorithm=DynamicOneFast --seed=$g | grep weight | grep -v spec; done  | awk '{if($NF>max){max=$NF}}END{print max}'
```

```console
for g in `seq 1 10`; do ./deploy/dynwmis examples/4elt.graph.seq --algorithm=DynamicOneStrong --seed=$g | grep weight | grep -v spec; done  | awk '{if($NF>max){max=$NF}}END{print max}'
```



Licence
=====
The program is licenced under MIT licence.
If you publish results using our algorithms, please acknowledge our work by quoting the following paper:


```
@misc{borowitz2024optimalneighborhoodexplorationdynamic,
      title={Optimal Neighborhood Exploration for Dynamic Independent Sets}, 
      author={Jannick Borowitz and Ernestine Großmann and Christian Schulz},
      year={2024},
      eprint={2407.06912},
      archivePrefix={arXiv},
      primaryClass={cs.DS},
      url={https://arxiv.org/abs/2407.06912}, 
}
```



