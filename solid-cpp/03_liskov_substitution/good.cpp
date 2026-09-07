// Liskov Substitution Principle (LSP) — ՃԻՇՏ

// Քառակուսին Ուղղանկյուն ՉԷ վարքի իմաստով, ուստի չենք ժառանգում։
// Փոխարենը ունենք ընդհանուր Shape աբստրակցիա, և ամեն ֆիգուրը
// լիարժեք ու ճիշտ իրականացնում է area()-ն։ Ցանկացած Shape
// կարելի է անվտանգ փոխարինել մյուսով։

#include <iostream>
#include <vector>
#include <memory>

class Shape {
public:
    virtual ~Shape() = default;
    virtual int area() const = 0;
};

class Rectangle : public Shape {
public:
    Rectangle(int width, int height) : width_(width), height_(height) {}
    void setWidth(int w) { width_ = w; }
    void setHeight(int h) { height_ = h; }
    int area() const override { return width_ * height_; }
private:
    int width_;
    int height_;
};

class Square : public Shape {
public:
    explicit Square(int side) : side_(side) {}
    void setSide(int s) { side_ = s; }
    int area() const override { return side_ * side_; }
private:
    int side_;
};

// Աշխատում է ցանկացած Shape-ի հետ առանց անակնկալների
void printArea(const Shape& shape) {
    std::cout << "Area = " << shape.area() << "\n";
}

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Rectangle>(5, 4)); // 20
    shapes.push_back(std::make_unique<Square>(4));        // 16

    for (const auto& shape : shapes) {
        printArea(*shape); // Ամեն դեպքում ճիշտ արդյունք
    }
    return 0;
}
