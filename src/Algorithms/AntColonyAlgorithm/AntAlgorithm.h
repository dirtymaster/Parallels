#ifndef PARALLELS_ANTCOLONYALGORITHM_H
#define PARALLELS_ANTCOLONYALGORITHM_H

#include <set>

#include "../AbstractAlgorithm.h"
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
        AntAlgorithm() = default;
        AntAlgorithm(S21Matrix &matrix, int N);
        void SolveWithoutUsingParallelism();
        void SolveUsingParallelism();

    private:
        S21Matrix pheromones_, pheromones_delta_, event_, matrix_;
        double count_of_nodes_, max_length_;
        TsmResult shortest_path_;
        int N;

        void MainIteration(bool multithreading);
        void BuildPath(int start, int end);
        void ApplyDeltaToPheromones();
        TsmResult AntColonyAlgorithm(bool multithreading);
        double GetEventPossibility(int rows, int cols, std::set<int> &nodes);
        int GetNextNode(int cur_pos, std::set<int> &nodes);
        double LastPositiveEvent(std::vector<double> &event_vec, int j);
        void IncreaseDelta(int path_of_cur, std::vector<int> &visited);
        TsmResult GetFullPath(std::vector<int> &visited);
        TsmResult GetShortestPath(int vertex1, int vertex2);
    };
}

#endif //PARALLELS_ANTCOLONYALGORITHM_H
