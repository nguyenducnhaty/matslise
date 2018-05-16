//
// Created by toon on 5/16/18.
//

#include "matslise.h"
#include "legendre.h"
#include "Array2D.h"
#include <array>
#include <cublas_v2.h>

#define EPS (1.e-12)


Matslise::Matslise(double (*V)(double), double xmin, double xmax, int sectorCount) : V(V), sectorCount(sectorCount) {
    sectors = new Sector *[sectorCount];
    double h = (xmax - xmin) / sectorCount;
    for (int i = 0; i < sectorCount; ++i)
        sectors[i] = new Sector(this, xmin + i * h, xmin + (i + 1) * h);
}

Matslise::Y Matslise::propagate(double E, Matslise::Y y, double a, double b) {
    if (a < b) {
        for (int i = 0; i < sectorCount; ++i) {
            Sector *sector = sectors[i];
            if (sector->xmax > a) {
                if (sector->xmin < a) // eerste
                    y = sector->calculateT(E, a - sector->xmin) / y;

                if (sector->xmax > b) { // laatste
                    y = sector->calculateT(E, b - sector->xmin) * y;
                    break;
                }

                y = sector->calculateT(E) * y;
            }
        }
        return y;
    }
}

Matslise::Sector::Sector(Matslise *s, double xmin, double xmax) : s(s), xmin(xmin), xmax(xmax) {
    h = xmax - xmin;
    vs = legendre::getCoefficients(5, s->V, xmin, xmax);

    calculateTCoeffs();
}

