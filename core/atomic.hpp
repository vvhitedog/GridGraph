/*
Copyright (c) 2014-2015 Xiaowei Zhu, Tsinghua University

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef ATOMIC_H
#define ATOMIC_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <type_traits>
#include <cstring>

template <typename ET, typename std::enable_if<sizeof(ET) == 4,int>::type = 0>
inline bool cas(ET *ptr, ET oldv, ET newv) {
  static_assert(sizeof(int) == 4,"size of int is not 4 bytes on this system");
  int int_oldv, int_newv;
  std::memcpy(&int_oldv,&oldv,sizeof(int));
  std::memcpy(&int_newv,&newv,sizeof(int));
  auto int_ptr = reinterpret_cast<int*>(ptr);
  return __sync_bool_compare_and_swap(int_ptr, int_oldv, int_newv);
}

template <class ET, typename std::enable_if<sizeof(ET) == 8,int>::type = 0>
inline bool cas(ET *ptr, ET oldv, ET newv) {
  static_assert(sizeof(long) == 8,"size of long is not 8 bytes on this system");
  long long_oldv, long_newv;
  std::memcpy(&long_oldv,&oldv,sizeof(long));
  std::memcpy(&long_newv,&newv,sizeof(long));
  auto long_ptr = reinterpret_cast<long*>(ptr);
  return __sync_bool_compare_and_swap(long_ptr, long_oldv, long_newv);
}

template <class ET, typename std::enable_if<sizeof(ET) != 8 && sizeof(ET) != 4,int>::type = 0>
inline bool cas(ET *ptr, ET oldv, ET newv) {
  static_assert(sizeof(ET) != 8 && sizeof(ET) != 4,"unsupported type in use of CAS");
  return {};
}

template <class ET>
inline bool write_min(ET *a, ET b) {
	ET c; bool r=0;
	do c = *a;
	while (c > b && !(r=cas(a,c,b)));
	return r;
}

template <class ET>
inline void write_add(ET *a, ET b) {
	volatile ET newV, oldV;
	do {oldV = *a; newV = oldV + b;}
	while (!cas(a, oldV, newV));
}

#endif
