#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <vector>

struct Point3d
{
    double x;
    double y;
    double z;

    Point3d(): x(0), y(0), z(0){}
    Point3d(double x, double y, double z):x(x), y(y), z(z){}

    Point3d operator- (const Point3d &p) {
        return Point3d(x - p.x, y - p.y, z - p.z);
    }

    Point3d operator^(const Point3d &p) {
        return Point3d(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
    }

    double operator*(const Point3d &p) {
        return x * p.x + y * p.y + z * p.z;
    }
    void print(){ std::cout << x << " " << y << " " << z << std::endl;}
};

struct Triangle
{
    Point3d a;
    Point3d b;
    Point3d c;
    Point3d normal;
    Triangle(Point3d a, Point3d b, Point3d c, Point3d normal): a(a), b(b), c(c), normal(normal){}
    Triangle(Point3d a, Point3d b, Point3d c): a(a), b(b), c(c) {}
    void print() {
        a.print();
        b.print();
        c.print();
        std::cout << std::endl;
    }

    bool underVertex(Point3d vertex) const{
        return a.z <= vertex.z || b.z <= vertex.z || c.z <= vertex.z;
    }
};

double calcVolume(Point3d a, Point3d b, Point3d c, Point3d d)
{
    return ((a - b) * ((b - d) ^ (c - d))) / 6;
}

double calcVolume(Triangle triangle, Point3d d)
{
    return calcVolume(triangle.a, triangle.b, triangle.c, d);
}

double calcTankVolume(Point3d vertex, const std::vector<Triangle> & triangles)
{
    double volume = 0;
    for(size_t i = 0; i < triangles.size(); ++i) {
        if(triangles[i].underVertex(vertex)) volume += calcVolume(triangles[i], vertex);
    }
    return volume;
}

int main(int argc, char *argv[])
{
    double V = atof(argv[1]);


    std::ifstream input("tank.stl");

    std::vector<Triangle> triangles;

    std::string trash;
    input >> trash >> trash;

    double maxX = -10000, maxY = -10000, minX = 10000, minY = 10000, maxZ = -10000, minZ = 10000;

    input >> trash;
    while  (trash != "endsolid") {
        input >> trash;
        double x, y, z;
        input >> x >> y >> z;
        Point3d normal(x, y, z);
        input >> trash >> trash;
        input >> trash >> x >> y >> z;
        Point3d a(x, y, z);
        if(x > maxX) maxX = x; if(x < minX) minX = x;
        if(y > maxY) maxY = y; if(y < minY) minY = y;
        if(z > maxZ) maxZ = z; if(z < minZ) minZ = z;
        input >> trash >> x >> y >> z;
        Point3d b(x, y, z);
        if(x > maxX) maxX = x; if(x < minX) minX = x;
        if(y > maxY) maxY = y; if(y < minY) minY = y;
        if(z > maxZ) maxZ = z; if(z < minZ) minZ = z;
        input >> trash >> x >> y >> z;
        Point3d c(x, y, z);
        if(x > maxX) maxX = x; if(x < minX) minX = x;
        if(y > maxY) maxY = y; if(y < minY) minY = y;
        if(z > maxZ) maxZ = z; if(z < minZ) minZ = z;
        input >> trash >> trash >> trash;
        triangles.push_back(Triangle (a, b, c, normal));
    }
    double x = (maxX + minX) / 2;
    double y = (maxY + minY) / 2;
    double z = (maxZ + minZ) / 2;

    Point3d vertex(x, y, z);
    double dv;
    double dz;
    size_t i = 0;
    do {
        ++i;
        double realV = calcTankVolume(vertex, triangles);
        dv = V - realV;
        dz = (maxZ - minZ);
        if(dv > 0) minZ = z;
        else maxZ = z;
        z = (maxZ + minZ) / 2;
        vertex.z = z;

    } while((i < 1000) && ((abs(dz) > 0.00001) || (abs(dv) > 0.00001)));
    
    std::cout << "Уровень жидкости = " << z << std::endl;
    return 0;
}