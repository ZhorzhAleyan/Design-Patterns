package com.example.patterns;

/** Abstract Factory: create a consistent family of UI components. */
public class AbstractFactoryDemo {
    interface Button { String render(); }
    interface Checkbox { String render(); }
    interface UiFactory {
        Button createButton();
        Checkbox createCheckbox();
    }

    static class WindowsFactory implements UiFactory {
        public Button createButton() { return () -> "[Windows Button]"; }
        public Checkbox createCheckbox() { return () -> "[Windows Checkbox]"; }
    }
    static class MacFactory implements UiFactory {
        public Button createButton() { return () -> "(Mac Button)"; }
        public Checkbox createCheckbox() { return () -> "(Mac Checkbox)"; }
    }

    static class SettingsScreen {
        private final Button button;
        private final Checkbox checkbox;

        SettingsScreen(UiFactory factory) {
            button = factory.createButton();
            checkbox = factory.createCheckbox();
        }
        String render() { return button.render() + " " + checkbox.render(); }
    }

    public static void main(String[] args) {
        System.out.println(new SettingsScreen(new WindowsFactory()).render());
        System.out.println(new SettingsScreen(new MacFactory()).render());
    }
}
