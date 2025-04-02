#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include "material.h"
#include "external/omp.h"
class camera
{
public:

    double aspect_ratio = 1.0;
    int image_width = 100;
    int samples_per_pixel = 10; 
    int max_depth = 10; // Maximum number of ray bounces

    double vfov = 90;
    vec3 lookFrom = point3(0, 0, 0);
    vec3 lookTo = point3(0, 0, -1);
    vec3 upAxis = vec3(0, 1, 0);

    double defocus_angle = 0;  // Variation angle of rays through each pixel
    double focus_dist = 10;    // Distance from camera lookfrom point to plane of perfect focus

    void render(const hittable& world)
    {
        initialize();
        // Render

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        #pragma omp parallel for num_threads(4) collapse(2)
        for (int j = 0; j < image_height; j++)
        {
            for (int i = 0; i < image_width; i++)
            {
                color pixel_color(0, 0, 0);
                for (int k = 0; k < samples_per_pixel; k++)
                {
                    ray sub_r = get_ray(i, j);
                    pixel_color += ray_color(sub_r, max_depth, world);
                }
                write_color(std::cout, pixel_color * pixel_samples_scale);

            }
        }
    }

private:
    int image_height;
    point3 center;
    point3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    double pixel_samples_scale;
    vec3 defocus_disk_u;
    vec3 defocus_disk_v;

    vec3 u, v, w;

    void initialize()
    {
        // Image
        image_height = int(image_width / aspect_ratio);
        image_height = image_height < 1 ? 1 : image_height;

        // Camera Center
        center = lookFrom;

        // Viewport
        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta / 2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (double(image_width) / image_height);

        w = unit_vector(lookFrom - lookTo);
        u = unit_vector(cross(upAxis, w));
        v = unit_vector(cross(w, u));

        // Vectors across the horizontal and vertical edges
        auto viewport_u = viewport_width * u;
        auto viewport_v = viewport_height * -v;

        // Pixel space i.e delta vector from pixel to pixel
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Location of upper left pixel
        auto viewport_upper_left = center - focus_dist * w - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
        pixel_samples_scale = 1.0 / samples_per_pixel;

        double defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = defocus_radius * u;
        defocus_disk_v = defocus_radius * v;

    }

    ray get_ray(int i, int j) const
    {
        auto offset = sample_square();
        auto pixel_sample = pixel00_loc + ((i + offset.x())) * pixel_delta_u + ((j + offset.y())) * pixel_delta_v;
        point3 ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        vec3 ray_direction = pixel_sample - ray_origin;
        auto ray_time = random_double();

        return ray(ray_origin, ray_direction, ray_time);
    }

    vec3 sample_square() const
    {
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    point3 defocus_disk_sample() const
    {
        vec3 p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    color ray_color(const ray& r, int depth, const hittable& world)
    {
        if (depth < 0)
            return color(0,0,0);

        hit_record rec;
        if (world.hit(r, interval(0.001, infinity), rec)) //shadow acne
        {
            ray scattered;
            color attenuation;
            if(rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth - 1, world);
            return color(0, 0, 0);
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5 * (unit_direction.y() + 1.0);

        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }

};
#endif
