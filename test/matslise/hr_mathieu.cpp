#include <cmath>
#include <vector>
#include <tuple>
#include "../catch.hpp"
#include "../../src/matslise.h"
#include "../../src/util/eigen.h"


using namespace matslise;
using namespace std;
using namespace Eigen;

double hr_mathieu(double x) {
    return -2 * cos(2 * x);
}

TEST_CASE("HR: Solving the mathieu problem (first 10)", "[halfrange][matslise][mathieu]") {
    HalfRange<double> ms(&hr_mathieu, M_PI_2, Matslise<double>::UNIFORM(8));

    vector<double> correct = {-0.11024881635796, 3.91702477214389, 9.04773925867679, 16.03297008079835,
                              25.02084082368434, 36.01428991115492, 49.01041825048373, 64.00793719066102,
                              81.00625032615399, 100.00505067428990, 121.00416676119610};

    Y<double> y0({0, 1}, {0, 0});
    vector<pair<int, double>> *eigenvalues = ms.computeEigenvaluesByIndex(0, (int) correct.size(), y0, y0);
    for (unsigned int i = 0; i < correct.size(); ++i) {
        REQUIRE(i == eigenvalues->at(i).first);
        double E = eigenvalues->at(i).second;
        double error = ms.computeEigenvalueError(E, y0);
        REQUIRE(Approx(correct[i]).margin(error) == E);
        REQUIRE(fabs(error) < 1e-6);
    }
    delete eigenvalues;
}

TEST_CASE("HR: Solving the mathieu problem (first 10) (auto)", "[halfrange][matslise][mathieu][auto]") {
    HalfRange<double> ms(&hr_mathieu, M_PI_2, Matslise<double>::AUTO(1e-8));

    vector<double> correct = {-0.11024881635796, 3.91702477214389, 9.04773925867679, 16.03297008079835,
                              25.02084082368434, 36.01428991115492, 49.01041825048373, 64.00793719066102,
                              81.00625032615399, 100.00505067428990, 121.00416676119610};

    Y<double> y0({0, 1}, {0, 0});
    vector<pair<int, double>> *eigenvalues = ms.computeEigenvaluesByIndex(0, (int) correct.size(), y0, y0);
    for (unsigned int i = 0; i < correct.size(); ++i) {
        REQUIRE(i == eigenvalues->at(i).first);
        double E = eigenvalues->at(i).second;
        double error = ms.computeEigenvalueError(E, y0, y0);
        REQUIRE(fabs(error) < 1e-6);
        REQUIRE(Approx(correct[i]).margin(error) == E);
    }
    delete eigenvalues;
}

TEST_CASE("HR: Solving the mathieu problem (skip 100)", "[halfrange][matslise][mathieu]") {
    HalfRange<double> ms(&hr_mathieu, M_PI_2, Matslise<double>::UNIFORM(8));

    vector<double> correct = {10201.000049019607, 10404.000048063059, 10609.000047134254, 10816.000046232084,
                              11025.000045355584, 11236.000044503782, 11449.000043675751, 11664.000042870637,
                              11881.000042087542, 12100.000041325728, 12321.000040584415};
    unsigned int offset = 100;
    vector<pair<int, double>> *eigenvalues = ms.computeEigenvaluesByIndex(
            offset, offset + (unsigned int) correct.size(), Y<double>({0, 1}, {0, 0}), Y<double>({0, 1}, {0, 0}));

    REQUIRE(correct.size() == eigenvalues->size());
    for (unsigned int i = 0; i < correct.size(); ++i) {
        REQUIRE(offset + i == eigenvalues->at(i).first);
        REQUIRE(Approx(correct[i]).margin(1e-12) == eigenvalues->at(i).second);
    }
    delete eigenvalues;
}

TEST_CASE("HR: Mathieu normalized", "[halfrange][mathieu][matslise][eigenfunctionCalculator]") {
    HalfRange<double> ms(&hr_mathieu, M_PI_2, Matslise<double>::UNIFORM(8));
    Y<double> ystart({0, 1}, {0, 0});

    vector<pair<int, double>> *eigenvalues = ms.computeEigenvaluesByIndex(0, 10, ystart, ystart);
    for (pair<int, double> ie : *eigenvalues) {
        double e = ie.second;
        function<Y<double>(double)> f = ms.eigenfunctionCalculator(e, ystart, ystart);

        int n = 61;
        double v = 0;
        for (int i = 0; i < n; ++i) {
            double q = f((i + .5) / n * M_PI - M_PI_2).y[0];
            v += q * q;
        }
        v *= M_PI / n;
        REQUIRE(Approx(v).margin(1e-7) == 1);
    }
    delete eigenvalues;
}