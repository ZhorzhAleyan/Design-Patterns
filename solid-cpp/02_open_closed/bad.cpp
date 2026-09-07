// Open/Closed Principle (OCP) — ԽԱԽՏՈՒՄ

// Խնդիրը՝ ամեն նոր ֆիգուր ավելացնելիս ստիպված ենք ՓՈՓՈԽԵԼ
// AreaCalculator-ի կոդը (նոր if/else)։ Կլասը «փակ» չէ փոփոխման։
// Ամեն նոր տեսակ = ռիսկ հին կոդը կոտրելու։

#include <iostream>

enum class ShapeType { Circle, Rectangle };

struct Shape {
    ShapeType type;
    double width;   // Rectangle-ի համար
    double height;  // Rectangle-ի համար
    double radius;  // Circle-ի համար
};

class AreaCalculator {
public:
    double area(const Shape& s) const {
        // Ամեն նոր ֆիգուրի դեպքում պիտի այստեղ նոր ճյուղ ավելացնենք
        if (s.type == ShapeType::Circle) {
            return 3.14159 * s.radius * s.radius;
        } else if (s.type == ShapeType::Rectangle) {
            return s.width * s.height;
        }
        // ... Triangle? Trapezoid? -> էլի պիտի փոխենք այս ֆայլը
        return 0.0;
    }
};

int main() {
    Shape circle{ShapeType::Circle, 0, 0, 5};
    Shape rect{ShapeType::Rectangle, 4, 6, 0};

    AreaCalculator calc;
    std::cout << "Circle area:    " << calc.area(circle) << "\n";
    std::cout << "Rectangle area: " << calc.area(rect) << "\n";
    return 0;
}
