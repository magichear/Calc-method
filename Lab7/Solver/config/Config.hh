#ifndef CONFIG_H
#define CONFIG_H

#include <cmath>

#ifdef USE_DOUBLE
using   Value =   double;
#define POW(x, y) pow(x, y)
#define FABS(x)   fabs(x)
#define LOG(x)    log(x)
#else
using   Value =   long double;
#define POW(x, y) powl(x, y)
#define FABS(x)   fabsl(x)
#define LOG(x)    logl(x)
#endif

#endif