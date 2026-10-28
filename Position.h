#ifndef POSITION_H
#define POSITION_H

struct Position {
    int y{0};
    int x{0};

    constexpr bool operator==(const Position& other) const = default;
};

#endif // POSITION_H

