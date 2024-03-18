
#ifndef PLANET_H
#define PLANET_H

#include "TwoDim.h"

class Planet {
    private:
        TwoDim velocity;
        TwoDim coordinate;
        TwoDim force;
        double time;
        double radius;
    public:
        Planet(double vx, double vy, int cx, int cy, double fx, double fy, int t, double r) :
            velocity(vx, vy), coordinate(cx, cy), force(fx, fy), time(t), radius(r) {}

    void setTime(double t);
    double getTime() const;

    void setRadius(double r);
    double getRadius() const;

    void setVelocity(const TwoDim& v);
    TwoDim getVelocity() const;

    void setCoordinate(const TwoDim& c);
    TwoDim getCoordinate() const;

    void setForce(const TwoDim& f);
    TwoDim getForce() const;
};

#endif
