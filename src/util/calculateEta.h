#ifndef SCHRODINGER_CALCULATE_ETA_H
#define SCHRODINGER_CALCULATE_ETA_H

#include <Eigen/Dense>


using namespace Eigen;

double *calculateEta(double Z, int etaCount);

MatrixXd *calculateEta(const VectorXd &Z, int n, int etaCount);

#endif