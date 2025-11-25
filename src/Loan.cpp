// src/Loan.cpp
#include "Loan.h"
#include <cmath>
#include <limits>

Loan::Loan(long double principal, long double annualInterestPercent, int months)
    : principal_(principal), annualRatePercent_(annualInterestPercent), months_(months) { }

void Loan::validate() const {
    if (!std::isfinite(principal_) || principal_ <= 0.0L) {
        throw std::invalid_argument("principal must be positive and finite");
    }
    if (!std::isfinite(annualRatePercent_) ||
        annualRatePercent_ < 0.0L ||
        annualInterestPercent_ >= MAX_REASONABLE_RATE) {
        throw std::invalid_argument("annual interest percent is invalid or out of range");
    }
    if (months_ <= 0 || static_cast<long double>(months_) > MAX_REASONABLE_MONTHS) {
        throw std::invalid_argument("months must be a positive reasonable integer");
    }
}

long double Loan::monthlyEMI() const {
    // monthly rate
    long double r = (annualRatePercent_ / 100.0L) / 12.0L;
    long double P = principal_;
    int n = months_;

    if (n <= 0) {
        throw std::logic_error("invalid tenure (months) in monthlyEMI");
    }

    if (r == 0.0L) {
        // zero-interest loan
        return P / (long double)n;
    }

    // Use long double powl for better precision/scale
    long double onePlusR = 1.0L + r;
    long double denomPart = std::powl(onePlusR, (long double)n);
    if (!std::isfinite(denomPart) || denomPart == 1.0L) {
        // fallback: avoid division by zero or infinity — use alternative stable method
        // EMI = P * r / (1 - (1+r)^-n)
        long double inv = std::powl(onePlusR, -(long double)n);
        if (!std::isfinite(inv) || (1.0L - inv) == 0.0L) {
            throw std::overflow_error("numeric overflow computing EMI");
        }
        return (P * r) / (1.0L - inv);
    }

    long double numerator = P * r * denomPart;
    long double denominator = denomPart - 1.0L;
    return numerator / denominator;
}

long double Loan::balanceAfterMonths(int paidMonths) const {
    if (paidMonths <= 0) return principal_;
    if (paidMonths >= months_) return 0.0L;

    long double r = (annualRatePercent_ / 100.0L) / 12.0L;
    long double emi = monthlyEMI();
    long double onePlusR = 1.0L + r;

    long double x_n = std::powl(onePlusR, (long double)months_);
    long double x_k = std::powl(onePlusR, (long double)paidMonths);

    // Using formula:
    // Remaining balance = P*(1+r)^n - EMI * ((1+r)^n - (1+r)^k) / r
    // then divide by (1+r)^n to get present outstanding principal
    if (r == 0.0L) {
        // zero interest: balance is simple principal - paidMonths*EMI
        return principal_ - (emi * (long double)paidMonths);
    }

    if (!std::isfinite(x_n) || !std::isfinite(x_k)) {
        throw std::overflow_error("numeric overflow computing balance");
    }

    long double rem = (principal_ * x_n - emi * ((x_n - x_k) / r)) / x_n;
    return rem;
}

long double Loan::principal() const noexcept { return principal_; }
long double Loan::annualRate() const noexcept { return annualRatePercent_; }
int Loan::months() const noexcept { return months_; }
