#include <gtest/gtest.h>

#include "PiSubmarine/Max1726/MicroAmpereHours.h"
#include "PiSubmarine/Max1726/MicroAmperes.h"
#include "PiSubmarine/Max1726/MicroVolts.h"
#include "PiSubmarine/Max1726/MilliCelcius.h"
#include "PiSubmarine/RegUtils.h"

namespace PiSubmarine::Max1726
{
    // ---------------------
    // MicroAmperes Tests
    // ---------------------

    TEST(MicroAmperesTest, FromAndToRaw)
    {
        constexpr int64_t rSense = 10000;
        constexpr double uAFactor = 1.5625 * rSense / 100;

        for (int32_t max1726Value = std::numeric_limits<int16_t>::min(); max1726Value <= std::numeric_limits<int16_t>::max(); max1726Value += 1)
        {
            MicroAmperes current = MicroAmperes::FromRaw(max1726Value);
            int16_t roundtrip = current.ToRaw();
            ASSERT_EQ(roundtrip, max1726Value);

            double uAD = max1726Value * uAFactor;
            auto uAi = current.GetMicroAmperes();
            EXPECT_NEAR(static_cast<double>(uAi), uAD, 1.0);
        }
    }

    TEST(MicroAmperesTest, LiteralsAndArithmetic)
    {
        auto a = 1000_uA;
        auto b = 500_uA;
        EXPECT_EQ((a + b).GetMicroAmperes(), 1500);
        EXPECT_EQ((a - b).GetMicroAmperes(), 500);
        a += b;
        EXPECT_EQ(a.GetMicroAmperes(), 1500);
        a -= b;
        EXPECT_EQ(a.GetMicroAmperes(), 1000);
    }

    TEST(MicroAmperesTest, FromRaw)
    {
        constexpr double uVFactor = 78.125 / 1000000;
        for (int32_t max1726Value = std::numeric_limits<int16_t>::min(); max1726Value <= std::numeric_limits<int16_t>::max(); max1726Value += 1)
        {
            MicroAmperes current = MicroAmperes::FromRaw(max1726Value);
            int16_t roundtrip = current.ToRaw();
            ASSERT_EQ(roundtrip, max1726Value);
        }
    }

    // ---------------------
    // MicroVolts Tests
    // ---------------------

    TEST(MicroVoltsTest, FromAndToRaw)
    {
        constexpr double uVFactor = 1.0/128.0 * 10000;

        for (uint16_t raw = 0; raw < 65535; raw += 1)
        {
            double uVD = raw * uVFactor;
            MicroVolts v = MicroVolts::FromRaw(raw);
            uint16_t roundtrip = v.ToRaw();
            
            EXPECT_EQ(roundtrip, raw);
            auto uVi = v.GetMicroVolts();
            EXPECT_NEAR(static_cast<double>(uVi), uVD, 1.0);
        }
    }

    TEST(MicroVoltsTest, LiteralsAndArithmetic)
    {
        auto a = 330000_uV;
        auto b = 120000_uV;
        EXPECT_EQ((a + b).GetMicroVolts(), 450000);
        EXPECT_EQ((a - b).GetMicroVolts(), 210000);
        a += b;
        EXPECT_EQ(a.GetMicroVolts(), 450000);
        a -= b;
        EXPECT_EQ(a.GetMicroVolts(), 330000);
    }

    // ---------------------
    // MicroAmpereHours Tests
    // ---------------------

    TEST(MicroAmpereHoursTest, FromAndToRaw)
    {
        for (int32_t max1726Value = std::numeric_limits<int16_t>::min(); max1726Value <= std::numeric_limits<int16_t>::max(); max1726Value += 1)
        {
            MicroAmpereHours c = MicroAmpereHours::FromRaw(max1726Value);
            int16_t roundtrip = c.ToRaw();
            if (roundtrip != max1726Value)
            {
                ASSERT_TRUE(false);
            }
            EXPECT_EQ(roundtrip, max1726Value);
        }
    }

    TEST(MicroAmpereHoursTest, LiteralsAndArithmetic)
    {
        auto a = 1000_uAh;
        auto b = 750_uAh;
        EXPECT_EQ((a + b).GetMicroAmpereHours(), 1750);
        EXPECT_EQ((a - b).GetMicroAmpereHours(), 250);
        a += b;
        EXPECT_EQ(a.GetMicroAmpereHours(), 1750);
        a -= b;
        EXPECT_EQ(a.GetMicroAmpereHours(), 1000);
    }

    // ---------------------
    // MilliCelsius Tests
    // ---------------------

    TEST(MilliCelsiusTest, FromAndToRaw)
    {
        for (int raw = -32768; raw <= 32767; raw += 1024)
        {
            uint16_t encoded = static_cast<uint16_t>(static_cast<int16_t>(raw));
            MilliCelsius t = MilliCelsius::FromRaw(encoded);
            uint16_t roundtrip = t.ToRaw();
            EXPECT_EQ(roundtrip, encoded);
        }
    }

    TEST(MilliCelsiusTest, LiteralsAndArithmetic)
    {
        auto t1 = 25000_mC; // 25.0 C
        auto t2 = 10000_mC; // 10.0 C
        EXPECT_EQ((t1 + t2).GetMilliCelsius(), 35000);
        EXPECT_EQ((t1 - t2).GetMilliCelsius(), 15000);
        t1 += t2;
        EXPECT_EQ(t1.GetMilliCelsius(), 35000);
        t1 -= t2;
        EXPECT_EQ(t1.GetMilliCelsius(), 25000);
    }
}