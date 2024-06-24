#include "bond_pricing.h"
#include <math.h>

double price_bond(double face_value, double coupon_rate, int periods, double yield_to_maturity) {
    double pv = 0.0;
    for (int i = 1; i <= periods; ++i) {
        pv += (face_value * coupon_rate) / pow(1 + yield_to_maturity, i);
    }
    pv += face_value / pow(1 + yield_to_maturity, periods);
    return pv;
}

