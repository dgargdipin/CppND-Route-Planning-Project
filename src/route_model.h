#ifndef ROUTE_MODEL_H
#define ROUTE_MODEL_H

#include "model.h"
#include <cmath>
#include <iostream>
#include <limits>
#include <unordered_map>

class RouteModel : public Model {

public:
  class Node : public Model::Node {
  public:
    Node *parent = nullptr;
    float h_value = std::numeric_limits<float>::max();
    float g_value = 0;
    bool visited = false;
    bool consumed = false;
    std::vector<Node *> neighbors;
    float distanceDijkstra=std::numeric_limits<float>::max();
    void FindNeighbors();
    float distance(Node other) const {
      return std::sqrt(std::pow((x - other.x), 2) + std::pow((y - other.y), 2));
    }
    float f_value() const { return h_value + g_value; }

    Node() {}
    Node(int idx, RouteModel *search_model, Model::Node node)
        : Model::Node(node), parent_model(search_model), index(idx) {}

  private:
    int index;
    Node *FindNeighbor(std::vector<int> node_indices);
    RouteModel *parent_model = nullptr;
  };

  RouteModel(const std::vector<std::byte> &xml);
  Node &FindClosestNode(float x, float y);
  auto &SNodes() { return m_Nodes; }
  std::vector<Node> path;

private:
  void CreateNodeToRoadHashmap();
  std::unordered_map<int, std::vector<const Model::Road *>> node_to_road;
  std::vector<Node> m_Nodes;
};

#endif
