#ifndef ROUTE_PLANNER_H
#define ROUTE_PLANNER_H

#include "route_model.h"
#include <iostream>
#include <string>
#include <vector>

class RoutePlanner {
public:
  RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x,
               float end_y);
  // Add public variables or methods declarations here.
  float GetDistance() const { return distance; }
  void AStarSearch();
  void Dijkstra();
  struct dijkstra_comparator {
    bool operator()(const RouteModel::Node *A, const RouteModel::Node *B) {
      return A->distanceDijkstra>B->distanceDijkstra;
    }
  };

  // The following methods have been made public so we can test them
  // individually.
  void AddNeighbors(RouteModel::Node *current_node);
  void AddNeighbors_Dijkstra(RouteModel::Node *current_node);
  float CalculateHValue(RouteModel::Node const *node);
  std::vector<RouteModel::Node> ConstructFinalPath(RouteModel::Node *);
  RouteModel::Node *NextNode();
  void SortOpenList();

private:
  // Add private variables or methods declarations here.
  std::vector<RouteModel::Node *> open_list;
  // std::unordered_map<RouteModel::Node *,RouteModel::Node *> prev_map;
  RouteModel::Node *start_node;
  RouteModel::Node *end_node;
  float distance = 0.0f;
  RouteModel &m_Model;
};

#endif