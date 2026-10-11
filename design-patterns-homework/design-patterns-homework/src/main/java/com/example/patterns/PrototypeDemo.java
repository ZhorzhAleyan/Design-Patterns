package com.example.patterns;

import java.util.ArrayList;
import java.util.List;

/** Prototype: make independent copies of an existing configured object. */
public class PrototypeDemo {
    static class GameCharacter {
        private final String name;
        private int health;
        private final List<String> equipment;

        GameCharacter(String name, int health, List<String> equipment) {
            this.name = name;
            this.health = health;
            this.equipment = new ArrayList<>(equipment);
        }

        GameCharacter copy() {
            return new GameCharacter(name, health, equipment); // deep-copy mutable list
        }

        void equip(String item) { equipment.add(item); }
        void damage(int points) { health -= points; }
        int health() { return health; }
        List<String> equipment() { return List.copyOf(equipment); }
        @Override public String toString() {
            return name + " (HP=" + health + ", equipment=" + equipment + ")";
        }
    }

    public static void main(String[] args) {
        GameCharacter template = new GameCharacter("Knight", 100, List.of("Sword", "Shield"));
        GameCharacter copy = template.copy();
        copy.damage(20);
        copy.equip("Potion");
        System.out.println("Template: " + template);
        System.out.println("Copy:     " + copy);
        if (template.health() != 100 || template.equipment().contains("Potion")) {
            throw new AssertionError("Prototype copy changed the original!");
        }
    }
}
