#ifndef TRAIN_ROUTE_H
#define TRAIN_ROUTE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

struct RouteEdge {
    std::string destination;
    int distance;
    std::string trainInfo;
};

class RailwayGraph {
private:
    std::unordered_map<std::string, std::vector<RouteEdge>> adjList;
    std::string dataFile = "trains.txt";

public:
    RailwayGraph();
    void loadRoutesFromFile();
    void saveRouteToFile(std::string trainNo, std::string trainName, std::string src, std::string dest, int distance);
    void addRoute(std::string src, std::string dest, int distance, std::string trainInfo, bool saveToDisk = false);
    void displayNetwork();
    void findShortestPath(std::string startStation, std::string endStation);
};

void runRoutePlanner(RailwayGraph& graph);

#endif
