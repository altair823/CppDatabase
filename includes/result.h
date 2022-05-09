//
// Created by 김태현 on 2022/05/05.
//

#ifndef CPPDATABASE_INCLUDES_RESULT_H_
#define CPPDATABASE_INCLUDES_RESULT_H_

#include <iostream>

#define _Ok true
#define Ok(ok) Result{_Ok, ok}
#define _Err false
#define Err(e, err) Result{_Err, e, err, std::string(__FILE__), std::string(__FUNCTION__), __LINE__}

template<typename T>
struct Result{
  Result(bool is_ok, T ok, const std::string& err = "", const std::string& file_path = "", const std::string& func_name = "", int line_num = 0)
  : _ok(std::move(ok)), is_ok(is_ok), err("File: " + file_path + "\nFunction: " + func_name + "\nError: " + err), line_num(line_num){}
  bool is_ok = false;
  T _ok;
  std::string err;
  int line_num;

  T unwrap() { if (is_ok) {return std::move(_ok);} else { std::cout << "panic!\n" + err + "\nLine: " << line_num << std::endl; exit(1);} }
};

#endif //CPPDATABASE_INCLUDES_RESULT_H_
