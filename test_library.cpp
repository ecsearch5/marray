//
// Created by pcuser on 03/07/2020.
//

#include <gtest/gtest.h>

#include "library.h"

class LibTest : public ::testing::Test {
protected:
    virtual void setUp() {}
    virtual void tearDown() {}
};

bool comp(int a, int b) {
    return (a<=b);
}

TEST_F(LibTest, test_bin_search) {
    int a[] = {0, 1, 3, 4, 6, 6, 10};
    EXPECT_EQ(3, bin_search<int>(&a[0], 7, 5, comp));
    EXPECT_EQ(0, bin_search<int>(&a[0], 7, 0, comp));
    EXPECT_EQ(-1, bin_search<int>(&a[0], 7, -2, comp));
    EXPECT_EQ(6, bin_search<int>(&a[0], 7, 12, comp));
    EXPECT_EQ(5, bin_search<int>(&a[0], 7, 6, comp));
}

TEST_F(LibTest, test_marray) {
    // MARRAY will allocate 8000 bytes for indexes and 8000 bytes for data (codes)
    // You will have a MARRAY with 16K data.
    Marray marray(1000, 8);
    std::string id = "8760ac9k-j876-h908-ma76-76hty98abc0t";
    unsigned char A[] = {8, 16, 29, 56, 7, 18, 90, 129};
    marray.put(id.c_str(), &A[0]);
    auto p = marray.pos(id.c_str());
    EXPECT_EQ(0, p);
    marray.put(id.c_str(), &A[0]);
    auto c = marray.get(id.c_str());
    for (int i = 0; i < 8; i++)
        EXPECT_EQ(c[i], A[i]);
}

int main (int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}