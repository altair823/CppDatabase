//
// Created by 김태현 on 2022/05/28.
//

#include <gtest/gtest.h>
#include "storage/bp_tree/db_io.h"

TEST(DBPOinterTest, SerializeTest){
  DBPointer db_pointer("test1.db", 17834288);
  db_pointer.serialize();
}