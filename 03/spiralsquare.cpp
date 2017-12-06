#include <spiralsquare.hpp>

#include <cassert>
#include <ostream>

int getRing(int n)
{
    int ring = 1;
    while(ring * ring < n) { ring += 2; }
    return ring;
}

int getDistance(int n)
{
    if(n == 1) return 0;
    int const ring = getRing(n);
    int const ring_diameter = (ring - 1) / 2;
    int const lower_ring = ring - 2;
    int const lower_min_distance = (lower_ring - 1) / 2;

    // The center can be reached by moving to an element in the lower ring that is closest to the origin
    // (which has distance lower_min_distance). The additional distance to such an element varies as we walk around
    // the ring: The value starts at ring_diameter and then oscillates between 1 and ring_diameter+1.
    bool descending = (ring != 3);
    int additional_distance = ring_diameter;
    for(int i = lower_ring*lower_ring + 1; i < n; ++i) {
        if(descending) {
            --additional_distance;
        } else {
            ++additional_distance;
        }
        if(additional_distance == 1) { descending = false; }
        else if(additional_distance > ring_diameter) { descending = true; }
    }

    return lower_min_distance + additional_distance;
}

Field::Field(int dimension)
    :m_dimension(dimension)
{
    assert((dimension - 1) % 2 == 0);
    m_field.resize(m_dimension*m_dimension);
}

int Field::getDimension() const
{
    return m_dimension;
}

void Field::setCell(int x, int y, int v)
{
    x += (m_dimension - 1) / 2;
    y += (m_dimension - 1) / 2;
    assert((x >= 0 && x < m_dimension) && (y >= 0 && y < m_dimension));
    m_field[y*m_dimension + x] = v;
}

int Field::getCell(int x, int y) const
{
    x += (m_dimension - 1) / 2;
    y += (m_dimension - 1) / 2;
    if(!(x >= 0 && x < m_dimension && y >= 0 && y < m_dimension)) {
        return 0;
    }
    return m_field[y*m_dimension + x];
}

void Field::fillField()
{
    int count = 1;
    fill([&count](int x, int y) { return ++count; });
}

void Field::printField(std::ostream& os)
{
    for(int y = 0; y < m_dimension; ++y) {
        for(int x = 0; x < m_dimension; ++x) {
            os << "\t" << m_field[y*m_dimension + x];
        }
        os << "\n";
    }
}

Field::Coordinates Field::getCoordinates(int n) const
{
    assert(n > 0 && n <= m_dimension*m_dimension);
    int limit = (m_dimension - 1) / 2;
    for(int x = -limit; x <= limit; ++x) {
        for(int y = -limit; y <= limit; ++y) {
            if(getCell(x, y) == n) {
                return Coordinates{x, y};
            }
        }
    }

    // should never be reached
    return Coordinates{};
}

int Field::getDistanceFromCenter(int n) const
{
    auto const c = getCoordinates(n);
    return std::abs(c.x) + std::abs(c.y);
}

int Field::fillComplicated(int search_for)
{
    int ret = 0;
    fill([this, search_for, &ret](int x, int y) {
        int acc = 0;
        acc += getCell(x-1, y);
        acc += getCell(x+1, y);
        acc += getCell(x, y-1);
        acc += getCell(x, y+1);
        acc += getCell(x+1, y+1);
        acc += getCell(x-1, y+1);
        acc += getCell(x+1, y-1);
        acc += getCell(x-1, y-1);
        if((ret == 0) && (acc > search_for)) { ret = acc; }
        return acc;
    });
    return ret;
}

void Field::fill(Turtle::MarkerFunc const& marker)
{
    setCell(0, 0, 1);
    Turtle turtle(*this, marker);
    int const limit = m_dimension * m_dimension;
    int step_size = 1;
    int steps = 0;
    while(steps + step_size < limit) {
        turtle.go(step_size);
        turtle.left();
        turtle.go(step_size);
        turtle.left();
        steps += 2*step_size;
        ++step_size;
    }
    turtle.go(step_size - 1);
}

Turtle::Turtle(Field& field, MarkerFunc const& marker_cb)
    :m_x(0), m_y(0), m_direction(Direction::Right), m_field(&field), m_marker(marker_cb)
{}

void Turtle::go(int n)
{
    for(int i=0; i<n; ++i) {
        switch(m_direction) {
        case Direction::Up:    --m_y; break;
        case Direction::Left:  --m_x; break;
        case Direction::Down:  ++m_y; break;
        case Direction::Right: ++m_x; break;
        }
        m_field->setCell(m_x, m_y, m_marker(m_x, m_y));
    }
}

void Turtle::left()
{
    switch(m_direction) {
    case Direction::Up:    m_direction = Direction::Left;   break;
    case Direction::Left:  m_direction = Direction::Down;   break;
    case Direction::Down:  m_direction = Direction::Right;  break;
    case Direction::Right: m_direction = Direction::Up;     break;
    }
}
