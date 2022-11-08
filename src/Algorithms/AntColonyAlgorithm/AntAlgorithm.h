#ifndef PARALLELS_ANTCOLONYALGORITHM_H
#define PARALLELS_ANTCOLONYALGORITHM_H

#include <set>
#include <mutex>
#include <vector>
#include <thread>

#include "../../DataStructures/Matrix/Matrix.h"

namespace s21 {

    struct TsmResult {
        std::vector<int> vertices;
        double distance;

        TsmResult() = default;
        TsmResult(std::vector<int> path, double distance) {
            this->distance = distance;
            vertices = path;
        }
    };

    class AntAlgorithm {
    public:
        void SetData(S21Matrix &matrix, int N);
        void SolveWithoutUsingParallelism();
        void SolveUsingParallelism();
        TsmResult &GetResult();

    private:
        S21Matrix pheromones_, pheromones_delta_, matrix_;
        std::thread it1, it2, it3, it4;
        std::mutex mt;
        double count_of_nodes_, max_length_;
        TsmResult shortest_path_;
        int N;

        void MainIteration(bool multithreading);
        void BuildPath(int end);
        void ApplyDeltaToPheromones();
        double GetEventPossibility(int rows, int cols, std::set<int> &nodes);
        int GetNextNode(int cur_pos, std::set<int> &nodes, S21Matrix &event_);
        double LastPositiveEvent(std::vector<double> &event_vec, int j);
        void IncreaseDelta(int path_of_cur, std::vector<int> &visited);
        TsmResult GetFullPath(std::vector<int> &visited);
        TsmResult GetShortestPath(int vertex1, int vertex2);
        void AntColonyAlgorithm(int end);
    };
}

#endif //PARALLELS_ANTCOLONYALGORITHM_H
