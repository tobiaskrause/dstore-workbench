#pragma once
#include <ostream>

namespace Color {
    enum Code {
        RED = 31,
        GREEN = 32,
        YELLOW = 33,
        DEFAULT = 39,
    };
    class Modifier {
        Code _code;
    public:
        Modifier(Code code) : _code(code) {}
        friend std::ostream&
        operator<<(std::ostream& os, const Modifier& modifier) {
            return os << "\033[" << modifier._code << "m";
        }
    };

    static Modifier Red(Color::RED);
    static Modifier Green(Color::GREEN);
    static Modifier Yellow(Color::YELLOW);
    static Modifier Default(Color::DEFAULT);
};

