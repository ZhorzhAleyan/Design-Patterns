// Single Responsibility Principle (SRP) — ՃԻՇՏ

// Լուծումը՝ ամեն պատասխանատվություն առանձին կլասում.
//   - User            -> միայն տվյալներ
//   - UserRepository  -> միայն բազայի հետ աշխատանք
//   - EmailService    -> միայն email ուղարկելը
// Հիմա ամեն կլաս ունի փոխվելու ընդամենը մեկ պատճառ։

#include <iostream>
#include <string>

// Միայն տվյալները
class User {
public:
    User(std::string name, std::string email)
        : name_(std::move(name)), email_(std::move(email)) {}

    std::string name() const { return name_; }
    std::string email() const { return email_; }

private:
    std::string name_;
    std::string email_;
};

// Միայն բազայի պատասխանատվությունը
class UserRepository {
public:
    void save(const User& user) const {
        std::cout << "[DB] Saving user " << user.name() << " to database...\n";
    }
};

// Միայն email-ի պատասխանատվությունը
class EmailService {
public:
    void sendWelcomeEmail(const User& user) const {
        std::cout << "[EMAIL] Sending welcome email to " << user.email() << "...\n";
    }
};

int main() {
    User user("Anna", "anna@example.com");

    UserRepository repository;
    EmailService emailService;

    repository.save(user);
    emailService.sendWelcomeEmail(user);
    return 0;
}
