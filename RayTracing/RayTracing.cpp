#include "../rtHeader.h"
#include "../hittable.h"
#include "../hittable_list.h"
#include "../sphere.h"
#include "../camera.h"
#include "../material.h"
#include "../quad.h"
#include "../bvh.h"



point3 g_sphereCentre = point3(0.0, 0.0, -1.0);


void bouncingSpheres()
{
    // World

    hittable_list world;

    //auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    //auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    //auto material_left = make_shared<dielectric>(1.50);
    //auto material_bubble = make_shared<dielectric>(1.00 / 1.50);
    //auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    ////auto material_ground = make_shared<lambertian>(color(1.00,0.44,0.00));
    ////auto material_center = make_shared<lambertian>(color(0.00,0.57,0.92));
    ////auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8), 0.2);
    ////auto material_right = make_shared<metal>(color(0.93,1.00,0.25), 0.4);

    //world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    //world.add(make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));
    //world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    //world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
    //world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));


    //camera cam;
    //cam.aspect_ratio = 16.0 / 9.0;
    //cam.image_width = 400;
    //cam.samples_per_pixel = 100;
    //cam.max_depth = 50;
    //cam.vfov = 20;
    //cam.lookFrom = point3(-2, 2, 1);
    //cam.lookTo = point3(0, 0, -1);
    //cam.upAxis = vec3(0, 1, 0);

    //cam.defocus_angle = 10.0;
    //cam.focus_dist = 3.4;

    auto checker = make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    auto center2 = center + vec3(0, random_double(0, 0.5), 0);
                    world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));


    //bvh
    //from a list of objects to one big root bvh node
    world = hittable_list(make_shared<bvh_node>(world));
    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookFrom = point3(13, 2, 3);
    cam.lookTo = point3(0, 0, 0);
    cam.upAxis = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    cam.render(world);
}

void checkeredSpheres() {
    hittable_list world;

    auto checker = make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));

    world.add(make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker)));
    world.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookFrom = point3(13, 2, 3);
    cam.lookTo = point3(0, 0, 0);
    cam.upAxis = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void earth()
{
    auto earth_texture = make_shared<image_texture>("earth.png");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookFrom = point3(0, 0, 12);
    cam.lookTo = point3(0, 0, 0);
    cam.upAxis = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(hittable_list(globe));
}

void quads()
{
    hittable_list world;

    // Materials
    auto left_red = make_shared<lambertian>(color(1.0, 0.2, 0.2));
    auto back_green = make_shared<lambertian>(color(0.2, 1.0, 0.2));
    auto right_blue = make_shared<lambertian>(color(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
    auto lower_teal = make_shared<lambertian>(color(0.2, 0.8, 0.8));

    // Quads
    world.add(make_shared<quad>(point3(-3, -2, 5), vec3(0, 0, -4), vec3(0, 4, 0), left_red));
    world.add(make_shared<quad>(point3(-2, -2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
    world.add(make_shared<quad>(point3(3, -2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    world.add(make_shared<quad>(point3(-2, -3, 5), vec3(4, 0, 0), vec3(0, 0, -4), lower_teal));

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 80;
    cam.lookFrom = point3(0, 0, 9);
    cam.lookTo = point3(0, 0, 0);
    cam.upAxis = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world);
}

int main()
{
    switch (4) {
    case 1: bouncingSpheres();  break;
    case 2: checkeredSpheres(); break;
    case 3: earth(); break;
    case 4: quads(); break;
    }
}

