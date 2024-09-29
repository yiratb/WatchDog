#include <vector> //std::vector

#include "point.hpp"

class Piece
{
public:
    enum Side
    {
        RIGHT = 0,
        LEFT
    };
    Piece();
    ~Piece();
    void Move(size_t dx_, size_t dy_);
    void Rotate(Side size_);
private:
    std::vector<Point> m_shape;
};