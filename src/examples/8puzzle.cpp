#include "../astar.h"
#include <array>

constexpr char width = 3;

struct State{
	std::array<char, width*width> puzzle{};
};

bool operator==(const State& lhs, const State& rhs) {
	return (lhs.puzzle == rhs.puzzle);
}

namespace std { //Hash function for the State type
  template <> struct hash<State> {
    size_t operator()(const State & x) const {
		int a = 0;
		int d = 1;
		for(auto& i: x.puzzle){
			a += d*i;
			d *= 4;
		}
		return a;
    }
  };
}


bool between(char value, char left, char right){
	return (value >= left) && (value <= right);
}

int main(){
	std::vector<std::tuple<char,char>> movements{
		{1,0}, {-1,0}, {0,1}, {0,-1}};

	auto nextNodes = [&](const auto& state){
		char x{0}, y{0};
		std::vector<State> nexts{};
		for(char i=0; i< width*width; i++){// search where the -1 is
			if(state.puzzle[i] == -1){
				x = i % width;
				y = i / width;
				break;
			}
		}
		for(auto[dx,dy]: movements){
			auto nx = dx+x;
			auto ny = dy+y;
			if(between(nx,0,width-1) && between(ny,0,width-1)){
				State newState{state.puzzle};
				newState.puzzle[x+width*y] = newState.puzzle[nx+width*ny];
				newState.puzzle[nx+width*ny] = -1;
				nexts.push_back(newState);
			}
		}
		return std::move(nexts);
	};

	auto heuristicLambda = [](const auto& state, const auto& goal){
		char differences = 0;
		for(char i=0; i<width*width; i++){
			if(state.puzzle[i] != goal.puzzle[i])
				differences += 1;
		}
		return 2*differences;
	};

	auto edgeCost = [](const auto& a, const auto& b){return 1;};

	State start {{1,8,2,-1,4,3,7,6,5}};
	State goal  {{1,2,3,4,5,6,7,8,-1}};

	std::cout << "Started search" << std::endl;
	auto path = fastAStar(start, goal, nextNodes, heuristicLambda, edgeCost);

	std::cout << "========" << std::endl;
	for(auto& state: path){
		for(int i=0; i<width; i++){
			for(int j=0; j<width; j++){
				std::cout << int(state.puzzle[j + width*i]) << " ";
			}
			std::cout << std::endl;
		}
		std::cout << "========" << std::endl;
	}

	return 0;
};



















