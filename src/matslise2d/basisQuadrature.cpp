#include "../matslise.h"
#include "./etaIntegrals.h"
#include "../util/legendre.h"

using namespace Eigen;

template<typename Scalar, int N = MATSLISE_N>
void legendreTransform(const Scalar &delta, Array<Scalar, N, 1> &quadratures) {
    static Matrix<Scalar, N, N> legendrePolynomials = (
            Matrix<Scalar, MATSLISE_N, MATSLISE_N>()
                    << 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    -1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    1, -6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    -1, 12, -30, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    1, -20, 90, -140, 70, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    -1, 30, -210, 560, -630, 252, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    1, -42, 420, -1680, 3150, -2772, 924, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    -1, 56, -756, 4200, -11550, 16632, -12012, 3432, 0, 0, 0, 0, 0, 0, 0, 0,
                    1, -72, 1260, -9240, 34650, -72072, 84084, -51480, 12870, 0, 0, 0, 0, 0, 0, 0,
                    -1, 90, -1980, 18480, -90090, 252252, -420420, 411840, -218790, 48620, 0, 0, 0, 0, 0, 0,
                    1, -110, 2970, -34320, 210210, -756756, 1681680, -2333760, 1969110, -923780, 184756, 0, 0, 0, 0, 0,
                    -1, 132, -4290, 60060, -450450, 2018016, -5717712, 10501920, -12471030, 9237800, -3879876, 705432, 0, 0, 0, 0,
                    1, -156, 6006, -100100, 900900, -4900896, 17153136, -39907296, 62355150, -64664600, 42678636, -16224936, 2704156, 0, 0, 0,
                    -1, 182, -8190, 160160, -1701700, 11027016, -46558512, 133024320, -261891630, 355655300, -327202876, 194699232, -67603900, 10400600, 0, 0,
                    1, -210, 10920, -247520, 3063060, -23279256, 116396280, -399072960, 960269310, -1636014380, 1963217256, -1622493600, 878850700, -280816200, 40116600, 0,
                    -1, 240, -14280, 371280, -5290740, 46558512, -271591320, 1097450640, -3155170590, 6544057520, -9816086280, 10546208400, -7909656300, 3931426800, -1163381400, 155117520
    ).finished().template topLeftCorner<N, N>();

    Scalar d = 1 / delta;
    for (int i = 1; i < N; ++i, d /= delta)
        quadratures(i) *= d;
    quadratures = (legendrePolynomials * quadratures.matrix()).array();
}

template<typename Scalar, int hmax, bool halfrange>
Matrix<Scalar, Dynamic, Dynamic>
matslise::BasisQuadrature<Scalar, hmax, halfrange>::dV(
        const typename matslise::Matslise2D<Scalar>::Sector &sector2d, const Scalar &y) {
    if (quadData.empty())
        calculateQuadData(sector2d);

    Index N = sector2d.se2d->N;
    Matrix<Scalar, Dynamic, Dynamic> dV
            = Matrix<Scalar, Dynamic, Dynamic>::Zero(N, N);

    for (int i = 0; i < N; ++i) {
        dV(i, i) = sector2d.eigenvalues[i];
    }


    Array<Scalar, hmax, 1> vDiff;
    auto quadIt = quadData.begin();

    for (auto sector1d : matslise->sectors) {

        if (sector1d->backward) {
            vDiff = Array<Scalar, hmax, 1>::Map(sector1d->vs.data());
            for (int i = 0; i < hmax; i += 2)
                vDiff(i) *= -1;
        } else {
            vDiff = -Array<Scalar, hmax, 1>::Map(sector1d->vs.data());
        }

        vDiff += Array<Scalar, hmax, 1>::Map(
                matslise::legendre::getCoefficients<hmax, Scalar, Scalar>(
                        [&](const Scalar &x) -> Scalar { return sector2d.se2d->potential(x, y); },
                        sector1d->min, sector1d->max
                ).data());

        Scalar h = sector1d->h;
        for (int i = 1; i < hmax; ++i, h *= sector1d->h)
            vDiff(i) *= h;

        for (int i = 0; i < N; ++i)
            for (int j = 0; j <= i; ++j) {
                if (halfrange && (i & 1) != (j & 1))
                    continue;
                Scalar v = ((*quadIt) * vDiff).sum();
                if constexpr (halfrange)
                    v += v;
                dV(i, j) += v;
                ++quadIt;
            }
    }
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < i; ++j) {
            dV(j, i) = dV(i, j);
        }
    return dV;
}

