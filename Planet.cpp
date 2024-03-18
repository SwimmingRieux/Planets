#define double long double
#include "Planet.h"

void Planet::setTime(double t) {
    time = t;
}
double Planet::getTime() const {
    return time;
}

void Planet::setRadius(double r) {
    radius = r;
}
double Planet::getRadius() const {
    return radius;
}

void Planet::setVelocity(const TwoDim& v) {
    velocity = v;
}
TwoDim Planet::getVelocity() const {
    return velocity;
}

void Planet::setCoordinate(const TwoDim& c) {
    coordinate = c;
}
TwoDim Planet::getCoordinate() const {
    return coordinate;
}

void Planet::setForce(const TwoDim& f) {
    force = f;
}
TwoDim Planet::getForce() const {
    return force;
}
