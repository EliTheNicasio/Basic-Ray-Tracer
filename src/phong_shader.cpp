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
    

    // For Diffuse
    for(size_t i = 0; i < world.lights.size(); i++)
    {
        double distance = (intersection_point - world.lights[i]->position).magnitude();
        double divisor = distance * distance;
        vec3 illum = world.lights[i]->Emitted_Light(ray) / (divisor);
        color += illum  * color_diffuse 
                 * std::max(0.0, dot(same_side_normal,
                                     (world.lights[i]->position - intersection_point).normalized()));
    }
    return color;
}
