# MIT6.837 Programming Assignment

## PA0 - Iterated Function Systems

PA0使用IFS(Iterated Function System)实现了各种自相似的分形结构(Fractal Like Barnsley Fern and Sierpinski Triangle)，同时熟悉了C++编程(Command Line Parser)以及数学库(vectors.h/matrix.h)和图像库(image.h)的使用。

**Rendering IFS**

```c++
for (int p = 0; p < point_nums; p++) {
    point = get_random_point(1, 1);

    for (int k = 0; k < iter_nums; k++) {
        int kth = 0;
        float random = get_random_num(0, 1);

        for (int i = 0; i < transform_nums; i++) {
            if (transform_probs[i] >= random) {
                kth = i;
                break;
            }
        }

        transforms[kth].Transform(point);
    }

    if (point.x() >= 0 && point.x() <= 1 && point.y() >= 0 && point.y() <= 1) {
        image.SetPixel(point.x() * width, point.y() * height, BLACK);
    }
}
```

**Output**

sierpinski triangle

<p align="center"><img src="https://raw.githubusercontent.com/LanFear-Li/MIT6.837-QUIZ/main/Appendix/pa0-1.png" style="zoom: 30%;text-align: left"/></p>

barnsley fern

<p align="center"><img src="https://raw.githubusercontent.com/LanFear-Li/MIT6.837-QUIZ/main/Appendix/pa0-2.png" style="zoom: 100%;text-align: left"/></p>

## PA1 - Ray Casting

PA1实现了Ray Caster的基本功能：包括光线投射，逐遍历屏幕空间像素并生成光线与场景求交；光线求交，通过判断与球心的距离用数值计算与球体求交；相机控制，实现正交投影(Orthographic Projection)生成光线。

**Ray Casting**

```c++
// bottom_left -> (0, 0), upper_right -> (width, height)
for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
        float u = i / width;
        float v = j / height;

        Hit hit;
        Ray ray = camera->generateRay(Vec2f(u, v));
        group->intersect(ray, hit, camera->getTMin());
    }
}
```

**Sphere Intersection**

```c++
Vec3f ori = r.getOrigin(), dir = r.getDirection();

// let a = ray's origin, b = another point at ray
// and c = sphere's center, rad = sphere's radius;
Vec3f a = ori, b = ori + dir * 1.0f;
Vec3f c = this->center;
float rad = this->radius;

// calculate distance from center to ray with dot production
Vec3f ab = b - a;
Vec3f ac = c - a;
float cosine = ab.Dot3(ac) / ab.Length() / ac.Length();
float sine = sqrt(1 - cosine * cosine);
float dis_to_ray = ac.Length() * sine;

// no intersection
if (dis_to_ray > rad) {
    return false;
}

// find max t and check legality

// enumerate all possible t and update hit info
```

**Orthographic Camera Ray Generation**

```c++
point -= Vec2f(0.5f, 0.5f);
Vec3f origin = this->center;

origin += this->horizontal * this->size * point.x();
origin += this->up * this->size * point.y();

return Ray(origin, this->direction);
```

**Output**

frame buffer

<p align="center"><img src="https://raw.githubusercontent.com/LanFear-Li/MIT6.837-QUIZ/main/Appendix/pa1-1.png" style="zoom: 100%;text-align: left"/></p>

depth buffer

<p align="center"><img src="https://raw.githubusercontent.com/LanFear-Li/MIT6.837-QUIZ/main/Appendix/pa1-2.png" style="zoom: 100%;text-align: left"/></p>

## PA2 - Transformations & Additional Primitives

PA2基于已有的Ray Caster实现了各类几何模型(Primitive)以及各类变换(Transformation)，包括了Plane、Triangle以及Transform变换。

光线与Plane的求交直接代入平面方程即可，光线与Triangle的求交需要判断计算的点是否在Triangle内部，这一点可以通过Cross Product朴素实现，也可以进一步使用重心坐标(Barycentric Coordinate)判断。同时在相机控制部分实现了透射投影(Perspective Projection)。

