#include <gtest/gtest.h>

#include "test.pb.h"  // 由 test.proto 生成，确保生成路径正确并被 CMake include

TEST(ProtoTest, SerializeDeserialize) {
    MyProto::Person person;
    person.set_name("Alice");
    person.set_id(123);

    // 序列化为字符串
    std::string serialized;
    ASSERT_TRUE(person.SerializeToString(&serialized));

    // 反序列化
    MyProto::Person deserialized;
    ASSERT_TRUE(deserialized.ParseFromString(serialized));

    // 验证字段一致性
    EXPECT_EQ(deserialized.name(), "Alice");
    EXPECT_EQ(deserialized.id(), 123);
}

TEST(ProtoTest, DefaultValues) {
    MyProto::Person person;

    // 未设置 name 和 id，应该是默认值
    EXPECT_EQ(person.name(), "");
    EXPECT_EQ(person.id(), 0);
}
