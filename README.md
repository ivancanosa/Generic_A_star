This is a header-only generic implementation of the A\* search algorithm in C++17. The only function that should be called is "fastAStar" in the file "astar.h", with the following parameters:
1. start of type State.
2. goal of type State.
3. nextNodesLambda of type State ->Container\<State\>.
4. heuristicLambda of type State\*State -> float.
5. edgeCostLambda of type State\*State -> float.

Additionally, the type State must have a hash function and == operator inside the std namespace.
The function fastAStar is thread-safe and doesn't free the memory that uses on its operation, so future calls doesn't have to use much mallocs. This feature makes it specially useful inside the game loop of a videogame, because it will be called continuosly. 
In the folder "examples" there are examples of use of the function for a grid world, the knight's tour problem and the 8 puzzle problem.

## TODO:
	Incorporate the std::priority_queue in the object pool somehow.