**Plane Intersection**

```c++
Vec3f ori = r.getOrigin(), dir = r.getDirection();

// ray parallel with plane
if (dir.Dot3(this->normal) == 0) {
    return false;
}

// (origin + t * direction) * normal = distance
float t = (this->distance - ori.Dot3(this->normal)) / (dir.Dot3(this->normal));

if (t < t_min) {
    return false;
}
```

**Triangle Intersection**

```c++
Vec3f ori = r.getOrigin(), dir = r.getDirection();

Vec3f normal;
Vec3f::Cross3(normal, this->a - this->b, this->a - this->c);
normal.Normalize();
float dis = this->a.Dot3(normal);

// ray parallel with plane
if (dir.Dot3(normal) == 0) {
    return false;
}

// (origin + t * direction) * normal = distance
float t = (dis - ori.Dot3(normal)) / (dir.Dot3(normal));

if (t < t_min) {
    return false;
}

// get intersect point with the plane formed by triangle
Vec3f p = ori + dir * t;

// use cross product to check whether inside the triangle
Vec3f ab = this->b - this->a, bc = this->c - this->b, ca = this->a - this->c;
Vec3f ap = p - this->a, bp = p - this->b, cp = p - this->c;

Vec3f cross;
Vec3f::Cross3(cross, ab, ap);
float cross_a = normal.Dot3(cross);
Vec3f::Cross3(cross, bc, bp);
float cross_b = normal.Dot3(cross);
Vec3f::Cross3(cross, ca, cp);
float cross_c = normal.Dot3(cross);

// TODO: ray-triangle intersection with barycentric coordinate
if (cross_a > 0 && cross_b > 0 && cross_c > 0 || cross_a <= 0 && cross_b <= 0 && cross_c <= 0) {
    h.set(t, material_ptr, normal, r);
    return true;
}
```

**Perspective Camera Ray Generation**

```c++
point -= Vec2f(0.5f, 0.5f);
Vec3f screen_loc = this->center + this->direction * this->near;
screen_loc += point.x() * this->horizontal + point.y() * this->up;

Vec3f origin = this->center;
Vec3f ray_dir = screen_loc - this->center;
ray_dir.Normalize();
return Ray(origin, ray_dir);
```

**Output**

bunny

<p align="center"><img src="https://raw.githubusercontent.com/LanFear-Li/MIT6.837-QUIZ/main/Appendix/pa2-1.png" style="zoom: 100%;text-align: left"/></p>

frame buffer

<p align="center"><img src="https://raw.githubusercontent.com/LanFear-Li/MIT6.837-QUIZ/main/Appendix/pa2-2.png" style="zoom: 100%;text-align: left"/></p>

## PA3 - OpenGL & Phong Shading

PA3基于Ray Caster实现了经典的Phong Shading Model同时使用OpenGL实现了各类Primitive的实时绘制。

Phong Shading由环境光照(Ambient)、漫反射(Diffuse Light)以及镜面反射(Specular Light)构成。漫反射通过入射光和着色点法线的夹角计算，镜面反射通过入射光和反射光构成的半程向量(Half Vector)计算。

**Phong Shading**

```c++
Vec3f normal = hit.getNormal();

// implement phong shading model: ambient + diffuse + specular
Vec3f diffuse_color = this->diffuseColor * fmax(normal.Dot3(dirToLight), 0);

Vec3f v = ray.getDirection();
v.Negate();
Vec3f half_vec = v + dirToLight;
half_vec.Normalize();
Vec3f specular_color = this->specularColor * pow(normal.Dot3(half_vec), this->exponent);

return (diffuse_color + specular_color) * lightColor;
```

**Output**

specular lighting

<p align="center"><img src="https://raw.githubusercontent.com/LanFear-Li/MIT6.837-QUIZ/main/Appendix/pa3-1.png" style="zoom: 100%;text-align: left"/></p>

## PA4 - Shadows, Reflection & Refraction

PA4实现了Ray Casting过程中反射、折射以及阴影的生成。

