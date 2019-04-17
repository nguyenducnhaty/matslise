#include <cmath>
#include <set>
#include <tuple>
#include "../catch.hpp"
#include <matslise/se2d.h>
#include <matslise/util/lobatto.h>
#include "checkOrthonormality.h"


using namespace matslise;
using namespace std;
using namespace Eigen;


using namespace Catch::Matchers;

void compareEigenfunctions(
        const SEnD<2> &p, double E, const vector<function<double(double, double)>> &exact) {
    int n = 50, m = 60;
    ArrayXd x = ArrayXd::LinSpaced(n, p.domain.getMin(0), p.domain.getMax(0));
    ArrayXd y = ArrayXd::LinSpaced(m, p.domain.getMin(1), p.domain.getMax(1));
    std::vector<ArrayXXd> *fs = p.computeEigenfunction(E, x, y);

    REQUIRE(exact.size() == fs->size());
    for (ArrayXXd &f :*fs) {
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j) {
                bool valid = false;
                for (const function<double(double, double)> &e : exact)
                    if (abs(abs(e(x[i], y[j])) - abs(f(i, j))) < 1e-7)
                        valid = true;
                CHECKED_ELSE(valid) {
                    std::stringstream ss;
                    ss << E << ": " << x[i] << ", " << y[j] << " : " << f(i, j);
                    FAIL(ss.str());
                }
            }
    }
    delete fs;
}

TEST_CASE("Eigenvalues V=0", "[se2d][eigenfunctions][zero]") {
    SEnD<2> p(
            [](double x, double y) -> double {
                return 0;
            },
            {{0, M_PI}, 0, M_PI},
            Options<2>().sectorCount(13).N(12).nested(Options<1>().sectorCount(13)));

    set<double> eigenvalues;
    for (int i = 1; i < 6; ++i) {
        for (int j = 1; j <= i; ++j) {
            double E = i * i + j * j;
            if (eigenvalues.find(E) != eigenvalues.end())
                continue;
            eigenvalues.insert(E);
            for (int k = -1; k <= 1; ++k)
                REQUIRE(Approx(p.findEigenvalue(E + k * 1e-2)).margin(1e-7) == E);

            vector<function<double(double, double)>> v;
            for (int k = 1; k * k < E; ++k) {
                int l = (int) round(sqrt(E - k * k));
                if (l * l == E - k * k) {
                    v.push_back([k, l](double x, double y) -> double {
                        return sin(x * k) * sin(y * l) / M_PI_2;
                    });
                }
            }
            compareEigenfunctions(p, E, v);
        }
    }

    checkOrthonormality(p, eigenvalues.begin(), eigenvalues.end());
}
