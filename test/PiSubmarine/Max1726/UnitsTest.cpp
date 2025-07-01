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
        constexpr uint8_t positive = 0b00001100;
        constexpr uint8_t twoComp = 0b10001100;
        constexpr int8_t decodedTwoComp = static_cast<int8_t>(twoComp);

        for (int32_t max1726Value = std::numeric_limits<int16_t>::min(); max1726Value <= std::numeric_limits<int16_t>::max(); max1726Value += 1)
        {
            MicroAmperes current = MicroAmperes::FromRaw(max1726Value);
            int16_t roundtrip = current.ToRaw();
            ASSERT_EQ(roundtrip, max1726Value);
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

    // ---------------------
    // MicroVolts Tests
    // ---------------------

    TEST(MicroVoltsTest, FromAndToRaw)
    {
        for (uint16_t raw = 0; raw < 65535; raw += 1)
        {
            MicroVolts v = MicroVolts::FromRaw(raw);
            uint16_t roundtrip = v.ToRaw();
            if (roundtrip != raw)
            {
                EXPECT_TRUE(false);
            }
            EXPECT_EQ(roundtrip, raw);
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