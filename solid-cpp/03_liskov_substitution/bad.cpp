// Liskov Substitution Principle (LSP) — ԽԱԽՏՈՒՄ

// Դասական օրինակ՝ Rectangle / Square։
// Square-ը ժառանգում է Rectangle-ից, բայց փոխում է վարքը այնպես,
// որ Rectangle-ի ակնկալիքները կոտրվում են։
// setWidth-ը «գաղտնի» փոխում է նաև height-ը => ֆունկցիան,
// որ աշխատում է Rectangle-ի հետ, սխալ արդյունք է տալիս Square-ի դեպքում։


#include <iostream>
#include <cassert>

class Rectangle {
public:
    virtual ~Rectangle() = default;
    virtual void setWidth(int w) { width_ = w; }
    virtual void setHeight(int h) { height_ = h; }
    int width() const { return width_; }
    int height() const { return height_; }
    int area() const { return width_ * height_; }
protected:
    int width_ = 0;
    int height_ = 0;
};

class Square : public Rectangle {
public:
    // Square-ը կողմերը պարտադրում է հավասար լինել
    void setWidth(int w) override { width_ = w; height_ = w; }
    void setHeight(int h) override { width_ = h; height_ = h; }
};

// Ֆունկցիա, որ ակնկալում է Rectangle-ի «նորմալ» վարք
void resizeAndCheck(Rectangle& r) {
    r.setWidth(5);
    r.setHeight(4);
    std::cout << "Expected area = 20, got " << r.area() << "\n";
    // Rectangle-ի դեպքում -> 20 (ճիշտ)
    // Square-ի դեպքում    -> 16 (ՍԽԱԼ), որովհետև setHeight-ը փոխեց width-ն էլ
}

int main() {
    Rectangle rect;
    resizeAndCheck(rect);   // OK

    Square square;
    resizeAndCheck(square); // Կոտրվում է ակնկալիքը
    return 0;
}
