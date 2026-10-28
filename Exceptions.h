#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>


class SimulationException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class OutOfBoundsException : public SimulationException {
public:
    OutOfBoundsException(int y, int x)
        : SimulationException("Coordinates (" + std::to_string(y) + ", " + std::to_string(x) + ") are out of world bounds.") {}
};

class CellOccupiedException : public SimulationException {
public:
    CellOccupiedException(int y, int x)
        : SimulationException("Cell (" + std::to_string(y) + ", " + std::to_string(x) + ") is already occupied.") {}
};

class EntityNotFoundException : public SimulationException {
public:
    EntityNotFoundException(int y, int x)
        : SimulationException("No organism found at position (" + std::to_string(y) + ", " + std::to_string(x) + ").") {}
};

class UnknownSpeciesException : public SimulationException {
public:
    explicit UnknownSpeciesException(const std::string& species)
        : SimulationException("Unknown organism species: " + species) {}
};

#endif // EXCEPTIONS_H

