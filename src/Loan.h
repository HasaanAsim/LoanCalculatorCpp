// src/Loan.h
#ifndef LOAN_H
#define LOAN_H

#include <stdexcept>
#include <string>

/**
 * @file Loan.h
 * @brief Loan/EMI calculator class interface.
 *
 * Uses long double internally to reduce overflow risk for large tenures.
 */

/**
 * @class Loan
 * @brief Represents a simple loan and provides EMI / balance calculations.
 *
 * The class does not perform I/O. Call validate() before using calculation methods.
 */
class Loan {
public:
    /**
     * @brief Construct a new Loan object
     * @param principal The loan principal (amount borrowed). Must be > 0.
     * @param annualInterestPercent Annual interest rate in percent (e.g., 12.0 for 12%).
     * @param months Number of months for the loan (tenure). Must be > 0.
     */
    Loan(long double principal, long double annualInterestPercent, int months);

    /**
     * @brief Validate inputs and throw std::invalid_argument if invalid.
     *
     * Checks:
     *  - principal is finite and > 0
     *  - annualInterestPercent is finite and >= 0 and < reasonable upper bound
     *  - months is > 0 and not unreasonably large
     */
    void validate() const;

    /**
     * @brief Calculate monthly EMI using the standard formula.
     * @return Monthly payment amount (EMI) as long double.
     *
     * Formula:
     *   EMI = P * r * (1+r)^n / ((1+r)^n - 1)
     * where r = monthly rate = annualRatePercent / 100 / 12
     *
     * If monthly rate is 0, EMI = P / n.
     */
    long double monthlyEMI() const;

    /**
     * @brief Remaining balance after paidMonths payments.
     * @param paidMonths number of payments already made (>= 0)
     * @return Remaining balance (if paidMonths >= months, returns 0.0).
     */
    long double balanceAfterMonths(int paidMonths) const;

    // Simple accessors
    long double principal() const noexcept;
    long double annualRate() const noexcept;
    int months() const noexcept;

private:
    long double principal_;
    long double annualRatePercent_;
    int months_;

    static constexpr long double MAX_REASONABLE_MONTHS = 1000000.0L; ///< safety cap
    static constexpr long double MAX_REASONABLE_RATE = 1000.0L;      ///< arbitrary cap to avoid insane inputs
};

#endif // LOAN_H
