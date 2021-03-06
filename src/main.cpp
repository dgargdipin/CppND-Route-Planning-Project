#include "render.h"
#include "route_model.h"
#include "route_planner.h"
#include <chrono>
#include <fstream>
#include <io2d.h>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

using namespace std::experimental;
using std::cin;
using std::cout;
static std::optional<std::vector<std::byte>> ReadFile(const std::string &path) {
  std::ifstream is{path, std::ios::binary | std::ios::ate};
  if (!is)
    return std::nullopt;

  auto size = is.tellg();
  std::vector<std::byte> contents(size);

  is.seekg(0);
  is.read((char *)contents.data(), size);

  if (contents.empty())
    return std::nullopt;
  return std::move(contents);
}

int main(int argc, const char **argv) {
  std::string osm_data_file = "";
  if (argc > 1) {
    for (int i = 1; i < argc; ++i)
      if (std::string_view{argv[i]} == "-f" && ++i < argc)
        osm_data_file = argv[i];
  } else {
    std::cout << "To specify a map file use the following format: "
              << std::endl;
    std::cout << "Usage: [executable] [-f filename.osm]" << std::endl;
    osm_data_file = "../map.osm";
  }

  std::vector<std::byte> osm_data;

  if (osm_data.empty() && !osm_data_file.empty()) {
    std::cout << "Reading OpenStreetMap data from the following file: "
              << osm_data_file << std::endl;
    auto data = ReadFile(osm_data_file);
    if (!data)
      std::cout << "Failed to read." << std::endl;
    else
      osm_data = std::move(*data);
  }

  // TODO 1: Declare floats `start_x`, `start_y`, `end_x`, and `end_y` and get
  // user input for these values using std::cin. Pass the user input to the
  // RoutePlanner object below in place of 10, 10, 90, 90.
  float start_x, start_y, end_x, end_y;
  cout << "Input start_x coordinates: ";
  cin >> start_x;
  cout << "\n";
  cout << "Input start_y coordinates: ";
  cin >> start_y;
  cout << "\n";
  cout << "Input end_x coordinates: ";
  cin >> end_x;
  cout << "\n";
  cout << "Input end_y coordinates: ";
  cin >> end_y;
  cout << "\n";
  // Build Model.
  RouteModel model{osm_data};

  // Create RoutePlanner object and perform A* search.
  RoutePlanner route_planner{model, start_x, start_y, end_x, end_y};
  cout << "Which algorithm to use: \n1: A* Search\n2:Dijkstra\n";
  int algo_input;
  cin >> algo_input;
  auto t_start = std::chrono::high_resolution_clock::now();

  // do something

  if (algo_input == 1)
    route_planner.AStarSearch();
  else
    route_planner.Dijkstra();
  auto t_end = std::chrono::high_resolution_clock::now();

  double elapsed_time_ms =
      std::chrono::duration<double, std::milli>(t_end - t_start).count();

  std::cout<<"Took "<<elapsed_time_ms<<" milliseconds\n";
  std::cout << "Distance: " << route_planner.GetDistance() << " meters. \n";

  // Render results of search.
  Render render{model};

  auto display = io2d::output_surface{400,
                                      400,
                                      io2d::format::argb32,
                                      io2d::scaling::none,
                                      io2d::refresh_style::fixed,
                                      30};
  display.size_change_callback([](io2d::output_surface &surface) {
    surface.dimensions(surface.display_dimensions());
  });
  display.draw_callback(
      [&](io2d::output_surface &surface) { render.Display(surface); });
  display.begin_show();
}
