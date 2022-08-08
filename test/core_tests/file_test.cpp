
#include "core/fs/DataFile.h"
#include "testHelper.h"

TEST(DataFile, base){
    rc::core::fs::DataFile fileVoid;
    EXPECT_EQ(fileVoid.getFullPath().parent_path().filename(), "data");
    rc::core::fs::DataFile fileTest("bob");
    EXPECT_EQ(fileVoid.getFullPath().parent_path(),fileTest.getFullPath().parent_path());
    EXPECT_FALSE(fileTest.exists());
    fileTest.touch();
    fileTest.touch();
    EXPECT_TRUE(fileTest.exists());
    fileTest.remove();
    EXPECT_FALSE(fileTest.exists());
}
