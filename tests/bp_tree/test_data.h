//
// Created by 김태현 on 2022/06/09.
//

#ifndef CPPDATABASE_TESTS_BP_TREE_TEST_DATA_H_
#define CPPDATABASE_TESTS_BP_TREE_TEST_DATA_H_

#include <f_int.h>

class TestData: Data<Int, Int> {
 public:
  TestData(const Int& key, const Int& value): key(key), value(value){ }
  [[nodiscard]] BinaryUnique serialize() const override {
    return {};
  }
  Result<BinaryIndex, DeserializeError> deserialize(const Binary &binary, BinaryIndex begin) override {
    return Ok((BinaryIndex)0);
  }
  [[nodiscard]] std::shared_ptr<Int> get_key() const override {
    return std::make_shared<Int>(key);
  }
  [[nodiscard]] std::shared_ptr<Int> get_value() const override {
    return std::make_shared<Int>(value);
  }
 private:
  [[nodiscard]] bool eq(const Data<Int, Int> &rhs) const override {
    return key == *rhs.get_key() && value == *rhs.get_value();
  }
  std::ostream &out(std::ostream &ostream) const override {
    ostream<< "test data!";
    return ostream;
  }
  Int key, value;
};

class TestDataFactory : public DataFactory<Int, Int, TestData> {
 public:
  TestDataFactory(): DataFactory<Int, Int, TestData>() {}

  std::unique_ptr<TestData> create_data() override {
    return std::make_unique<TestData>(Int(0), Int(0));
  }
};
#endif //CPPDATABASE_TESTS_BP_TREE_TEST_DATA_H_
