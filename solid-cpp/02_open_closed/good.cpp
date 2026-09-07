// Open/Closed Principle (OCP) — ՃԻՇՏ

// Լուծումը՝ Shape աբստրակտ ինտերֆեյս area() մեթոդով։
// Ամեն ֆիգուր ինքն է հաշվում իր մակերեսը։
// Նոր ֆիգուր ավելացնելը = ՆՈՐ կլաս, ԱՌԱՆՑ հին կոդը փոխելու։
//   -> բաց ընդլայնման համար, փակ փոփոխման համար։


#include <iostream>
#include <vector>
#include <memory>

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
};

class Circle : public Shape {
public:
    explicit Circle(double radius) : radius_(radius) {}
    double area() const override { return 3.14159 * radius_ * radius_; }
private:
    double radius_;
};

class Rectangle : public Shape {
public:
    Rectangle(double width, double height) : width_(width), height_(height) {}
    double area() const override { return width_ * height_; }
private:
    double width_;
    double height_;
};

// ՆՈՐ ֆիգուր — ոչ մի հին ֆայլ չենք դիպչում
class Triangle : public Shape {
public:
    Triangle(double base, double height) : base_(base), height_(height) {}
    double area() const override { return 0.5 * base_ * height_; }
private:
    double base_;
    double height_;
};

// Այս կլասը երբեք պետք չէ փոխել նոր ֆիգուրների համար
class AreaCalculator {
public:
    double totalArea(const std::vector<std::unique_ptr<Shape>>& shapes) const {
        double total = 0.0;
        for (const auto& shape : shapes) {
            total += shape->area();
        }
        return total;
    }
};

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(5));
    shapes.push_back(std::make_unique<Rectangle>(4, 6));
    shapes.push_back(std::make_unique<Triangle>(3, 8));

    AreaCalculator calc;
    std::cout << "Total area: " << calc.totalArea(shapes) << "\n";
    return 0;
}