光线反射和折射时需要计算光线重新弹射的方向，同时需要累积弹射次数(Bounces)。对于阴影的生成，不同于Rasterization过程中的Shadow Mapping，Ray Caster对于每一个着色点，通过向光源发射光线是否存在遮挡判断此处是否需要进行着色。

**Reflection**

```c++
// generate reflection color
Vec3f reflected_color = material_ptr->getReflectiveColor();
if (material_ptr->getReflectiveColor().Length() > epsilon) {
    Vec3f dir_mirror = mirrorDirection(hit.getNormal(), ray.getDirection());
    Ray ray_reflect(intersect, dir_mirror);
    Hit hit_reflect;

    color += reflected_color * traceRay(ray_reflect, epsilon, bounces + 1, weight * reflected_color.Length() 	 / sqrtf(3), indexOfRefraction, hit_reflect);

    if (bounces < input_parser->max_bounces) {
        RayTree::AddReflectedSegment(ray_reflect, 0, hit_reflect.getT());
    }
}
```

**Refraction**

```c++
// generate refraction color
Vec3f refracted_color = material_ptr->getTransparentColor();
if (material_ptr->getTransparentColor().Length() > epsilon) {
    float max_indexOfRefraction = back_face ? 1.0f : material_ptr->getIndexOfRefraction();
    Vec3f dir_transmit;
    bool is_transmitted = transmittedDirection(hit.getNormal(), ray.getDirection(), indexOfRefraction,
                                               max_indexOfRefraction, dir_transmit);

    if (is_transmitted) {
        Ray ray_refract(intersect, dir_transmit);
        Hit hit_refract;

        color += refracted_color * raceRay(ray_refract, epsilon, bounces + 1, weigh * 	  						 	refracted_color.Length() / sqrtf(3), ax_indexOfRefraction, hit_refract);

        if (bounces < input_parser->max_bounces) {
            RayTree::AddTransmittedSegment(ray_refract, 0, hit_refract.getT());
        }
    }
}
```

**Shadow Test**

```c++
for (int k = 0; k < light_num; k++) {
    Vec3f dir_to_light, light_color;
    float dis_to_light;
    scene_parser->getLight(k)->getIllumination(intersect, dir_to_light, light_color, dis_to_light);

    // generate shadows
    if (input_parser->shadows) {
        Ray ray_shadow(intersect, dir_to_light);
        Hit hit_shadow;
        if (group->intersect(ray_shadow, hit_shadow, epsilon) && dis_to_light > hit_shadow.getT()) {
            RayTree::AddShadowSegment(ray_shadow, 0, hit_shadow.getT());
            continue;
        } else {
            RayTree::AddShadowSegment(ray_shadow, 0, dis_to_light);
        }
    }

    color += material_ptr->Shade(ray, hit, dir_to_light, light_color);
}
```

**Output**

point light circle

<p align="center"><img src="https://raw.githubusercontent.com/LanFear-Li/MIT6.837-QUIZ/main/Appendix/pa4-1.png" style="zoom: 100%;text-align: left"/></p>

## PA5 - Voxel Rendering

PA5实现了体素渲染。首先是实现了对于各类Primitive的包围盒(Bounding Box)的计算，同时添加了Primitive Grid用于管理其他的Primitive，Grid内部存储场景中每一个Primitive的包围盒的情况。

同时，Ray Casting过程使用Ray marching遍历Grid实现计算，便于后续Grid Acceleration的实现。

**Grid Primitive Insertion**

```c++
int min_idx[3], max_idx[3];

g->get_index(bbox_ptr->getMin(), min_idx);
g->get_index(bbox_ptr->getMax(), max_idx);
for (int i = min_idx[0]; i <= max_idx[0]; i++) {
    for (int j = min_idx[1]; j <= max_idx[1]; j++) {
        for (int k = min_idx[2]; k <= max_idx[2]; k++) {
            g->cell_state[i][j][k].push_back(this);
        }
    }
}
```

