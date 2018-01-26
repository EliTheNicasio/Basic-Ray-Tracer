#include "plane.h"
#include "ray.h"
#include <cfloat>


// Intersect with the half space defined by the plane.  The plane's normal
// points outside.  If the ray starts on the "inside" side of the plane, be sure
// to record a hit with t=0 as the first entry in hits.
bool Plane::
Intersection(const Ray& ray, std::vector<Hit>& hits) const
{
    Hit hit;
    hit.object = this;
    hit.ray_exiting = true;

    double divisor = dot(normal, ray.direction);

    if(divisor == 0)
    {
        hit.t = 0;
        hits.push_back(hit);
        return true;
    }
    else
    {
        double t = dot(normal, x1 - ray.endpoint) / divisor;
        if(t > 0)
        {
                hit.t = t;
            hits.push_back(hit);
            return true;
        }
        else
            return false;
    }
}

vec3 Plane::
Normal(const vec3& point) const
{
    return normal;
}
