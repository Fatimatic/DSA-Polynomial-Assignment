#include "polynomial.h"
#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
#include <functional>

class DerivedPolynomial : public Polynomial {
private:
    std::map<int, int, std::greater<int>> terms; // exponent -> coefficient

public:
    void insertTerm(int coefficient, int exponent) override {
        if (exponent < 0 || coefficient == 0) return;
        terms[exponent] += coefficient;
        if (terms[exponent] == 0) terms.erase(exponent);
    }

    std::string toString() const override {
        if (terms.empty()) return "0";
        std::ostringstream out;
        bool first = true;
        for (auto &t : terms) {
            int e = t.first;
            int c = t.second;
            if (!first) out << (c >= 0 ? " + " : " - ");
            else if (c < 0) out << "-";
            first = false;
            int absC = std::abs(c);
            if (e == 0) out << absC;
            else {
                if (absC != 1) out << absC;
                out << "x";
                if (e != 1) out << "^" << e;
            }
        }
        return out.str();
    }

    DerivedPolynomial addPoly(const DerivedPolynomial &other) const {
        DerivedPolynomial result = *this;
        for (auto &t : other.terms) {
            result.terms[t.first] += t.second;
            if (result.terms[t.first] == 0) result.terms.erase(t.first);
        }
        return result;
    }

    DerivedPolynomial multiplyPoly(const DerivedPolynomial &other) const {
        DerivedPolynomial result;
        for (auto &a : terms) {
            for (auto &b : other.terms) {
                result.terms[a.first + b.first] += a.second * b.second;
                if (result.terms[a.first + b.first] == 0)
                    result.terms.erase(a.first + b.first);
            }
        }
        return result;
    }

    DerivedPolynomial derivativePoly() const {
        DerivedPolynomial result;
        for (auto &t : terms) {
            if (t.first > 0) result.terms[t.first - 1] = t.first * t.second;
        }
        return result;
    }
};

#ifdef TEST_POLYNOMIAL
int main() {
    DerivedPolynomial p1, p2, p3;

    p1.insertTerm(3, 4);
    p1.insertTerm(2, 2);
    p1.insertTerm(-1, 1);
    p1.insertTerm(5, 0);

    p2.insertTerm(1, 4);
    p2.insertTerm(1, 0);

    DerivedPolynomial sum = p1.addPoly(p2);
    p3.insertTerm(2, 1);

    DerivedPolynomial prod = sum.multiplyPoly(p3);
    DerivedPolynomial deriv = p1.derivativePoly();

    std::cout << "p1.toString(): " << p1.toString() << std::endl;
    std::cout << "sum.toString(): " << sum.toString() << std::endl;
    std::cout << "prod.toString(): " << prod.toString() << std::endl;
    std::cout << "deriv.toString(): " << deriv.toString() << std::endl;
    return 0;
}
#endif
