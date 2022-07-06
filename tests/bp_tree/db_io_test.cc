//
// Created by 김태현 on 2022/05/28.
//

#include <gtest/gtest.h>
#include "bp_tree/db_io.h"

TEST(DBPointerTest, DeserializeTest){
  DBPointer db_pointer;

  DBPointer origin("test1twejhetj.db", 17834288, 98415);
  auto origin_binary = origin.serialize();
  db_pointer.deserialize(*origin_binary, 0).unwrap();

  ASSERT_EQ(origin, db_pointer);
}

TEST(DBFileTest, GetFreeFileTest){
  if (std::filesystem::exists("test0.bin")){
    std::filesystem::remove("test0.bin");
  }
  DBFile db_file("test.bin", 10);
  //std::cout << db_file << std::endl;
  auto db = std::fstream(db_file.get_free_file(), std::ios::in | std::ios::out | std::ios::trunc);
  db << "testtesttes";
  db.close();
  //std::cout << db_file.get_free_file();
  ASSERT_EQ(db_file.get_free_file(), "test1.bin");
  std::filesystem::remove("test0.bin");
}