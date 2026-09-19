#pragma once

#include <string>
#include <vector>

// ============================================================
// VARIANT 2 — Fluent (modern) Builder
//
// No abstract Builder interface and no Director here: there is
// only one way to build a Pizza, so a single nested Builder class
// is enough. Each setter returns *this (a reference to the
// builder), so calls can be chained; build() performs the actual
// construction and returns the finished, immutable Product.
//
// Roles in this example:
//   Product -> Pizza
//   Builder -> Pizza::Builder (nested, chainable)
// ============================================================
class Pizza {
public:
    class Builder {
    public:
        Builder& setSize(int inches);
        Builder& setCrust(const std::string& crust);
        Builder& setStuffedCrust(bool stuffed);
        Builder& addTopping(const std::string& topping);

        // Performs validation/defaults and produces the final, immutable Product.
        Pizza build() const;

    private:
        friend class Pizza;

        int size_ = 12;
        std::string crust_ = "regular";
        bool stuffedCrust_ = false;
        std::vector<std::string> toppings_;
    };

    void showSpecification() const;

private:
    // Private constructor: a Pizza can only be created through its Builder.
    explicit Pizza(const Builder& b);

    int size_;
    std::string crust_;
    bool stuffedCrust_;
    std::vector<std::string> toppings_;
};
