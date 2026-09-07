// Interface Segregation Principle (ISP) — ԽԱԽՏՈՒՄ

// Խնդիրը՝ մեկ «գեր» ինտերֆեյս (IMachine), որ պարունակում է
// print / scan / fax։ Պարզ տպիչը (SimplePrinter) ստիպված է
// իրականացնել scan()-ն ու fax()-ը, որոնք իրեն պետք չեն =>
// հարկադրված է հենվել մեթոդների վրա, որ չի օգտագործում։

#include <iostream>
#include <stdexcept>

// Չափազանց մեծ ինտերֆեյս
class IMachine {
public:
    virtual ~IMachine() = default;
    virtual void print(const std::string& doc) = 0;
    virtual void scan(const std::string& doc) = 0;
    virtual void fax(const std::string& doc) = 0;
};

// Multifunction սարք — իրեն երեքն էլ պետք են, OK
class MultiFunctionPrinter : public IMachine {
public:
    void print(const std::string& doc) override { std::cout << "Print: " << doc << "\n"; }
    void scan(const std::string& doc) override  { std::cout << "Scan: "  << doc << "\n"; }
    void fax(const std::string& doc) override   { std::cout << "Fax: "   << doc << "\n"; }
};

// Պարզ տպիչ — ստիպված է իրականացնել իրեն ավելորդ մեթոդները
class SimplePrinter : public IMachine {
public:
    void print(const std::string& doc) override { std::cout << "Print: " << doc << "\n"; }

    // Ստիպված ենք գրել, բայց իմաստ չունեն -> վատ նշան
    void scan(const std::string&) override {
        throw std::logic_error("SimplePrinter cannot scan!");
    }
    void fax(const std::string&) override {
        throw std::logic_error("SimplePrinter cannot fax!");
    }
};

int main() {
    SimplePrinter printer;
    printer.print("report.pdf");

    try {
        printer.scan("report.pdf"); // Runtime-ում կկոտրվի
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
    return 0;
}
