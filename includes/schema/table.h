//
// Created by 김태현 on 2022/05/05.
//

#ifndef CPPDATABASE_INCLUDES_RECORD_TABLE_H_
#define CPPDATABASE_INCLUDES_RECORD_TABLE_H_

#include <schema.h>
#include <storage_interface.h>
#include <ostream>

template <typename Key, typename Value>
class Table{
 public:
  Table(SchemaShared schema, StorageShared<Key, Value> storage);
  Result<bool> add_record(Value record);


  SchemaShared schema;
  friend std::ostream &operator<<(std::ostream &os, const Table<Key, Value> &table);
  StorageShared<Key, RecordUnique> storage;
 private:
};
template<typename Key, typename Value>
Table<Key, Value>::Table(SchemaShared schema, StorageShared<Key, Value> storage)
: schema(std::move(schema)), storage(std::move(storage)){

}
template<typename Key, typename Value>
Result<bool> Table<Key, Value>::add_record(Value record) {
  if (!schema->verify_record(record.get())){
    return Err(false, "The record does not match with schema!");
  }
  auto key = record->get_field(schema->pk_name).unwrap()->data;
  if (!storage->insert(key, std::move(record)).is_ok){
    return Err(false, "Cannot insert record in the storage!");
  }
  return Ok(true);
}

template<typename Key, typename Value>
std::ostream &operator<<(std::ostream &os, const Table<Key, Value> &table) {
  os << "schema: " << table.schema << " storage: " << table.storage;
  return os;
}

#endif //CPPDATABASE_INCLUDES_RECORD_TABLE_H_
