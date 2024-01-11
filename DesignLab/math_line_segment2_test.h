
/// @file      math_line_segment2_test.h
/// @author    Hasegawa
/// @copyright © 埼玉大学 設計工学研究室 2024. All right reserved.

#ifndef DESIGNLAB_MATH_LINE_SEGMENT2_TEST_H_
#define DESIGNLAB_MATH_LINE_SEGMENT2_TEST_H_

#include <doctest.h>

#include "math_line_segment2.h"


TEST_SUITE("LineSegment2")
{
    using designlab::LineSegment2;
    using designlab::Vector2;


    TEST_CASE("ConstructorTest_ShouldZeroForStartAndEnd")
    {
        // デフォルトコンストラクタで初期化した場合，start と end が 0 で初期化される．
        const LineSegment2 line;

        CHECK_EQ(line.start, Vector2(0.0f, 0.0f));
        CHECK_EQ(line.end, Vector2(0.0f, 0.0f));
    }

    TEST_CASE("ConstructorTest_ShouldSetStartAndEnd")
    {
        SUBCASE("Vector2")
        {
            const LineSegment2 line(Vector2(1.0f, 2.0f), Vector2(3.0f, 4.0f));

            CHECK_EQ(line.start, Vector2(1.0f, 2.0f));
            CHECK_EQ(line.end, Vector2(3.0f, 4.0f));
        }

        SUBCASE("float")
        {
            const LineSegment2 line(1.0f, 2.0f, 3.0f, 4.0f);

            CHECK_EQ(line.start, Vector2(1.0f, 2.0f));
            CHECK_EQ(line.end, Vector2(3.0f, 4.0f));
        }
    }

    TEST_CASE("IsParallelTest_WhenParallel_ReturnTrue")
    {
        struct { LineSegment2 line1; LineSegment2 line2; }
        test_table[] = {
            { LineSegment2(0.0f, 0.0f, 1.0f, 1.0f), LineSegment2(0.0f, 1.0f, 1.0f, 2.0f) },
            { LineSegment2(0.0f, 0.0f, 1.0f, 1.0f), LineSegment2(1.0f, 1.0f, 2.0f, 2.0f) },
            { LineSegment2(0.0f, 0.0f, 1.0f, 1.0f), LineSegment2(-1.0f, -1.0f, 0.0f, 0.0f) },
            { LineSegment2(0.0f, 0.0f, 1.0f, 1.0f), LineSegment2(-2.0f, -2.0f, -1.0f, -1.0f) },
        };

        for (const auto& t : test_table)
        {
            INFO("line1 start : " << t.line1.start << ", end : " << t.line1.end);
            INFO("line2 start : " << t.line2.start << ", end : " << t.line2.end);
            CHECK(t.line1.IsParallel(t.line2));
        }
    }

    TEST_CASE("IsParallelTest_WhenNotParallel_ReturnFalse")
    {
        struct { LineSegment2 line1; LineSegment2 line2; }
        test_table[] = {
            { LineSegment2(0.0f, 0.0f, 1.0f, 1.0f), LineSegment2(0.0f, 1.0f, 1.0f, 0.0f) },
            { LineSegment2(0.0f, 0.0f, 1.0f, 0.0f), LineSegment2(0.0f, 0.0f, 0.0f, 1.0f) },
            { LineSegment2(0.0f, 0.0f, 1.0f, 1.0f), LineSegment2(0.0f, 0.0f, 1.0f, 2.0f) },
        };

        for (const auto& t : test_table)
        {
            INFO("line1 start : " << t.line1.start << ", end : " << t.line1.end);
            INFO("line2 start : " << t.line2.start << ", end : " << t.line2.end);
            CHECK_FALSE(t.line1.IsParallel(t.line2));
        }
    }

    TEST_CASE("GetLengthTest")
    {
        struct { LineSegment2 line; float expected; }
        test_table[] = {
            { LineSegment2(0.0f, 0.0f, 1.0f, 1.0f), std::sqrt(2.0f) },
            { LineSegment2(0.0f, 0.0f, 2.0f, -2.0f), std::sqrt(8.0f) },
            { LineSegment2(0.0f, 0.0f, -4.0f, 4.0f), std::sqrt(32.0f) },
            { LineSegment2(2.0f, -3.0f, 7.0f, 2.0f), std::sqrt(50.0f) },
            { LineSegment2(10.0f, 10.0f, 13.0f, 13.0f), std::sqrt(18.0f) },
        };

        for (const auto& t : test_table)
        {
            const auto length = t.line.GetLength();

            INFO("line: " << t.line.start << ", " << t.line.end);

            CHECK_EQ(length, t.expected);
        }
    }

    TEST_CASE("GetIntersectionTest_WhenHasIntersection_ReturnIntersection")
    {
        struct { LineSegment2 line1; LineSegment2 line2; Vector2 expected; }
        test_table[] = {
            { LineSegment2(0.f, 0.f, 1.f, 1.f), LineSegment2(0.f, 1.f, 1.f, 0.f), Vector2(0.5f, 0.5f) },
            { LineSegment2(1.f, 0.f, 1.f, 1.f), LineSegment2(0.f, 1.f, 1.f, 1.f), Vector2(1.0f, 1.f) },
            { LineSegment2(0.f, 0.f, 1.f, 3.f), LineSegment2(0.f, 3.f, 1.f, 0.f), Vector2(0.5f, 1.5f) },
            { LineSegment2(0.f, 0.f, 2.f, 2.f), LineSegment2(0.f, 2.f, 2.f, 0.f), Vector2(1.0f, 1.0f) },
        };

        for (const auto& t : test_table)
        {
            const auto intersection = t.line1.GetIntersection(t.line2);

            INFO("line1 start : " << t.line1.start << ", end : " << t.line1.end);
            INFO("line2 start : " << t.line2.start << ", end : " << t.line2.end);

            CHECK_EQ(intersection, t.expected);
        }
    }

    TEST_CASE("GetIntersectionTest_WhenNotHasIntersection_ReturnZeroVector")
    {
        struct { LineSegment2 line1; LineSegment2 line2; }
        test_table[] = {
            { LineSegment2(0.0f, 0.0f, 1.0f, 1.0f), LineSegment2(1.0f, 0.0f, 2.0f, 1.0f) },
            { LineSegment2(1.0f, 0.0f, 1.0f, 1.0f), LineSegment2(2.0f, 0.0f, 2.0f, 1.0f) },
            { LineSegment2(1.0f, 0.0f, 0.0f, 1.0f), LineSegment2(0.0f, 0.0f, 0.4f, 0.4f) },
            { LineSegment2(0.0f, 0.0f, 1.0f, 1.0f), LineSegment2(-2.0f, -2.0f, -3.0f, -3.0f) },
        };

        for (const auto& t : test_table)
        {
            const auto intersection = t.line1.GetIntersection(t.line2);

            INFO("line1 start : " << t.line1.start << ", end : " << t.line1.end);
            INFO("line2 start : " << t.line2.start << ", end : " << t.line2.end);

            CHECK_EQ(intersection, Vector2(0.0f, 0.0f));
        }
    }

    TEST_CASE("GetIntersectionTest_WhenParallel_ReturnZeroVector")
    {
        struct { LineSegment2 line1; LineSegment2 line2; }
        test_table[] = {
            { LineSegment2(0.0f, 0.0f, 1.0f, 1.0f), LineSegment2(0.0f, 1.0f, 1.0f, 2.0f) },
            { LineSegment2(0.0f, 0.0f, 1.0f, 1.0f), LineSegment2(1.0f, 1.0f, 2.0f, 2.0f) },
            { LineSegment2(0.0f, 0.0f, 1.0f, 1.0f), LineSegment2(-1.0f, -1.0f, 0.0f, 0.0f) },
            { LineSegment2(0.0f, 0.0f, 1.0f, 1.0f), LineSegment2(-2.0f, -2.0f, -1.0f, -1.0f) },
        };

        for (const auto& t : test_table)
        {
            const auto intersection = t.line1.GetIntersection(t.line2);

            INFO("line1 start : " << t.line1.start << ", end : " << t.line1.end);
            INFO("line2 start : " << t.line2.start << ", end : " << t.line2.end);

            CHECK_EQ(intersection, Vector2(0.0f, 0.0f));
        }
    }

    TEST_CASE("HasIntersectionTest_WhenHasIntersection_ReturnTrue")
    {
        struct { LineSegment2 line1; LineSegment2 line2; }
        test_table[] = {
            { LineSegment2(0.0f, 0.0f, 1.0f, 1.0f), LineSegment2(0.0f, 1.0f, 1.0f, 0.0f) },
            { LineSegment2(0.0f, 0.0f, 1.0f, 0.0f), LineSegment2(0.0f, 0.0f, 0.0f, 1.0f) },
            { LineSegment2(0.0f, 0.0f, 1.0f, 1.0f), LineSegment2(0.0f, 0.0f, 1.0f, 2.0f) },
            { LineSegment2(0.0f, 0.0f, 2.0f, 2.0f), LineSegment2(0.0f, 2.0f, 2.0f, 0.0f) },
        };

        for (const auto& t : test_table)
        {
            INFO("line1 start : " << t.line1.start << ", end : " << t.line1.end);
            INFO("line2 start : " << t.line2.start << ", end : " << t.line2.end);

            CHECK(t.line1.HasIntersection(t.line2));
        }
    }
}


#endif  // DESIGNLAB_MATH_LINE_SEGMENT2_TEST_H_
