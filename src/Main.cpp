#include <iostream>
#include <thread>
#include <chrono>

#include "Common.h"

#include "Color.h"
#include "Hittable_List.h"
#include "Sphere.h"
#include "Camera.h"
#include "Material.h"

#define MT 0

class Timer
{
public: 
    Timer()
    {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        Stop();
    }

    void Stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        auto start = std::chrono::time_point_cast<std::chrono::seconds>(m_StartTimepoint).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::seconds>(endTimepoint).time_since_epoch().count();

        auto seconds = end - start;

        std::cerr << "Seconds elapsed:" << seconds <<std::endl;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
};


Color ray_color(const Ray& ray, const Hittable& world, int depth)
{
    HitRecord record;

    if (depth <= 0)
        return Color(0, 0, 0);

    if (world.hit(ray, 0.001, infinity, record))
    {
        Ray scattered;
        Color attenuation;
        if(record.material_ptr->scatter(ray, record, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        return Color(0, 0, 0);
    }
        
    Vector3 unit_direction = unit_vector(ray.direction());
    float t = (unit_direction.y() + 1.f) * 0.5f;
    return (1.f - t) * Color(1.f, 1.f, 1.f) + t * Color(0.5f, 0.7f, 1.f);
}

Hittable_List random_scene();

#if MT
void calculateColor(Color* arr, int iteration, int num_of_threads, int lines_per_thread, int image_width, int image_height, int max_depth, int samples_per_pixel, const Camera& cam, const Hittable_List& world);
#endif

int main()
{
    //Image dimesions
    const float aspect_ratio = 16.f / 9.f;
    const int image_width = 1280;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;


    //World
    float R = cos(pi / 4.f);
    Hittable_List world = random_scene();

    //Camera
    Point3D lookfrom(13.f, 2.f, 3.f);
    Point3D lookat(0.f, 0.f, 0.f);
    Vector3 vup(0.f, 1.f, 0.f);
    float dist_to_focus = 10.f;
    float aperture = 0.1f;
    Camera cam(lookfrom, lookat, vup, 25.f, aspect_ratio, aperture, dist_to_focus);

    //Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    //Timer for measuring performance
    Timer timer;

#if MT
    const int num_of_threads = std::thread::hardware_concurrency();
    const int lines_per_thread = static_cast<int>(image_height / num_of_threads);
    Color* ColorArr = new Color[image_height*image_width];
    
    std::thread* threads = new std::thread[num_of_threads];
    for (int i = 0; i < num_of_threads; i++)
    {
        threads[i] = std::thread(calculateColor, ColorArr, i, num_of_threads, lines_per_thread, image_width, image_height, max_depth, samples_per_pixel, cam, world);
    }
    
    for (int i = 0; i < num_of_threads; i++)
    {
        threads[i].join();
    }

    for (int i = 0; i < image_width * image_height; i++)
    {
        write_color(std::cout, ColorArr[i], samples_per_pixel);
    }
#else
    for (int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;


        for (int i = 0; i < image_width; ++i) 
        {
            Color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s)
            {

                float h = (i + random_float()) / (image_width - 1);
                float v = (j + random_float()) / (image_height - 1);
                Ray ray = cam.get_ray(h, v);
                pixel_color = pixel_color + ray_color(ray, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
#endif // MT
    std::cerr << "\nDone.\n";
}

#if MT
void calculateColor(Color* arr, int iteration, int num_of_threads, int lines_per_thread, int image_width, int image_height, int max_depth, int samples_per_pixel, const Camera& cam, const Hittable_List& world)
{
    int upper_limit = image_height - iteration * lines_per_thread - 1;
    int lower_limit = image_height - (iteration + 1) * lines_per_thread;
    for (int j = upper_limit; j >= lower_limit; --j)
    {
        std::cerr << "Lines in thread " << iteration << " remaining: " << j - (image_height - (iteration + 1) * lines_per_thread) << std::endl;
        for (int i = 0; i < image_width; ++i)
        {
            Color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s)
            {

                float h = (i + random_float()) / (image_width - 1);
                float v = (j + random_float()) / (image_height - 1);
                Ray ray = cam.get_ray(h, v);
                pixel_color = pixel_color + ray_color(ray, world, max_depth);
            }
            int element_number = i + image_width * (image_height - j - 1);
            arr[element_number] = pixel_color;

        }
    }
}
#endif // MT

Hittable_List random_scene()
{
    Hittable_List world;

    shared_ptr<Lambertian> ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(Point3D(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = random_float();
            Point3D center(a + 0.9f * random_float(), 0.2f, b + 0.9f * random_float());

            if ((center - Point3D(4.f, 0.2f, 0.f)).length() > 0.9f) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    Color albedo = Color::random() * Color::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2f, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    Color albedo = Color::random() * Color::random();
                    float fuzz = random_float(0.f, 0.5f);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2f, sphere_material));
                }
                else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5f);
                    world.add(make_shared<Sphere>(center, 0.2f, sphere_material));
                }
            }
        }
    }

    shared_ptr<Dielectric> material1 = make_shared<Dielectric>(1.5f);
    world.add(make_shared<Sphere>(Point3D(0.f, 1.f, 0.f), 1.0f, material1));

    shared_ptr<Lambertian> material2 = make_shared<Lambertian>(Color(0.4f, 0.2f, 0.1f));
    world.add(make_shared<Sphere>(Point3D(-4.f, 1.f, 0.f), 1.0f, material2));

    shared_ptr<Metal> material3 = make_shared<Metal>(Color(0.7f, 0.6f, 0.5f), 0.0f);
    world.add(make_shared<Sphere>(Point3D(4.f, 1.f, 0.f), 1.0f, material3));

    return world;
}