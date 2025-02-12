#include <iostream>
#include "../vec3.h"
#include "../color.h"
#include "../ray.h"


point3 g_sphereCentre = point3(0.0, 0.0, -1.0);

double hitSphere(const ray& r, const point3& sphereCentre, double radius)
{
    vec3 CQ = sphereCentre - r.origin();
    double a = r.direction().length_sqr();
    double b = -2.0 * dot(r.direction(), CQ);
    double c = CQ.length_sqr() - radius * radius;

    double determ = b * b - 4 * a * c;

    if (determ < 0.0)
    {
        return -1.0;
    }
    else
    {
        return (-b - std::sqrt(determ)) / (2 * a);
    }
}

color ray_color(const ray& r)
{
    double t = hitSphere(r, g_sphereCentre, 0.5);
    if (t >= 0.0)
    {
        vec3 normal = unit_vector(r.at(t) - g_sphereCentre);
        return 0.5 * color(normal.x() + 1.0, normal.y() + 1.0, normal.z() + 1.0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);

    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

int main()
{
    // Image
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = int(image_width / aspect_ratio);
    image_height = image_height < 1 ? 1 : image_height;

    // Viewport
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width) / image_height);
    auto focal_length = 1.0;
    auto camera_center = point3(0, 0, 0);

    // Vectors across the horizontal and vertical edges
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // Pixel space i.e delta vector from pixel to pixel
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Location of upper left pixel
    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++)
    {
        for (int i = 0; i < image_width; i++)
        {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);

        }
    }
}

