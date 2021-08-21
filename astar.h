#include <iostream>
#include <vector>
#include <unordered_set>
#include <queue>
#include <mutex>
#include <algorithm>

template <typename State>
struct AstarDS{
	struct Node{
		State state;
		float cost;
		int parent; //Position to the parent node in the vector
	};
	std::vector<Node> nodesVector{};
	std::unordered_set<State> expanded{};
};

template <typename State, typename NextLambda, typename HeuristicLambda, typename EdgeCostLambda>
std::vector<State> innerAstar(State start, State goal, NextLambda nextNodesLambda, HeuristicLambda heuristicLambda, EdgeCostLambda edgeCostLambda, AstarDS<State>& reserved){
	if(start == goal) return {start};
	class Compare{ //This is for the priority queue
		public:
			bool operator() (std::tuple<int, float> a1, std::tuple<int, float> a2){
				return std::get<1>(a1) > std::get<1>(a2);
			}
	};

	reserved.nodesVector.clear();
	reserved.expanded.clear();

	std::priority_queue<std::tuple<int,float>, std::vector<std::tuple<int,float>>, Compare> frontier{};
	
	reserved.nodesVector.push_back({start, 0, -1});
	frontier.push({0, 0});
	while(frontier.size() != 0){
		auto[position, _] = frontier.top();
		frontier.pop();
		auto node = reserved.nodesVector[position];
		auto newStates = nextNodesLambda(node.state); // something iterable
		reserved.expanded.insert(node.state);
		for(auto& state: newStates){
			if(state == goal){
				std::vector<State> solution{};
				solution.push_back(state);
				solution.push_back(node.state);
				position = node.parent;
				while(position != -1){
					node = reserved.nodesVector[position];
					solution.push_back(node.state);
					position = node.parent;
				}
				std::reverse(solution.begin(), solution.end());

	//			std::cout << "Expanded nodes: " << reserved.expanded.size() << std::endl;
				return std::move(solution);
			}
			if(reserved.expanded.find(state) == reserved.expanded.end()){
				auto heuristic = heuristicLambda(state, goal);
				auto cost = edgeCostLambda(node.state, state);
				reserved.nodesVector.push_back( {state, node.cost + cost, position} );			
				frontier.push({reserved.nodesVector.size() - 1, node.cost + cost + heuristic});
			}
		}
	}

	return {}; //There is no path
}

template <typename State, typename NextLambda, typename HeuristicLambda, typename EdgeCostLambda>
std::vector<State> fastAStar(State start, State goal, NextLambda nextNodesLambda, HeuristicLambda heuristicLambda, EdgeCostLambda edgeCostLambda){
	static std::vector<AstarDS<State>> reservedVector{}; // The object pool
	static std::mutex mutex{};

	mutex.lock();
	if(reservedVector.empty())
		reservedVector.push_back({});
	auto reserved = std::move(reservedVector.back());
	reservedVector.pop_back();
	mutex.unlock();

	auto path{innerAstar(start, goal, std::move(nextNodesLambda), std::move(heuristicLambda), std::move(edgeCostLambda), reserved)};

	mutex.lock();
	reservedVector.push_back(std::move(reserved));
	mutex.unlock();

	return std::move(path);
}


