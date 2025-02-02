//
// Created by cv2 on 03.02.2025.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <stdexcept>

// Function to perform topological sort
std::vector<std::string> topological_sort(const std::unordered_map<std::string, std::unordered_set<std::string>>& graph) {
    std::unordered_map<std::string, int> in_degree;
    std::deque<std::string> queue;
    std::vector<std::string> sorted;

    // Calculate in-degree (number of incoming edges) for each node
    for (const auto& [node, edges] : graph) {
        if (in_degree.find(node) == in_degree.end()) {
            in_degree[node] = 0;  // Initialize node with 0 in-degree
        }
        for (const auto& neighbor : edges) {
            in_degree[neighbor]++;
        }
    }

    // Add all nodes with 0 in-degree to the queue
    for (const auto& [node, degree] : in_degree) {
        if (degree == 0) {
            queue.push_back(node);
        }
    }

    // Process nodes from the queue
    while (!queue.empty()) {
        std::string node = queue.front();
        queue.pop_front();
        sorted.push_back(node);

        // Reduce in-degree for neighbors and add to queue if they now have 0 in-degree
        if (graph.find(node) != graph.end()) {
            for (const auto& neighbor : graph.at(node)) {
                in_degree[neighbor]--;
                if (in_degree[neighbor] == 0) {
                    queue.push_back(neighbor);
                }
            }
        }
    }

    // If the sorted list size is less than the number of nodes, there's a cycle
    if (sorted.size() != in_degree.size()) {
        throw std::runtime_error("Error: Graph contains a cycle.");
    }

    return sorted;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: cryonix_tsort <input_file>\n";
        return 1;
    }

    std::string filename = argv[1];
    std::ifstream infile(filename);

    if (!infile) {
        std::cerr << "Error: Could not open file: " << filename << "\n";
        return 1;
    }

    // Read the file and build the graph
    std::unordered_map<std::string, std::unordered_set<std::string>> graph;
    std::string line;

    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string first, second;
        if (!(iss >> first >> second)) {
            std::cerr << "Error: Invalid input format.\n";
            return 1;
        }

        // Add the edge (first -> second)
        graph[first].insert(second);

        // Ensure both nodes are in the graph (even if they have no outgoing edges)
        if (graph.find(second) == graph.end()) {
            graph[second] = {};
        }
        if (graph.find(first) == graph.end()) {
            graph[first] = {};
        }
    }

    try {
        std::vector<std::string> sorted = topological_sort(graph);

        // Output the sorted list
        for (const auto& node : sorted) {
            std::cout << node << "\n";
        }
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}