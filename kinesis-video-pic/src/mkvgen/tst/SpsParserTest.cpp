#include "MkvgenTestFixture.h"

class SpsParserTest : public MkvgenTestBase {
};

TEST_F(SpsParserTest, spsParser_InvalidInput)
{
    PBYTE cpd = (PBYTE) 100;
    UINT16 width, height;

    EXPECT_NE(STATUS_SUCCESS, getVideoWidthAndHeightFromH264Sps(NULL, 100, &width, &height));
    EXPECT_NE(STATUS_SUCCESS, getVideoWidthAndHeightFromH264Sps(cpd, 0, &width, &height));
    EXPECT_NE(STATUS_SUCCESS, getVideoWidthAndHeightFromH264Sps(cpd, MIN_H264_H265_CPD_SIZE - 1, &width, &height));
    EXPECT_NE(STATUS_SUCCESS, getVideoWidthAndHeightFromH264Sps(cpd, 100, NULL, &height));
    EXPECT_NE(STATUS_SUCCESS, getVideoWidthAndHeightFromH264Sps(cpd, 100, &width, NULL));
    EXPECT_NE(STATUS_SUCCESS, getVideoWidthAndHeightFromH264Sps(NULL, 0, NULL, NULL));

    EXPECT_NE(STATUS_SUCCESS, parseH264SpsGetResolution(NULL, 100, &width, &height));
    EXPECT_NE(STATUS_SUCCESS, parseH264SpsGetResolution(cpd, 0, &width, &height));
    EXPECT_NE(STATUS_SUCCESS, parseH264SpsGetResolution(cpd, 100, NULL, &height));
    EXPECT_NE(STATUS_SUCCESS, parseH264SpsGetResolution(cpd, 100, &width, NULL));
    EXPECT_NE(STATUS_SUCCESS, parseH264SpsGetResolution(cpd, 100, NULL, NULL));
}

TEST_F(SpsParserTest, spsParser_Raw_Sps)
{
    BYTE cpd[] = {0x67, 0x42, 0x40, 0x1f, 0x96, 0x54, 0x02, 0x80,
                  0x2d, 0xc8, 0x68, 0xce, 0x38, 0x80};
    UINT32 cpdSize = SIZEOF(cpd);
    UINT16 width, height;

    EXPECT_EQ(STATUS_SUCCESS, getVideoWidthAndHeightFromH264Sps(cpd, cpdSize, &width, &height));
    EXPECT_EQ(1280, width);
    EXPECT_EQ(720, height);
}

TEST_F(SpsParserTest, spsParser_AnnexB)
{
    BYTE cpd1[] = {0x00, 0x00, 0x00, 0x01, 0x67, 0x42, 0x40, 0x1f,
                   0x96, 0x54, 0x02, 0x80, 0x2d, 0xc8, 0x00, 0x00,
                   0x00, 0x01, 0x68, 0xce, 0x38, 0x80};
    BYTE cpd2[] = {0x00, 0x00, 0x00, 0x01, 0x67, 0x42, 0x00, 0x1e,
                   0xa9, 0x50, 0x14, 0x07, 0xb4, 0x20, 0x00, 0x00,
                   0x7d, 0x00, 0x00, 0x1d, 0x4c, 0x00, 0x80, 0x00,
                   0x00, 0x00, 0x01, 0x68, 0xce, 0x3c, 0x80};

    UINT16 width, height;

    EXPECT_EQ(STATUS_SUCCESS, getVideoWidthAndHeightFromH264Sps(cpd1, SIZEOF(cpd1), &width, &height));
    EXPECT_EQ(1280, width);
    EXPECT_EQ(720, height);

    EXPECT_EQ(STATUS_SUCCESS, getVideoWidthAndHeightFromH264Sps(cpd2, SIZEOF(cpd2), &width, &height));
    EXPECT_EQ(640, width);
    EXPECT_EQ(480, height);
}