template<typename Scalar, int hmax, bool halfrange>
void matslise::BasisQuadrature<Scalar, hmax, halfrange>::calculateQuadData(
        const typename matslise::Matslise2D<Scalar>::Sector &sector2d) {

    for (auto sector1d : matslise->sectors) {
        Index N = sector2d.se2d->N;
        Array<Scalar, MATSLISE_ETA_delta, MATSLISE_HMAX_delta> u = sector1d->t_coeff.unaryExpr(
                [](const Matrix<Scalar, 2, 2, DontAlign> &T) {
                    return T(0, 0);
                });
        Array<Scalar, MATSLISE_ETA_delta, MATSLISE_HMAX_delta> v = sector1d->t_coeff.unaryExpr(
                [](const Matrix<Scalar, 2, 2, DontAlign> &T) {
                    return T(0, 1);
                });

        Array<Scalar, MATSLISE_ETA_delta * MATSLISE_ETA_delta, Dynamic>
                uu = etaProduct<Scalar, MATSLISE_INTEGRATE_delta>(u, u);
        Array<Scalar, MATSLISE_ETA_delta * MATSLISE_ETA_delta, Dynamic>
                uv = etaProduct<Scalar, MATSLISE_INTEGRATE_delta>(u, v);
        Array<Scalar, MATSLISE_ETA_delta * MATSLISE_ETA_delta, Dynamic>
                vu = etaProduct<Scalar, MATSLISE_INTEGRATE_delta>(v, u);
        Array<Scalar, MATSLISE_ETA_delta * MATSLISE_ETA_delta, Dynamic>
                vv = etaProduct<Scalar, MATSLISE_INTEGRATE_delta>(v, v);

        std::vector<matslise::Y<Scalar>> y0;
        y0.reserve(N);
        for (auto &f : sector2d.eigenfunctions) {
            if (sector1d->backward) {
                y0.emplace_back(f(sector1d->max));
                y0.back().reverse();
            } else {
                y0.emplace_back(f(sector1d->min));
            }
        }

        for (int i = 0; i < N; ++i)
            for (int j = 0; j <= i; ++j) {
                if (halfrange && (i & 1) != (j & 1))
                    continue;
                Array<Scalar, MATSLISE_ETA_delta * MATSLISE_ETA_delta, Dynamic>
                        eta = (i == j
                               ? eta_integrals<Scalar, true>(sector1d->h,
                                                             sector1d->vs[0] - sector2d.eigenvalues[i],
                                                             sector1d->vs[0] - sector2d.eigenvalues[j])
                               : eta_integrals<Scalar, false>(sector1d->h,
                                                              sector1d->vs[0] - sector2d.eigenvalues[i],
                                                              sector1d->vs[0] - sector2d.eigenvalues[j]));

                Array<Scalar, hmax, 1> quadratures = Array<Scalar, hmax, 1>::Zero();
                {
                    Scalar suu = y0[i].y(0) * y0[j].y(0);
                    Scalar suv = y0[i].y(0) * y0[j].y(1);
                    Scalar svu = y0[i].y(1) * y0[j].y(0);
                    Scalar svv = y0[i].y(1) * y0[j].y(1);

                    for (Index n = 0; n < MATSLISE_INTEGRATE_delta; ++n) {
                        if (n <= MATSLISE_INTEGRATE_delta - hmax) {
                            quadratures += (
                                    (suu * uu + suv * uv + svu * vu + svv * vv).matrix()
                                            .template block<MATSLISE_ETA_delta * MATSLISE_ETA_delta, 1>(0, n)
                                            .transpose()
                                    * eta.matrix().template block<MATSLISE_ETA_delta * MATSLISE_ETA_delta, hmax>(
                                            0, n)).array();
                        } else {
                            Matrix<Scalar, MATSLISE_ETA_delta * MATSLISE_ETA_delta, 1> coln
                                    = (suu * uu + suv * uv + svu * vu + svv * vv).matrix().col(n);
                            for (Index m = 0; m < hmax && m + n < MATSLISE_INTEGRATE_delta; ++m) {
                                quadratures(m) += coln.dot(
                                        eta.template block<MATSLISE_ETA_delta * MATSLISE_ETA_delta, 1>(0, m+n)
                                                .matrix());
                            }
                        }
                    }
                }
                legendreTransform<Scalar>(sector1d->h, quadratures);
                quadData.push_back(quadratures);

                if (sector1d->backward) {
                    auto &quad = quadData.back();
                    for (Index k = 1; k < hmax; k += 2)
                        quad(k) *= -1;
                }
            }
    }
}

#define INSTANTIATE_MORE(Scalar) \
    template class matslise::BasisQuadrature<Scalar, 8, true>; \
    template class matslise::BasisQuadrature<Scalar, 8, false>;

#include "../util/instantiate.h"