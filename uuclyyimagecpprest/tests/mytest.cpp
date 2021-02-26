#include "gtest/gtest.h"
#include "imagescalerservice.h"
#include "mycmaketest.h"
#include <vips/vips8>

TEST(blaTest, test1) {
    vips_init(NULL);
    auto in = vips::VImage::new_from_file("/Users/svenweisker/Downloads/big_png_big.png", NULL);
	auto out = in.resize( 0.3, vips::VImage::option() ->set( "vscale", 0.3 ) );

    EXPECT_EQ(out.width(), 1200);
    EXPECT_EQ(out.height(), 600);
    vips_shutdown();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}