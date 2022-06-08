//
// Created by 김태현 on 2022/06/02.
//

#include <gtest/gtest.h>
#include <data_node.h>
#include <data.h>
#include <bp_tree_data.h>
#include <f_string.h>
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

TEST(DataNodeTest, InsertionTest){
  auto data1 = std::make_unique<TestData>(TestData(Int(1), Int(1)));
  auto data2 = std::make_unique<TestData>(TestData(Int(2), Int(2)));
  auto data3 = std::make_unique<TestData>(TestData(Int(3), Int(3)));
  auto data4 = std::make_unique<TestData>(TestData(Int(4), Int(4)));
  auto data_node = DataNodeFactory<Int, Int, TestData>::create();
  data_node->push_back(std::move(data1));
  data_node->insert(1, std::move(data2));
  data_node->insert(1, std::move(data3));
  ASSERT_EQ(*data_node->get_key(0), Int(1));
  ASSERT_EQ(*data_node->get_key(1), Int(3));
  ASSERT_EQ(*data_node->get_key(2), Int(2));
}

TEST(DataNodeTest, SearchTest){
  auto data1 = std::make_unique<TestData>(TestData(Int(1), Int(1)));
  auto data2 = std::make_unique<TestData>(TestData(Int(2), Int(2)));
  auto data3 = std::make_unique<TestData>(TestData(Int(4), Int(3)));
  auto data4 = std::make_unique<TestData>(TestData(Int(5), Int(4)));
  auto data_node = DataNodeFactory<Int, Int, TestData>::create();
  data_node->push_back(std::move(data1));
  data_node->push_back(std::move(data2));
  data_node->push_back(std::move(data3));
  data_node->push_back(std::move(data4));

  ASSERT_EQ(data_node->search(Int(2)), 1);
  ASSERT_EQ(data_node->search(Int(3)), 2);
  ASSERT_EQ(data_node->search(Int(4)), 2);
  ASSERT_EQ(data_node->search(Int(5)), 3);
  ASSERT_EQ(data_node->search(Int(6)), 4);
}

TEST(DataNodeTest, SerializeTest){
  auto schema = SchemaBuilder("test schema").set_field(TypeKind::STRING, "pk_str", KeyType::PK).unwrap()
      ->set_field(TypeKind::STRING, "fk_str", KeyType::FK).unwrap()
      ->set_field(TypeKind::DATETIME, "Created date").unwrap()
      ->build().unwrap();

  auto data1 = std::make_unique<BPTreeData>(BPTreeData("pk_str", Record(*schema)));
  auto data2 = std::make_unique<BPTreeData>(BPTreeData("pk_str", Record(*schema)));
  auto data3 = std::make_unique<BPTreeData>(BPTreeData("pk_str", Record(*schema)));
  auto data4 = std::make_unique<BPTreeData>(BPTreeData("pk_str", Record(*schema)));
  auto data_node = DataNodeFactory<String, Record, BPTreeData>::create();
  data_node->push_back(std::move(data1));
  data_node->push_back(std::move(data2));
  data_node->push_back(std::move(data3));
  data_node->push_back(std::move(data4));

  auto binary = data_node->serialize();

  auto new_data_node = DataNodeFactory<Int, Int, TestData>::create();
  new_data_node->deserialize(*binary, 0);

  //ASSERT_EQ(*data_node, *new_data_node);
}