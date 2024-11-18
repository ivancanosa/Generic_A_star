#include "../astar.h"
#include <array>

/*****************************************************
 * 	THIS SHOULD NEVER BE USED, EXTREMELY INEFICIENT *
******************************************************/

namespace std { //Hash function for the std::tuple<char,char> type
  template <> struct hash<std::tuple<char,char>> {
    size_t operator()(const std::tuple<char,char> & x) const {
		return std::get<0>(x) + std::get<1>(x) * 64;
    }
  };
}

struct State{
	std::tuple<char,char> position{0,0};
	std::unordered_set<std::tuple<char,char>> traveled{};
};

bool operator==(const State& lhs, const State& rhs) {
	return (lhs.traveled == rhs.traveled);
}

namespace std { //Hash function for the std::tuple<char,char> type
  template <> struct hash<State> {
    size_t operator()(const State & state) const {
		int i = 0;
		std::hash<std::tuple<char,char>> hasher;
		for(auto& x: state.traveled)
			i += (int)hasher(x);
		i += (int)hasher(state.position);
		return i;
    }
  };
}

bool between(char value, char left, char right){
	return (value >= left) && (value <= right);
}

//This is not very efficient without an heuristic
int main(){
	int width = 5;
	std::vector<std::tuple<char,char>> movements = {
		{2,1}, {-2,1}, {2,-1}, {-2,-1},
		{1,2}, {-1,2}, {1,-2}, {-1,-2}};

	auto nextNodes = [&](const auto& state){
		auto[x,y] = state.position;
		std::vector<State> nextStates{};
		for(auto[dx,dy] : movements){
			char nx = x+dx;
			char ny = y+dy;
			std::tuple<char,char> newPosition{nx,ny};
			if(between(nx, 0, width-1) && between(ny, 0, width-1) && (state.traveled.find(newPosition) == state.traveled.end())){
				State newState{};
				newState.position = newPosition;
				newState.traveled = state.traveled;
				newState.traveled.insert(newPosition);
				nextStates.push_back(std::move(newState));
			}
		}
		return std::move(nextStates);
	};

	auto heuristicLambda = [](const auto& a, const auto& b){return 0;};
	auto edgeCost = [](const auto& a, const auto& b){return -1;};
	State start;
	start.position = {0,0};
	start.traveled.insert({0,0});

	State goal{};
	for(int i=0; i<width; i++)
		for(int j=0; j<width; j++)
			goal.traveled.insert({i,j});

	std::cout << "Started search" << std::endl;
	auto path = fastAStar(start, goal, nextNodes, heuristicLambda, edgeCost); 
	for(auto s: path ){
		auto[x, y] = s.position;
		std::cout << int(x) << " " << int(y) << std::endl;
	}
}
