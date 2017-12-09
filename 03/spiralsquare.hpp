#ifndef ADVENT_OF_CODE_03_SPIRALSQUARE_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_03_SPIRALSQUARE_HPP_INCLUDE_GUARD

#include <iosfwd>
#include <functional>
#include <vector>

/** The spiral is formed of rings of odd numbers: 1, 3, 5, 7...
 * The complete spiral up to that ring is made up of the ring^2 numbers: 1, 9, 25, 49...
 */
int getRing(int n);

int getDistance(int n);

class Field;

class Turtle {
public:
    using MarkerFunc = std::function<int(int, int)>;
private:
    int m_x;
    int m_y;
    enum class Direction { Up, Left, Down, Right } m_direction;
    Field* m_field;
    MarkerFunc m_marker;
public:
    Turtle(Field& field, MarkerFunc const& marker_cb);

    void go(int n);

    void left();
};

class Field {
public:
    struct Coordinates { int x; int y; };
private:
    int m_dimension;
    std::vector<int> m_field;
public:
    Field(int dimension);

    void setCell(int x, int y, int v);

    int getCell(int x, int y) const;

    void fillField();

    void printField(std::ostream& os);

    Coordinates getCoordinates(int n) const;

    int getDistanceFromCenter(int n) const;

    int fillComplicated(int search_for);
private:
    void fill(Turtle::MarkerFunc const& marker);
};



#endif
