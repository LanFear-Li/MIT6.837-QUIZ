#ifndef _PA_COLOR_H_
#define _PA_COLOR_H_

#include "vectors.h"

const Vec3f BLACK(0, 0, 0);
const Vec3f WHITE(1, 1, 1);

const Vec3f GRAY(0.5, 0.5, 0.5);
const Vec3f PINKY(1, 0, 0.5);
const Vec3f PURPLE_SLIGHT(1, 0, 1);
const Vec3f PURPLE_NORMAL(0.5, 0, 1);
const Vec3f BLUE_DEEP(0, 0, 1);
const Vec3f BLUE_NORMAL(0, 0.5, 1);
const Vec3f BLUE_SLIGHT(0, 1, 1);
const Vec3f GREEN_DEEP(0, 1, 0.5);
const Vec3f GREEN_NORMAL(0, 1, 0);
const Vec3f GREEN_SLIGHT(0.5, 1, 0);
const Vec3f YELLOW(1, 1, 0);
const Vec3f ORANGE(1, 0.5, 0);
const Vec3f RED(1, 0, 0);

const Vec3f COLOR_LIST[13] = {
        GRAY,
        PINKY,
        PURPLE_SLIGHT,
        PURPLE_NORMAL,
        BLUE_DEEP,
        BLUE_NORMAL,
        BLUE_SLIGHT,
        GREEN_DEEP,
        GREEN_NORMAL,
        GREEN_SLIGHT,
        YELLOW,
        ORANGE,
        RED
};

#endif
