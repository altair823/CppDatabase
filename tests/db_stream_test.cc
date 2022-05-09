//
// Created by 김태현 on 2022/05/09.
//

#include <gtest/gtest.h>

#include "../includes/storage/b_tree/db_stream.h"
#include <field_data.h>

TEST(DBStreamTest, WriteTest){
//  IndexNode<std::string, int> node;
//  node.add_key("1").unwrap()
//  ->add_key("2").unwrap()
//  ->add_key("3").unwrap()
//  ->add_pointer(4).unwrap()
//  ->add_pointer(5).unwrap();
//  auto stream = DBStream<std::string, int>();
//  std::cout<<stream.write(node).unwrap();

std::fstream a;
std::fstream b;
a.open("test1.txt", std::ios::in | std::ios::out | std::ios::trunc);
a.write("hello", 5);
b.open("test2.txt", std::ios::out);
a.seekg(0, std::ios::beg);
b << a.rdbuf();
a.close();
b.close();
}

