#include <functional>
#include "../matslise.h"
#include "matscs_formulas.h"
#include "../util/legendre.h"
#include "../util/calculateEta.h"
#include "../util/horner.h"
#include "../util/constants.h"

#define EPS (1.e-12)

using namespace std;
using namespace matslise;
using namespace Eigen;

template<typename Scalar>
Matscs<Scalar>::Sector::Sector(const Matscs *s, const Scalar &min, const Scalar &max, bool backward)
        : Sector(legendre::getCoefficients<MATSCS_N>(s->V, min, max), min, max, backward) {
}


template<typename Scalar>
Matscs<Scalar>::Sector::Sector(const std::array<Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic>, MATSCS_N> &vs,
                               const Scalar &min, const Scalar &max, bool backward)
        : vs(vs), min(min), max(max), backward(backward) {
    h = max - min;
    SelfAdjointEigenSolver<Matrix<Scalar, Dynamic, Dynamic>> es(vs[0]);
    n = vs[0].rows();
    diagonalize = es.eigenvectors();

    for (int i = 0; i < MATSCS_N; ++i)
        this->vs[i] = (backward && i % 2 == 1 ? -1 : 1) * diagonalize.transpose() * vs[i] * diagonalize;

    calculateTCoeffs();
}

template<typename Scalar>
void Matscs<Scalar>::Sector::calculateTCoeffs() {
    calculate_tcoeff_matrix(n, h, vs, t_coeff, t_coeff_h);
}

template<typename Scalar, typename T>
Matrix<Scalar, Dynamic, Dynamic> diagonalMatrix(const T &arr) {
    return arr.matrix().asDiagonal();
}

template<typename Scalar>
T<Scalar, Dynamic> Matscs<Scalar>::Sector::calculateT(const Scalar &E, const Scalar &delta, bool use_h) const {
    MatrixXs zero = MatrixXs::Zero(n, n);
    MatrixXs one = MatrixXs::Identity(n, n);

    if (abs(delta) <= EPS) {
        return T<Scalar, Dynamic>(n);
    }
    if (use_h && abs(delta - h) <= EPS)
        return calculateT(E);

    ArrayXs VEd = (vs[0].diagonal() - Matrix<Scalar, Dynamic, 1>::Constant(n, E)) * delta;
    Array<Scalar, MATSCS_ETA_delta, Dynamic> eta = calculateEta<Scalar, MATSCS_ETA_delta>(VEd * delta);
    T<Scalar, Dynamic> t(n);
    t.t << zero, zero, diagonalMatrix<Scalar>(eta.row(1).transpose() * VEd), zero;
    t.dt << zero, zero, diagonalMatrix<Scalar>(
            -delta * eta.row(1).transpose() - (delta * delta / 2) * eta.row(2).transpose() * VEd), zero;

    for (int i = 0; i < MATSCS_ETA_delta; ++i) {
        MatrixXs hor = horner<MatrixXs>(
                t_coeff.row(i), delta, MATSCS_HMAX_delta);
        t.t += hor * kroneckerProduct(Matrix<Scalar, 2, 2>::Identity(), diagonalMatrix<Scalar>(eta.row(i)));

        if (i + 1 < MATSCS_ETA_delta)
            t.dt += hor * kroneckerProduct(
                    Matrix<Scalar, 2, 2>::Identity(), diagonalMatrix<Scalar>(eta.row(i + 1) * (-delta * delta / 2.)));
    }

    t.t = kroneckerProduct(Matrix<Scalar, 2, 2>::Identity(), diagonalize) * t.t *
          kroneckerProduct(Matrix<Scalar, 2, 2>::Identity(), diagonalize.transpose());
    t.dt = kroneckerProduct(Matrix<Scalar, 2, 2>::Identity(), diagonalize) * t.dt *
           kroneckerProduct(Matrix<Scalar, 2, 2>::Identity(), diagonalize.transpose());
    return t;
}

