package com.example.patterns;

public class FactoryDemo {
    interface Notification { String send(String recipient, String message); }

    static class EmailNotification implements Notification {
        public String send(String recipient, String message) {
            return "EMAIL to " + recipient + ": " + message;
        }
    }
    static class SmsNotification implements Notification {
        public String send(String recipient, String message) {
            return "SMS to " + recipient + ": " + message;
        }
    }

    enum Channel { EMAIL, SMS }

    static class NotificationFactory {
        Notification create(Channel channel) {
            return switch (channel) {
                case EMAIL -> new EmailNotification();
                case SMS -> new SmsNotification();
            };
        }
    }

    public static void main(String[] args) {
        NotificationFactory factory = new NotificationFactory();
        System.out.println(factory.create(Channel.EMAIL).send("alice@example.com", "Welcome!"));
        System.out.println(factory.create(Channel.SMS).send("+123456789", "Your code is 1234"));
    }
}
