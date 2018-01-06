# DiscreteOptimizationProblems
Knapsack, Graph Coloring, Vehicle Routing Problems of Discrete Optimization Course from Master of Computer Engineering Department in Istanbul Technical University

## 0/1 Knapsack Problem
``Branch and bound technique is used to solve 0/1 Knapsack problem.``

In this technique, it is determined whether a node is promising in the state space tree by computing a bound
and children of a promising node which has the best bound to reach quickly an optimal solution with Breadth-first-search is visited.
If bound is not better than the best solution, then node is not promising.
In this approach, Best-first-search strategy as improved version of Breadth-first-search is used, and it expands the node with best bound first.Best-first-search actually uses the same strategy as Breadth-first-search, but it uses priority queue to prioritize the most promising object.
Let v as cumulative value of items already selected in the subset,
	w as cumulative weight of items already selected in the subset
	valuePerWeight[i+1] as best of remaining items
Then, Upper bound is calculated as v + (Capacity -w)* valuePerWeight[i+1]

## Graph Coloring Problem
``A heuristic sequential technique is used for graph coloring.``

In this heuristic technique, after a node is colored, the color of this node does not change.
In order to provide better strategy, picking the next node to be colored is carefully done.
Degree based ordering technique is used to order nodes as selection criteria.
Specifically, Largest Degree Ordering(LDO) is applied to order nodes. However, when there are two nodes having the same degree,
the node which has the higher number as the sum of the degrees of neighboring nodes than other same degreed node is selected.
After ordering was completed, all nodes are scanned, and sequentially colored.

## Vehicle Routing Problem
``A greedy solution is used to optimize Vehicle Routing Problem.``

In this approach, customers except warehouse are marked after routed.
For current location of a vehicle, a customer having the minimum distance, and was not used before is selected
as candidate customer. These candidate customers are added to the route and marked.
If there is a violation of capacity, and algorithm does not select a customer, vehicle returns the warehouse(This especially causes uneffective results) and a new vehicle begins to route. This approach is repeated until all customers are marked, and are used by a vehicle. However, in a few of test cases, some customers may remain as unvisited since the number of vehicles may be insufficient. Therefore, this greedy approach does not guarantee a feasible solution.
