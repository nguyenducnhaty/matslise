//
// Created by toon on 5/16/18.
//

#ifndef SCHRODINGER_MATSLISE_H
#define SCHRODINGER_MATSLISE_H

#include <iostream>
#include <array>
#include <vector>
#include <tuple>
#include <functional>
#include <Eigen/Dense>
#include "Array2D.h"
#include "Matrix2D.h"

#define MATSLISE_HMAX 17
#define MATSLISE_ETA 9

using namespace Eigen;

namespace matslise {

    template<typename D=double>
    class Y {
    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        Vector2D<D> y, dy;

        Y() : y({D(0), D(0)}), dy({D(0), D(0)}) {}

        Y(Vector2D<D> y) : y(y), dy({D(0), D(0)}) {}

        Y(Vector2D<D> y, Vector2D<D> dy) : y(y), dy(dy) {}

        Y<D> operator-() const {
            return Y(-y, -dy);
        }

        double theta() const {
            return atan(y[0] / y[1]);
        }

        friend std::ostream &operator<<(std::ostream &os, const Y<D> &m) {
            return os << "(" << m.y[0] << "," << m.y[1] << ")" << "(" << m.dy[0] << "," << m.dy[1] << ")";
        }
    };

    template<typename D=double>
    class T {
    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        Matrix2D<D> t;
        Matrix2D<D> dt;

        T() : t({D(1), D(0), D(0), D(1)}), dt({D(0), D(0), D(0), D(0)}) {}

        T(Matrix2D<D> t, Matrix2D<D> dt) : t(t), dt(dt) {}

        Y<D> operator*(Y<D> y) const {
            return Y<D>(t * y.y, t * y.dy + dt * y.y);
        }

        Y<D> operator/(Y<D> y) const {
            Matrix2D<D> ti, dti;
            ti = {t.d, -t.b, -t.c, t.a};
            dti = {dt.d, -dt.b, -dt.c, dt.a};
            return Y<D>(ti * y.y, ti * y.dy + dti * y.y);
        }

        friend std::ostream &operator<<(std::ostream &os, const T<D> &m) {
            return os << m.t;
        }
    };

    namespace matslise_sector {
        class Sector;
    }

    class Matslise {
    public:
        std::function<double(double)> V;
        double xmin, xmax;
        int sectorCount;
        double match;
        matslise::matslise_sector::Sector **sectors;

    public:
        Matslise(std::function<double(double)> V, double xmin, double xmax, int sectorCount);

        std::tuple<matslise::Y<double>, double>
        propagate(double E, const matslise::Y<double> &y, double a, double b) const;

        Eigen::Array<matslise::Y<double>, Eigen::Dynamic, 1>
        computeEigenfunction(double E, const matslise::Y<double> &left, const matslise::Y<double> &right,
                             const Eigen::ArrayXd &x) const;

        std::tuple<double, double, double>
        calculateError(double E, const matslise::Y<double> &left, const matslise::Y<double> &right) const;

        std::vector<std::tuple<unsigned int, double>> *
        computeEigenvalues(double Emin, double Emax, const matslise::Y<double> &left,
                           const matslise::Y<double> &right) const;

        std::vector<std::tuple<unsigned int, double>> *
        computeEigenvaluesByIndex(unsigned int Imin, unsigned int Imax, const matslise::Y<double> &left,
                                  const matslise::Y<double> &right) const;

        std::vector<std::tuple<unsigned int, double>> *
        computeEigenvalues(double Emin, double Emax, unsigned int Imin, unsigned int Imax,
                           const matslise::Y<double> &left,
                           const matslise::Y<double> &right) const;

        virtual ~Matslise();
    };

    class HalfRange {
    public:
        const Matslise *ms;

    public:
        HalfRange(std::function<double(double)> V, double xmax, int sectorCount);

        Array<matslise::Y<double>, Dynamic, 1>
        computeEigenfunction(double E, const matslise::Y<double> &side, const ArrayXd &x) const;

        std::vector<std::tuple<unsigned int, double>> *
        computeEigenvalues(double Emin, double Emax, const matslise::Y<double> &side) const;

        std::vector<std::tuple<unsigned int, double>> *
        computeEigenvaluesByIndex(unsigned int Imin, unsigned int Imax, const matslise::Y<double> &side) const;

        virtual ~HalfRange();
    };

    namespace matslise_sector {
        class Sector {
        public:
            Matslise *s;
            double *vs;
            double xmin, xmax, h;
            Array2D<Matrix2D<>, MATSLISE_ETA, MATSLISE_HMAX> t_coeff;
            Matrix2D<> t_coeff_h[MATSLISE_ETA];

            Sector(Matslise *problem, double xmin, double xmax);

            void calculateTCoeffs();

            T<double> calculateT(double E) const;

            T<double> calculateT(double E, double delta) const;

            Y<double> propagate(double E, const Y<double> &y0, double delta, double &theta) const;

            double prufer(double E, double delta, const Y<double> &y0, const Y<double> &y1) const;

            virtual ~Sector();
        };
    }
}


#endif //SCHRODINGER_MATSLISE_H