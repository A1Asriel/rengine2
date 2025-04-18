#include "MathUtils.h"

float MathUtils::continuousAverage(float avg, float value, int size) {
    return avg + (value - avg) / (size + 1);
}