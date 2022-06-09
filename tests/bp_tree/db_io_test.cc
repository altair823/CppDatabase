//
// Created by 김태현 on 2022/05/28.
//

#include <gtest/gtest.h>
#include "storage/bp_tree/db_io.h"

TEST(DBPointerTest, DeserializeTest){
  DBPointer db_pointer;

  DBPointer origin("test1twejhetj.db", 17834288);
  auto origin_binary = origin.serialize();
  db_pointer.deserialize(*origin_binary, 0).unwrap();

  ASSERT_EQ(origin, db_pointer);
}