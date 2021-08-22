#include <iostream>
#include <vector>

#include "../astar.h"

namespace std { //Hash function for the std::tuple<int,int> type
  template <> struct hash<std::tuple<int,int>> {
    size_t operator()(const std::tuple<int,int> & x) const {
		return std::get<0>(x) + std::get<1>(x) * 1000000;
    }
  };
}

std::vector<std::tuple<int,int>> gridPathfinding(std::vector<bool>& mesh, int width, int height, std::tuple<int,int> start, std::tuple<int,int> goal){
	auto nextNodes = [&](const auto& position){
		auto[x, y] = position;
		std::vector<std::tuple<int,int>> nexts{};
		if(x > 0) if(!mesh[x-1 + y*width]) nexts.push_back({x-1, y});
		if(x < width - 1) if(!mesh[x+1 + y*width]) nexts.push_back({x+1, y});
		if(y > 0) if(!mesh[x + (y-1)*width]) nexts.push_back({x, y-1});
		if(y < height - 1) if(!mesh[x + (y+1)*width]) nexts.push_back({x, y+1});
		return std::move(nexts);
	};
	auto heuristicLambda = [](const auto& cell1, const auto& cell2){
		auto[x1, y1] = cell1;
		auto[x2, y2] = cell2;
		//return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
		return (x2-x1) + (y2-y1);
	};

	//The cost between two adjacent cells is always 1
	auto edgeCost = [](const auto& s1, const auto& s2){
		return 1;
	};

	return std::move(fastAStar(start, goal, nextNodes, heuristicLambda, edgeCost));
}

void testAStar(){
	std::vector<bool> mesh{};
	int width = 50;
	int height = 50;
	for(int i = 0; i < width*height; i++){ // Create an empty world of width*height size
		mesh.emplace_back(0);
	}
	std::tuple<int, int> start{1, 1};
	std::tuple<int, int> goal{width-3, height-3};

	//Execute the same path 1000 times to test efficiency over multiple calls
	for(int i=0; i<1000; i++){
		auto path = gridPathfinding(mesh, width, height, start, goal); 
	}
	
//	for(auto[x,y]: path){
//		std::cout << x << " " << y << std::endl;
//	}
}

int main(){
	testAStar();
	return 0;
}