template<typename Scalar>
T<Scalar, Dynamic> Matscs<Scalar>::Sector::calculateT(const Scalar &E, bool use_h) const {
    if (!use_h)
        return calculateT(E, h, false);
    int N = n;
    MatrixXs zero = MatrixXs::Zero(N, N);
    MatrixXs one = MatrixXs::Identity(N, N);

    ArrayXs VEd = (vs[0].diagonal().array() - Array<Scalar, Dynamic, 1>::Constant(N, E)) * h;
    Array<Scalar, MATSCS_ETA_h, Dynamic> eta = calculateEta<Scalar, MATSCS_ETA_h>(VEd * h);
    T<Scalar, Dynamic> t(N);
    t.t << zero, zero, diagonalMatrix<Scalar>(eta.row(1).transpose() * VEd), zero;
    t.dt << zero, zero, diagonalMatrix<Scalar>(
            -h * eta.row(1).transpose() - (h * h / 2) * eta.row(2).transpose() * VEd), zero;

    for (int i = 0; i < MATSCS_ETA_h; ++i) {
        t.t += t_coeff_h[i] * kroneckerProduct(Matrix<Scalar, 2, 2>::Identity(), diagonalMatrix<Scalar>(eta.row(i)));

        if (i + 1 < MATSCS_ETA_h)
            t.dt += t_coeff_h[i] * kroneckerProduct(
                    Matrix<Scalar, 2, 2>::Identity(), diagonalMatrix<Scalar>(eta.row(i + 1) * (-h * h / 2)));
    }

    t.t = kroneckerProduct(Matrix<Scalar, 2, 2>::Identity(), diagonalize) * t.t *
          kroneckerProduct(Matrix<Scalar, 2, 2>::Identity(), diagonalize.transpose());
    t.dt = kroneckerProduct(Matrix<Scalar, 2, 2>::Identity(), diagonalize) * t.dt *
           kroneckerProduct(Matrix<Scalar, 2, 2>::Identity(), diagonalize.transpose());
    return t;
}

template<typename Scalar>
template<int r>
Y<Scalar, Dynamic, r> Matscs<Scalar>::Sector::propagateDeltaColumn(
        const Scalar &E, const Y<Scalar, Dynamic, r> &y0, const Scalar &_delta, bool use_h) const {
    Scalar delta = _delta;
    if (backward)
        delta *= -1;
    bool rightDirection = delta >= 0;
    if (!rightDirection)
        delta = -delta;
    if (delta > h)
        delta = h;

    const T<Scalar, Dynamic> &t = calculateT(E, delta, use_h);
    Y<Scalar, Dynamic, r> y = y0;
    if (backward)
        y.reverse();
    Y<Scalar, Dynamic, r> y1 = rightDirection ? t * y : t / y;
    if (backward)
        y1.reverse();

    return y1;
}

template<typename Scalar>
typename Matscs<Scalar>::MatrixXcs Matscs<Scalar>::Sector::theta(const Y<Scalar, Dynamic> &y) const {
    return (y.getY(1) - y.getY(0) * complex<Scalar>(0, 1))
            .transpose()
            .partialPivLu()
            .solve((y.getY(1) + y.getY(0) * complex<Scalar>(0, 1)).transpose())
            .transpose();
}

template<typename Scalar, typename InputMatrix>
inline Array<Scalar, Dynamic, 1> angle(const InputMatrix &m) {
    Array<Scalar, Dynamic, 1> angles = m.eigenvalues().array().arg();

    const Scalar PI2 = constants<Scalar>::PI * 2;
    for (Eigen::Index i = 0; i < angles.size(); ++i) {
        if (angles[i] <= 0)
            angles[i] += PI2;
    }
    return angles;
}

template<typename Scalar>
pair<Y<Scalar, Dynamic>, Scalar> Matscs<Scalar>::Sector::propagateDelta(
        const Scalar &E, const Y<Scalar, Dynamic> &y0, const Scalar &delta, bool use_h) const {
    Y<Scalar, Dynamic> y1 = propagateDeltaColumn(E, y0, delta, use_h);

    Scalar d = (delta > h ? h : delta < -h ? -h : delta);

    ArrayXs Z = d * d * (vs[0].diagonal().array() - ArrayXs::Constant(n, E));

    Array<Scalar, 2, Dynamic> eta = calculateEta<Scalar, 2>(Z);
    Scalar argdet = 0;
    for (int i = 0; i < n; ++i) {
        if (Z[i] < 0) {
            Scalar sZ = (d < 0 ? -1 : 1) * sqrt(-Z[i]);
            argdet += (sZ + atan2(
                    (d - sZ) * eta(1, i) * eta(0, i),
                    1 + (d * sZ + Z[i]) * eta(1, i) * eta(1, i)));
        } else {
            argdet += atan2(d * eta(1, i), eta(0, i));
        }
    }
    argdet *= 2;

    MatrixXcs thetaZ0 = theta(y0);
    MatrixXcs thetaZ = theta(y1);
    ArrayXs betas = angle<Scalar>(thetaZ);
    const complex<Scalar> i_delta(0, d);
    ArrayXs alphas = angle<Scalar>(
            ((eta.row(0) + i_delta * eta.row(1)) / (eta.row(0) - i_delta * eta.row(1))).matrix().asDiagonal() * thetaZ0
    );
    argdet += (betas - alphas).sum();

    return {y1, argdet};
}