**Ray Marching**

```c++
for (int i = 0; i < 3; i++) {
    mi.sign[i] = r.getDirection()[i] > 0.0f ? 1 : -1;
    mi.d_t[i] = step[i] / r.getDirection()[i] * mi.sign[i];
}

// generate grid vertices

// construct intersect planes and its normals
// sequence: up, down, left, right, front, back

Vec3f point_hit;
Vec3f point_start = r.pointAtParameter(t_min);
int enter_face_index;

// check if point_start point is inside the grid
if (validate_point(point_start)) {
    mi.hit_cell = true;
    point_hit = point_start;
} else {
    // if not, find the nearest point on the grid
    // find closest ray-plane intersection
    Hit hit_closest;
    for (int i = 0; i < 6; i++) {
        Hit hit;
        if (plane[i].intersect(r, hit, t_min)) {
            Vec3f p = hit.getIntersectionPoint();
            if (!validate_point(p)) {
                continue;
            }

            if (hit_closest.getMaterial() == nullptr || hit.getT() < hit_closest.getT()) {
                hit_closest = hit;
                mi.cell_normal = normals[i];
                mi.hit_cell = true;

                enter_face_index = i;
            }Voxel Rendering

        }
    }

    mi.t_min = hit_closest.getT();
    point_hit = hit_closest.getIntersectionPoint();
}

// construct the marching info: next grid index and the t-value to it
get_index(point_hit, mi.grid_index);
for (int i = 0; i < 3; i++) {
    float index_next = mi.grid_index[i] + (mi.sign[i] + 1) / 2;
    mi.t_next[i] = (minn[i] + step[i] * index_next - r.getOrigin()[i]) / r.getDirection()[i];
}
```

**Output**

bunny

<p align="center"><img src="https://raw.githubusercontent.com/LanFear-Li/MIT6.837-QUIZ/main/Appendix/pa5-1.png" style="zoom: 100%;text-align: left"/></p>

## PA6 - Grid Acceleration & Solid Textures

PA6实现了基于Grid的光线求交加速结构以及基于Perlin Noise的各类Procedural Solid Texture。

Ray Casting在March Grid的过程中会选择最近的区域前进，同时对于具有无限面积的Primitive进行了特殊处理。

**Intersect Primitive With Grid**

```c++
MarchingInfo info;
initializeRayMarch(info, r, t_min);

if (info.hit_cell) {
    info.hit_cell = false;

    while (validate_index(info.grid_index)) {
        int x = info.grid_index[0], y = info.grid_index[1], z = info.grid_index[2];
        float t_next_min = fmin(info.t_next[0], fmin(info.t_next[1], info.t_next[2]));

        // find the nearest object inside this cell
        Hit hit_object_closest;
        for (auto object: cell_bucket[x][y][z]) {
            Hit hit_object;

            if (object->intersect(r, hit_object, t_min)) {
                if (hit_object.getT() < t_next_min + epsilon) {
                    info.hit_cell = true;

                    if (hit_object_closest.getMaterial() == nullptr ||
                        hit_object.getT() < hit_object_closest.getT()) {
                        hit_object_closest = hit_object;
                    }
                }
            }
        }

        if (info.hit_cell) {
            h = hit_object_closest;
            break;
        }

        info.nextCell();
    }
}

// intersect with infinite objects and find the nearest
bool infinite_intersected = false;
Hit hit_infinite_closest;
for (auto object : infinite_objects) {
    Hit hit_infinite;
    if (object->intersect(r, hit_infinite, t_min)) {
        if (h.getMaterial() && hit_infinite.getT() >= h.getT()) {
            continue;
        }

        if (hit_infinite_closest.getMaterial() == nullptr || hit_infinite.getT() < 				                     hit_infinite_closest.getT()) {
            hit_infinite_closest = hit_infinite;
            infinite_intersected = true;
        }
    }
}

if (infinite_intersected) {
    h = hit_infinite_closest;
}

return info.hit_cell || infinite_intersected;
```

**Output**

