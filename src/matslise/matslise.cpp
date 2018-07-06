//
// Created by toon on 5/16/18.
//

#include <cmath>
#include <array>
#include <vector>
#include <queue>
#include <Eigen/Dense>
#include "matslise_formulas.h"
#include "../util/legendre.h"
#include "../util/calculateEta.h"

#define EPS (1.e-12)

using namespace matslise;
using namespace matslise::matslise_sector;
using namespace std;
using namespace Eigen;

Matslise::Matslise(function<double(double)> V, double xmin, double xmax, int sectorCount)
        : V(V), xmin(xmin), xmax(xmax), sectorCount(sectorCount) {
    sectors = new Sector *[sectorCount];
    double h = (xmax - xmin) / sectorCount;
    double mid = (xmax + xmin) / 2;
    for (int i = 0; i < sectorCount; ++i) {
        double a = xmin + i * h;
        double b = a + h;
        if (b - 1.e-5 > mid) {
            match = b;
            mid = xmax + 1;
        }
        sectors[i] = new Sector(this, a, b);
    }
}


tuple<Y<double>, double> Matslise::propagate(double E, const Y<double> &_y, double a, double b) const {
    Y<double> y = _y;
    double theta = y.theta();
    if (a < b) {
        for (int i = 0; i < sectorCount; ++i) {
            Sector *sector = sectors[i];
            if (sector->xmax > a) {
                if (sector->xmin < a) // first
                    y = sector->propagate(E, y, sector->xmin - a, theta);

                if (sector->xmax >= b) { // last
                    y = sector->propagate(E, y, b - sector->xmin, theta);
                    break;
                }

                y = sector->propagate(E, y, sector->xmax - sector->xmin, theta);
            }
        }
    } else {
        for (int i = sectorCount - 1; i >= 0; --i) {
            Sector *sector = sectors[i];
            if (sector->xmin < a) {
                if (sector->xmax > a) // first
                    y = sector->propagate(E, y, sector->xmin - a, theta);
                else
                    y = sector->propagate(E, y, sector->xmin - sector->xmax, theta);

                if (sector->xmin <= b) { // last
                    y = sector->propagate(E, y, b - sector->xmin, theta);
                    break;
                }

            }
        }
    }
    return make_tuple(y, theta);
}

tuple<double, double, double>
Matslise::calculateError(double E, const Y<double> &left, const Y<double> &right) const {
    Y<double> l, r;
    double thetaL, thetaR;
    tie(l, thetaL) = propagate(E, left, xmin, match);
    tie(r, thetaR) = propagate(E, right, xmax, match);
    return make_tuple(l.y[1] * r.y[0] - r.y[1] * l.y[0],
                      l.dy[1] * r.y[0] + l.y[1] * r.dy[0] - (r.dy[1] * l.y[0] + r.y[1] * l.dy[0]),
                      thetaL - thetaR);
}

tuple<unsigned int, double> newtonIteration(const Matslise *ms, double E, const Y<double> &left, const Y<double> &right, double tol) {
    double adjust, error, derror, theta;
    int i = 0;
    do {
        tie(error, derror, theta) = ms->calculateError(E, left, right);
        adjust = error / derror;
        E = E - adjust;
        if (++i > 50) {
            cerr << "Newton-iteration did not converge for E=" << E << endl;
            break;
        }
    } while (fabs(adjust) > tol);

    int index = (int) (round(theta / M_PI) - 1);
    if (index < 0)
        index = 0;
    return make_tuple(index, E);
}

vector<tuple<unsigned int, double>> *
Matslise::computeEigenvaluesByIndex(unsigned int Imin, unsigned int Imax, const Y<double> &left, const Y<double> &right) const {
    double Emin = -1;
    double Emax = 1;
    while (true) {
        unsigned int i = (unsigned int) floor(get<2>(calculateError(Emax, left, right)) / M_PI);
        if (i >= Imax)
            break;
        else {
            if (i < Imin)
                Emin = Emax;
            Emax *= 2;
        }
    }
    if (Emin == -1) {
        while (true) {
            unsigned int i = (unsigned int) floor(get<2>(calculateError(Emin, left, right)) / M_PI);
            if (i <= Imin)
                break;
            else {
                if (i > Imax)
                    Emax = Emin;
                Emin *= 2;
            }
        }
    }
    return computeEigenvalues(Emin, Emax, Imin, Imax, left, right);
};

