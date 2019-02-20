#include <cmath>
#include <vector>
#include <tuple>
#include "../catch.hpp"
#include <matslise/se2d.h>


using namespace matslise;
using namespace std;
using namespace Eigen;

TEST_CASE("Eigenfunctions ixaru", "[se2d][eigenfunctions][ixaru]") {
    double m = 0.001;
    SEnD<2> p2(
            [m](double x, double y) -> double {
                return (1 + x * x) * (1 + y * y);
            },
            {{-5.5, 5.5}, -5.5, 5.5},
            Options<2>().sectorCount(23));
    pair<double, int> eigenvalues[] = {
            {3.1959181,  1},
            {5.5267439,  2},
            {7.5578033,  1},
            {8.0312723,  1},
            {8.4445814,  1},
            {9.9280611,  2},
            {11.3118171, 2},
            {12.1032536, 1},
            {12.2011790, 1},
            {13.3323313, 1}
    };


    ArrayXd x(3);
    x << -1, 0, 1;
    double E;
    int multiplicity;
    for (auto &Em  : eigenvalues) {
        tie(E, multiplicity) = Em;
        const pair<double, double> &error = p2.calculateError(E);
        REQUIRE(abs(error.first) < 1e-3);

        const vector<Array<double, -1, -1>> *f = p2.computeEigenfunction(E, x, x);
        REQUIRE(f->size() == multiplicity);
        delete f;
    }
}