#include <cmath>
#include <array>
#include "matslise_formulas.h"
#include "../util/legendre.h"
#include "../util/calculateEta.h"
#include "../util/horner.h"
#include "../util/theta.h"
#include "../util/constants.h"

#define EPS (1.e-12)

using namespace std;
using namespace matslise;

template<typename Scalar>
Matslise<Scalar>::Sector::Sector(Matslise<Scalar> *s, const Scalar &min, const Scalar &max, bool backward)
        : s(s), min(min), max(max), backward(backward) {
    h = max - min;
    vs = legendre::getCoefficients(MATSLISE_N, s->V, min, max);
    if (backward) {
        for (int i = 1; i < MATSLISE_N; i += 2)
            vs[i] *= -1;
    }

    calculateTCoeffs();
}

template<typename Scalar>
void Matslise<Scalar>::Sector::calculateTCoeffs() {
    calculate_tcoeff_matrix(h, vs, t_coeff, t_coeff_h);
}

template<typename Scalar>
T<Scalar> Matslise<Scalar>::Sector::calculateT(const Scalar &E, const Scalar &delta, bool use_h) const {
    if (abs(delta) <= EPS)
        return T<Scalar>();
    if (use_h && abs(delta - h) <= EPS)
        return calculateT(E);

    const Scalar dd = delta * delta;
    const Scalar vsE = vs[0] - E;
    Scalar *eta = calculateEta(vsE * dd, MATSLISE_ETA_delta);
    T<Scalar> t(1);
    t.t << 0, 0, vsE * delta * eta[1], 0;
    t.dt << 0, 0, -delta * eta[1] - vsE * dd * delta * eta[2] / 2, 0;

    for (int i = 0; i < MATSLISE_ETA_delta; ++i) {
        Matrix<Scalar, 2, 2> hor = horner<Matrix<Scalar, 2, 2>>(t_coeff.row(i), delta, MATSLISE_HMAX_delta);
        t.t += hor * eta[i];

        if (i + 1 < MATSLISE_ETA_delta)
            t.dt += hor * (-dd * eta[i + 1] / 2);
    }

    delete[] eta;
    return t;
}

template<typename Scalar>
T<Scalar> Matslise<Scalar>::Sector::calculateT(const Scalar &E, bool use_h) const {
    if (!use_h)
        return calculateT(E, h, false);
    Scalar *eta = calculateEta((vs[0] - E) * h * h, MATSLISE_ETA_h);
    T<Scalar> t(1);
    t.t << 0, 0, (vs[0] - E) * h * eta[1], 0;
    t.dt << 0, 0, -h * eta[1] + -(vs[0] - E) * h * h * h * eta[2] / 2, 0;

    for (int i = 0; i < MATSLISE_ETA_h; ++i) {
        t.t += t_coeff_h[i] * eta[i];

        if (i + 1 < MATSLISE_ETA_h)
            t.dt += t_coeff_h[i] * (-h * h * eta[i + 1] / 2);
    }
    delete[] eta;
    return t;
}

template<typename Scalar>
Scalar rescale(const Scalar &theta, const Scalar &sigma) {
    Scalar sinTheta = sin(theta);
    Scalar cosTheta = cos(theta);
    return theta + atan2((sigma - 1) * sinTheta * cosTheta, 1 + (sigma - 1) * sinTheta * sinTheta);
}

template<typename Scalar>
Scalar Matslise<Scalar>::Sector::prufer(
        const Scalar &E, const Scalar &delta, const Y<Scalar> &y0, const Y<Scalar> &y1) const {
    Scalar ff = E - vs[0];
    if (ff > 0) {
        // page 56 (PhD Ledoux)
        Scalar omega = sqrt(ff);
        Scalar theta0 = atan_safe(y0.y[0] * omega, y0.y[1]);
        Scalar phi_star = atan_safe(y1.y[0] * omega, y1.y[1]);
        Scalar phi_bar = omega * delta + theta0;
        phi_bar -= floor(phi_bar / constants<Scalar>::PI) * constants<Scalar>::PI;
        Scalar theta1 = phi_star - phi_bar;
        if (theta1 < -constants<Scalar>::PI / 2)
            theta1 += constants<Scalar>::PI;
        else if (theta1 > constants<Scalar>::PI / 2)
            theta1 -= constants<Scalar>::PI;
        theta1 += theta0 + omega * delta;

        if (theta1 < theta0) {
            // theta has to be increasing
            theta1 += constants<Scalar>::PI;
        }

        return rescale(theta1, 1 / omega) - rescale(theta0, 1 / omega);
    } else {
        Scalar theta0 = atan_safe(y0.y[0], y0.y[1]);
        Scalar theta1 = atan_safe(y1.y[0], y1.y[1]);
        if (y0.y[0] * y1.y[0] >= 0) {
            if (theta0 > 0 && theta1 < 0)
                theta1 += constants<Scalar>::PI;
            else if (theta0 < 0 && theta1 > 0)
                theta1 -= constants<Scalar>::PI;
        } else if (theta0 * theta1 > 0) {
            theta1 += constants<Scalar>::PI;
        }

        return theta1 - theta0;
    }
}

template<typename Scalar>
pair<Y<Scalar>, Scalar> Matslise<Scalar>::Sector::propagateDelta(
        const Scalar &E, const Y<Scalar> &y0, Scalar delta, bool use_h) const {
    if (backward)
        delta *= -1;
    bool forward = delta >= 0;
    if (!forward)
        delta = -delta;
    if (delta > h)
        delta = h;

    const T<Scalar> &t = calculateT(E, delta, use_h);
    Y<Scalar> y = y0;
    if (backward)
        y.reverse();
    Y<Scalar> y1 = forward ? t * y : t / y;
    if (backward)
        y1.reverse();

    return {
            y1,
            forward != backward ?
            prufer(E, delta, y0, y1) :
            -prufer(E, delta, y1, y0)
    };
}

template<typename Scalar>
pair<Y<Scalar>, Scalar> Matslise<Scalar>::Sector::propagate(
        const Scalar &E, const Y<Scalar> &y0, const Scalar &a, const Scalar &b, bool use_h) const {
    Y<Scalar> y = y0;
    Scalar argdet = 0;
    Scalar theta;
    if (!((a >= max && b >= max) || (a <= min && b <= min))) {
        if (!backward) { // forward
            if (a > min) {
                tie(y, theta) = propagateDelta(E, y, min - a, use_h);
                argdet += theta;
            }
            if (b > min) {
                tie(y, theta) = propagateDelta(E, y, b - min, use_h);
                argdet += theta;
            }
        } else {
            if (a < max) {
                tie(y, theta) = propagateDelta(E, y, max - a, use_h);
                argdet += theta;
            }
            if (b < max) {
                tie(y, theta) = propagateDelta(E, y, b - max, use_h);
                argdet += theta;
            }
        }
    }
    return {y, argdet};
}

template<typename Scalar>
Scalar Matslise<Scalar>::Sector::calculateError() const {
    return (
            (calculateT(vs[0], true).t - calculateT(vs[0], false).t).array() *
            (Array<Scalar, 2, 2>() << 1, 1 / h, h, 1).finished()
    ).cwiseAbs().sum();
}

template<typename Scalar>
Matslise<Scalar>::Sector::~Sector() {
    delete[]vs;
}

#include "../util/instantiate.h"