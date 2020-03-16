#include <cmath>
#include <vector>
#include <tuple>
#include "../catch.hpp"
#include "../../src/matslise.h"


using namespace matslise;
using namespace std;
using namespace Eigen;

TEST_CASE("Eigenfunctions henon", "[se2d][eigenfunctions][henon]") {
    SE2D<> p2(
            [](double x, double y) -> double {
                return (x * x + y * y) + 1 / (2 * sqrt(5)) * x * (y * y - x * x / 3);
            },
            {{-6, 6}, -6, 6},
            Options2<>().sectorCount(15).stepsPerSector(4).nested(Options1<>().sectorCount(16)));
    pair<double, int> eigenvalues[] = {
            {0.998594690530479, 1},
            {1.99007660445524,  2},
            {2.95624333869018,  1},
            {2.98532593386986,  2},
            {3.92596412795287,  2},
            {3.98241882458866,  1},
            {3.98575763690663,  1},
            {4.87014557482289,  1},
            {4.89864497284387,  2}};

    CHECK(Approx(p2.findFirstEigenvalue()).margin(1e-7) == eigenvalues[0].first);

    int n = 2;
    ArrayXd x = ArrayXd::LinSpaced(n, -5, 5);
    double E;
    int multiplicity;
    for (auto &Em  : eigenvalues) {
        tie(E, multiplicity) = Em;
        E *= 2;

        const pair<double, double> &error = p2.calculateError(E);
        REQUIRE(abs(error.first) < 1e-3);
        REQUIRE(Approx(p2.findEigenvalue(E)).margin(1e-7) == E);

        const vector<Array<double, -1, -1>> f = p2.computeEigenfunction(E, x, x);
        REQUIRE(f.size() == multiplicity);
        vector<function<double(double, double)>> funcs = p2.eigenfunctionCalculator(E);
        REQUIRE(funcs.size() == multiplicity);

        for (int k = 0; k < multiplicity; ++k) {
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < n; ++j)
                    REQUIRE(Approx(f[k](i, j)).margin(1e-7) == funcs[k](x[i], x[j]));
        }
    }
}
