//
// Created by 김태현 on 2022/06/01.
//

#ifndef CPPDATABASE_INCLUDES_SERIALIZABLE_H_
#define CPPDATABASE_INCLUDES_SERIALIZABLE_H_

#include "mem_core.h"
#include "error.h"
#include "result.h"

class Serializable {
 public:
  [[nodiscard]] virtual BinaryUnique serialize() const = 0;
  virtual Result<BinaryIndex, DeserializeError> deserialize(const Binary &binary, BinaryIndex begin) = 0;
};

#endif //CPPDATABASE_INCLUDES_SERIALIZABLE_H_