void Matslise::Sector::calculateTCoeffs() {
    double v1 = vs[1],
            v2 = vs[2],
            v3 = vs[3],
            v4 = vs[4],
            v5 = vs[5];

    // @formatter:off
    u = {1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,   0.0, 0.0, (((((((h*h*h*h*h)*v5)*-0.5)+(((h*h*h*h)*v4)*0.5))+(((h*h*h)*v3)*-0.5))+(((h*h)*v2)*0.5))+((h*v1)*-0.5)), (((((((h*h*h*h)*v5)*7.5)+(((h*h*h)*v4)*-5.0))+(((h*h)*v3)*3.0))+((h*v2)*-1.5))+(v1*0.5)), ((((((h*h*h)*v5)*-35.0)+(((h*h)*v4)*15.0))+((h*v3)*-5.0))+v2), (((((h*h)*v5)*70.0)+((h*v4)*-17.5))+(v3*2.5)), (((h*v5)*-63.0)+(v4*7.0)), (v5*21.0),   0.0, 0.0, 0.0, (((((((h*h*h*h)*v5)*-7.5)+(((h*h*h)*v4)*5.0))+(((h*h)*v3)*-3.0))+((h*v2)*1.5))+(v1*-0.5)), (((((((((((v3*v4)+(v2*v5))*(h*h*h*h*h*h*h))*-0.25)+(((((v3*v3)+((v2*v4)*2.0))+((v1*v5)*2.0))*(h*h*h*h*h*h))*0.125))+((((v2*v3)+(v1*v4))*(h*h*h*h*h))*-0.25))+((((v2*v2)+((v1*v3)*2.0))*(h*h*h*h))*0.125))+((((v1*v2)+(v5*-210.0))*(h*h*h))*-0.25))+((((v1*v1)+(v4*-180.0))*(h*h))*0.125))+((h*v3)*7.5))+(v2*-1.5)), (((((((((((v3*v4)*8.0)+((v2*v5)*9.0))*(h*h*h*h*h*h))*0.5)+((((((v3*v3)*6.0)+((v2*v4)*13.0))+((v1*v5)*16.0))*(h*h*h*h*h))*-0.25))+(((((v2*v3)*9.0)+((v1*v4)*11.0))*(h*h*h*h))*0.25))+(((((v2*v2)*3.0)+((v1*v3)*7.0))*(h*h*h))*-0.25))+(((v1*v2)+(v5*-140.0))*(h*h)))+((((v1*v1)+(v4*-140.0))*h)*-0.25))+(v3*-5.0)), (((((((((((v3*v4)*100.0)+((v2*v5)*117.0))*(h*h*h*h*h))*-0.25)+((((((v3*v3)*28.0)+((v2*v4)*62.0))+((v1*v5)*85.0))*(h*h*h*h))*0.25))+(((((v2*v3)*3.0)+((v1*v4)*4.0))*(h*h*h))*-2.5))+(((((v2*v2)*13.0)+((v1*v3)*32.0))*(h*h))*0.125))+((((v1*v2)+(v5*-126.0))*h)*-1.25))+((v1*v1)*0.125))+(v4*-17.5)), ((((((((((v3*v4)*16.0)+((v2*v5)*19.0))*(h*h*h*h))*5.0)+((((((v3*v3)*13.0)+((v2*v4)*29.0))+((v1*v5)*42.0))*(h*h*h))*-1.25))+(((((v2*v3)*47.0)+((v1*v4)*65.0))*(h*h))*0.25))+(((((v2*v2)*2.0)+((v1*v3)*5.0))*h)*-0.75))+((v1*v2)*0.5))+(v5*-63.0)),   0.0, 0.0, 0.0, 0.0, 0.0, (((((((((((v3*v4)*8.0)+((v2*v5)*9.0))*(h*h*h*h*h*h))*-0.5)+((((((v3*v3)*6.0)+((v2*v4)*13.0))+((v1*v5)*16.0))*(h*h*h*h*h))*0.25))+(((((v2*v3)*9.0)+((v1*v4)*11.0))*(h*h*h*h))*-0.25))+(((((v2*v2)*3.0)+((v1*v3)*7.0))*(h*h*h))*0.25))+((((v1*v2)+(v5*-70.0))*(h*h))*-1.0))+((((v1*v1)+(v4*-70.0))*h)*0.25))+(v3*2.5)), ((((((((((((v1*(v2*v2))+((v1*v1)*v3))+((v3*v4)*-800.0))+((v2*v5)*-852.0))*(h*h*h*h*h))*-0.0625)+(((((((v1*v1)*v2)+((v3*v3)*-228.0))+((v2*v4)*-472.0))+((v1*v5)*-560.0))*(h*h*h*h))*0.0625))+(((((v1*v1*v1)+((v2*v3)*-720.0))+((v1*v4)*-820.0))*(h*h*h))*-0.0208333333333))+(((((v2*v2)*27.0)+((v1*v3)*58.0))*(h*h))*-0.125))+((((v1*v2)+(v5*-63.0))*h)*2.5))+((v1*v1)*-0.291666666667))+(v4*17.5)), ((((((((((((v1*(v2*v2))*7.0)+(((v1*v1)*v3)*8.0))+((v3*v4)*-3680.0))+((v2*v5)*-4000.0))*(h*h*h*h))*0.0625)+(((((((v1*v1)*v2)+((v3*v3)*-152.0))+((v2*v4)*-320.0))+((v1*v5)*-392.0))*(h*h*h))*-0.3125))+(((((v1*v1*v1)+((v2*v3)*-544.0))+((v1*v4)*-640.0))*(h*h))*0.0625))+(((((v2*v2)*9.0)+((v1*v3)*20.0))*h)*0.5))+((v1*v2)*-1.5))+(v5*94.5)),   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, ((((((((((((v1*(v2*v2))*7.0)+(((v1*v1)*v3)*8.0))+((v3*v4)*-1280.0))+((v2*v5)*-1520.0))*(h*h*h*h))*-0.0625)+(((((((v1*v1)*v2)+((v3*v3)*-52.0))+((v2*v4)*-116.0))+((v1*v5)*-168.0))*(h*h*h))*0.3125))+(((((v1*v1*v1)+((v2*v3)*-188.0))+((v1*v4)*-260.0))*(h*h))*-0.0625))+(((((v2*v2)*2.0)+((v1*v3)*5.0))*h)*-0.75))+((v1*v2)*0.5))+(v5*-31.5))};
    up = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,   0.0, (((((((h*h*h*h*h)*v5)*-0.5)+(((h*h*h*h)*v4)*0.5))+(((h*h*h)*v3)*-0.5))+(((h*h)*v2)*0.5))+((h*v1)*-0.5)), (((((((h*h*h*h)*v5)*7.5)+(((h*h*h)*v4)*-5.0))+(((h*h)*v3)*3.0))+((h*v2)*-1.5))+(v1*0.5)), (((((((((((v3*v4)+(v2*v5))*(h*h*h*h*h*h*h))*-0.25)+(((((v3*v3)+((v2*v4)*2.0))+((v1*v5)*2.0))*(h*h*h*h*h*h))*0.125))+((((v2*v3)+(v1*v4))*(h*h*h*h*h))*-0.25))+((((v2*v2)+((v1*v3)*2.0))*(h*h*h*h))*0.125))+((((v1*v2)+(v5*210.0))*(h*h*h))*-0.25))+((((v1*v1)+(v4*180.0))*(h*h))*0.125))+((h*v3)*-7.5))+(v2*1.5)), (((((((((((v3*v4)*8.0)+((v2*v5)*9.0))*(h*h*h*h*h*h))*0.5)+((((((v3*v3)*6.0)+((v2*v4)*13.0))+((v1*v5)*16.0))*(h*h*h*h*h))*-0.25))+(((((v2*v3)*9.0)+((v1*v4)*11.0))*(h*h*h*h))*0.25))+(((((v2*v2)*3.0)+((v1*v3)*7.0))*(h*h*h))*-0.25))+(((v1*v2)+(v5*140.0))*(h*h)))+((((v1*v1)+(v4*140.0))*h)*-0.25))+(v3*5.0)), (((((((((((v3*v4)*100.0)+((v2*v5)*117.0))*(h*h*h*h*h))*-0.25)+((((((v3*v3)*28.0)+((v2*v4)*62.0))+((v1*v5)*85.0))*(h*h*h*h))*0.25))+(((((v2*v3)*3.0)+((v1*v4)*4.0))*(h*h*h))*-2.5))+(((((v2*v2)*13.0)+((v1*v3)*32.0))*(h*h))*0.125))+((((v1*v2)+(v5*126.0))*h)*-1.25))+((v1*v1)*0.125))+(v4*17.5)), ((((((((((v3*v4)*16.0)+((v2*v5)*19.0))*(h*h*h*h))*5.0)+((((((v3*v3)*13.0)+((v2*v4)*29.0))+((v1*v5)*42.0))*(h*h*h))*-1.25))+(((((v2*v3)*47.0)+((v1*v4)*65.0))*(h*h))*0.25))+(((((v2*v2)*2.0)+((v1*v3)*5.0))*h)*-0.75))+((v1*v2)*0.5))+(v5*63.0)), (((((((((v3*v4)*41.0)+((v2*v5)*49.0))*(h*h*h))*-3.5)+((((((v3*v3)*80.0)+((v2*v4)*179.0))+((v1*v5)*266.0))*(h*h))*0.25))+(((((v2*v3)*5.0)+((v1*v4)*7.0))*h)*-1.75))+((v2*v2)*0.5))+((v1*v3)*1.25)),   0.0, 0.0, 0.0, (((((((((((v3*v4)+(v2*v5))*(h*h*h*h*h*h*h))*-0.25)+(((((v3*v3)+((v2*v4)*2.0))+((v1*v5)*2.0))*(h*h*h*h*h*h))*0.125))+((((v2*v3)+(v1*v4))*(h*h*h*h*h))*-0.25))+((((v2*v2)+((v1*v3)*2.0))*(h*h*h*h))*0.125))+((((v1*v2)+(v5*-210.0))*(h*h*h))*-0.25))+((((v1*v1)+(v4*-180.0))*(h*h))*0.125))+((h*v3)*7.5))+(v2*-1.5)), (((((((((((v3*v4)*8.0)+((v2*v5)*9.0))*(h*h*h*h*h*h))*0.5)+((((((v3*v3)*6.0)+((v2*v4)*13.0))+((v1*v5)*16.0))*(h*h*h*h*h))*-0.25))+(((((v2*v3)*9.0)+((v1*v4)*11.0))*(h*h*h*h))*0.25))+(((((v2*v2)*3.0)+((v1*v3)*7.0))*(h*h*h))*-0.25))+(((v1*v2)+(v5*-210.0))*(h*h)))+((((v1*v1)+(v4*-210.0))*h)*-0.25))+(v3*-7.5)), ((((((((((((v1*(v2*v2))+((v1*v1)*v3))+((v3*v4)*400.0))+((v2*v5)*552.0))*(h*h*h*h*h))*-0.0625)+(((((((v1*v1)*v2)+((v3*v3)*108.0))+((v2*v4)*272.0))+((v1*v5)*460.0))*(h*h*h*h))*0.0625))+(((((v1*v1*v1)+((v2*v3)*360.0))+((v1*v4)*620.0))*(h*h*h))*-0.0208333333333))+(((((v2*v2)*6.0)+((v1*v3)*19.0))*(h*h))*0.25))+((((v1*v2)+(v5*-252.0))*h)*-1.25))+((v1*v1)*0.0833333333333))+(v4*-35.0)), ((((((((((((v1*(v2*v2))*7.0)+(((v1*v1)*v3)*8.0))+((v3*v4)*1440.0))+((v2*v5)*2080.0))*(h*h*h*h))*0.0625)+(((((((v1*v1)*v2)+((v3*v3)*56.0))+((v2*v4)*144.0))+((v1*v5)*280.0))*(h*h*h))*-0.3125))+(((((v1*v1*v1)+((v2*v3)*208.0))+((v1*v4)*400.0))*(h*h))*0.0625))+(((((v2*v2)*3.0)+((v1*v3)*10.0))*h)*-0.5))+((v1*v2)*0.5))+(v5*-157.5)), (((((((((((v1*(v2*v2))*19.0)+(((v1*v1)*v3)*23.0))+((v3*v4)*3036.0))+((v2*v5)*4284.0))*(h*h*h))*-0.0625)+((((((((v1*v1)*v2)*9.0)+((v3*v3)*408.0))+((v2*v4)*1016.0))+((v1*v5)*2044.0))*(h*h))*0.0625))+(((((v1*v1*v1)+((v2*v3)*180.0))+((v1*v4)*336.0))*h)*-0.0625))+((v2*v2)*0.6))+((v1*v3)*1.75)),   0.0, 0.0, 0.0, 0.0, 0.0, ((((((((((((v1*(v2*v2))+((v1*v1)*v3))+((v3*v4)*-800.0))+((v2*v5)*-852.0))*(h*h*h*h*h))*-0.0625)+(((((((v1*v1)*v2)+((v3*v3)*-228.0))+((v2*v4)*-472.0))+((v1*v5)*-560.0))*(h*h*h*h))*0.0625))+(((((v1*v1*v1)+((v2*v3)*-720.0))+((v1*v4)*-820.0))*(h*h*h))*-0.0208333333333))+(((((v2*v2)*27.0)+((v1*v3)*58.0))*(h*h))*-0.125))+((((v1*v2)+(v5*-63.0))*h)*2.5))+((v1*v1)*-0.291666666667))+(v4*17.5)), ((((((((((((v1*(v2*v2))*7.0)+(((v1*v1)*v3)*8.0))+((v3*v4)*-6080.0))+((v2*v5)*-6480.0))*(h*h*h*h))*0.0625)+(((((((v1*v1)*v2)+((v3*v3)*-252.0))+((v2*v4)*-524.0))+((v1*v5)*-616.0))*(h*h*h))*-0.3125))+(((((v1*v1*v1)+((v2*v3)*-900.0))+((v1*v4)*-1020.0))*(h*h))*0.0625))+(((((v2*v2)*6.0)+((v1*v3)*13.0))*h)*1.25))+((v1*v2)*-2.5))+(v5*157.5)), (((((((((((v1*(v2*v2))*48.0)+(((v1*v1)*v3)*71.0))+((v3*v4)*-55332.0))+((v2*v5)*-60228.0))*(h*h*h))*-0.0208333333333)+((((((((v1*v1)*v2)*23.0)+((v3*v3)*-7866.0))+((v2*v4)*-16572.0))+((v1*v5)*-20328.0))*(h*h))*0.0208333333333))+(((((v1*v1*v1)+((v2*v3)*-1710.0))+((v1*v4)*-2016.0))*h)*-0.0416666666667))+((v2*v2)*-4.275))+((v1*v3)*-9.5)),   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, (((((((((((v1*(v2*v2))*123.0)+(((v1*v1)*v3)*136.0))+((v3*v4)*-20664.0))+((v2*v5)*-24696.0))*(h*h*h))*0.0208333333333)+((((((((v1*v1)*v2)*29.0)+((v3*v3)*-1431.0))+((v2*v4)*-3252.0))+((v1*v5)*-4578.0))*(h*h))*-0.0416666666667))+(((((v1*v1*v1)+((v2*v3)*-180.0))+((v1*v4)*-252.0))*h)*0.145833333333))+((v2*v2)*1.425))+((v1*v3)*4.0))};
    v = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,   0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,   0.0, 0.0, 0.0, (((((((h*h*h*h*h)*v5)*-0.5)+(((h*h*h*h)*v4)*0.5))+(((h*h*h)*v3)*-0.5))+(((h*h)*v2)*0.5))+((h*v1)*-0.5)), (((((((h*h*h*h)*v5)*7.5)+(((h*h*h)*v4)*-5.0))+(((h*h)*v3)*3.0))+((h*v2)*-1.5))+(v1*0.5)), ((((((h*h*h)*v5)*-35.0)+(((h*h)*v4)*15.0))+((h*v3)*-5.0))+v2), (((((h*h)*v5)*70.0)+((h*v4)*-17.5))+(v3*2.5)), (((h*v5)*-63.0)+(v4*7.0)),   0.0, 0.0, 0.0, 0.0, 0.0, (((((((((((v3*v3)+((v2*v4)*2.0))+((v1*v5)*2.0))*(h*h*h*h*h*h))*0.125)+((((v2*v3)+(v1*v4))*(h*h*h*h*h))*-0.25))+((((v2*v2)+((v1*v3)*2.0))*(h*h*h*h))*0.125))+((((v1*v2)+(v5*-70.0))*(h*h*h))*-0.25))+((((v1*v1)+(v4*-60.0))*(h*h))*0.125))+((h*v3)*2.5))+(v2*-0.5)), (((((((((((v3*v3)*6.0)+((v2*v4)*13.0))+((v1*v5)*16.0))*(h*h*h*h*h))*-0.25)+(((((v2*v3)*9.0)+((v1*v4)*11.0))*(h*h*h*h))*0.25))+(((((v2*v2)*3.0)+((v1*v3)*7.0))*(h*h*h))*-0.25))+(((v1*v2)+(v5*-70.0))*(h*h)))+((((v1*v1)+(v4*-70.0))*h)*-0.25))+(v3*-2.5)), (((((((((((v3*v3)*28.0)+((v2*v4)*62.0))+((v1*v5)*85.0))*(h*h*h*h))*0.25)+(((((v2*v3)*3.0)+((v1*v4)*4.0))*(h*h*h))*-2.5))+(((((v2*v2)*13.0)+((v1*v3)*32.0))*(h*h))*0.125))+(((((v1*v2)*5.0)+(v5*-378.0))*h)*-0.25))+((v1*v1)*0.125))+(v4*-10.5)),   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, ((((((((((((v1*v1)*v2)+((v3*v3)*-44.0))+((v2*v4)*-104.0))+((v1*v5)*-160.0))*(h*h*h*h))*0.0625)+(((((v1*v1*v1)+((v2*v3)*-144.0))+((v1*v4)*-220.0))*(h*h*h))*-0.0208333333333))+(((((v2*v2)*5.0)+((v1*v3)*14.0))*(h*h))*-0.125))+((((v1*v2)+(v5*-63.0))*h)*0.5))+((v1*v1)*-0.0416666666667))+(v4*3.5))};
    vp = {1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,   0.0, 0.0, (((((((h*h*h*h*h)*v5)*-0.5)+(((h*h*h*h)*v4)*0.5))+(((h*h*h)*v3)*-0.5))+(((h*h)*v2)*0.5))+((h*v1)*-0.5)), (((((((h*h*h*h)*v5)*7.5)+(((h*h*h)*v4)*-5.0))+(((h*h)*v3)*3.0))+((h*v2)*-1.5))+(v1*0.5)), ((((((h*h*h)*v5)*-35.0)+(((h*h)*v4)*15.0))+((h*v3)*-5.0))+v2), (((((h*h)*v5)*70.0)+((h*v4)*-17.5))+(v3*2.5)), (((h*v5)*-63.0)+(v4*7.0)), (v5*21.0),   0.0, 0.0, 0.0, (((((((h*h*h*h)*v5)*7.5)+(((h*h*h)*v4)*-5.0))+(((h*h)*v3)*3.0))+((h*v2)*-1.5))+(v1*0.5)), (((((((((((v3*v3)+((v2*v4)*2.0))+((v1*v5)*2.0))*(h*h*h*h*h*h))*0.125)+((((v2*v3)+(v1*v4))*(h*h*h*h*h))*-0.25))+((((v2*v2)+((v1*v3)*2.0))*(h*h*h*h))*0.125))+((((v1*v2)+(v5*210.0))*(h*h*h))*-0.25))+((((v1*v1)+(v4*180.0))*(h*h))*0.125))+((h*v3)*-7.5))+(v2*1.5)), (((((((((((v3*v3)*6.0)+((v2*v4)*13.0))+((v1*v5)*16.0))*(h*h*h*h*h))*-0.25)+(((((v2*v3)*9.0)+((v1*v4)*11.0))*(h*h*h*h))*0.25))+(((((v2*v2)*3.0)+((v1*v3)*7.0))*(h*h*h))*-0.25))+(((v1*v2)+(v5*140.0))*(h*h)))+((((v1*v1)+(v4*140.0))*h)*-0.25))+(v3*5.0)), (((((((((((v3*v3)*28.0)+((v2*v4)*62.0))+((v1*v5)*85.0))*(h*h*h*h))*0.25)+(((((v2*v3)*3.0)+((v1*v4)*4.0))*(h*h*h))*-2.5))+(((((v2*v2)*13.0)+((v1*v3)*32.0))*(h*h))*0.125))+((((v1*v2)+(v5*126.0))*h)*-1.25))+((v1*v1)*0.125))+(v4*17.5)), ((((((((((v3*v3)*13.0)+((v2*v4)*29.0))+((v1*v5)*42.0))*(h*h*h))*-1.25)+(((((v2*v3)*47.0)+((v1*v4)*65.0))*(h*h))*0.25))+(((((v2*v2)*2.0)+((v1*v3)*5.0))*h)*-0.75))+((v1*v2)*0.5))+(v5*63.0)),   0.0, 0.0, 0.0, 0.0, 0.0, (((((((((((v3*v3)*6.0)+((v2*v4)*13.0))+((v1*v5)*16.0))*(h*h*h*h*h))*-0.25)+(((((v2*v3)*9.0)+((v1*v4)*11.0))*(h*h*h*h))*0.25))+(((((v2*v2)*3.0)+((v1*v3)*7.0))*(h*h*h))*-0.25))+(((v1*v2)+(v5*-70.0))*(h*h)))+((((v1*v1)+(v4*-70.0))*h)*-0.25))+(v3*-2.5)), ((((((((((((v1*v1)*v2)+((v3*v3)*180.0))+((v2*v4)*392.0))+((v1*v5)*520.0))*(h*h*h*h))*0.0625)+(((((v1*v1*v1)+((v2*v3)*576.0))+((v1*v4)*740.0))*(h*h*h))*-0.0208333333333))+(((((v2*v2)*21.0)+((v1*v3)*50.0))*(h*h))*0.125))+(((((v1*v2)*4.0)+(v5*-315.0))*h)*-0.5))+((v1*v1)*0.208333333333))+(v4*-17.5)), (((((((((((v1*v1)*v2)+((v3*v3)*104.0))+((v2*v4)*232.0))+((v1*v5)*336.0))*(h*h*h))*-0.3125)+(((((v1*v1*v1)+((v2*v3)*376.0))+((v1*v4)*520.0))*(h*h))*0.0625))+(((((v2*v2)*2.0)+((v1*v3)*5.0))*h)*-1.5))+(v1*v2))+(v5*-94.5)),   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, (((((((((((v1*v1)*v2)+((v3*v3)*-52.0))+((v2*v4)*-116.0))+((v1*v5)*-168.0))*(h*h*h))*-0.3125)+(((((v1*v1*v1)+((v2*v3)*-188.0))+((v1*v4)*-260.0))*(h*h))*0.0625))+(((((v2*v2)*2.0)+((v1*v3)*5.0))*h)*0.75))+((v1*v2)*-0.5))+(v5*31.5))};
    // @formatter:on

    for (int i = 0; i < ETA; ++i) {
        hu[i] = hup[i] = hv[i] = hvp[i] = 0;
        double H = 1;
        for (int j = 0; j < HMAX; ++j, H *= h) {
            hu[i] += H * u[i][j];
            hup[i] += H * up[i][j];
            hv[i] += H * v[i][j];
            hvp[i] += H * vp[i][j];
        }
    }
}

