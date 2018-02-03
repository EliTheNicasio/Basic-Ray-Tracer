#include "refractive_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Refractive_Shader::
Shade_Surface(const Ray& ray, const vec3& intersection_point,
        const vec3& same_side_normal, int recursion_depth,bool is_exiting) const
{
    //Hints: Use REFRACTIVE_INDICES::AIR for air refractive_index
    //       Use is_exiting to decide the refractive indices on the ray and transmission sides
    vec3 reflection_color;
    vec3 refraction_color;
    double reflectance_ratio=-1;
    if(!world.disable_fresnel_refraction)
    {
        //TODO (Test 27+): Compute the refraction_color:
        // - Check if it is total internal reflection. 
        //      If so update the reflectance_ratio for total internal refraction
        //
        //      else, follow the instructions below
        //
        //        (Test 28+): Update the reflectance_ratio 
        //
        //        (Test 27+): Cast the refraction ray and compute the refraction_color
        //
        double indI, indR;
        vec3 n;


        if(is_exiting)
        {
            indI = refractive_index;
            indR = REFRACTIVE_INDICES::AIR;
            n = -1.0 * same_side_normal;
        }
        else
        {
            indI = REFRACTIVE_INDICES::AIR;
            indR = refractive_index;
            n = same_side_normal;
        }

        vec3 d = ray.direction; 
        double cosI = (dot(d, n));

        double cosRSquared = 1.0 - (((indI / indR) * (indI / indR)) * (1.0 - (cosI * cosI)));

        if(cosRSquared < 0)
        {
            reflectance_ratio = 1;
            refraction_color = vec3(0,0,0);
        }
        else
        {
            double cosR = sqrt(cosRSquared);
            vec3 T = ((indI / indR) * (d - (dot(n, d) * n))) - (cosR * n);

            Ray refrRay(intersection_point, T);
         //   refrRay.endpoint = refrRay.Point(.001);

            refraction_color = shader->world.Cast_Ray(refrRay, recursion_depth);

            double rPar, rPerp;            

            if(cosI < 0)
                cosI = cosI * -1.0;
//            if(cosR < 0)
  //              cosR = cosR * -1.0;

            rPar = ((indR * cosI) - (indI * cosR)) / ((indR * cosI) + (indI * cosR));
            rPerp = ((indI * cosI) - (indR * cosR)) / ((indI * cosI) + (indR * cosR));
            reflectance_ratio = (rPar * rPar + rPerp * rPerp) / 2.0;
        }
    }

    if(!world.disable_fresnel_reflection)
    {
        //TODO:(Test 26+): Compute reflection_color:
        // - Cast Reflection Ray andd get color
        vec3 d = ray.direction.normalized();// - ray.endpoint).normalized();
        vec3 n = same_side_normal;
        vec3 r = d - (2.0 * dot(d, n) * n);
        vec3 e = intersection_point;

        Ray reflRay(e, r);
       // reflRay.endpoint = reflRay.Point(.001);

        reflection_color = shader->world.Cast_Ray(reflRay, recursion_depth);
    }


    Enforce_Refractance_Ratio(reflectance_ratio);
    vec3 color;
    // TODO: (Test 26+) Compute final 'color' by blending reflection_color and refraction_color using 
    //                  reflectance_ratio
    //
    color = reflectance_ratio * reflection_color + (1 - reflectance_ratio) * refraction_color;

    return color;
}

void Refractive_Shader::
Enforce_Refractance_Ratio(double& reflectance_ratio) const
{
    if(world.disable_fresnel_reflection) reflectance_ratio=0;
    else if(world.disable_fresnel_refraction) reflectance_ratio=1;
}