TEST_F(SpsParserTest, spsParser_Avcc) {
    BYTE cpd1[] = {0x01, 0x64, 0x00, 0x28, 0xff, 0xe1, 0x00, 0x2e,
                  0x67, 0x64, 0x00, 0x28, 0xac, 0x2c, 0xa4, 0x01,
                  0xe0, 0x08, 0x9f, 0x97, 0xff, 0x00, 0x01, 0x00,
                  0x01, 0x52, 0x02, 0x02, 0x02, 0x80, 0x00, 0x01,
                  0xf4, 0x80, 0x00, 0x75, 0x30, 0x70, 0x10, 0x00,
                  0x16, 0xe3, 0x60, 0x00, 0x08, 0x95, 0x45, 0xf8,
                  0xc7, 0x07, 0x68, 0x58, 0xb4, 0x48, 0x01, 0x00,
                  0x05, 0x68, 0xeb, 0x73, 0x52, 0x50, 0xfd, 0xf8,
                  0xf8, 0x00};
    BYTE cpd2[] = {0x01, 0x42, 0x40, 0x15, 0xFF, 0xE1, 0x00, 0x0a,
                   0x67, 0x42, 0x40, 0x33, 0x95, 0xa0, 0x1e, 0x00,
                   0xb5, 0x90, 0x01, 0x00, 0x04, 0x68, 0xce, 0x3c,
                   0x80};
    BYTE cpd3[] = {0x01, 0x42, 0xC0, 0x0D, 0xFF, 0xE1, 0x00, 0x21, 0x67, 0x42, 0xC0, 0x0D, 0xAB, 0x40, 0xA0, 0xFD,
                   0x08, 0x00, 0x00, 0x03, 0x00, 0x08, 0x00, 0x00, 0x03, 0x01, 0x47, 0x02, 0x00, 0x0C, 0x00, 0x00,
                   0x06, 0x00, 0x16, 0xB1, 0xB0, 0x3C, 0x50, 0xAA, 0x80, 0x01, 0x00, 0x04, 0x28, 0xCE, 0x3C, 0x80};

    UINT16 width, height;

    EXPECT_EQ(STATUS_SUCCESS, getVideoWidthAndHeightFromH264Sps(cpd1, SIZEOF(cpd1), &width, &height));
    EXPECT_EQ(1920, width);
    EXPECT_EQ(1080, height);

    EXPECT_EQ(STATUS_SUCCESS, getVideoWidthAndHeightFromH264Sps(cpd2, SIZEOF(cpd2), &width, &height));
    EXPECT_EQ(1920, width);
    EXPECT_EQ(1440, height);

    EXPECT_EQ(STATUS_SUCCESS, getVideoWidthAndHeightFromH264Sps(cpd3, SIZEOF(cpd3), &width, &height));
    EXPECT_EQ(320, width);
    EXPECT_EQ(240, height);
}

