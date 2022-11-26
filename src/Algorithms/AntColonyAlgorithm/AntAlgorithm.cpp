#include "AntAlgorithm.h"

namespace s21 {
void AntAlgorithm::SetData(S21Matrix &matrix, int N) {
    matrix_ = std::move(matrix);
    count_of_nodes_ = matrix_.get_rows();
    this->N = N;
}

void AntAlgorithm::SolveWithoutUsingParallelism() { MainIteration(false); }

void AntAlgorithm::SolveUsingParallelism() { MainIteration(true); }

TsmResult &AntAlgorithm::GetResult() { return shortest_path_; }

void AntAlgorithm::FillEmptyNodes() {
    double max = matrix_(0, 0);
    for (int i = 0; i < matrix_.get_rows(); i++) {
        for (int j = 0; j < matrix_.get_cols(); j++) {
            max = std::max(max, matrix_(i, j));
        }
    }
    for (int i = 0; i < matrix_.get_rows(); i++) {
        for (int j = 0; j < matrix_.get_cols(); j++) {
            if (i != j && matrix_(i, j) == 0.0) {
                max += 10.0;
                matrix_(i, j) = max;
            }
        }
    }
}

void AntAlgorithm::MainIteration(bool multithreading) {
    shortest_path_ = TsmResult({}, -1.0);
    FillEmptyNodes();
    pheromones_ = pheromones_delta_ = S21Matrix(count_of_nodes_, count_of_nodes_);
    for (int i = 0; i < matrix_.get_rows(); ++i) {
        for (int j = 0; j < matrix_.get_cols(); ++j) {
            if (matrix_(i, j) != 0.0) {
                pheromones_(i, j) = 0.2;
            }
        }
    }
    if (multithreading) {
        it1 = std::thread(&AntAlgorithm::AntColonyAlgorithm, this, 5000);
        it2 = std::thread(&AntAlgorithm::AntColonyAlgorithm, this, 5000);
        it3 = std::thread(&AntAlgorithm::AntColonyAlgorithm, this, 5000);
        it4 = std::thread(&AntAlgorithm::AntColonyAlgorithm, this, 5000);
        it1.join();
        it2.join();
        it3.join();
        it4.join();
    } else {
        AntColonyAlgorithm(20000);
    }
    for (size_t i = 0; i < shortest_path_.vertices.size(); ++i) {
        shortest_path_.vertices[i]++;
    }
}

void AntAlgorithm::AntColonyAlgorithm(int end) {
    for (int iteration = 0; iteration < N; iteration++) {
        if (iteration > 0) {
            ApplyDeltaToPheromones();
        }
        BuildPath(end);
    }
}

void AntAlgorithm::ApplyDeltaToPheromones() {
    const double vape = 0.5;
    for (int i = 0; i < matrix_.get_rows(); i++) {
        mt.lock();
        for (int j = 0; j < matrix_.get_cols(); j++) {
            if (matrix_(i, j) != 0.0) {
                pheromones_(i, j) = vape * pheromones_(i, j) + pheromones_delta_(i, j);
            }
        }
        mt.unlock();
    }
}

void AntAlgorithm::BuildPath(int end) {
    TsmResult min = TsmResult({}, -1.0);
    S21Matrix event(count_of_nodes_, count_of_nodes_);
    for (int start_ind = 0; start_ind < end; start_ind++) {
        std::vector<int> visited;
        std::set<int> available_nodes;
        int ants_path = 0;
        for (int i = 0; i < count_of_nodes_; ++i) available_nodes.insert(i);
        int current_pos = 0;
        while (true) {
            visited.push_back(current_pos);
            available_nodes.erase(current_pos);
            if (available_nodes.size() == 0) break;
            event.FillWithDigit(0.0);
            for (int j = 1; j < count_of_nodes_ && available_nodes.size() > 1; ++j) {
                if (matrix_(current_pos, j) != 0.0) {
                    event(current_pos, j) = GetEventPossibility(current_pos, j, available_nodes);
                }
            }
            int old_pos = current_pos;
            current_pos = GetNextNode(current_pos, available_nodes, event);
            ants_path += matrix_(old_pos, current_pos);
        }
        TsmResult tmp = GetFullPath(visited);
        if (min.distance == -1.0 || tmp.distance < min.distance) {
            min = tmp;
        }
        IncreaseDelta(ants_path, visited);
    }
    mt.lock();
    if (shortest_path_.distance == -1 || min.distance < shortest_path_.distance) {
        shortest_path_ = min;
    }
    mt.unlock();
}

double AntAlgorithm::GetEventPossibility(int rows, int cols, std::set<int> &nodes) {
    double denominator = 0.0;
    for (auto iterator : nodes) {
        if (matrix_(rows, iterator) != 0.0) {
            denominator += pheromones_(rows, iterator) * (1.0 / matrix_(rows, iterator));
        }
    }
    double nominator = pheromones_(rows, cols) * (1.0 / matrix_(rows, cols));
    return (nominator / denominator);
}

int AntAlgorithm::GetNextNode(int cur_pos, std::set<int> &nodes, S21Matrix &event_) {
    if (nodes.size() == 1) {
        return *(nodes.begin());
    }
    std::vector<double> event_vec;
    double sum = 0.0;
    for (int j = 0; j < matrix_.get_rows(); ++j) {
        if (matrix_(cur_pos, j) != 0.0 && nodes.find(j) != nodes.end()) {
            sum += event_(cur_pos, j);
            event_vec.push_back(sum);
        } else {
            event_vec.push_back(0.0);
        }
    }
    int ind = -1;
    double random_value = (double)rand() / (RAND_MAX);
    for (size_t j = 0; j < event_vec.size(); ++j) {
        if (event_vec[j] != 0.0 &&
            (event_vec[j] > random_value && (ind == -1 || random_value > LastPositiveEvent(event_vec, j)))) {
            ind = j;
        }
    }
    return ind;
}

double AntAlgorithm::LastPositiveEvent(std::vector<double> &event_vec, int j) {
    --j;
    while (j >= 0 && event_vec[j] == 0.0) {
        --j;
    }
    return event_vec[j];
}

void AntAlgorithm::IncreaseDelta(int path_of_cur, std::vector<int> &visited) {
    int last_ind = visited[0];
    const double Q = 10.0;
    mt.lock();
    for (size_t i = 1; i < visited.size(); ++i) {
        pheromones_delta_(last_ind, visited[i]) += Q / path_of_cur;
        last_ind = visited[i];
    }
    mt.unlock();
}

TsmResult AntAlgorithm::GetFullPath(std::vector<int> &visited) {
    double cur_path = 0.0;

    S21Matrix available(pheromones_);
    int cur_pos = 0;
    for (size_t i = 1; i < visited.size(); ++i) {
        cur_path += matrix_(cur_pos, visited[i]);
        cur_pos = visited[i];
    }
    // Reversed path from last visited node to home
    TsmResult reversed = GetShortestPath(visited.back() + 1, 1);
    for (int i = reversed.vertices.size() - 2; i >= 0; --i) {
        visited.push_back(reversed.vertices[i]);
    }
    cur_path += reversed.distance;
    return TsmResult(visited, cur_path);
}

TsmResult AntAlgorithm::GetShortestPath(int vertex1, int vertex2) {
    int size = matrix_.get_rows();
    std::vector<int> pos(size), node(size), parent(size);
    int big_number = std::numeric_limits<int>::max();
    for (int i = 0; i < size; ++i) {
        pos[i] = big_number;
        node[i] = 0;
        parent[i] = -1;
    }
    int min = 0, index_min = 0;
    pos[vertex1 - 1] = 0;
    for (int i = 0; i < size; ++i) {
        min = big_number;
        for (int j = 0; j < size; ++j) {
            if (!node[j] && pos[j] < min) {
                min = pos[j];
                index_min = j;
            }
        }
        node[index_min] = 1;
        for (int j = 0; j < size; ++j) {
            if (!node[j] && matrix_(index_min, j) && pos[index_min] != big_number &&
                pos[index_min] + matrix_(index_min, j) < pos[j]) {
                pos[j] = pos[index_min] + matrix_(index_min, j);
                parent[j] = index_min;
            }
        }
    }

    std::vector<int> temp;
    for (int i = vertex2 - 1; i != -1; i = parent[i]) {
        temp.push_back(i);
    }
    return TsmResult(temp, pos[vertex2 - 1]);
}
}  // namespace s21
