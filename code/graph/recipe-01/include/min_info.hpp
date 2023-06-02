#pragma once

// priority queue data used by minimum_path() and min_spanning_tree() algorithms
class MinInfo
{
public:
    int endV;
    int pathWeight;

    friend bool operator< (MinInfo lhs, MinInfo rhs)
    { return lhs.pathWeight < rhs.pathWeight; }

    friend bool operator== (MinInfo lhs, MinInfo rhs)
    { return lhs.pathWeight == rhs.pathWeight; }

    friend bool operator> (MinInfo lhs, MinInfo rhs)
    { return lhs.pathWeight > rhs.pathWeight; }
};
