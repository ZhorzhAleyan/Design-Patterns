#include "Pizza.h"

#include <iostream>

Pizza::Builder& Pizza::Builder::setSize(int inches) { size_ = inches; return *this; }
Pizza::Builder& Pizza::Builder::setCrust(const std::string& crust) { crust_ = crust; return *this; }
Pizza::Builder& Pizza::Builder::setStuffedCrust(bool stuffed) { stuffedCrust_ = stuffed; return *this; }
Pizza::Builder& Pizza::Builder::addTopping(const std::string& topping) { toppings_.push_back(topping); return *this; }

Pizza Pizza::Builder::build() const { return Pizza(*this); }

Pizza::Pizza(const Builder& b)
    : size_(b.size_), crust_(b.crust_), stuffedCrust_(b.stuffedCrust_), toppings_(b.toppings_) {}

void Pizza::showSpecification() const {
    std::cout << "Pizza specification:\n";
    std::cout << "  Size          : " << size_ << " inches\n";
    std::cout << "  Crust         : " << crust_ << (stuffedCrust_ ? " (stuffed)" : "") << "\n";
    std::cout << "  Toppings      : ";
    for (size_t i = 0; i < toppings_.size(); ++i) {
        std::cout << toppings_[i] << (i + 1 < toppings_.size() ? ", " : "");
    }
    std::cout << (toppings_.empty() ? "none" : "") << "\n";
}
