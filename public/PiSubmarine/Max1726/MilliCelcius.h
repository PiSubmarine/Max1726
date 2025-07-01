#pragma once

#include <cstdint>
#include <limits>

struct MilliCelsius
{
    constexpr static size_t BitShift = 5;

    // Constructor
    constexpr explicit MilliCelsius(int64_t mC = 0) : value(mC << BitShift) {}

    constexpr int64_t GetMilliCelsius() const
    {
        return value >> BitShift;
    }

    // Convert from raw device value (uint16_t with 2's complement) to MilliCelsius
    static constexpr MilliCelsius FromRaw(int16_t raw)
    {
        int64_t temp = (raw << BitShift) * 1000ULL / 256ULL;
        MilliCelsius result;
        result.value = temp;
        return result;
    }

    // Convert back to raw device value (uint16_t with 2's complement)
    constexpr int16_t ToRaw() const
    {
        int64_t temp = (value * 256 / 1000) >> BitShift;
        return static_cast<int16_t>(temp);
    }


    // Arithmetic operators
    constexpr MilliCelsius operator+(const MilliCelsius& other) const
    {
        MilliCelsius result;
        result.value = this->value + other.value;
        return result;
    }

    constexpr MilliCelsius operator-(const MilliCelsius& other) const
    {
        MilliCelsius result;
        result.value = this->value - other.value;
        return result;
    }

    constexpr MilliCelsius& operator+=(const MilliCelsius& other)
    {
        this->value += other.value;
        return *this;
    }

    constexpr MilliCelsius& operator-=(const MilliCelsius& other)
    {
        this->value -= other.value;
        return *this;
    }

    friend constexpr MilliCelsius operator"" _mC(long double mC);

private:
    int64_t value;
};

// Integer literal: 25375_mC
constexpr MilliCelsius operator"" _mC(unsigned long long mC)
{
    return MilliCelsius(static_cast<int64_t>(mC));
}

// Double literal (PC-only)
constexpr MilliCelsius operator"" _mC(long double mC)
{
    uint64_t value = static_cast<uint64_t>(mC * (1 << MilliCelsius::BitShift));
    MilliCelsius result;
    result.value = value;
    return result;
}
