package com.example.patterns;

import java.math.BigDecimal;
import java.util.List;

/** Run with: java -ea -cp out com.example.patterns.PatternTests */
public class PatternTests {
    public static void main(String[] args) {
        var original = new PrototypeDemo.GameCharacter("Mage", 90, List.of("Staff"));
        var copied = original.copy();
        copied.equip("Ring");
        copied.damage(10);
        assert original.health() == 90;
        assert !original.equipment().contains("Ring");
        assert copied.equipment().contains("Ring");

        var factory = new FactoryDemo.NotificationFactory();
        assert factory.create(FactoryDemo.Channel.EMAIL) instanceof FactoryDemo.EmailNotification;
        assert factory.create(FactoryDemo.Channel.SMS) instanceof FactoryDemo.SmsNotification;

        assert new AbstractFactoryDemo.SettingsScreen(new AbstractFactoryDemo.WindowsFactory())
                .render().contains("Windows Checkbox");
        assert new AbstractFactoryDemo.SettingsScreen(new AbstractFactoryDemo.MacFactory())
                .render().contains("Mac Button");

        var adapter = new AdapterDemo.LegacyGatewayAdapter(new AdapterDemo.LegacyGateway());
        assert adapter.pay(new BigDecimal("10.50")).equals("Charged 1050 cents");
        boolean rejected = false;
        try { adapter.pay(new BigDecimal("1.005")); }
        catch (ArithmeticException expected) { rejected = true; }
        assert rejected;

        var root = new CompositeDemo.FolderNode("root");
        var nested = new CompositeDemo.FolderNode("nested");
        nested.add(new CompositeDemo.FileNode("x", 50));
        root.add(new CompositeDemo.FileNode("y", 25));
        root.add(nested);
        assert root.sizeInBytes() == 75;
        boolean cycleRejected = false;
        try { nested.add(root); }
        catch (IllegalArgumentException expected) { cycleRejected = true; }
        assert cycleRejected;
        System.out.println("All 5 pattern tests passed.");
    }
}
