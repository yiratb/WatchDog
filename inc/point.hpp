#include <iostream>

class Point
{
public:
    Point(size_t x_, size_t y_);
    ~Point() = default;
    Point(const Point &other_) = default;
    Point& operator=(const Point &other_) = default;
private:
    size_t m_x;
    size_t m_y;
};
