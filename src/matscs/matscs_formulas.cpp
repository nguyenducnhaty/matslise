#include "matscs_formulas.h"

void calculate_tcoeff_matrix(int n, double h, MatrixXd *vs, Array2D<Matrix2D<MatrixXd>, MATSCS_ETA, MATSCS_HMAX> &t) {
    MatrixXd zero = MatrixXd::Zero(n, n);
    MatrixXd I = MatrixXd::Identity(n, n);
    MatrixXd &v0 = vs[0];
    MatrixXd &v1 = vs[1];
    MatrixXd &v2 = vs[2];
    MatrixXd &v3 = vs[3];
    MatrixXd &v4 = vs[4];
    MatrixXd &v5 = vs[5];
    MatrixXd &v6 = vs[6];
    t[0][0] = {(I), zero, zero, (I)}; 
    t[0][1] = {zero, zero, ((((-0.5*v1))+(((0.5*v2))+(((-0.5*v3))+(((0.5*v4))+(((-0.5*v5))+((0.5*v6))*h)*h)*h)*h)*h)*h), zero}; 
    t[0][2] = {zero, zero, (((0.5*v1))+(((-1.5*v2))+(((3.0*v3))+(((-5.0*v4))+(((7.5*v5))+((-10.5*v6))*h)*h)*h)*h)*h), zero}; 
    t[0][3] = {zero, zero, ((v2)+(((-5.0*v3))+(((15.0*v4))+(((-35.0*v5))+((70.0*v6))*h)*h)*h)*h), zero}; 
    t[0][4] = {zero, zero, (((2.5*v3))+(((-17.5*v4))+(((70.0*v5))+((-210.0*v6))*h)*h)*h), zero}; 
    t[0][5] = {zero, zero, (((7.0*v4))+(((-63.0*v5))+((315.0*v6))*h)*h), zero}; 
    t[0][6] = {zero, zero, (((21.0*v5))+((-231.0*v6))*h), zero}; 
    t[0][7] = {zero, zero, ((66.0*v6)), zero}; 
    t[0][8] = {zero, zero, zero, zero}; 
    t[1][0] = {zero, zero, zero, zero}; 
    t[1][1] = {zero, (I), ((((-0.5*v1))+(((0.5*v2))+(((-0.5*v3))+(((0.5*v4))+(((-0.5*v5))+((0.5*v6))*h)*h)*h)*h)*h)*h), zero}; 
    t[1][2] = {((((-0.5*v1))+(((0.5*v2))+(((-0.5*v3))+(((0.5*v4))+(((-0.5*v5))+((0.5*v6))*h)*h)*h)*h)*h)*h), zero, (((0.5*v1))+(((-1.5*v2))+(((3.0*v3))+(((-5.0*v4))+(((7.5*v5))+((-10.5*v6))*h)*h)*h)*h)*h), ((((-0.5*v1))+(((0.5*v2))+(((-0.5*v3))+(((0.5*v4))+(((-0.5*v5))+((0.5*v6))*h)*h)*h)*h)*h)*h)}; 
    t[1][3] = {(((0.5*v1))+(((-1.5*v2))+(((3.0*v3))+(((-5.0*v4))+(((7.5*v5))+((-10.5*v6))*h)*h)*h)*h)*h), zero, (((1.5*v2))+(((-7.5*v3)+(-0.125*(v0*v1))+(0.125*(v1*v0)))+(((0.125*(v1*v1))+(0.125*(v0*v2))+(22.5*v4)+(-0.125*(v2*v0)))+(((0.125*(v3*v0))+(-0.125*(v2*v1))+(-0.125*(v0*v3))+(-52.5*v5)+(-0.125*(v1*v2)))+(((0.125*(v2*v2))+(0.125*(v1*v3))+(0.125*(v0*v4))+(0.125*(v3*v1))+(-0.125*(v4*v0))+(105.0*v6))+((-0.125*(v2*v3))+(-0.125*(v4*v1))+(-0.125*(v1*v4))+(0.125*(v5*v0))+(-0.125*(v3*v2))+(-0.125*(v0*v5)))*h)*h)*h)*h)*h), (((0.5*v1))+(((-1.5*v2))+(((3.0*v3))+(((-5.0*v4))+(((7.5*v5))+((-10.5*v6))*h)*h)*h)*h)*h)}; 
    t[1][4] = {((v2)+(((-5.0*v3))+(((15.0*v4))+(((-35.0*v5))+((70.0*v6))*h)*h)*h)*h), zero, (((5.0*v3)+(0.08333333333333333*(v0*v1))+(-0.08333333333333333*(v1*v0)))+(((-0.25*(v1*v1))+(-0.25*(v0*v2))+(-35.0*v4)+(0.25*(v2*v0)))+(((-0.5*(v3*v0))+(0.5833333333333334*(v2*v1))+(0.5*(v0*v3))+(140.0*v5)+(0.4166666666666667*(v1*v2)))+(((-0.75*(v2*v2))+(-0.6666666666666666*(v1*v3))+(-0.8333333333333334*(v0*v4))+(-1.0833333333333333*(v3*v1))+(0.8333333333333334*(v4*v0))+(-420.0*v6))+((v2*v3)+(1.75*(v4*v1))+(v1*v4)+(-1.25*(v5*v0))+(1.25*(v3*v2))+(1.25*(v0*v5)))*h)*h)*h)*h), ((v2)+(((-5.0*v3))+(((15.0*v4))+(((-35.0*v5))+((70.0*v6))*h)*h)*h)*h)}; 
    t[1][5] = {(((2.5*v3))+(((-17.5*v4))+(((70.0*v5))+((-210.0*v6))*h)*h)*h), zero, (((0.125*(v1*v1))+(0.125*(v0*v2))+(17.5*v4)+(-0.125*(v2*v0)))+(((0.625*(v3*v0))+(-0.75*(v2*v1))+(-0.625*(v0*v3))+(-157.5*v5)+(-0.5*(v1*v2)))+(((1.625*(v2*v2))+(1.375*(v1*v3))+(1.875*(v0*v4))+(2.625*(v3*v1))+(-1.875*(v4*v0))+(787.5*v6))+((-3.25*(v2*v3))+(-6.875*(v4*v1))+(-3.125*(v1*v4))+(4.375*(v5*v0))+(-4.25*(v3*v2))+(-4.375*(v0*v5)))*h)*h)*h), (((2.5*v3))+(((-17.5*v4))+(((70.0*v5))+((-210.0*v6))*h)*h)*h)}; 
    t[1][6] = {(((7.0*v4))+(((-63.0*v5))+((315.0*v6))*h)*h), zero, (((-0.25*(v3*v0))+(0.3*(v2*v1))+(0.25*(v0*v3))+(63.0*v5)+(0.2*(v1*v2)))+(((-1.5*(v2*v2))+(-1.25*(v1*v3))+(-1.75*(v0*v4))+(-2.5*(v3*v1))+(1.75*(v4*v0))+(-693.0*v6))+((5.05*(v2*v3))+(11.5*(v4*v1))+(4.75*(v1*v4))+(-7.0*(v5*v0))+(6.7*(v3*v2))+(7.0*(v0*v5)))*h)*h), (((7.0*v4))+(((-63.0*v5))+((315.0*v6))*h)*h)}; 
    t[1][7] = {(((21.0*v5))+((-231.0*v6))*h), zero, (((0.5*(v2*v2))+(0.4166666666666667*(v1*v3))+(0.5833333333333334*(v0*v4))+(0.8333333333333334*(v3*v1))+(-0.5833333333333334*(v4*v0))+(231.0*v6))+((-3.75*(v2*v3))+(-8.75*(v4*v1))+(-3.5*(v1*v4))+(5.25*(v5*v0))+(-5.0*(v3*v2))+(-5.25*(v0*v5)))*h), (((21.0*v5))+((-231.0*v6))*h)}; 
    t[1][8] = {((66.0*v6)), zero, ((1.0714285714285714*(v2*v3))+(2.5*(v4*v1))+(v1*v4)+(-1.5*(v5*v0))+(1.4285714285714286*(v3*v2))+(1.5*(v0*v5))), ((66.0*v6))}; 
    t[2][0] = {zero, zero, zero, zero}; 
    t[2][1] = {zero, zero, zero, zero}; 
    t[2][2] = {zero, zero, zero, zero}; 
    t[2][3] = {(((-0.5*v1))+(((1.5*v2))+(((-3.0*v3))+(((5.0*v4))+(((-7.5*v5))+((10.5*v6))*h)*h)*h)*h)*h), ((((-0.5*v1))+(((0.5*v2))+(((-0.5*v3))+(((0.5*v4))+((-0.5*v5))*h)*h)*h)*h)*h), (((-1.5*v2))+(((7.5*v3)+(-0.125*(v0*v1))+(0.125*(v1*v0)))+(((0.125*(v1*v1))+(0.125*(v0*v2))+(-22.5*v4)+(-0.125*(v2*v0)))+(((0.125*(v3*v0))+(-0.125*(v2*v1))+(-0.125*(v0*v3))+(52.5*v5)+(-0.125*(v1*v2)))+(((0.125*(v2*v2))+(0.125*(v1*v3))+(0.125*(v0*v4))+(0.125*(v3*v1))+(-0.125*(v4*v0))+(-105.0*v6))+((-0.125*(v2*v3))+(-0.125*(v4*v1))+(-0.125*(v1*v4))+(0.125*(v5*v0))+(-0.125*(v3*v2))+(-0.125*(v0*v5)))*h)*h)*h)*h)*h), (((0.5*v1))+(((-1.5*v2))+(((3.0*v3))+(((-5.0*v4))+(((7.5*v5))+((-10.5*v6))*h)*h)*h)*h)*h)}; 
    t[2][4] = {(((-1.5*v2))+(((7.5*v3)+(-0.125*(v0*v1))+(0.125*(v1*v0)))+(((0.125*(v1*v1))+(0.125*(v0*v2))+(-22.5*v4)+(-0.125*(v2*v0)))+(((0.125*(v3*v0))+(-0.125*(v2*v1))+(-0.125*(v0*v3))+(52.5*v5)+(-0.125*(v1*v2)))+((0.125*(v2*v2))+(0.125*(v1*v3))+(0.125*(v0*v4))+(0.125*(v3*v1))+(-0.125*(v4*v0))+(-105.0*v6))*h)*h)*h)*h), (((0.5*v1))+(((-1.5*v2))+(((3.0*v3))+(((-5.0*v4))+((7.5*v5))*h)*h)*h)*h), (((-7.5*v3))+(((-0.25*(v1*v1))+(52.5*v4))+(((0.75*(v2*v1))+(-210.0*v5)+(0.25*(v1*v2)))+(((-0.75*(v2*v2))+(-0.25*(v1*v3))+(-1.5*(v3*v1))+(630.0*v6))+((0.75*(v2*v3))+(2.5*(v4*v1))+(0.25*(v1*v4))+(1.5*(v3*v2)))*h)*h)*h)*h), (((1.5*v2))+(((-7.5*v3)+(-0.125*(v0*v1))+(0.125*(v1*v0)))+(((0.125*(v1*v1))+(0.125*(v0*v2))+(22.5*v4)+(-0.125*(v2*v0)))+(((0.125*(v3*v0))+(-0.125*(v2*v1))+(-0.125*(v0*v3))+(-52.5*v5)+(-0.125*(v1*v2)))+((0.125*(v2*v2))+(0.125*(v1*v3))+(0.125*(v0*v4))+(0.125*(v3*v1))+(-0.125*(v4*v0))+(105.0*v6))*h)*h)*h)*h)}; 
    t[2][5] = {(((-5.0*v3)+(0.08333333333333333*(v0*v1))+(-0.08333333333333333*(v1*v0)))+(((-0.25*(v1*v1))+(-0.25*(v0*v2))+(35.0*v4)+(0.25*(v2*v0)))+(((-0.5*(v3*v0))+(0.5833333333333334*(v2*v1))+(0.5*(v0*v3))+(-140.0*v5)+(0.4166666666666667*(v1*v2)))+((-0.75*(v2*v2))+(-0.6666666666666666*(v1*v3))+(-0.8333333333333334*(v0*v4))+(-1.0833333333333333*(v3*v1))+(0.8333333333333334*(v4*v0))+(420.0*v6))*h)*h)*h), ((v2)+(((-5.0*v3))+(((15.0*v4))+((-35.0*v5))*h)*h)*h), (((0.08333333333333333*(v1*v1))+(-35.0*v4))+(((-0.020833333333333332*(v1*v0*v0))+(-1.0*(v2*v1))+(-0.020833333333333332*(v0*v0*v1))+(0.041666666666666664*(v0*v1*v0))+(315.0*v5)+(-0.25*(v1*v2)))+(((-0.041666666666666664*(v1*v1*v0))+(1.5*(v2*v2))+(0.020833333333333332*(v0*v1*v1))+(0.5*(v1*v3))+(0.020833333333333332*(v2*v0*v0))+(4.25*(v3*v1))+(0.020833333333333332*(v1*v0*v1))+(0.020833333333333332*(v0*v0*v2))+(-1575.0*v6)+(-0.041666666666666664*(v0*v2*v0)))+((0.041666666666666664*(v1*v2*v0))+(-0.020833333333333332*(v2*v0*v1))+(-0.020833333333333332*(v0*v0*v3))+(-0.020833333333333332*(v1*v0*v2))+(-0.020833333333333332*(v3*v0*v0))+(-0.020833333333333332*(v0*v2*v1))+(-2.25*(v2*v3))+(-12.083333333333334*(v4*v1))+(-0.020833333333333332*(v0*v1*v2))+(-0.020833333333333332*(v1*v1*v1))+(-0.8333333333333334*(v1*v4))+(0.041666666666666664*(v2*v1*v0))+(-5.25*(v3*v2))+(0.041666666666666664*(v0*v3*v0)))*h)*h)*h), (((5.0*v3)+(0.08333333333333333*(v0*v1))+(-0.08333333333333333*(v1*v0)))+(((-0.25*(v1*v1))+(-0.25*(v0*v2))+(-35.0*v4)+(0.25*(v2*v0)))+(((-0.5*(v3*v0))+(0.5833333333333334*(v2*v1))+(0.5*(v0*v3))+(140.0*v5)+(0.4166666666666667*(v1*v2)))+((-0.75*(v2*v2))+(-0.6666666666666666*(v1*v3))+(-0.8333333333333334*(v0*v4))+(-1.0833333333333333*(v3*v1))+(0.8333333333333334*(v4*v0))+(-420.0*v6))*h)*h)*h)}; 
    t[2][6] = {(((0.125*(v1*v1))+(0.125*(v0*v2))+(-17.5*v4)+(-0.125*(v2*v0)))+(((0.625*(v3*v0))+(-0.75*(v2*v1))+(-0.625*(v0*v3))+(157.5*v5)+(-0.5*(v1*v2)))+((1.625*(v2*v2))+(1.375*(v1*v3))+(1.875*(v0*v4))+(2.625*(v3*v1))+(-1.875*(v4*v0))+(-787.5*v6))*h)*h), (((2.5*v3))+(((-17.5*v4))+((70.0*v5))*h)*h), (((0.010416666666666666*(v1*v0*v0))+(0.375*(v2*v1))+(0.010416666666666666*(v0*v0*v1))+(-0.020833333333333332*(v0*v1*v0))+(-157.5*v5)+(0.125*(v1*v2)))+(((0.07291666666666667*(v1*v1*v0))+(-1.5*(v2*v2))+(-0.03125*(v0*v1*v1))+(-0.625*(v1*v3))+(-0.03125*(v2*v0*v0))+(-4.375*(v3*v1))+(-0.041666666666666664*(v1*v0*v1))+(-0.03125*(v0*v0*v2))+(1732.5*v6)+(0.0625*(v0*v2*v0)))+((-0.11458333333333333*(v1*v2*v0))+(0.10416666666666667*(v2*v0*v1))+(0.0625*(v0*v0*v3))+(0.0625*(v1*v0*v2))+(0.0625*(v3*v0*v0))+(0.07291666666666667*(v0*v2*v1))+(4.125*(v2*v3))+(23.125*(v4*v1))+(0.052083333333333336*(v0*v1*v2))+(0.0625*(v1*v1*v1))+(1.875*(v1*v4))+(-0.17708333333333334*(v2*v1*v0))+(8.875*(v3*v2))+(-0.125*(v0*v3*v0)))*h)*h), (((0.125*(v1*v1))+(0.125*(v0*v2))+(17.5*v4)+(-0.125*(v2*v0)))+(((0.625*(v3*v0))+(-0.75*(v2*v1))+(-0.625*(v0*v3))+(-157.5*v5)+(-0.5*(v1*v2)))+((1.625*(v2*v2))+(1.375*(v1*v3))+(1.875*(v0*v4))+(2.625*(v3*v1))+(-1.875*(v4*v0))+(787.5*v6))*h)*h)}; 
    t[2][7] = {(((-0.25*(v3*v0))+(0.3*(v2*v1))+(0.25*(v0*v3))+(-63.0*v5)+(0.2*(v1*v2)))+((-1.5*(v2*v2))+(-1.25*(v1*v3))+(-1.75*(v0*v4))+(-2.5*(v3*v1))+(1.75*(v4*v0))+(693.0*v6))*h), (((7.0*v4))+((-63.0*v5))*h), (((-0.029166666666666667*(v1*v1*v0))+(0.6*(v2*v2))+(0.0125*(v0*v1*v1))+(0.25*(v1*v3))+(0.0125*(v2*v0*v0))+(1.5*(v3*v1))+(0.016666666666666666*(v1*v0*v1))+(0.0125*(v0*v0*v2))+(-693.0*v6)+(-0.025*(v0*v2*v0)))+((0.1125*(v1*v2*v0))+(-0.125*(v2*v0*v1))+(-0.0625*(v0*v0*v3))+(-0.0625*(v1*v0*v2))+(-0.0625*(v3*v0*v0))+(-0.075*(v0*v2*v1))+(-3.75*(v2*v3))+(-19.25*(v4*v1))+(-0.05*(v0*v1*v2))+(-0.0625*(v1*v1*v1))+(-1.75*(v1*v4))+(0.2*(v2*v1*v0))+(-7.5*(v3*v2))+(0.125*(v0*v3*v0)))*h), (((-0.25*(v3*v0))+(0.3*(v2*v1))+(0.25*(v0*v3))+(63.0*v5)+(0.2*(v1*v2)))+((-1.5*(v2*v2))+(-1.25*(v1*v3))+(-1.75*(v0*v4))+(-2.5*(v3*v1))+(1.75*(v4*v0))+(-693.0*v6))*h)}; 
    t[2][8] = {((0.5*(v2*v2))+(0.4166666666666667*(v1*v3))+(0.5833333333333334*(v0*v4))+(0.8333333333333334*(v3*v1))+(-0.5833333333333334*(v4*v0))+(-231.0*v6)), ((21.0*v5)), ((-0.0375*(v1*v2*v0))+(0.041666666666666664*(v2*v0*v1))+(0.020833333333333332*(v0*v0*v3))+(0.020833333333333332*(v1*v0*v2))+(0.020833333333333332*(v3*v0*v0))+(0.025*(v0*v2*v1))+(1.25*(v2*v3))+(5.833333333333333*(v4*v1))+(0.016666666666666666*(v0*v1*v2))+(0.020833333333333332*(v1*v1*v1))+(0.5833333333333334*(v1*v4))+(-0.06666666666666667*(v2*v1*v0))+(2.5*(v3*v2))+(-0.041666666666666664*(v0*v3*v0))), ((0.5*(v2*v2))+(0.4166666666666667*(v1*v3))+(0.5833333333333334*(v0*v4))+(0.8333333333333334*(v3*v1))+(-0.5833333333333334*(v4*v0))+(231.0*v6))}; 
    t[3][0] = {zero, zero, zero, zero}; 
    t[3][1] = {zero, zero, zero, zero}; 
    t[3][2] = {zero, zero, zero, zero}; 
    t[3][3] = {zero, zero, zero, zero}; 
    t[3][4] = {zero, zero, zero, zero}; 
    t[3][5] = {(((2.5*v3)+(-0.16666666666666666*(v0*v1))+(0.16666666666666666*(v1*v0)))+(((0.25*(v1*v1))+(0.5*(v0*v2))+(-17.5*v4)+(-0.5*(v2*v0)))+(((v3*v0)+(-0.4166666666666667*(v2*v1))+(-1.0*(v0*v3))+(70.0*v5)+(-0.5833333333333334*(v1*v2)))+((0.75*(v2*v2))+(1.0833333333333333*(v1*v3))+(1.6666666666666667*(v0*v4))+(0.6666666666666666*(v3*v1))+(-1.6666666666666667*(v4*v0))+(-210.0*v6))*h)*h)*h), (((-0.5*v2))+(((2.5*v3)+(-0.125*(v0*v1))+(0.125*(v1*v0)))+(((0.125*(v1*v1))+(0.125*(v0*v2))+(-7.5*v4)+(-0.125*(v2*v0)))+((0.125*(v3*v0))+(-0.125*(v2*v1))+(-0.125*(v0*v3))+(17.5*v5)+(-0.125*(v1*v2)))*h)*h)*h), (((-0.2916666666666667*(v1*v1))+(-0.375*(v0*v2))+(17.5*v4)+(0.375*(v2*v0)))+(((-1.875*(v3*v0))+(-0.020833333333333332*(v1*v0*v0))+(1.25*(v2*v1))+(1.875*(v0*v3))+(-0.020833333333333332*(v0*v0*v1))+(0.041666666666666664*(v0*v1*v0))+(-157.5*v5)+(1.25*(v1*v2)))+(((-0.041666666666666664*(v1*v1*v0))+(-3.375*(v2*v2))+(0.020833333333333332*(v0*v1*v1))+(-3.625*(v1*v3))+(0.020833333333333332*(v2*v0*v0))+(-5.625*(v0*v4))+(-3.625*(v3*v1))+(0.020833333333333332*(v1*v0*v1))+(5.625*(v4*v0))+(0.020833333333333332*(v0*v0*v2))+(787.5*v6)+(-0.041666666666666664*(v0*v2*v0)))+((0.041666666666666664*(v1*v2*v0))+(-0.020833333333333332*(v2*v0*v1))+(-0.020833333333333332*(v0*v0*v3))+(-13.125*(v5*v0))+(-0.020833333333333332*(v1*v0*v2))+(-0.020833333333333332*(v3*v0*v0))+(-0.020833333333333332*(v0*v2*v1))+(7.5*(v2*v3))+(8.541666666666666*(v4*v1))+(-0.020833333333333332*(v0*v1*v2))+(-0.020833333333333332*(v1*v1*v1))+(8.541666666666666*(v1*v4))+(0.041666666666666664*(v2*v1*v0))+(7.5*(v3*v2))+(13.125*(v0*v5))+(0.041666666666666664*(v0*v3*v0)))*h)*h)*h), (((-2.5*v3)+(0.08333333333333333*(v0*v1))+(-0.08333333333333333*(v1*v0)))+(((-0.25*(v1*v1))+(-0.25*(v0*v2))+(17.5*v4)+(0.25*(v2*v0)))+(((-0.5*(v3*v0))+(0.5833333333333334*(v2*v1))+(0.5*(v0*v3))+(-70.0*v5)+(0.4166666666666667*(v1*v2)))+((-0.75*(v2*v2))+(-0.6666666666666666*(v1*v3))+(-0.8333333333333334*(v0*v4))+(-1.0833333333333333*(v3*v1))+(0.8333333333333334*(v4*v0))+(210.0*v6))*h)*h)*h)}; 
    t[3][6] = {(((-0.2916666666666667*(v1*v1))+(-0.375*(v0*v2))+(17.5*v4)+(0.375*(v2*v0)))+(((-1.875*(v3*v0))+(-0.020833333333333332*(v1*v0*v0))+(1.25*(v2*v1))+(1.875*(v0*v3))+(-0.020833333333333332*(v0*v0*v1))+(0.041666666666666664*(v0*v1*v0))+(-157.5*v5)+(1.25*(v1*v2)))+((-0.041666666666666664*(v1*v1*v0))+(-3.375*(v2*v2))+(0.020833333333333332*(v0*v1*v1))+(-3.625*(v1*v3))+(0.020833333333333332*(v2*v0*v0))+(-5.625*(v0*v4))+(-3.625*(v3*v1))+(0.020833333333333332*(v1*v0*v1))+(5.625*(v4*v0))+(0.020833333333333332*(v0*v0*v2))+(787.5*v6)+(-0.041666666666666664*(v0*v2*v0)))*h)*h), (((-2.5*v3)+(0.08333333333333333*(v0*v1))+(-0.08333333333333333*(v1*v0)))+(((-0.25*(v1*v1))+(-0.25*(v0*v2))+(17.5*v4)+(0.25*(v2*v0)))+((-0.5*(v3*v0))+(0.5833333333333334*(v2*v1))+(0.5*(v0*v3))+(-70.0*v5)+(0.4166666666666667*(v1*v2)))*h)*h), (((1.25*(v3*v0))+(-0.010416666666666666*(v1*v0*v0))+(-1.375*(v2*v1))+(-1.25*(v0*v3))+(-0.010416666666666666*(v0*v0*v1))+(0.020833333333333332*(v0*v1*v0))+(157.5*v5)+(-1.125*(v1*v2)))+(((0.052083333333333336*(v1*v1*v0))+(7.5*(v2*v2))+(-0.010416666666666666*(v0*v1*v1))+(6.875*(v1*v3))+(0.03125*(v2*v0*v0))+(8.75*(v0*v4))+(9.375*(v3*v1))+(-0.041666666666666664*(v1*v0*v1))+(-8.75*(v4*v0))+(0.03125*(v0*v0*v2))+(-1732.5*v6)+(-0.0625*(v0*v2*v0)))+((-0.010416666666666666*(v1*v2*v0))+(0.14583333333333334*(v2*v0*v1))+(-0.0625*(v0*v0*v3))+(35.0*(v5*v0))+(0.020833333333333332*(v1*v0*v2))+(-0.0625*(v3*v0*v0))+(0.052083333333333336*(v0*v2*v1))+(-26.375*(v2*v3))+(-38.125*(v4*v1))+(-0.010416666666666666*(v0*v1*v2))+(0.0625*(v1*v1*v1))+(-25.625*(v1*v4))+(-0.19791666666666666*(v2*v1*v0))+(-29.875*(v3*v2))+(-35.0*(v0*v5))+(0.125*(v0*v3*v0)))*h)*h), (((0.20833333333333334*(v1*v1))+(0.125*(v0*v2))+(-17.5*v4)+(-0.125*(v2*v0)))+(((0.625*(v3*v0))+(-0.020833333333333332*(v1*v0*v0))+(-1.25*(v2*v1))+(-0.625*(v0*v3))+(-0.020833333333333332*(v0*v0*v1))+(0.041666666666666664*(v0*v1*v0))+(157.5*v5)+(-0.75*(v1*v2)))+((-0.041666666666666664*(v1*v1*v0))+(2.625*(v2*v2))+(0.020833333333333332*(v0*v1*v1))+(1.875*(v1*v3))+(0.020833333333333332*(v2*v0*v0))+(1.875*(v0*v4))+(4.375*(v3*v1))+(0.020833333333333332*(v1*v0*v1))+(-1.875*(v4*v0))+(0.020833333333333332*(v0*v0*v2))+(-787.5*v6)+(-0.041666666666666664*(v0*v2*v0)))*h)*h)}; 
    t[3][7] = {(((v3*v0)+(0.010416666666666666*(v1*v0*v0))+(-0.825*(v2*v1))+(-1.0*(v0*v3))+(0.010416666666666666*(v0*v0*v1))+(-0.020833333333333332*(v0*v1*v0))+(94.5*v5)+(-0.675*(v1*v2)))+((0.07291666666666667*(v1*v1*v0))+(4.5*(v2*v2))+(-0.03125*(v0*v1*v1))+(4.375*(v1*v3))+(-0.03125*(v2*v0*v0))+(7.0*(v0*v4))+(5.625*(v3*v1))+(-0.041666666666666664*(v1*v0*v1))+(-7.0*(v4*v0))+(-0.03125*(v0*v0*v2))+(-1039.5*v6)+(0.0625*(v0*v2*v0)))*h), (((0.125*(v1*v1))+(0.125*(v0*v2))+(-10.5*v4)+(-0.125*(v2*v0)))+((0.625*(v3*v0))+(-0.75*(v2*v1))+(-0.625*(v0*v3))+(94.5*v5)+(-0.5*(v1*v2)))*h), (((0.0125*(v1*v1*v0))+(-4.275*(v2*v2))+(-0.008333333333333333*(v0*v1*v1))+(-3.5*(v1*v3))+(-0.01875*(v2*v0*v0))+(-4.375*(v0*v4))+(-6.0*(v3*v1))+(-0.004166666666666667*(v1*v0*v1))+(4.375*(v4*v0))+(-0.01875*(v0*v0*v2))+(1039.5*v6)+(0.0375*(v0*v2*v0)))+((-0.075*(v1*v2*v0))+(-0.15625*(v2*v0*v1))+(0.09375*(v0*v0*v3))+(-39.375*(v5*v0))+(0.03125*(v1*v0*v2))+(0.09375*(v3*v0*v0))+(-0.0125*(v0*v2*v1))+(31.875*(v2*v3))+(55.125*(v4*v1))+(0.04375*(v0*v1*v2))+(0.0078125*(v0*v0*v1*v0))+(0.0026041666666666665*(v1*v0*v0*v0))+(-0.041666666666666664*(v1*v1*v1))+(28.875*(v1*v4))+(0.16875*(v2*v1*v0))+(39.375*(v3*v2))+(39.375*(v0*v5))+(-0.0026041666666666665*(v0*v0*v0*v1))+(-0.1875*(v0*v3*v0))+(-0.0078125*(v0*v1*v0*v0)))*h), (((-0.25*(v3*v0))+(0.010416666666666666*(v1*v0*v0))+(0.675*(v2*v1))+(0.25*(v0*v3))+(0.010416666666666666*(v0*v0*v1))+(-0.020833333333333332*(v0*v1*v0))+(-94.5*v5)+(0.325*(v1*v2)))+((0.07291666666666667*(v1*v1*v0))+(-3.0*(v2*v2))+(-0.03125*(v0*v1*v1))+(-1.875*(v1*v3))+(-0.03125*(v2*v0*v0))+(-1.75*(v0*v4))+(-5.625*(v3*v1))+(-0.041666666666666664*(v1*v0*v1))+(1.75*(v4*v0))+(-0.03125*(v0*v0*v2))+(1039.5*v6)+(0.0625*(v0*v2*v0)))*h)}; 
    t[3][8] = {((-0.029166666666666667*(v1*v1*v0))+(-1.9*(v2*v2))+(0.0125*(v0*v1*v1))+(-1.8333333333333333*(v1*v3))+(0.0125*(v2*v0*v0))+(-2.9166666666666665*(v0*v4))+(-2.6666666666666665*(v3*v1))+(0.016666666666666666*(v1*v0*v1))+(2.9166666666666665*(v4*v0))+(0.0125*(v0*v0*v2))+(462.0*v6)+(-0.025*(v0*v2*v0))), ((-0.25*(v3*v0))+(0.3*(v2*v1))+(0.25*(v0*v3))+(-42.0*v5)+(0.2*(v1*v2))), ((0.0375*(v1*v2*v0))+(0.016666666666666666*(v2*v0*v1))+(-0.041666666666666664*(v0*v0*v3))+(15.75*(v5*v0))+(-0.016666666666666666*(v1*v0*v2))+(-0.041666666666666664*(v3*v0*v0))+(-0.0125*(v0*v2*v1))+(-13.0*(v2*v3))+(-25.666666666666668*(v4*v1))+(-0.020833333333333332*(v0*v1*v2))+(-0.003125*(v0*v0*v1*v0))+(-0.0010416666666666667*(v1*v0*v0*v0))+(-11.666666666666666*(v1*v4))+(-0.004166666666666667*(v2*v1*v0))+(-17.0*(v3*v2))+(-15.75*(v0*v5))+(0.0010416666666666667*(v0*v0*v0*v1))+(0.08333333333333333*(v0*v3*v0))+(0.003125*(v0*v1*v0*v0))), ((-0.029166666666666667*(v1*v1*v0))+(1.1*(v2*v2))+(0.0125*(v0*v1*v1))+(0.6666666666666666*(v1*v3))+(0.0125*(v2*v0*v0))+(0.5833333333333334*(v0*v4))+(2.3333333333333335*(v3*v1))+(0.016666666666666666*(v1*v0*v1))+(-0.5833333333333334*(v4*v0))+(0.0125*(v0*v0*v2))+(-462.0*v6)+(-0.025*(v0*v2*v0)))}; 
    t[4][0] = {zero, zero, zero, zero}; 
    t[4][1] = {zero, zero, zero, zero}; 
    t[4][2] = {zero, zero, zero, zero}; 
    t[4][3] = {zero, zero, zero, zero}; 
    t[4][4] = {zero, zero, zero, zero}; 
    t[4][5] = {zero, zero, zero, zero}; 
    t[4][6] = {zero, zero, zero, zero}; 
    t[4][7] = {(((-0.75*(v3*v0))+(-0.03125*(v1*v0*v0))+(0.275*(v2*v1))+(0.75*(v0*v3))+(-0.03125*(v0*v0*v1))+(0.0625*(v0*v1*v0))+(-31.5*v5)+(0.225*(v1*v2)))+((-0.09375*(v1*v1*v0))+(-1.5*(v2*v2))+(0.052083333333333336*(v0*v1*v1))+(-1.875*(v1*v3))+(0.09375*(v2*v0*v0))+(-5.25*(v0*v4))+(-1.875*(v3*v1))+(0.041666666666666664*(v1*v0*v1))+(5.25*(v4*v0))+(0.09375*(v0*v0*v2))+(346.5*v6)+(-0.1875*(v0*v2*v0)))*h), (((-0.041666666666666664*(v1*v1))+(-0.125*(v0*v2))+(3.5*v4)+(0.125*(v2*v0)))+((-0.625*(v3*v0))+(-0.020833333333333332*(v1*v0*v0))+(0.25*(v2*v1))+(0.625*(v0*v3))+(-0.020833333333333332*(v0*v0*v1))+(0.041666666666666664*(v0*v1*v0))+(-31.5*v5)+(0.25*(v1*v2)))*h), (((0.1*(v1*v1*v0))+(1.425*(v2*v2))+(-0.04583333333333333*(v0*v1*v1))+(2.0*(v1*v3))+(-0.05625*(v2*v0*v0))+(4.375*(v0*v4))+(2.0*(v3*v1))+(-0.05416666666666667*(v1*v0*v1))+(-4.375*(v4*v0))+(-0.05625*(v0*v0*v2))+(-346.5*v6)+(0.1125*(v0*v2*v0)))+((-0.4125*(v1*v2*v0))+(0.21875*(v2*v0*v1))+(0.28125*(v0*v0*v3))+(39.375*(v5*v0))+(0.21875*(v1*v0*v2))+(0.28125*(v3*v0*v0))+(0.2125*(v0*v2*v1))+(-13.125*(v2*v3))+(-18.375*(v4*v1))+(0.19375*(v0*v1*v2))+(0.0078125*(v0*v0*v1*v0))+(0.0026041666666666665*(v1*v0*v0*v0))+(0.14583333333333334*(v1*v1*v1))+(-18.375*(v1*v4))+(-0.43125*(v2*v1*v0))+(-13.125*(v3*v2))+(-39.375*(v0*v5))+(-0.0026041666666666665*(v0*v0*v0*v1))+(-0.5625*(v0*v3*v0))+(-0.0078125*(v0*v1*v0*v0)))*h), (((0.5*(v3*v0))+(0.010416666666666666*(v1*v0*v0))+(-0.225*(v2*v1))+(-0.5*(v0*v3))+(0.010416666666666666*(v0*v0*v1))+(-0.020833333333333332*(v0*v1*v0))+(31.5*v5)+(-0.275*(v1*v2)))+((0.07291666666666667*(v1*v1*v0))+(1.5*(v2*v2))+(-0.03125*(v0*v1*v1))+(1.875*(v1*v3))+(-0.03125*(v2*v0*v0))+(3.5*(v0*v4))+(1.875*(v3*v1))+(-0.041666666666666664*(v1*v0*v1))+(-3.5*(v4*v0))+(-0.03125*(v0*v0*v2))+(-346.5*v6)+(0.0625*(v0*v2*v0)))*h)}; 
    t[4][8] = {((0.1*(v1*v1*v0))+(1.425*(v2*v2))+(-0.04583333333333333*(v0*v1*v1))+(2.0*(v1*v3))+(-0.05625*(v2*v0*v0))+(4.375*(v0*v4))+(2.0*(v3*v1))+(-0.05416666666666667*(v1*v0*v1))+(-4.375*(v4*v0))+(-0.05625*(v0*v0*v2))+(-346.5*v6)+(0.1125*(v0*v2*v0))), ((0.5*(v3*v0))+(0.010416666666666666*(v1*v0*v0))+(-0.225*(v2*v1))+(-0.5*(v0*v3))+(0.010416666666666666*(v0*v0*v1))+(-0.020833333333333332*(v0*v1*v0))+(31.5*v5)+(-0.275*(v1*v2))), ((0.3*(v1*v2*v0))+(-0.2625*(v2*v0*v1))+(-0.125*(v0*v0*v3))+(-31.5*(v5*v0))+(-0.175*(v1*v0*v2))+(-0.125*(v3*v0*v0))+(-0.1625*(v0*v2*v1))+(13.5*(v2*v3))+(19.25*(v4*v1))+(-0.125*(v0*v1*v2))+(0.00625*(v0*v0*v1*v0))+(0.0020833333333333333*(v1*v0*v0*v0))+(-0.14583333333333334*(v1*v1*v1))+(17.5*(v1*v4))+(0.425*(v2*v1*v0))+(12.75*(v3*v2))+(31.5*(v0*v5))+(-0.0020833333333333333*(v0*v0*v0*v1))+(0.25*(v0*v3*v0))+(-0.00625*(v0*v1*v0*v0))), ((-0.04583333333333333*(v1*v1*v0))+(-1.575*(v2*v2))+(0.016666666666666666*(v0*v1*v1))+(-1.75*(v1*v3))+(0.00625*(v2*v0*v0))+(-2.625*(v0*v4))+(-1.75*(v3*v1))+(0.029166666666666667*(v1*v0*v1))+(2.625*(v4*v0))+(0.00625*(v0*v0*v2))+(346.5*v6)+(-0.0125*(v0*v2*v0)))}; 
}
