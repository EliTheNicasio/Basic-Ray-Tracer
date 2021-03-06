#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& same_side_normal,int recursion_depth,bool is_exiting) const 
{
    vec3 color;
    // For Ambient
    color = world.ambient_color * world.ambient_intensity * color_ambient;

    Ray shadowRay;

    // For loop for each light
    for(size_t i = 0; i < world.lights.size(); i++)
    {
        // lightVector is vector from point to light
        vec3 lightVector = (world.lights[i]->position - intersection_point).normalized();

        double distance = (world.lights[i]->position - intersection_point).magnitude();

        if(world.enable_shadows)
        {
            Hit shadowHit;
            Ray shadowRay(intersection_point, lightVector); 
           // shadowRay.endpoint = shadowRay.Point(.001);
        
            if(world.Closest_Intersection(shadowRay, shadowHit))
                if(distance > shadowHit.t)
                    continue;

        }

        double divisor = distance * distance;

        // illum is light intensity
        vec3 illum = world.lights[i]->Emitted_Light(shadowRay) / (divisor);
 

        // For Diffuse
        color += illum  * color_diffuse 
                 * std::max(0.0, dot(same_side_normal, lightVector));

        // For Specular
        
        // ref is reflection of lightVector
        vec3 ref = -1.0 * lightVector + 2.0 * (dot(lightVector, same_side_normal)) * same_side_normal;

        // viewer is vector to viewer, or the camera in this case
        vec3 viewer = (world.camera.position - intersection_point).normalized();

        color += illum * color_specular
                 * pow(std::max(0.0, dot(ref, viewer)), specular_power);
    }
    return color;
}