template<typename Scalar>
template<int r>
Y<Scalar, Dynamic, r> Matscs<Scalar>::Sector::propagateColumn(
        const Scalar &E, const Y<Scalar, Dynamic, r> &y0, const Scalar &a, const Scalar &b, bool use_h) const {
    Y<Scalar, Dynamic, r> y = y0;
    if (!((a >= max && b >= max) || (a <= min && b <= min))) {
        if (!backward) { // forward
            if (a > min)
                y = propagateDeltaColumn(E, y, min - a, use_h);
            if (b > min)
                y = propagateDeltaColumn(E, y, b - min, use_h);
        } else {
            if (a < max)
                y = propagateDeltaColumn(E, y, max - a, use_h);
            if (b < max)
                y = propagateDeltaColumn(E, y, b - max, use_h);
        }
    }
    return y;
}

template<typename Scalar>
pair<Y<Scalar, Dynamic>, Scalar> Matscs<Scalar>::Sector::propagate(
        const Scalar &E, const Y<Scalar, Dynamic> &y0, const Scalar &a, const Scalar &b, bool use_h) const {
    Y<Scalar, Dynamic> y = y0;
    Scalar argdet = 0;
    Scalar dTheta;
    if (!((a >= max && b >= max) || (a <= min && b <= min))) {
        if (!backward) { // forward
            if (a > min) {
                tie(y, dTheta) = propagateDelta(E, y, min - a, use_h);
                argdet += dTheta;
            }
            if (b > min) {
                tie(y, dTheta) = propagateDelta(E, y, b - min, use_h);
                argdet += dTheta;
            }
        } else {
            if (a < max) {
                tie(y, dTheta) = propagateDelta(E, y, max - a, use_h);
                argdet += dTheta;
            }
            if (b < max) {
                tie(y, dTheta) = propagateDelta(E, y, b - max, use_h);
                argdet += dTheta;
            }
        }
    }
    return {y, argdet};
}


template<typename Scalar>
Matrix<Scalar, Dynamic, Dynamic>
propagatePsi_delta(const typename Matscs<Scalar>::Sector *sector, Scalar E, const Matrix<Scalar, Dynamic, Dynamic> &psi,
                   Scalar delta) {
    if (sector->backward)
        delta *= -1;
    bool forward = delta >= 0;
    if (!forward)
        delta = -delta;
    if (delta > sector->h)
        delta = sector->h;

    // TODO: verify
    Scalar extra = sector->backward ? -1 : 1;
    if (delta > 0) {
        T<Scalar, Dynamic> T = sector->calculateT(E, delta);
        return extra * (T.getT(1, 1) + extra * T.getT(1, 0) * psi).transpose()
                .colPivHouseholderQr()
                .solve((T.getT(0, 1) + extra * T.getT(0, 0) * psi).transpose())
                .transpose();
    } else if (delta < 0) {
        T<Scalar, Dynamic> T = sector->calculateT(E, -delta);
        return extra * (T.getT(1, 1) - extra * psi * T.getT(0, 1))
                .colPivHouseholderQr()
                .solve((extra * psi * T.getT(0, 0) - T.getT(1, 0)));
    } else {
        return psi;
    }
}

template<typename Scalar>
Matrix<Scalar, Dynamic, Dynamic>
Matscs<Scalar>::Sector::propagatePsi(
        const Scalar &E, const Matrix<Scalar, Dynamic, Dynamic> &_psi, const Scalar &a, const Scalar &b) const {
    Matrix<Scalar, Dynamic, Dynamic> psi = _psi;
    if (!((a >= max && b >= max) || (a <= min && b <= min))) {
        if (!backward) { // forward
            if (a > min)
                psi = propagatePsi_delta(this, E, psi, min - a);
            if (b > min)
                psi = propagatePsi_delta(this, E, psi, b - min);
        } else {
            if (a < max)
                psi = propagatePsi_delta(this, E, psi, max - a);
            if (b < max)
                psi = propagatePsi_delta(this, E, psi, b - max);
        }
    }
    return psi;
}

template<typename Scalar>
Scalar Matscs<Scalar>::Sector::error() const {
    Scalar E = vs[0].diagonal().minCoeff();
    Scalar error = (calculateT(E, true).t - calculateT(E, false).t).cwiseAbs().mean();
    if (isnan(error))
        return numeric_limits<Scalar>::infinity();
    return error;
}

#define INSTANTIATE_PROPAGATE(Scalar, r)\
template Y<Scalar, Dynamic, r>\
Matscs<Scalar>::Sector::propagateColumn<r>(\
    const Scalar &E, const Y<Scalar, Dynamic, r> &y0, const Scalar &a, const Scalar &b, bool use_h) const;

#define INSTANTIATE_MORE(Scalar)\
INSTANTIATE_PROPAGATE(Scalar, 1)\
INSTANTIATE_PROPAGATE(Scalar, -1)

#include "../util/instantiate.h"