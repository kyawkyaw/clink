// Copyright (c) 2016 Martin Ridgers
// License: http://opensource.org/licenses/MIT

#include "pch.h"

#include <core/str_iter.h>

//------------------------------------------------------------------------------
TEST_CASE("String iterator (str_iter)", "[str_iter]") {
    SECTION("Basic") {
        str_iter iter("123");
        REQUIRE(iter.next() == '1');
        REQUIRE(iter.next() == '2');
        REQUIRE(iter.next() == '3');
        REQUIRE(iter.next() == 0);
    }

    SECTION("Subset") {
        str_iter iter("123", 2);
        REQUIRE(iter.next() == '1');
        REQUIRE(iter.next() == '2');
        REQUIRE(iter.next() == 0);
    }

    SECTION("UTF-8") {
        str_iter iter("\xc2\x9b\xc2\x9b\xc2\x9b");
        REQUIRE(iter.next() == 0x9b);
        REQUIRE(iter.next() == 0x9b);
        REQUIRE(iter.next() == 0x9b);
        REQUIRE(iter.next() == 0);
    }

    SECTION("Partial UTF-8") {
        str_iter iter("\xc2\x9b\xe0\xa0");
        REQUIRE(iter.next() == 0x9b);
        REQUIRE(iter.next() == 0);

        new (&iter) str_iter("\xc2\x9b", 1);
        REQUIRE(iter.next() == 0);
    }
}

//------------------------------------------------------------------------------
TEST_CASE("String iterator (wstr_iter)", "[wstr_iter]") {
    SECTION("Basic") {
        wstr_iter iter(L"123");
        REQUIRE(iter.next() == '1');
        REQUIRE(iter.next() == '2');
        REQUIRE(iter.next() == '3');
        REQUIRE(iter.next() == 0);
    }

    SECTION("Subset") {
        wstr_iter iter(L"123", 2);
        REQUIRE(iter.next() == '1');
        REQUIRE(iter.next() == '2');
        REQUIRE(iter.next() == 0);
    }

    SECTION("UTF-16") {
        wstr_iter iter(L"\x0001\xd800\xdc00");
        REQUIRE(iter.next() == 1);
        REQUIRE(iter.next() == 0x10000);
        REQUIRE(iter.next() == 0);

        new (&iter) wstr_iter(L"\xdbff\xdfff");
        REQUIRE(iter.next() == 0x10ffff);
        REQUIRE(iter.next() == 0);
    }

    SECTION("Partial UTF-16") {
        wstr_iter iter(L"\x0001\xd800");
        REQUIRE(iter.next() == 1);
        REQUIRE(iter.next() == 0);

        new (&iter) wstr_iter(L"\xd9ff");
        REQUIRE(iter.next() == 0);

        new (&iter) wstr_iter(L"\xdfff");
        REQUIRE(iter.next() == 0xdfff);
        REQUIRE(iter.next() == 0);
    }
}
