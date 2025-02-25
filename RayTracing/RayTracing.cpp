#include "../rtHeader.h"
#include "../hittable.h"
#include "../hittable_list.h"
#include "../sphere.h"
#include "../camera.h"
#include "../material.h"



point3 g_sphereCentre = point3(0.0, 0.0, -1.0);


int main()
{
    // World

    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left = make_shared<dielectric>(1.50);
    auto material_bubble = make_shared<dielectric>(1.00 / 1.50);
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    //auto material_ground = make_shared<lambertian>(color(1.00,0.44,0.00));
    //auto material_center = make_shared<lambertian>(color(0.00,0.57,0.92));
    //auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8), 0.2);
    //auto material_right = make_shared<metal>(color(0.93,1.00,0.25), 0.4);

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));


    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.vfov = 20;
    cam.lookFrom = point3(-2, 2, 1);
    cam.lookTo = point3(0, 0, -1);
    cam.upAxis = vec3(0, 1, 0);

    cam.render(world);
}