TEST_F(SpsParserTest, spsParser_Mjpg) {
    BYTE cpd[] = {0x28, 0x00, 0x00, 0x00, 0x80, 0x02, 0x00, 0x00,
                  0xe0, 0x01, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00,
                  0x4d, 0x4a, 0x50, 0x47, 0x00, 0x10, 0x0e, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    UINT16 width, height;

    EXPECT_EQ(STATUS_SUCCESS, getVideoWidthAndHeightFromBih(cpd, SIZEOF(cpd), &width, &height));
    EXPECT_EQ(640, width);
    EXPECT_EQ(480, height);

    EXPECT_EQ(STATUS_MKV_INVALID_BIH_CPD, getVideoWidthAndHeightFromBih(cpd, SIZEOF(cpd) - 1, &width, &height));
}

TEST_F(SpsParserTest, spsParser_Hevc) {
    // These are series of codec private data bits extracted from an
    // GStreamer pipeline for H265 for various resolutions
    BYTE cpd[4][1000] = {
            {0x01, 0x01, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7b, 0xf0,
                    0x00, 0xfc, 0xfd, 0xf8, 0xf8, 0x00, 0x00, 0x0f, 0x03, 0x20, 0x00, 0x01, 0x00, 0x18,
                    0x40, 0x01, 0x0c, 0x01, 0xff, 0xff, 0x01, 0x40, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03,
                    0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x7b, 0xac, 0x09, 0x21, 0x00, 0x01, 0x00,
                    0x23, 0x42, 0x01, 0x01, 0x01, 0x40, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x00,
                    0x03, 0x00, 0x00, 0x03, 0x00, 0x7b, 0xa0, 0x01, 0x98, 0x20, 0x02, 0x68, 0x7c, 0x4e,
                    0x5a, 0xee, 0x42, 0x11, 0x12, 0xe2, 0x6c, 0x01, 0x22, 0x00, 0x01, 0x00, 0x07, 0x44,
                    0x01, 0xc0, 0x77, 0xc0, 0xcc, 0x90, 0x00, 0x00, 0x00},

            {0x01, 0x01, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7b, 0xf0,
                    0x00, 0xfc, 0xfd, 0xf8, 0xf8, 0x00, 0x00, 0x0f, 0x03, 0x20, 0x00, 0x01, 0x00, 0x18,
                    0x40, 0x01, 0x0c, 0x01, 0xff, 0xff, 0x01, 0x40, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03,
                    0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x7b, 0xac, 0x09, 0x21, 0x00, 0x01, 0x00,
                    0x22, 0x42, 0x01, 0x01, 0x01, 0x40, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x00,
                    0x03, 0x00, 0x00, 0x03, 0x00, 0x7b, 0xa0, 0x03, 0xc0, 0x80, 0x16, 0x85, 0x96, 0xbb,
                    0x90, 0x84, 0x44, 0xb8, 0x9b, 0x00, 0x40, 0x22, 0x00, 0x01, 0x00, 0x07, 0x44, 0x01,
                    0xc0, 0x77, 0xc0, 0xcc, 0x90, 0x00, 0x00, 0x00, 0xff},

            {0x01, 0x21, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x96, 0xf0,
                    0x00, 0xfc, 0xfd, 0xf8, 0xf8, 0x00, 0x00, 0x0f, 0x03, 0x20, 0x00, 0x01, 0x00, 0x18,
                    0x40, 0x01, 0x0c, 0x01, 0xff, 0xff, 0x21, 0x40, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03,
                    0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x96, 0xac, 0x09, 0x21, 0x00, 0x01, 0x00,
                    0x21, 0x42, 0x01, 0x01, 0x21, 0x40, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x00,
                    0x03, 0x00, 0x00, 0x03, 0x00, 0x96, 0xa0, 0x0c, 0x08, 0x04, 0x85, 0x96, 0xbb, 0x90,
                    0x84, 0x44, 0xb8, 0x9b, 0x00, 0x40, 0x22, 0x00, 0x01, 0x00, 0x07, 0x44, 0x01, 0xc0,
                    0x77, 0xc0, 0xcc, 0x90, 0x00, 0x00, 0x00, 0xff, 0xff},

            {0x01, 0x21, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x96, 0xf0,
                    0x00, 0xfc, 0xfd, 0xf8, 0xf8, 0x00, 0x00, 0x0f, 0x03, 0x20, 0x00, 0x01, 0x00, 0x18,
                    0x40, 0x01, 0x0c, 0x01, 0xff, 0xff, 0x21, 0x40, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03,
                    0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x96, 0xac, 0x09, 0x21, 0x00, 0x01, 0x00,
                    0x22, 0x42, 0x01, 0x01, 0x21, 0x40, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x00,
                    0x03, 0x00, 0x00, 0x03, 0x00, 0x96, 0xa0, 0x0a, 0x08, 0x04, 0x07, 0xc4, 0xe5, 0xae,
                    0xe4, 0x21, 0x11, 0x2e, 0x26, 0xc0, 0x10, 0x22, 0x00, 0x01, 0x00, 0x07, 0x44, 0x01,
                    0xc0, 0x77, 0xc0, 0xcc, 0x90, 0x00, 0x00, 0x00, 0xff}
    };

    UINT32 cpdSizes[4] = {107, 106, 105, 106};

    UINT16 widths[4] = {3264, 1920, 384, 320};
    UINT16 heights[4] = {2448, 1440, 288, 240};

    UINT16 i, width, height;

    for (i = 0; i < 4; i++) {
        width = height = 0;
        EXPECT_EQ(STATUS_SUCCESS, getVideoWidthAndHeightFromH265Sps(cpd[i], cpdSizes[i], &width, &height)) << "Failed on iteration " << i;
        EXPECT_EQ(widths[i], width) << "Failed on iteration " << i;
        EXPECT_EQ(heights[i], height) << "Failed on iteration " << i;
    }
}

TEST_F(SpsParserTest, spsParser_AnnexB_H265) {
    // These are series of codec private data bits extracted from an
    // Android device for H265 for various resolutions
    // Some are from other samples

    // Define the number of CPDs
    const UINT32 NUMBER_OF_H265_CPDS = 13;

    BYTE cpd[NUMBER_OF_H265_CPDS][1000] = {
            {0x00, 0x00, 0x00, 0x01, 0x40, 0x01, 0x0c, 0x01, 0xff, 0xff, 0x01, 0x60, 0x00, 0x00, 0x03, 0x00,
                    0xb0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x3c, 0xac, 0x59, 0x00, 0x00, 0x00, 0x01, 0x42,
                    0x01, 0x01, 0x01, 0x60, 0x00, 0x00, 0x03, 0x00, 0xb0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00,
                    0x3c, 0xa0, 0x18, 0x20, 0x28, 0x71, 0x31, 0x39, 0x6b, 0xb9, 0x32, 0x4b, 0xb9, 0x48, 0x28, 0x10,
                    0x10, 0x17, 0x68, 0x50, 0x94, 0x00, 0x00, 0x00, 0x01, 0x44, 0x01, 0xc0, 0xf1, 0x80, 0x04, 0x20},

            {0x00, 0x00, 0x00, 0x01, 0x40, 0x01, 0x0c, 0x01, 0xff, 0xff, 0x01, 0x60, 0x00, 0x00, 0x03, 0x00,
                    0xb0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x3c, 0xac, 0x59, 0x00, 0x00, 0x00, 0x01, 0x42,
                    0x01, 0x01, 0x01, 0x60, 0x00, 0x00, 0x03, 0x00, 0xb0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00,
                    0x3c, 0xa0, 0x0a, 0x08, 0x04, 0x07, 0xc4, 0xe5, 0xae, 0xe4, 0xc9, 0x2e, 0xe5, 0x20, 0xa0, 0x40,
                    0x40, 0x5d, 0xa1, 0x42, 0x50, 0x00, 0x00, 0x00, 0x01, 0x44, 0x01, 0xc0, 0xf1, 0x80, 0x04, 0x20},

            {0x00, 0x00, 0x00, 0x01, 0x40, 0x01, 0x0c, 0x01, 0xff, 0xff, 0x01, 0x60, 0x00, 0x00, 0x03, 0x00,
                    0xb0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x5a, 0xac, 0x59, 0x00, 0x00, 0x00, 0x01, 0x42,
                    0x01, 0x01, 0x01, 0x60, 0x00, 0x00, 0x03, 0x00, 0xb0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00,
                    0x5a, 0xa0, 0x05, 0x02, 0x01, 0xe1, 0x65, 0xae, 0xe4, 0xc9, 0x2e, 0xe5, 0x20, 0xa0, 0x40, 0x40},

            {0x00, 0x00, 0x00, 0x01, 0x40, 0x01, 0x0c, 0x01, 0xff, 0xff, 0x01, 0x60, 0x00, 0x00, 0x03, 0x00,
                    0xb0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x5a, 0xac, 0x59, 0x00, 0x00, 0x00, 0x01, 0x42,
                    0x01, 0x01, 0x01, 0x60, 0x00, 0x00, 0x03, 0x00, 0xb0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00,
                    0x5a, 0xa0, 0x06, 0x02, 0x01, 0xc1, 0xf1, 0x39, 0x6b, 0xb9, 0x32, 0x4b, 0xb9, 0x48, 0x28, 0x10,
                    0x10, 0x17, 0x68, 0x50, 0x94, 0x00, 0x00, 0x00, 0x01, 0x44, 0x01, 0xc0, 0xf1, 0x80, 0x04, 0x20},

            {0x00, 0x00, 0x00, 0x01, 0x40, 0x01, 0x0c, 0x01, 0xff, 0xff, 0x01, 0x60, 0x00, 0x00, 0x03, 0x00,
                    0xb0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x5a, 0xac, 0x59, 0x00, 0x00, 0x00, 0x01, 0x42,
                    0x01, 0x01, 0x01, 0x60, 0x00, 0x00, 0x03, 0x00, 0xb0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00,
                    0x5a, 0xa0, 0x05, 0xc2, 0x01, 0xe1, 0xc4, 0xf9, 0x6b, 0xb9, 0x32, 0x4b, 0xb9, 0x48, 0x28, 0x10,
                    0x10, 0x17, 0x68, 0x50, 0x94, 0x00, 0x00, 0x00, 0x01, 0x44, 0x01, 0xc0, 0xf1, 0x80, 0x04, 0x20},

            {0x00, 0x00, 0x00, 0x01, 0x40, 0x01, 0x0c, 0x01, 0xff, 0xff, 0x01, 0x60, 0x00, 0x00, 0x03, 0x00,
                    0xb0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x5d, 0xac, 0x59, 0x00, 0x00, 0x00, 0x01, 0x42,
                    0x01, 0x01, 0x01, 0x60, 0x00, 0x00, 0x03, 0x00, 0xb0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00,
                    0x5d, 0xa0, 0x07, 0x82, 0x00, 0xb8, 0x7c, 0x4e, 0x5a, 0xee, 0x4c, 0x92, 0xee, 0x52, 0x0a, 0x04,
                    0x04, 0x05, 0xda, 0x14, 0x25, 0x00, 0x00, 0x00, 0x01, 0x44, 0x01, 0xc0, 0xf1, 0x80, 0x04, 0x20},

            {0x00, 0x00, 0x00, 0x01, 0x40, 0x01, 0x0c, 0x01, 0xff, 0xff, 0x01, 0x60, 0x00, 0x00, 0x03, 0x00,
                    0xb0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x5d, 0xac, 0x59, 0x00, 0x00, 0x00, 0x01, 0x42,
                    0x01, 0x01, 0x01, 0x60, 0x00, 0x00, 0x03, 0x00, 0xb0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00,
                    0x5d, 0xa0, 0x02, 0x80, 0x80, 0x2e, 0x1f, 0x13, 0x96, 0xbb, 0x93, 0x24, 0xbb, 0x94, 0x82, 0x81,
                    0x01, 0x01, 0x76, 0x85, 0x09, 0x40, 0x00, 0x00, 0x00, 0x01, 0x44, 0x01, 0xc0, 0xf1, 0x80, 0x04},

            {0x00, 0x00, 0x00, 0x01, 0x40, 0x01, 0x0c, 0x01, 0xff, 0xff, 0x01, 0x60, 0x00, 0x00, 0x03, 0x00,
                    0xb0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x5d, 0xac, 0x59, 0x00, 0x00, 0x00, 0x01, 0x42,
                    0x01, 0x01, 0x01, 0x60, 0x00, 0x00, 0x03, 0x00, 0xb0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00,
                    0x5d, 0xa0, 0x02, 0x80, 0x80, 0x3c, 0x16, 0x5a, 0xee, 0x4c, 0x92, 0xee, 0x52, 0x0a, 0x04, 0x04},

            // From http://jell.yfish.us/ jellyfish-3-mbps-hd-hevc.mkv sample
            {0x01, 0x01, 0x60, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0xf0, 0x00, 0xff,
                    0xfd, 0xf8, 0xf8, 0x00, 0x00, 0x0f, 0x03, 0xa0, 0x00, 0x01, 0x00, 0x18, 0x40, 0x01, 0x0c, 0x01,
                    0xff, 0xff, 0x01, 0x60, 0x00, 0x00, 0x03, 0x00, 0x90, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00,
                    0x78, 0x99, 0x98, 0x09, 0xa1, 0x00, 0x01, 0x00, 0x29, 0x42, 0x01, 0x01, 0x01, 0x60, 0x00, 0x00,
                    0x03, 0x00, 0x90, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x78, 0xa0, 0x03, 0xc0, 0x80, 0x10,
                    0xe5, 0x96, 0x66, 0x69, 0x24, 0xca, 0xf0, 0x10, 0x10, 0x00, 0x00, 0x06, 0x40, 0x00, 0x00, 0xbb,
                    0x50, 0x80, 0xa2, 0x00, 0x01, 0x00, 0x07, 0x44, 0x01, 0xc1, 0x72, 0xb4, 0x62, 0x40},

            // From http://jell.yfish.us/ jellyfish-3-mbps-hd-hevc-10bit.mkv sample
            {0x01, 0x02, 0x20, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0xf0, 0x00, 0xff,
                    0xfd, 0xfa, 0xfa, 0x00, 0x00, 0x0f, 0x03, 0xa0, 0x00, 0x01, 0x00, 0x19, 0x40, 0x01, 0x0c, 0x01,
                    0xff, 0xff, 0x02, 0x20, 0x00, 0x00, 0x03, 0x00, 0x90, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00,
                    0x78, 0x99, 0x8a, 0x02, 0x40, 0xa1, 0x00, 0x01, 0x00, 0x2b, 0x42, 0x01, 0x01, 0x02, 0x20, 0x00,
                    0x00, 0x03, 0x00, 0x90, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x78, 0xa0, 0x03, 0xc0, 0x80,
                    0x10, 0xe4, 0xd9, 0x66, 0x62, 0xa4, 0x90, 0x84, 0x6b, 0xc0, 0x40, 0x40, 0x00, 0x00, 0x19, 0x00,
                    0x00, 0x03, 0x02, 0xed, 0x42, 0xa2, 0x00, 0x01, 0x00, 0x07, 0x44, 0x01, 0xc1, 0x76, 0xb6, 0x62,
                    0x40},

            // From https://developer.apple.com/streaming/examples/advanced-stream-hevc.html
            {0x01, 0x02, 0x00, 0x00, 0x00, 0x04, 0xb0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7b, 0xf0, 0x00, 0xfc,
                    0xfd, 0xfa, 0xfa, 0x00, 0x00, 0x0f, 0x03, 0xa0, 0x00, 0x01, 0x00, 0x18, 0x40, 0x01, 0x0c, 0x01,
                    0xff, 0xff, 0x02, 0x20, 0x00, 0x00, 0x03, 0x00, 0xb0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00,
                    0x7b, 0x18, 0xb0, 0x24, 0xa1, 0x00, 0x01, 0x00, 0x3c, 0x42, 0x01, 0x01, 0x02, 0x20, 0x00, 0x00,
                    0x03, 0x00, 0xb0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x7b, 0xa0, 0x02, 0x80, 0x80, 0x2d,
                    0x13, 0x67, 0x18, 0xb9, 0x24, 0x48, 0x05, 0x38, 0x88, 0x89, 0x2c, 0xf2, 0x4a, 0x69, 0x27, 0x2c,
                    0x91, 0x24, 0x92, 0x2d, 0xc9, 0x1a, 0xa4, 0x8f, 0xca, 0x22, 0x3f, 0xf0, 0x00, 0x10, 0x00, 0x16,
                    0xa0, 0x20, 0x20, 0x20, 0x10, 0xa2, 0x00, 0x01, 0x00, 0x08, 0x44, 0x01, 0xc0, 0x25, 0x2f, 0x05,
                    0x32, 0x40},

            // From https://bitmovin.com/demos/multi-codec-streaming
            {0x01, 0x01, 0x60, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5d, 0xf0, 0x00, 0xff,
                    0xfd, 0xf8, 0xf8, 0x00, 0x00, 0x0f, 0x03, 0xa0, 0x00, 0x01, 0x00, 0x18, 0x40, 0x01, 0x0c, 0x01,
                    0xff, 0xff, 0x01, 0x60, 0x00, 0x00, 0x03, 0x00, 0x90, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00,
                    0x5d, 0x99, 0x98, 0x09, 0xa1, 0x00, 0x01, 0x00, 0x28, 0x42, 0x01, 0x01, 0x01, 0x60, 0x00, 0x00,
                    0x03, 0x00, 0x90, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x5d, 0xa0, 0x02, 0x80, 0x80, 0x2d,
                    0x16, 0x59, 0x99, 0xa4, 0x93, 0x0b, 0x80, 0x40, 0x00, 0x00, 0x03, 0x00, 0x40, 0x00, 0x00, 0x06,
                    0x42, 0xa2, 0x00, 0x01, 0x00, 0x07, 0x44, 0x01, 0xc1, 0x72, 0xb0, 0x62, 0x40},

            // From http://dash.akamaized.net/dash264/TestCasesHEVC/1a/5/video_10M_init.mp4
            {0x01, 0x02, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xba, 0xf0, 0x00, 0xfc,
                    0xfd, 0xfa, 0xfa, 0x00, 0x00, 0x0f, 0x03, 0x20, 0x00, 0x01, 0x00, 0x19, 0x40, 0x01, 0x0c, 0x01,
                    0xff, 0xff, 0x02, 0x60, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03,
                    0x00, 0xba, 0x98, 0x90, 0x24, 0x21, 0x00, 0x01, 0x00, 0x37, 0x42, 0x01, 0x01, 0x02, 0x60, 0x00,
                    0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0xba, 0xa0, 0x01, 0xe0,
                    0x20, 0x02, 0x1c, 0x4d, 0x96, 0x62, 0x64, 0x93, 0x21, 0x4e, 0x22, 0x22, 0x48, 0x92, 0x49, 0xaa,
                    0x92, 0x44, 0xf5, 0x24, 0xde, 0xb9, 0x26, 0x94, 0x95, 0x37, 0x93, 0xa9, 0xac, 0xdc, 0xa2, 0x2c,
                    0x80, 0x22, 0x00, 0x01, 0x00, 0x07, 0x44, 0x01, 0xc0, 0x71, 0x82, 0xd9, 0x20},
    };

    UINT32 cpdSizes[NUMBER_OF_H265_CPDS] = {80, 80, 64, 80, 80, 80, 80, 64, 110, 113, 130, 109, 125};

    UINT16 widths[NUMBER_OF_H265_CPDS] = {176, 320, 640, 768, 720, 960, 1280, 1280, 1920, 1920, 1280, 1280, 3840};
    UINT16 heights[NUMBER_OF_H265_CPDS] = {144, 240, 480, 432, 480, 720, 720, 960, 1080, 1080, 720, 720, 2160};

    UINT16 i, width, height;

    for (i = 0; i < NUMBER_OF_H265_CPDS; i++) {
        width = height = 0;
        EXPECT_EQ(STATUS_SUCCESS, getVideoWidthAndHeightFromH265Sps(cpd[i], cpdSizes[i], &width, &height))
                            << "Failed on iteration " << i;
        EXPECT_EQ(widths[i], width) << "Failed on iteration " << i;
        EXPECT_EQ(heights[i], height) << "Failed on iteration " << i;
    }
}