grid accelerated bunny

<p align="center"><img src="https://raw.githubusercontent.com/LanFear-Li/MIT6.837-QUIZ/main/Appendix/pa6-1.png" style="zoom: 100%;text-align: left"/></p>

grid accelerated marble vase

<p align="center"><img src="https://raw.githubusercontent.com/LanFear-Li/MIT6.837-QUIZ/main/Appendix/pa6-2.png" style="zoom: 30%;text-align: left"/></p>

grid accelerated MIT6.837 logo

<p align="center"><img src="https://raw.githubusercontent.com/LanFear-Li/MIT6.837-QUIZ/main/Appendix/pa6-3.png" style="zoom: 30%;text-align: left"/></p>

## PA7 - Supersampling and Antialiasing

PA7实现了各类Sampler对于屏幕空间的像素区域进行超采样，同时实现了各类Filter进行滤波，提高图像质量。 

**Sampler**

```c++
Vec2f RandomSampler::getSamplePosition(int n) {
    // random generate points within this pixel [0, 0] -> [1, 1]
    float x = (float) rand() / (float) RAND_MAX;
    float y = (float) rand() / (float) RAND_MAX;
    return {x, y};
}

Vec2f UniformSampler::getSamplePosition(int n) {
    int x = n % this->rank, y = n / this->rank;
    return {this->step * (x + 0.5f), this->step * (y + 0.5f)};
}

Vec2f JitteredSampler::getSamplePosition(int n) {
    Vec2f uniform_loc = UniformSampler::getSamplePosition(n);

    float x = ((float) rand() / (float) RAND_MAX - 0.5f) * this->step;
    float y = ((float) rand() / (float) RAND_MAX - 0.5f) * this->step;

    Vec2f jittered_loc = uniform_loc + Vec2f(x, y);
    jittered_loc.Clamp();
    return jittered_loc;
}
```

**Filter**

```c++
int radius = getSupportRadius();

float weight_sum = 0.0f;
Vec3f color;

// iterate pixels around current pixel (i, j) within radius range
for (int offset_x = -radius; offset_x <= radius; offset_x++) {
    for (int offset_y = -radius; offset_y <= radius; offset_y++) {
        int x = i + offset_x, y = j + offset_y;
        if (x < 0 || x >= film->getWidth() || y < 0 || y >= film->getHeight()) {
            continue;
        }

        // literate all sample of pixel (i + offset_x, j + offset_y)  and accumulate weighted color
        for (int n = 0; n < film->getNumSamples(); n++) {
            Sample sample = film->getSample(x, y, n);

            Vec2f sample_loc = sample.getPosition();
            Vec2f inside_loc(sample_loc.x() - 0.5f + offset_x, sample_loc.y() - 0.5f + offset_y);

            float weight = getWeight(inside_loc.x(), inside_loc.y());
            color += sample.getColor() * weight;
            weight_sum += weight;
        }
    }
}

return color * (1.0f / weight_sum);
```

**Output**

anti-aliased marble vase

<p align="center"><img src="https://raw.githubusercontent.com/LanFear-Li/MIT6.837-QUIZ/main/Appendix/pa7-1.png" style="zoom: 50%;text-align: left"/></p>

anti-aliased MIT6.837 logo

<p align="center"><img src="https://raw.githubusercontent.com/LanFear-Li/MIT6.837-QUIZ/main/Appendix/pa7-2.png" style="zoom: 30%;text-align: left"/></p>

anti-aliased sphere

<p align="center"><img src="https://raw.githubusercontent.com/LanFear-Li/MIT6.837-QUIZ/main/Appendix/pa7-3.png" style="zoom: 30%;text-align: left"/></p>

## PA8 - Curves & Surfaces

PA8实现了Bezier Curve和Bspline Curve的生成以及由Bezier Patch构成的Surface的生成。

Bezier Curve和Bspline Curve都通过四个控制点生成曲线，通过De Casteljau算法结合Bernstein Basis生成了两类Curve。而Surface则通过多条Bezier Curve生成Curve并插值实现。

