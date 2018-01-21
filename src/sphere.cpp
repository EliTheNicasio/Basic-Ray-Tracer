#include "sphere.h"
#include "ray.h"


// Determine if the ray intersects with the sphere
bool Sphere::Intersection(const Ray& ray, std::vector<Hit>& hits) const
{
    // a, b, and c are respective terms in quadratic equation, for formula
    // for intersection of sphere
    double a = 1;
    double b = 2 * dot(ray.direction, ray.endpoint - center);
    double c = dot(ray.endpoint - center, ray.endpoint - center) - radius * radius;
    // discr is discriminant
    double discr = b * b - 4 * a * c;

    if(discr < 0)
    {
        return false;
    }
    else
    {
        // creating hit objects with info about intersection
        Hit hit1, hit2;

        hit1.object = this;
        hit2.object = this;

        hit1.ray_exiting = false;
        hit2.ray_exiting = true;

        if(discr == 0)
        {
            double t = (-1 * b) / (2 * a);
            if(t > 0)
            {
                hit1.t = 0;
                hit2.t = t;
                hits.push_back(hit1);
                hits.push_back(hit2);
            }
        }
        else
        {
            //std::cout << "a: " << a << " b: " << b << " c: " << c << " discr: " << discr << std::endl;
            if(((-1 * b) - sqrt(discr)) / (2 * a) > 0)
            {
                hit1.t = ((-1 * b) - sqrt(discr)) / (2 * a);
                hits.push_back(hit1);
            }
            if(((-1 * b) + sqrt(discr)) / (2 * a) > 0)
            {
                hit2.t = ((-1 * b) + sqrt(discr)) / (2 * a);
                hits.push_back(hit2);
            }
        }
        return true;
    }
}

vec3 Sphere::Normal(const vec3& point) const
{
    vec3 normal;
    
    normal = (point - center).normalized();

    return normal;
}
