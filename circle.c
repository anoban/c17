#include <assert.h>
#include <stdio.h>

void __forceinline CalculateCirclePoints(float* axes_x, float* axes_y, short length, float radius, float xcentre, float ycentre) {
    /*
     * axes_x and axes_y are write buffers to be provided by the caller.
     * length is the number of elements a write buffer can hold.
     * make sure length(axes_x) == length(axes_y)!
     * radius is the radius of the circle
     * xcentre is the x axis value where the centre of the circle lies.
     * ycentre is the y axis value where the centre of the circle lies.
     */

    /*
     * First, make an array with x axes data points, at the given step from either directions from the centre.
     * For every point in x axes, there should be two points in y axes to make a circle.
     * Except when x == centre +- radius
     */

    // Do not accept even numbers for length, for now.
    assert(length % 2 != 0);

    double diameter  = radius * 2;
    double increment = diameter / (length - 1);

    // x axis, points before centre
    for (short i = 0; i <= (length - 1) / 2; ++i) axes_x[i] = xcentre - radius + (i * increment);

    // x axis, points after centre
    for (short i = (length + 3) / 2; i <= length; ++i) axes_x[i] = xcentre + (i * increment);

    return;
}

int main(void) { return 0; }