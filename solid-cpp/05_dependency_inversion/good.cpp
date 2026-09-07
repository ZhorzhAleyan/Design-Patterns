// Dependency Inversion Principle (DIP) — ՃԻՇՏ

// Լուծումը՝ երկու մակարդակներն էլ կախված են ԱԲՍՏՐԱԿՑԻԱՅԻՑ
// (IMessageSender), ոչ թե միմյանցից։
// NotificationService-ը ստանում է կախվածությունը դրսից
// (dependency injection) => հեշտ է փոխել Email -> SMS -> Push
// առանց NotificationService-ը դիպչելու, և հեշտ է test անել։

#include <iostream>
#include <string>
#include <memory>

// Աբստրակցիա, որից կախված են երկու կողմերն էլ
class IMessageSender {
public:
    virtual ~IMessageSender() = default;
    virtual void send(const std::string& message) = 0;
};

// Կոնկրետ իրականացումներ
class EmailSender : public IMessageSender {
public:
    void send(const std::string& message) override {
        std::cout << "[EMAIL] " << message << "\n";
    }
};

class SmsSender : public IMessageSender {
public:
    void send(const std::string& message) override {
        std::cout << "[SMS] " << message << "\n";
    }
};

class PushSender : public IMessageSender {
public:
    void send(const std::string& message) override {
        std::cout << "[PUSH] " << message << "\n";
    }
};

// Բարձր մակարդակի մոդուլ՝ կախված միայն աբստրակցիայից
class NotificationService {
public:
    // Կախվածությունը ներարկվում է դրսից (constructor injection)
    explicit NotificationService(std::shared_ptr<IMessageSender> sender)
        : sender_(std::move(sender)) {}

    void notify(const std::string& message) {
        sender_->send(message);
    }
private:
    std::shared_ptr<IMessageSender> sender_;
};

int main() {
    // Ընտրում ենք իրականացումը դրսից՝ առանց NotificationService-ը փոխելու
    NotificationService emailService(std::make_shared<EmailSender>());
    emailService.notify("Your order has shipped!");

    NotificationService smsService(std::make_shared<SmsSender>());
    smsService.notify("Your code is 1234");

    NotificationService pushService(std::make_shared<PushSender>());
    pushService.notify("You have a new message");
    return 0;
}