double *calculateEta(double Z) {
    double *eta;
    if (fabs(Z) < 0.5) {
        static double eta9[] = {1.527349308567059e-009, 0.36365459727787e-10, 0.00395276736172e-10,
                                0.00002635178241e-10, 0.00000012199899e-10, 0.00000000042069e-10,
                                0.00000000000113e-10, 0};
        static double eta8[] = {2.901963686277412e-008, 0.76367465428353e-9, 0.00909136493195e-9,
                                0.00006587945603e-9, 0.00000032939728e-9, 0.00000000121999e-9,
                                0.00000000000351e-9, 0.00000000000001e-9};

        double e9 = 0, e8 = 0;
        double z = 1;
        for (int i = 0; i < 8; ++i, z *= Z) {
            e9 += z * eta9[i];
            e8 += z * eta8[i];
        }

        eta = new double[11]{0, 0, 0, 0, 0, 0, 0, 0, 0, e8, e9};
        for (int i = 8; i >= 0; --i)
            eta[i] = Z * eta[i + 2] + (2 * i + 1) * eta[i + 1];
    } else {
        eta = new double[ETA];

        if (Z > 1000) {
            throw "WHAAAA";
            double sZ = sqrt(Z);
            eta[0] = .5 * (1 + exp(-2 * sZ));
            eta[1] = .5 * (1 - exp(-2 * sZ)) / sZ;
        } else if (Z > 0) {
            double sZ = sqrt(Z);
            eta[0] = cosh(sZ);
            eta[1] = sinh(sZ) / sZ;
        } else {
            double sZ = sqrt(-Z);
            eta[0] = cos(sZ);
            eta[1] = sin(sZ) / sZ;
        }

        for (int i = 2; i < ETA; ++i) {
            eta[i] = (eta[i - 2] - (2 * i - 3) * eta[i - 1]) / Z;
        }
    }

    return eta;
}

Matslise::T Matslise::Sector::calculateT(double E, double delta) {
    if (fabs(delta) <= EPS)
        return T(1, 0, 0, 1);
    if (fabs(delta - h) <= EPS)
        return calculateT(E);

    double *eta = calculateEta((vs[0] - E) * delta * delta);
    T t(0, (vs[0] - E) * delta * eta[1], 0, 0);

    for (int i = 0; i < ETA; ++i) {
        double D = 1;
        for (int j = 0; j < HMAX; ++j, D *= delta) {
            t.u += D * eta[j] * u[i][j];
            t.up += D * eta[j] * up[i][j];
            t.v += D * eta[j] * v[i][j];
            t.vp += D * eta[j] * vp[i][j];
        }
    }

    delete eta;
    return t;
}

Matslise::T Matslise::Sector::calculateT(double E) {
    double *eta = calculateEta((vs[0] - E) * h * h);
    T t(0, (vs[0] - E) * h * eta[1], 0, 0);

    for (int i = 0; i < ETA; ++i) {
        t.u += eta[i] * hu[i];
        t.up += eta[i] * hup[i];
        t.v += eta[i] * hv[i];
        t.vp += eta[i] * hvp[i];
    }
    delete eta;
    return t;
}
