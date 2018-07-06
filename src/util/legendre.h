//
// Created by toon on 5/16/18.
//

#ifndef SCHRODINGER_LEGENDRE_H
#define SCHRODINGER_LEGENDRE_H

#include <functional>
#include <iostream>

namespace legendre {
    template<class D>
    D *getCoefficients(int n, std::function<D(double)> V, double a, double b) {
        static double legendrePoints[17][16] = {{1,                         1,                         1,                          1,                         1,                         1,                         1,                          1,                        1,                        1,                          1,                         1,                         1,                         1,                          1,                         1},
                                                {-0.98940093499164993260,   -0.94457502307323257608,   -0.86563120238783174388,    -0.75540440835500303389,   -0.61787624440264374845,   -0.45801677765722738634,   -0.28160355077925891323,    -0.09501250983763744019,  0.09501250983763744019,   0.28160355077925891323,     0.45801677765722738634,    0.61787624440264374845,    0.75540440835500303389,    0.86563120238783174388,     0.94457502307323257608,    0.98940093499164993260},
                                                {0.96837131524352664401,    0.83833296132069678021,    0.62397606782110498211,     0.35595373024325826616,    0.072656580095673325125,   -0.18533094707673489811,   -0.38104916028277002008,    -0.4864589344615292730,   -0.4864589344615292730,   -0.38104916028277002008,    -0.18533094707673489811,   0.072656580095673325125,   0.35595373024325826616,    0.62397606782110498211,     0.83833296132069678021,    0.96837131524352664401},
                                                {-0.93724518454063179248,   -0.69006394508875920580,   -0.32313445482346942601,    0.055454577243060588091,   0.33709620486733831140,    0.44681899040523203034,    0.36657702811464426885,     0.1403744803853127548,    -0.1403744803853127548,   -0.36657702811464426885,    -0.44681899040523203034,   -0.33709620486733831140,   -0.055454577243060588091,  0.32313445482346942601,     0.69006394508875920580,    0.93724518454063179248},
                                                {0.89651622189396953288,    0.51193032100436350722,    0.021519665792279851706,    -0.34027390387996794756,   -0.41898897493693158272,   -0.21914032951000378391,   0.10513543289749142819,     0.3415038703736621037,    0.3415038703736621037,    0.10513543289749142819,     -0.21914032951000378391,   -0.41898897493693158272,   -0.34027390387996794756,   0.021519665792279851706,    0.51193032100436350722,    0.89651622189396953288},
                                                {-0.84682903108622948647,   -0.31835071452324386976,   0.22497699434421564105,     0.41831627087592137521,    0.19631303781038983723,    -0.17678928668573871721,   -0.34655334268168198690,    -0.1707044360264941011,   0.1707044360264941011,    0.34655334268168198690,     0.17678928668573871721,    -0.19631303781038983723,   -0.41831627087592137521,   -0.22497699434421564105,    0.31835071452324386976,    0.84682903108622948647},
                                                {0.78896777950213752937,    0.12468597727601452858,    -0.37496941605383685621,    -0.29576799779490376121,   0.12677934780992021620,    0.33106645018055982315,    0.091303334280325882412,    -0.2548516209813431131,   -0.2548516209813431131,   0.091303334280325882412,    0.33106645018055982315,    0.12677934780992021620,    -0.29576799779490376121,   -0.37496941605383685621,    0.12468597727601452858,    0.78896777950213752937},
                                                {-0.72384239668753330229,   0.054146558418323361508,   0.40996371116288619083,     0.056374316677758896977,   -0.31374564880946707939,   -0.13007230471615555784,   0.24929608505469453035,     0.1912871162931606924,    -0.1912871162931606924,   -0.24929608505469453035,    0.13007230471615555784,    0.31374564880946707939,    -0.056374316677758896977,  -0.40996371116288619083,    -0.054146558418323361508,  0.72384239668753330229},
                                                {0.65247258806545882305,    -0.20499801761774507147,   -0.33729684888284324866,    0.17894935931482821645,    0.25254803913644040099,    -0.17797946040447256532,   -0.21152041014547912253,    0.1889176639487679226,    0.1889176639487679226,    -0.21152041014547912253,    -0.17797946040447256532,   0.25254803913644040099,    0.17894935931482821645,    -0.33729684888284324866,    -0.20499801761774507147,   0.65247258806545882305},
                                                {-0.57596995935581177361,   0.31762662837940415790,    0.18709553525759011055,     -0.30544886963337671053,   -0.015863687413683339403,  0.26959747553010343783,    -0.10908504499225227820,    -0.2039376815800771194,   0.2039376815800771194,    0.10908504499225227820,     -0.26959747553010343783,   0.015863687413683339403,   0.30544886963337671053,    -0.18709553525759011055,    -0.31762662837940415790,   0.57596995935581177361},
                                                {0.49551858173719833843,    -0.38554392582131206334,   -0.0041487289836441961919,  0.27734667964805483951,    -0.20866982357986247490,   -0.074430802948934947042,  0.24873396754372477981,     -0.1332102986968462371,   -0.1332102986968462371,   0.24873396754372477981,     -0.074430802948934947042,  -0.20866982357986247490,   0.27734667964805483951,    -0.0041487289836441961919,  -0.38554392582131206334,   0.49551858173719833843},
                                                {-0.41235435600443723145,   0.40649201194568901382,    -0.16323079074068586173,    -0.12229075428093316944,   0.26056468539311584124,    -0.18000664257033620854,   -0.034552844339247258762,   0.2095605779022140381,    -0.2095605779022140381,   0.034552844339247258762,    0.18000664257033620854,    -0.26056468539311584124,   0.12229075428093316944,    0.16323079074068586173,     -0.40649201194568901382,   0.41235435600443723145},
                                                {0.32774355538335211548,    -0.38251228765894147859,   0.27462352740819538531,     -0.077174754481372466816,  -0.11729639275154780508,   0.22624985561153359055,    -0.20935657990879221320,    0.08394692723887177866,   0.08394692723887177866,   -0.20935657990879221320,    0.22624985561153359055,    -0.11729639275154780508,   -0.077174754481372466816,  0.27462352740819538531,     -0.38251228765894147859,   0.32774355538335211548},
                                                {-0.24296094086855060502,   0.31960651386020941706,    -0.30648445130541273836,    0.22499559962267636276,    -0.10114691221806107990,   -0.033121233420858077561,  0.14527100300799274730,     -0.2087790108522797645,   0.2087790108522797645,    -0.14527100300799274730,    0.033121233420858077561,   0.10114691221806107990,    -0.22499559962267636276,   0.30648445130541273836,     -0.31960651386020941706,   0.24296094086855060502},
                                                {0.15926784969177756368,    -0.22703094113843247742,   0.25664726816455721507,     -0.25612287305423045974,   0.22944660790243192215,    -0.18083256761833364557,   0.11550693724727067523,     -0.03969438377842736480,  -0.03969438377842736480,  0.11550693724727067523,     -0.18083256761833364557,   0.22944660790243192215,    -0.25612287305423045974,   0.25664726816455721507,     -0.22703094113843247742,   0.15926784969177756368},
                                                {-0.077890656694384388316,  0.11629958289453816823,    -0.14346081988790148753,    0.16405837863121677945,    -0.17968412480105881257,   0.19104022769712913164,    -0.19847211923316558748,    0.2021519053186715665,    -0.2021519053186715665,   0.19847211923316558748,     -0.19104022769712913164,   0.17968412480105881257,    -0.16405837863121677945,   0.14346081988790148753,     -0.11629958289453816823,   0.077890656694384388316},
                                                {6.5917734335564784523e-21, 5.9702240867800434591e-21, -6.2465165261658089836e-21, 1.7125480885470566850e-21, 3.3270853255958221339e-21, 1.6209826808362485664e-21, -1.6462636462892663762e-21, 6.755447898217375706e-22, 6.755447898217375706e-22, -1.6462636462892663762e-21, 1.6209826808362485664e-21, 3.3270853255958221339e-21, 1.7125480885470566850e-21, -6.2465165261658089836e-21, 5.9702240867800434591e-21, 6.5917734335564784523e-21}};

        static double nodes[16] = {-0.98940093499164993260, -0.94457502307323257608, -0.86563120238783174388,
                                   -0.75540440835500303389, -0.61787624440264374845, -0.45801677765722738634,
                                   -0.28160355077925891323, -0.09501250983763744019, 0.09501250983763744019,
                                   0.28160355077925891323, 0.45801677765722738634, 0.61787624440264374845,
                                   0.75540440835500303389, 0.86563120238783174388, 0.94457502307323257608,
                                   0.98940093499164993260};
        static double weigths[16] = {0.02715245941175409485, 0.06225352393864789286, 0.09515851168249278481,
                                     0.12462897125553387205, 0.14959598881657673208, 0.16915651939500253819,
                                     0.18260341504492358887, 0.18945061045506849628, 0.18945061045506849628,
                                     0.18260341504492358887, 0.16915651939500253819, 0.14959598881657673208,
                                     0.12462897125553387205, 0.09515851168249278481, 0.06225352393864789286,
                                     0.02715245941175409485};
        double m = (a + b) / 2;
        double h = (b - a) / 2;
        D aV[16];
        for (int j = 0; j < 16; ++j)
            aV[j] = V(m + nodes[j] * h);

        D zero = aV[0] * 0;

        D *coeffs = new D[n];
        double H = 1;
        for (int i = 0; i < n; ++i) {
            coeffs[i] = zero;
            for (int j = 0; j < 16; ++j)
                coeffs[i] += aV[j] * weigths[j] * legendrePoints[i][j] * (i + .5) / H;
            H *= h * 2;
        }

        return coeffs;
    }
};


#endif //SCHRODINGER_LEGENDRE_H