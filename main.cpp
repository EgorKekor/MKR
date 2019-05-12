#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <functional>

#define LEFT 6.0
#define DOWN 8.0
#define RIGHT_UP_TEMP           100
#define BOTTOM_TEMP             50
#define LEFT_TEMP               200
#define a               0.1


class Node {
public:
    Node() {x = 0; y = 0;};
    size_t i;
    size_t j;
    double x;
    double y;
    double temperature;
    bool station;
};


class Surface {
public:
    Surface(size_t xNodes_, size_t yNodes_);
    void InitSurface();
    void Traversal(const std::function<void(Node& node)>& visit);
    double GetDx() { return dx;};
    double GetDy() { return dy;};
private:
    double dx;
    double dy;
    size_t xNodes;
    size_t yNodes;
    std::vector<std::vector<Node>> Nodes;
};

//void (*visit)(Node&, size_t &cnt)
void Surface::Traversal(const std::function<void(Node& node)>& visit) {
    for (auto row = Nodes.rbegin(); row != Nodes.rend(); ++row) {
        for (auto col = row->begin(); col != row->end(); ++col) {
            visit(*col);
        }
        std::cout << "\n";
    }
}


Surface::Surface(size_t xNodes_, size_t yNodes_) {
    dx = DOWN / xNodes_;
    dy = LEFT / yNodes_;
    for (size_t i = 0; i < yNodes_; ++i) {
        std::vector<Node> layer(yNodes_, Node());
        for (size_t j = 0; j < xNodes_; ++j) {
            layer[j].x = dx * j;
            layer[j].y = dy* i;
            layer[j].i = i;
            layer[j].j = j;
            layer[j].station = (i == 0 || i == yNodes_ - 1 || j == 0 || j == xNodes_ - 1);
            if (j == 0) {
                layer[j].temperature = LEFT_TEMP;
            } else if (j == xNodes_ - 1) {
                layer[j].temperature = RIGHT_UP_TEMP;
            } else if (i == 0) {
                layer[j].temperature = BOTTOM_TEMP;
            } else if (i == yNodes_ - 1) {
                layer[j].temperature = RIGHT_UP_TEMP;
            } else {
                layer[j].temperature = 0;
            }
        }

        Nodes.push_back(layer);
    }
}

class Solver {
public:
    Solver(Surface &surf_, size_t dt_);
    void Resurf (Surface &surf_) {surf = surf_;};
    void Solve();
private:
    std::vector<Node*> index;
    Surface &surf;

    std::vector<int> MatA;
    std::vector<int> MatB;
    std::vector<int> MatC;


    double diag;
    double y_const;
    double x_const;
    double dx;
    double dy;
    double dt;
};


Solver::Solver(Surface &surf_, size_t dt_) : surf(surf_), dt(dt_) {
    std::vector<Node*> &ind = index;
    surf.Traversal([&ind](Node &node) mutable {
        if (!node.station) {
            ind.push_back(&node);
        }
    });

    dx = surf.GetDx();
    dy = surf.GetDy();
    diag = (-2)*dt*a/dx/dx - 2*dt*a/dy/dy -1;
    y_const = dt*a/dy/dy;
    x_const = dt*a/dx/dx;
}


void Solver::Solve() {

}



int main() {
    Surface surf(10, 10);
    surf.Traversal([&](Node &node) {
        std::cout.width(3);
        std::cout << node.temperature << " ";
    });

    size_t dt = 1;
    Solver slv(surf, dt);

    return 0;
}