#include <cmath>
#include <vector>
#include <tuple>
#include "../catch.hpp"
#include "checkOrthonormality.h"
#include "../../src/se2d.h"


using namespace matslise;
using namespace std;
using namespace Eigen;

void testQuartic(double a, double c, double alpha, const vector<double> &eigenvalues, int sectorCount = 23,
                 double tolerance = 0.0001) {
    SEnD<2> p(
            [a, c](double x, double y) -> double {
                return x * x + y * y + c * (x * x * x * x + 2 * a * x * x * y * y + y * y * y * y);
            },
            {{-alpha, alpha}, -alpha, alpha},
            Options<2>().sectorCount(sectorCount).nested(
                    Options<1>().tolerance(tolerance).symmetric(true)));
    for (const double &E : eigenvalues) {
        CHECK(Approx(E).margin(1e-8) == p.findEigenvalue(E));
    }
}

TEST_CASE("Eigenfunctions quartic: c=-3", "[se2d][eigenvalues][quartic]") {
    testQuartic(-1, 1e-3, 7.5,
                {2.0009985054698104735, 4.0029925416713028329, 6.0029940290428079650, 6.0069702421328217062,
                 6.0089687360251750823, 8.0056893858679641592, 8.0162095893107613409, 10.006398993925805298,
                 10.008948049206928896, 10.014940522380068574});
    testQuartic(0, 1e-3, 7.5,
                {2.0014973853463713991, 4.0044884408419148160, 6.0074794963374582330, 6.0104605654612931866,
                 8.0134516209568366035, 8.0194012847307019984, 10.019423745576214974, 10.022392340226245415,
                 10.031298258747896515, 12.028364464845623786});
    testQuartic(1, 1e-3, 7.5,
                {2.0019955220947085337, 4.0059806337201560486, 6.0119494490457070830, 6.0139360981896530736,
                 8.0198961283737167314, 8.0238606392924854844, 10.029814877549869265, 10.035748547202912722,
                 10.037726447540990997, 12.041699947383956422});
}

TEST_CASE("Eigenfunctions quartic: c=0", "[se2d][eigenvalues][quartic]") {
    testQuartic(-1, 1, 6,
                {2.5616265756400316393, 5.3968039831941313950, 7.5491560629022652232, 8.4359873223484674428,
                 9.6175877647334193257, 10.366020696842355027, 12.818706298776658594, 12.886584502841939840,
                 13.406949149052421685, 15.336126803517827432});
    testQuartic(0, 1, 5,
                {2.7847032830605837113, 6.0411643457423693920, 9.2976254084241550728, 10.047401599289601544,
                 13.303862661971387225, 14.549155539580166935, 17.310099915518619376, 17.805616602261952616,
                 19.449909077833544750, 21.811853855809184767});
    testQuartic(1, 1, 5,
                {2.9520500919628742871, 6.4629059998638721377, 10.390627295503782127, 10.882435576819807244,
                 14.658513813565503097, 15.482771577251666477, 19.217523495888984907, 20.293829707535892571,
                 20.661082690597886009, 24.033166193470850317});
}

TEST_CASE("Eigenfunctions quartic: c=3", "[se2d][eigenvalues][quartic]") {
    // unsolvable
    /* testQuartic(-1, 1e3, 2.8,
                {17.686909350775717644, 37.965440109662577256, 48.868379100596636584, 56.219242763197933124,
                 65.547004077171159352, 76.242494474221982538, 79.726698208795028150, 79.927690262545928550,
                 88.286001081585014158, 98.836631846113076108},
                71, 0.000001); */
    testQuartic(0, 1e3, 1.5,
                {21.279577422656092127, 48.726622170710310149, 76.173666918764528170, 85.321192911492859325,
                 112.76823765954707735, 127.24298764862115750, 149.36280840032962652, 154.69003239667537552,
                 173.44216290830327624, 191.28460313745792470});
    testQuartic(1, 1e3, 1.5,
                {23.513389183129853963, 54.054855795519439394, 89.433434033749367277, 95.437449804059634223,
                 128.61961618091473035, 138.28303842944239989, 170.99777828093744127, 183.30633810778597643,
                 187.54903714202645897, 216.15194758663360719});
}