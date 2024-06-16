#include "bond_pricing.h"
#include <math.h>

double price_bond(double face_value, double coupon_rate, int periods, double yield_to_maturity) {
    double price = 0.0;
    double coupon_payment = face_value * coupon_rate;

    for (int t = 1; t <= periods; ++t) {
        price += coupon_payment / pow(1 + yield_to_maturity, t);
    }

    price += face_value / pow(1 + yield_to_maturity, periods);
    return price;
}

