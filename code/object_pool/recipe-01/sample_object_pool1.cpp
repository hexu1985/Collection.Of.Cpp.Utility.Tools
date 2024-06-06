#include "object_pool.hpp"
#include <iostream>

struct Point {
  int x, y;
  void deinit() { std::cout << "Hello " << x << ", " << y << "\n"; }
};

int main() {
  ObjectPool<Point, 10> points;

  Point& a = points.get(); 
  a.x = 10; a.y=20;
  std::cout << "Point a (" << a.x << ", " << a.y << ") initialized, requested " <<
    points.requested() << std::endl;

  Point& b = points.get(); 
  std::cout << "Point b (" << b.x << ", " << b.y << ") not initialized, requested " <<
    points.requested() << std::endl;

  points.free(a);
  std::cout << "Point a(" << a.x << ", " << a.y << ") returned, requested " <<
    points.requested() << std::endl;

  Point& c = points.get();
  std::cout << "Point c(" << c.x << ", " << c.y << ") not intialized, requested " <<
    points.requested() << std::endl;

  Point local;
  try {
    points.free(local);
  } catch (std::runtime_error e) {
    std::cout << "Exception caught: " << e.what() << std::endl;
  }
}
