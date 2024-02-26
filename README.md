# Ant colony optimization algorithms

In computer science and operations research, the ant colony optimization algorithm (ACO) is a probabilistic technique for solving computational problems which can be reduced to finding good paths through graphs.

This project solves the problem of mail delivery by robots in a logistics warehouse. Several robots move around the warehouse and transport mails from the point of receipt of the parcel to the point of unloading the parcel. 

There is one start point (__S__) and several finish points (__F__) on the map, there are obstacles (__!__) and free movement zones (__.__). 

Example map:

```
. . . . F . . . .
. . . . . . . . .
. . . . . . . . .
. . . . . . . . .
F . . ! ! ! . . F
. . . . . . . . .
. . . . . . . . .
. . . . S . . . .
. . . . ! . . . .
```

For each finishing point, the probability is set that the next parcel is destined for this direction.

The project simulates the operation of such a warehouse and optimizes the total delivery time of all parcels.

### How to build?

```
cd AntColonyOptimization
mkdir build && cd build
cmake ..
cmake --build .
```

### How to run?
```
./aco_project ../examples/map.txt ../examples/target_probs.txt
```