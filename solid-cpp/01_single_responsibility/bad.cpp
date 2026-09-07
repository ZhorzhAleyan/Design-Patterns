// Single Responsibility Principle (SRP) — ԽԱԽՏՈՒՄ

// Խնդիրը՝ User կլասը շատ բան է անում միանգամից.
//   1) պահում է օգտատիրոջ տվյալները
//   2) պահպանում է բազայում
//   3) ուղարկում է email
// Այս կլասն ունի փոխվելու մի քանի պատճառ (բազայի, email-ի, տվյալների
// փոփոխություն) => խախտում է SRP-ն։

#include <iostream>
#include <string>

class User {
public:
    User(std::string name, std::string email)
        : name_(std::move(name)), email_(std::move(email)) {}

    // Պատասխանատվություն 1: տվյալների կառավարում
    std::string name() const { return name_; }
    std::string email() const { return email_; }

    // Պատասխանատվություն 2: բազայի հետ աշխատանք (այստեղ չպիտի լիներ)
    void saveToDatabase() const {
        std::cout << "[DB] Saving user " << name_ << " to database...\n";
        // ... SQL կոդ ...
    }

    // Պատասխանատվություն 3: email ուղարկելը (այստեղ չպիտի լիներ)
    void sendWelcomeEmail() const {
        std::cout << "[EMAIL] Sending welcome email to " << email_ << "...\n";
        // ... SMTP կոդ ...
    }

private:
    std::string name_;
    std::string email_;
};

int main() {
    User user("Anna", "anna@example.com");
    user.saveToDatabase();
    user.sendWelcomeEmail();
    return 0;
}
