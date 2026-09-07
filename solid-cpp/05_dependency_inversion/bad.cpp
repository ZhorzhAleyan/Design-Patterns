// Dependency Inversion Principle (DIP) — ԽԱԽՏՈՒՄ

// Խնդիրը՝ բարձր մակարդակի կլասը (NotificationService) ուղղակիորեն
// կախված է ցածր մակարդակի կոնկրետ իրականացումից (EmailSender)։
// Այն ինքն է ստեղծում EmailSender-ը ներսում =>
//   - չես կարող փոխել SMS-ի կամ push-ի առանց այս կլասը փոխելու
//   - դժվար է test անել (չես կարող mock դնել)

#include <iostream>
#include <string>

// Ցածր մակարդակի մոդուլ
class EmailSender {
public:
    void sendEmail(const std::string& message) {
        std::cout << "[EMAIL] " << message << "\n";
    }
};

// Բարձր մակարդակի մոդուլ՝ կոշտ կապված EmailSender-ին
class NotificationService {
public:
    void notify(const std::string& message) {
        // Ուղղակի կախվածություն կոնկրետ կլասից
        emailSender_.sendEmail(message);
    }
private:
    EmailSender emailSender_; // «hardcoded» կախվածություն
};

int main() {
    NotificationService service;
    service.notify("Your order has shipped!");
    // Ի՞նչ կանենք, եթե ուզենք SMS ուղարկել -> պիտի փոխենք այս կլասը
    return 0;
}
