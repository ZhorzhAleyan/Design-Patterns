#include "FluentBuilderDemo.h"

#include <iostream>

#include "Pizza.h"

void runFluentBuilderDemo() {
    Pizza margherita = Pizza::Builder()
                            .setSize(12)
                            .setCrust("thin")
                            .addTopping("mozzarella")
                            .addTopping("basil")
                            .build();

    Pizza meatFeast = Pizza::Builder()
                           .setSize(16)
                           .setCrust("deep pan")
                           .setStuffedCrust(true)
                           .addTopping("pepperoni")
                           .addTopping("sausage")
                           .addTopping("bacon")
                           .build();

    std::cout << "--- Margherita ---\n";
    margherita.showSpecification();

    std::cout << "\n--- Meat feast ---\n";
    meatFeast.showSpecification();
}
