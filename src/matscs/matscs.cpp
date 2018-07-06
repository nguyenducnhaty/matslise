#include <functional>
#include <Eigen/Dense>
#include "../matscs.h"

#define EPS (1.e-12)

using namespace std;
using namespace matslise;
using namespace matslise::matscs_sector;

Matscs::Matscs(function<MatrixXd(double)> V, int n, double xmin, double xmax, int sectorCount)
        : V(V), n(n), xmin(xmin), xmax(xmax), sectorCount(sectorCount) {
    sectors = new Sector *[sectorCount];
    double h = (xmax - xmin) / sectorCount;
    for (int i = 0; i < sectorCount; ++i)
        sectors[i] = new Sector(this, xmin + i * h, xmin + (i + 1) * h);

}

Y<MatrixXd> Matscs::propagate(const double E, const Y<MatrixXd> &_y, const double a, const double b) const {
    Y<MatrixXd> y = _y;
    if (a < b) {
        for (int i = 0; i < sectorCount; ++i) {
            Sector *sector = sectors[i];
            if (sector->xmax > a) {
                if (sector->xmin < a) // first
                    y = sector->propagate(E, y, sector->xmin - a);

                if (sector->xmax >= b) { // last
                    y = sector->propagate(E, y, b - sector->xmin);
                    break;
                }

                y = sector->propagate(E, y, sector->xmax - sector->xmin);
            }
        }
    } else {
        for (int i = sectorCount - 1; i >= 0; --i) {
            Sector *sector = sectors[i];
            if (sector->xmin < a) {
                if (sector->xmax > a) // first
                    y = sector->propagate(E, y, sector->xmin - a);
                else
                    y = sector->propagate(E, y, sector->xmin - sector->xmax);

                if (sector->xmin <= b) { // last
                    y = sector->propagate(E, y, b - sector->xmin);
                    break;
                }

            }
        }
    }
    return y;

}

Matscs::~Matscs() {
    for (int i = 0; i < sectorCount; ++i)
        delete sectors[i];
    delete[] sectors;
}

vector<Y<MatrixXd>> *Matscs::computeEigenfunction(double E, vector<double> &x) const {
    sort(x.begin(), x.end());
    vector<Y<MatrixXd>> *ys = new vector<Y<MatrixXd>>();

    auto iterator = x.begin();

    while (iterator != x.end() && *iterator < xmin - EPS)
        iterator = x.erase(iterator);

    Sector *sector;
    Y<MatrixXd> y({MatrixXd::Zero(n, n), MatrixXd::Identity(n, n)}, {MatrixXd::Zero(n, n), MatrixXd::Zero(n, n)});
    for (int i = 0; iterator != x.end(); ++iterator) {
        while ((sector = sectors[i])->xmax < *iterator) {
            y = sector->calculateT(E) * y;
            ++i;
            if (i >= sectorCount)
                goto allSectorsDone;
        }

        ys->push_back(sector->calculateT(E, *iterator - sector->xmin) * y);
    }

    allSectorsDone:
    while (iterator != x.end() && *iterator > xmax + EPS)
        iterator = x.erase(iterator);

    return ys;
}