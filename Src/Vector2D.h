#pragma once

class Vector2D
{
public:
    Vector2D() { x = y = 0; }
    Vector2D(int y, int x) { this->y = y; this->x = x; }
    int y;
    int x;

    // Addition operator
    Vector2D operator+(const Vector2D& other) const
    {
        return Vector2D(y + other.y, x + other.x);
    }

    // Subtraction operator
    Vector2D operator-(const Vector2D& other) const
    {
        return Vector2D(y - other.y, x - other.x);
    }

    // Prefix increment operator
    Vector2D& operator++()
    {
        ++y;
        ++x;
        return *this;
    }

    // Postfix increment operator
    Vector2D operator++(int)
    {
        Vector2D tmp(*this);
        ++* this;
        return tmp;
    }

    // Prefix decrement operator
    Vector2D& operator--()
    {
        --y;
        --x;
        return *this;
    }

    // Postfix decrement operator
    Vector2D operator--(int)
    {
        Vector2D tmp(*this);
        --* this;
        return tmp;
    }

    // Compound assignment addition operator
    Vector2D& operator+=(const Vector2D& other)
    {
        y += other.y;
        x += other.x;
        return *this;
    }

    // Compound assignment subtraction operator
    Vector2D& operator-=(const Vector2D& other)
    {
        y -= other.y;
        x -= other.x;
        return *this;
    }

    // Less than operator
    bool operator<(const Vector2D& other) const
    {
        return (x < other.x) || ((x == other.x) && (y < other.y));
    }

    bool operator==(const Vector2D& other) const
    {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Vector2D& other) const
    {
        return x != other.x || y != other.y;
    }
};