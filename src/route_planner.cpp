#include "route_planner.h"
#include <algorithm>
#include <queue>
#include <vector>
RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y,
                           float end_x, float end_y)
    : m_Model(model) {
  // Convert inputs to percentage:
  start_x *= 0.01;
  start_y *= 0.01;
  end_x *= 0.01;
  end_y *= 0.01;
  start_node = &(model.FindClosestNode(start_x, start_y));
  end_node = &(model.FindClosestNode(end_x, end_y));

  // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to
  // the starting and ending coordinates. Store the nodes you find in the
  // RoutePlanner's start_node and end_node attributes.
}

// TODO 3: Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another
// node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
  return end_node->distance(*node);
}

// TODO 4: Complete the AddNeighbors method to expand the current node by adding
// all unvisited neighbors to the open list. Tips:
// - Use the FindNeighbors() method of the current_node to populate
// current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the
// g_value.
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and
// set the node's visited attribute to true.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
  current_node->FindNeighbors();
  for (auto &neighbour : current_node->neighbors) {
    float new_g_val =
        current_node->g_value + current_node->distance(*neighbour);
    if (neighbour->visited && neighbour->g_value < new_g_val)
      continue;
    neighbour->parent = current_node;
    neighbour->g_value = new_g_val;
    neighbour->h_value = CalculateHValue(neighbour);
    if (!neighbour->visited)
      open_list.push_back(neighbour);
    neighbour->visited = 1;
  }
}

// TODO 5: Complete the NextNode method to sort the open list and return the
// next node. Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.

bool compare(const RouteModel::Node *A, const RouteModel::Node *B) {
  return A->f_value() > B->f_value();
}
void RoutePlanner::SortOpenList() {
  sort(open_list.begin(), open_list.end(), compare);
}
RouteModel::Node *RoutePlanner::NextNode() {
  SortOpenList();
  RouteModel::Node *candidate = open_list.back();
  open_list.pop_back();
  return candidate;
}

// TODO 6: Complete the ConstructFinalPath method to return the final path found
// from your A* search. Tips:
// - This method should take the current (final) node as an argument and
// iteratively follow the
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to
// the distance variable.
// - The returned vector should be in the correct order: the start node should
// be the first element
//   of the vector, the end node should be the last element.

std::vector<RouteModel::Node>
RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
  // Create path_found vector
  distance = 0.0f;
  std::vector<RouteModel::Node> path_found;
  RouteModel::Node *curr = current_node;
  while (curr != start_node) {
    path_found.push_back(*curr);
    distance += curr->distance(*(curr->parent));
    curr = curr->parent;
  }
  path_found.push_back(*start_node);
  reverse(path_found.begin(), path_found.end());
  // TODO: Implement your solution here.

  distance *= m_Model.MetricScale(); // Multiply the distance by the scale of
                                     // the map to get meters.
  return path_found;
}

// TODO 7: Write the A* Search algorithm here.
// Tips:
// - Use the AddNeighbors method to add all of the neighbors of the current node
// to the open_list.
// - Use the NextNode() method to sort the open_list and return the next node.
// - When the search has reached the end_node, use the ConstructFinalPath method
// to return the final path that was found.
// - Store the final path in the m_Model.path attribute before the method exits.
// This path will then be displayed on the map tile.

void RoutePlanner::AStarSearch() {
  RouteModel::Node *current_node = nullptr;
  open_list.push_back(start_node);
  start_node->h_value = CalculateHValue(start_node);
  start_node->g_value = 0;
  start_node->visited = 1;
  while (!open_list.empty()) {

    current_node = NextNode();
    if (current_node == end_node) {

      m_Model.path = ConstructFinalPath(current_node);

      return;
    }
    AddNeighbors(current_node);
  }

  // TODO: Implement your solution here.
}

void RoutePlanner::Dijkstra() {
  std::priority_queue<RouteModel::Node *, std::vector<RouteModel::Node *>,
                      dijkstra_comparator>
      pq;
  start_node->distanceDijkstra = 0;
  pq.push(start_node);
  while (!pq.empty()) {
    auto current_node = pq.top();
    pq.pop();
    if (current_node->visited)
      continue;
    current_node->visited = 1;
    if (current_node == end_node) {
      m_Model.path = ConstructFinalPath(current_node);
      return;
    }
    current_node->FindNeighbors();
    for (auto &neighbour : current_node->neighbors) {
      if (neighbour->distanceDijkstra <
          current_node->distanceDijkstra + current_node->distance(*neighbour))
        continue;
      neighbour->distanceDijkstra =
          current_node->distanceDijkstra + current_node->distance(*neighbour);
      neighbour->parent=current_node;
      pq.push(neighbour);
    }
  }
}