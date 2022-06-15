//
// Created by 김태현 on 2022/05/05.
//

#ifndef CPPDATABASE_INCLUDES_RECORD_TABLE_H_
#define CPPDATABASE_INCLUDES_RECORD_TABLE_H_

#include <schema.h>
#include <ostream>
#include <record.h>
#include "bp_tree.h"

class Table{
 public:
  Table(SchemaShared schema, BPTreeShared storage);
  Result<bool, InsertionError> add_record(const Record& record);


  SchemaShared schema;
  friend std::ostream &operator<<(std::ostream &os, const Table &table);
  BPTreeShared storage;
 private:
};


Table::Table(SchemaShared schema, BPTreeShared storage)
: schema(std::move(schema)), storage(std::move(storage)){

}

Result<bool, InsertionError> Table::add_record(const Record& record){
  auto key = record.get_pk_field().unwrap();
  if (!storage->insert(std::make_shared<Field>(key), std::make_shared<Record>(record), false)){
    return Err(InsertionError("Cannot insert record in the storage!"));
  }
  return Ok(true);
}

std::ostream &operator<<(std::ostream &os, const Table &table) {
  os << "schema: " << table.schema << " storage: " << table.storage;
  return os;
}

#endif //CPPDATABASE_INCLUDES_RECORD_TABLE_H_