vector<tuple<unsigned int, double>> *
Matslise::computeEigenvalues(double Emin, double Emax, const Y<double> &left, const Y<double> &right) const {
    return computeEigenvalues(Emin, Emax, 0, UINT_MAX, left, right);
};

vector<tuple<unsigned int, double>> *
Matslise::computeEigenvalues(double Emin, double Emax, unsigned int Imin, unsigned int Imax, const Y<double> &left,
                             const Y<double> &right) const {
    vector<tuple<unsigned int, double>> *eigenvalues = new vector<tuple<unsigned int, double>>();
    queue<tuple<double, double, double, double, unsigned int>> toCheck;

    toCheck.push(make_tuple(Emin, get<2>(calculateError(Emin, left, right)) / M_PI,
                            Emax, get<2>(calculateError(Emax, left, right)) / M_PI,
                            0));

    double a, ta, b, tb, c, tc;
    unsigned int ia, ib, depth;
    while (!toCheck.empty()) {
        tie(a, ta, b, tb, depth) = toCheck.front();
        toCheck.pop();
        ia = (unsigned int) floor(ta);
        ib = (unsigned int) floor(tb);
        if (ta >= tb || ia == ib || ib <= Imin || Imax <= ia)
            continue;
        if (ia + 1 == ib)
            ++depth;

        c = (a + b) / 2;
        if (tb - ta < 0.05 || depth > 20)
            eigenvalues->push_back(newtonIteration(this, c, left, right, 1e-9));
        else {
            tc = get<2>(calculateError(c, left, right)) / M_PI;
            toCheck.push(make_tuple(a, ta, c, tc, depth));
            toCheck.push(make_tuple(c, tc, b, tb, depth));
        }
    }

    sort(eigenvalues->begin(), eigenvalues->end());

    return eigenvalues;
}

Matslise::~Matslise() {
    for (int i = 0; i < sectorCount; ++i)
        delete sectors[i];
    delete[] sectors;
}

Array<Y<double>, Dynamic, 1> Matslise::computeEigenfunction(double E, const matslise::Y<double> &left, const matslise::Y<double> &right,
                                                    const ArrayXd &x) const {
    long n = x.size();
    for (int i = 1; i < n; ++i)
        if (x[i - 1] > x[i])
            throw runtime_error("Matslise::computeEigenfunction(): x has to be sorted");

    Array<Y<double>, Dynamic, 1> ys(n);

    long forward = 0;
    Y<double> y = left;
    int iLeft = 0;
    { // left
        while (forward < n && x[forward] < xmin - EPS)
            ++forward;

        Sector *sector = sectors[0];
        for (; forward < n; ++forward) {
            while (sector->xmax < x[forward]) {
                y = sector->calculateT(E) * y;
                ++iLeft;
                sector = sectors[iLeft];
                if (iLeft >= sectorCount || sector->xmin > match)
                    goto allLeftSectorsDone;
            }

            ys[forward] = sector->calculateT(E, x[forward] - sector->xmin) * y;
        }
        allLeftSectorsDone:;
    }

    { // right
        Y<double> yLeft = y;

        long reverse = n;
        while (reverse-- > forward && x[reverse] > xmax + EPS);
        long lastValid = reverse;

        Sector *sector = sectors[sectorCount - 1];
        y = sector->calculateT(E) / right;
        for (int i = sectorCount - 1; reverse >= 0; --reverse) {
            while (sector->xmin > x[reverse]) {
                --i;
                sector = sectors[i];
                if (i < iLeft || sector->xmax < match)
                    goto allRightSectorsDone;
                y = sector->calculateT(E) / y;
            }

            ys[reverse] = sector->calculateT(E, x[reverse] - sector->xmin) * y;
        }
        allRightSectorsDone:;

        Y<double> yRight = y;
        double scale = yLeft.y[0] / yRight.y[0];
        for (long i = reverse + 1; i <= lastValid; ++i) {
            ys[i].y *= scale;
            ys[i].dy *= scale;
        }
    }

    return ys;
}