**Curve Calculation**

```c++
static float Bernstein_Bezier[16] = {
        -1.f, 3.f, -3.f, 1.f,
        3.f, -6.f, 3.f, 0.f,
        -3.f, 3.f, 0.f, 0.f,
        1.f, 0.f, 0.f, 0.f
};

static float Bernstein_BSpline[16] = {
        -1.f, 3.f, -3.f, 1.f,
        3.f, -6.f, 0.f, 4.f,
        -3.f, 3.f, 3.f, 1.f,
        1.f, 0.f, 0.f, 0.f
};

Vec3f CurveSection::calculatePoint(float t) const {
    auto basis = Vec4f(t * t * t, t * t, t, 1);
    (this->G * *this->B).Transform(basis);
    basis.DivideByW();
    return basis.xyz();
}
```

**Surface Generation**

```c++
vector<Vec3f> points = this->curve->tessellatePoint(args);
float angle = 2.0f * M_PI / args->revolution_tessellation;
Matrix rotateMatrix = Matrix::MakeYRotation(angle);

auto *mesh = new TriangleNet(points.size() - 1, args->revolution_tessellation);
for (int i = 0; i <= args->revolution_tessellation; i++) {
    for (int j = 0; j < points.size(); j++) {
        mesh->SetVertex(j, i, points[j]);
        rotateMatrix.Transform(points[j]);
    }
}

return mesh;
```

**Output**

bezier patch

<p align="center"><img src="https://raw.githubusercontent.com/LanFear-Li/MIT6.837-QUIZ/main/Appendix/pa8-1.png" style="zoom: 60%;text-align: left"/></p>

teapot

<p align="center"><img src="https://raw.githubusercontent.com/LanFear-Li/MIT6.837-QUIZ/main/Appendix/pa8-2.png" style="zoom: 60%;text-align: left"/></p>

## PA9 - Particle Systems

PA9实现了计算机动画领域中的粒子系统。实现了各类力场、积分器和粒子生成器的生成。

**ForceField**

```c++
Vec3f GravityForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    return this->gravity;
}

Vec3f ConstantForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    Vec3f result = this->force;
    result /= mass;
    return result;
}

Vec3f RadialForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    Vec3f f = position * this->magnitude;
    f.Negate();
    return f * (1.0f / mass);
}

Vec3f VerticalForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    Vec3f f = Vec3f(0, -position.y(), 0) * this->magnitude;

    return f * (1.0f / mass);
}
```

**Euler Integrator**

```c++
void EulerIntegrator::Update(Particle *particle, ForceField *force_field, float t, float dt) {
    Vec3f acceleration = force_field->getAcceleration(particle->getPosition(), particle->getMass(), t);

    Vec3f position = particle->getPosition();
    Vec3f velocity = particle->getVelocity();

    position += velocity * dt;
    particle->setPosition(position);
    velocity += acceleration * dt;
    particle->setVelocity(velocity);

    particle->increaseAge(dt);
}
```

**Generator**

```c++
HoseGenerator::HoseGenerator(const Vec3f &position, float position_randomness, const Vec3f &velocity,
                             float velocity_randomness) {
    this->position = position;
    this->position_randomness = position_randomness;
    this->velocity = velocity;
    this->velocity_randomness = velocity_randomness;
}

RingGenerator::RingGenerator(float position_randomness, const Vec3f &velocity, float velocity_randomness) {
    this->position_randomness = position_randomness;
    this->velocity = velocity;
    this->velocity_randomness = velocity_randomness;
}
```

**Output**

hose force particle

<p align="center"><img src="https://raw.githubusercontent.com/LanFear-Li/MIT6.837-QUIZ/main/Appendix/pa9-1.png" style="zoom: 80%;text-align: left"/></p>

fire

<p align="center"><img src="https://raw.githubusercontent.com/LanFear-Li/MIT6.837-QUIZ/main/Appendix/pa9-2.png" style="zoom: 80%;text-align: left"/></p>
