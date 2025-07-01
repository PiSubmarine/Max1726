#pragma once

#include <cstdint>
#include <limits>

struct MilliCelsius
{
    constexpr static size_t BitShift = 8;

    // Constructor
    constexpr explicit MilliCelsius(int64_t mC = 0) : value(mC) {}

    constexpr int64_t GetMilliCelsius() const
    {
        return value >> BitShift;
    }

    // Convert from raw device value (uint16_t with 2's complement) to MilliCelsius
    static constexpr MilliCelsius FromRaw(int16_t raw)
    {
        int64_t temp = raw * 256;
        MilliCelsius result;
        result.value = temp;
        return result;
    }

    // Convert back to raw device value (uint16_t with 2's complement)
    constexpr uint16_t ToRaw() const
    {
        int64_t temp = value * 32 + 62;
        int64_t raw = temp / 125;

        if (raw > std::numeric_limits<int16_t>::max()) raw = std::numeric_limits<int16_t>::max();
        if (raw < std::numeric_limits<int16_t>::min()) raw = std::numeric_limits<int16_t>::min();

        return static_cast<uint16_t>(static_cast<int16_t>(raw)); // reinterpret as uint16_t
    }


    // Arithmetic operators
    constexpr MilliCelsius operator+(const MilliCelsius& other) const
    {
        return MilliCelsius(this->value + other.value);
    }

    constexpr MilliCelsius operator-(const MilliCelsius& other) const
    {
        return MilliCelsius(this->value - other.value);
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

private:
    int64_t value;
};

// Integer literal: 25375_mC
constexpr MilliCelsius operator"" _mC(unsigned long long mC)
{
    return MilliCelsius(static_cast<int64_t>(mC));
}

// Double literal (PC-only)
inline MilliCelsius operator"" _mC(long double mC)
{
    return MilliCelsius(static_cast<int64_t>(mC * 1000.0));
}
