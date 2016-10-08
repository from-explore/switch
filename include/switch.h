//! @file
//! @author Serguei Okladnikov oklaspec@gmail.com
//! @date 2006.07.04

/*-----------------------------------------------------------------------------

Software License Agreement (BSD License)
=================================================

Copyright (c) 2006-present, Serguei Okladnikov. All rights reserved.
--------------------------------------------------------------------

Redistribution and use of this software in source and binary forms, with or
without modification, are permitted provided that the following conditions
are met:

  * Redistributions of source code must retain the above copyright notice,this
    list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  * Neither the name of the copyright holder nor the names of its contributors
    may be used to endorse or promote products derived from this software
    without specific prior written permission of copyright holder.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

-----------------------------------------------------------------------------*/


#ifndef __SWITCH__H__
#define __SWITCH__H__

#include <string.h>

#ifdef __cplusplus

#if ( __cplusplus >= 201103L || defined(SWITCH_NG) )
#define SWITCH_DECLTYPE decltype
#else
#define SWITCH_DECLTYPE typeof
#endif

#if __cplusplus >= 201103L && defined(SWITCH_QUICK)

#include <vector>
#include <map>
#include <functional>

template<class T>
struct SWITCH__D_A_T_A {
  typedef std::function<bool()> t_cb;
  typedef std::map<T,int> t_map;
  struct Entry {
    bool fall;
    t_cb cb;
  };
  const T* data;
  t_cb dfcb;
  bool havedf;
  bool initialized;
  std::vector<Entry> entries;
  t_map map;
  SWITCH__D_A_T_A() : havedf(false), initialized(false) {}
  inline SWITCH__D_A_T_A& base_init( const T& arg ) {
    data = &arg;
    return *this;
  }
  inline SWITCH__D_A_T_A& initialize_done() {
    initialized = true;
    return *this;
  }
  inline SWITCH__D_A_T_A& transition(bool ndeflt, const T& cnst, t_cb cb, bool fall) {
    if( !ndeflt ) {
      dfcb = cb;
      havedf = true;
      return *this;
    }
    int pos = entries.size();
    entries.push_back( Entry({fall, cb}) );
    map[cnst] = pos;
    return *this;
  }
  inline void doit() {
    initialized = true;
    typename t_map::iterator it = map.find(*data);
    if( map.end() == it ) {
      if( havedf ) dfcb();
      return;
    }
    int pos = it->second;
    do {
      Entry& e = entries[pos];
      bool fall = e.cb();
      if( !( e.fall && fall ) ) return;
      ++pos;
      if( entries.size() == pos ) {
        if( havedf ) dfcb();
        return;
      }
    } while(true);
    return;
  }
  void cpp11(){};
};

#define SWITCH(arg) SWITCH_DYNAMIC(arg)

#define SWITCH_STATIC(arg) if(1){ \
  static SWITCH__D_A_T_A< SWITCH_DECLTYPE(arg) > switch__d_a_t_a; \
  switch__d_a_t_a.base_init(arg); \
  switch__d_a_t_a.initialized ? switch__d_a_t_a.doit() : switch__d_a_t_a
#define SWITCH_DYNAMIC(arg) if(1){ \
  SWITCH__D_A_T_A< SWITCH_DECLTYPE(arg) > switch__d_a_t_a; \
  switch__d_a_t_a.base_init(arg)

#define CASE(cnst)  .transition(true, cnst, [&]()->bool {

#define BREAK       ;return true; }, false)

#define FALL        ;return true; }, true)

#define DEFAULT     .transition(false, *switch__d_a_t_a.data, [&]()->bool {

#define END         ;return true; }, false).initialize_done().doit();}



#else // SWITCH_QUICK


template<class T>
struct SWITCH__D_A_T_A {
  bool bEnterFall;
  bool bEnterDefault;
  bool bDone;
  T strPtrThrSw;
  SWITCH__D_A_T_A( T arg ) : strPtrThrSw(arg) {}
  inline bool transition(bool fall, const T& cnst, bool ndeflt) {
    if(bDone)
       return false;

    if(fall && bEnterFall)
      return 1;

    if(!fall && bEnterFall) {
      bDone = 1;
      return 0;
    }

    if(ndeflt) {
      bEnterFall = strPtrThrSw == cnst;
      if(bEnterFall) bEnterDefault=false;
    }

    return ndeflt ?
      bEnterFall :
      bEnterDefault;
  }
  void cpp97(){};
};


#if ( __cplusplus >= 201103L || defined(SWITCH_NG) )
#define SWITCH(arg) if(1){SWITCH__D_A_T_A< SWITCH_DECLTYPE(arg) > switch__d_a_t_a(arg); \
 switch__d_a_t_a.bEnterDefault=true;switch__d_a_t_a.bEnterFall=false; \
 switch__d_a_t_a.bDone=false;if(switch__d_a_t_a.transition(false,
#else
#define SWITCH(arg) if(1){SWITCH__D_A_T_A< SWITCH_DECLTYPE(arg) > switch__d_a_t_a(arg); \
 switch__d_a_t_a.bEnterDefault=true;switch__d_a_t_a.bEnterFall=false; \
 switch__d_a_t_a.bDone=false;if(switch__d_a_t_a.transition(false,
#endif

#define CASE(cnst)  cnst,true)){

#define BREAK       switch__d_a_t_a.bDone=true; \
                    ;} if(switch__d_a_t_a.transition(false,

#define FALL        ;} if(switch__d_a_t_a.transition(true,

#define DEFAULT     switch__d_a_t_a.strPtrThrSw,false)){

#define END         ;}};


#endif // SWITCH_QUICK


#else // not defined __cplusplus

typedef struct tagSWITCH__D_A_T_A
  {
  int bEnterFall : 1;
  int bEnterDefault : 1;
  int bDone : 1;
  const char* strPtrThrSw;
  } SWITCH__D_A_T_A;


int SWITCH__D_A_T_A_transition(
  SWITCH__D_A_T_A* data, int fall, const char*cnst, int ndeflt);

#ifdef SWITCH_IMPL
inline
int SWITCH__D_A_T_A_transition(
  SWITCH__D_A_T_A* data, int fall, const char*cnst, int ndeflt) {

  if(data->bDone)
    return 0;

  if(fall && data->bEnterFall)
    return 1;

  if(!fall && data->bEnterFall) {
    data->bDone = 1;
    return 0;
  }

  if(ndeflt) {
    data->bEnterFall=!strcmp(data->strPtrThrSw,cnst);
    if(data->bEnterFall) data->bEnterDefault=0;
  }

  return ndeflt ?
    data->bEnterFall :
    data->bEnterDefault;
}
#endif // SWITCH_IMPL

#define SWITCH(arg) if(1){SWITCH__D_A_T_A switch__d_a_t_a; \
 switch__d_a_t_a.strPtrThrSw=arg; \
 switch__d_a_t_a.bEnterDefault=1; \
 switch__d_a_t_a.bEnterFall=0; \
 switch__d_a_t_a.bDone=0; \
 if(SWITCH__D_A_T_A_transition(&switch__d_a_t_a, 0,

#define CASE(cnst)  cnst,1)){

#define BREAK       switch__d_a_t_a.bDone=1; \
                    ;} if(SWITCH__D_A_T_A_transition(&switch__d_a_t_a, 0,

#define FALL        ;} if(SWITCH__D_A_T_A_transition(&switch__d_a_t_a, 1,

#define DEFAULT     switch__d_a_t_a.strPtrThrSw,0)){

#define END         ;}};


#endif // defined __cplusplus / not defined __cplusplus

#endif // __SWITCH__H__