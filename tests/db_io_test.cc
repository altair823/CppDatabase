//
// Created by 김태현 on 2022/05/28.
//

#include <gtest/gtest.h>
#include "storage/bp_tree/db_io.h"

TEST(DBPointerTest, SerializeTest){
  DBPointer db_pointer("test1.db", 17834288);
  db_pointer.serialize();
}

TEST(DBPointerTest, OffsetTest){
  std::cout<<sizeof(off_t)<<std::endl;
}