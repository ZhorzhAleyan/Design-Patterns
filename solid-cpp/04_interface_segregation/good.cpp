// Interface Segregation Principle (ISP) — ՃԻՇՏ

// Լուծումը՝ մի մեծ ինտերֆեյսը բաժանում ենք փոքր, կոնկրետների.
//   IPrinter, IScanner, IFax։
// Ամեն կլաս իրականացնում է ՄԻԱՅՆ այն, ինչ իրեն պետք է։
// SimplePrinter-ը այլևս ստիպված չէ իմանալ scan/fax-ի մասին։

#include <iostream>
#include <string>

// Փոքր, կենտրոնացած ինտերֆեյսներ
class IPrinter {
public:
    virtual ~IPrinter() = default;
    virtual void print(const std::string& doc) = 0;
};

class IScanner {
public:
    virtual ~IScanner() = default;
    virtual void scan(const std::string& doc) = 0;
};

class IFax {
public:
    virtual ~IFax() = default;
    virtual void fax(const std::string& doc) = 0;
};

// Պարզ տպիչ — միայն այն, ինչ պետք է
class SimplePrinter : public IPrinter {
public:
    void print(const std::string& doc) override { std::cout << "Print: " << doc << "\n"; }
};

// Multifunction սարք — համատեղում է մի քանի ինտերֆեյս
class MultiFunctionPrinter : public IPrinter, public IScanner, public IFax {
public:
    void print(const std::string& doc) override { std::cout << "Print: " << doc << "\n"; }
    void scan(const std::string& doc) override  { std::cout << "Scan: "  << doc << "\n"; }
    void fax(const std::string& doc) override   { std::cout << "Fax: "   << doc << "\n"; }
};

int main() {
    SimplePrinter simple;
    simple.print("report.pdf"); // Ոչ մի ավելորդ մեթոդ

    MultiFunctionPrinter mfp;
    mfp.print("report.pdf");
    mfp.scan("photo.jpg");
    mfp.fax("contract.pdf");
    return 0;
}
