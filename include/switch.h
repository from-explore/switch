//! @file
//! @author Serguei Okladnikov
//! @date 2006.07.04


#ifndef __SWITCH__H__
#define __SWITCH__H__

#include <string.h>

#ifdef __cplusplus

template<class T>
struct SWITCH__D_A_T_A
  {
  bool bEnterFall;
  bool bEnterDefault;
  T strPtrThrSw;
  SWITCH__D_A_T_A( T arg ) : strPtrThrSw(arg) {}
  inline bool cmp(const T& cnst, bool ndeflt)
    {
      if(ndeflt)
        {
          bEnterFall = strPtrThrSw == cnst; // !strcmp(strPtrThrSw,cnst);
          if(bEnterFall) bEnterDefault=false;
        }
      return ndeflt ? bEnterFall:
             bEnterDefault;
    }
  inline bool fall(const T& cnst, bool ndeflt)
    {
      if(bEnterFall)
        {
          return true;
        }
      else
        {
          if(ndeflt)
            {
	          bEnterFall = strPtrThrSw == cnst; // !strcmp(strPtrThrSw,cnst);
              if(bEnterFall) bEnterDefault=false;
              return bEnterFall;
            }
          else
            return bEnterDefault;
        }
	}
  };


#if __cpluplus > 199711
#define SWITCH(arg) if(1){SWITCH__D_A_T_A< typeof(arg) > switch__d_a_t_a(arg); \
 switch__d_a_t_a.bEnterDefault=true;if(switch__d_a_t_a.cmp(
#else
#define SWITCH(arg) if(1){SWITCH__D_A_T_A< decltype(arg) > switch__d_a_t_a(arg); \
 switch__d_a_t_a.bEnterDefault=true;if(switch__d_a_t_a.cmp(
#endif

#define CASE(cnst)  cnst,true)){

#define BREAK       switch__d_a_t_a.bEnterFall=false;}else if(switch__d_a_t_a.cmp(

#define FALL        }if(switch__d_a_t_a.fall(

#define DEFAULT     switch__d_a_t_a.strPtrThrSw,false)){

#define END         }};

#else // not defined __cplusplus

typedef struct tagSWITCH__D_A_T_A
  {
  int bEnterFall;
  int bEnterDefault;
  const char* strPtrThrSw;
  } SWITCH__D_A_T_A;

  int SWITCH__D_A_T_A_cmp(SWITCH__D_A_T_A* data, const char*cnst, int ndeflt);
  int SWITCH__D_A_T_A_fall(SWITCH__D_A_T_A* data, const char* cnst, int ndeflt);

#define SWITCH(arg) if(1){SWITCH__D_A_T_A switch__d_a_t_a; \
 switch__d_a_t_a.strPtrThrSw=arg; \
 switch__d_a_t_a.bEnterDefault=1; \
 if(SWITCH__D_A_T_A_cmp(&switch__d_a_t_a,

#define CASE(cnst)  cnst,1)){

#define BREAK       switch__d_a_t_a.bEnterFall=0;}else if(SWITCH__D_A_T_A_cmp(&switch__d_a_t_a,

#define FALL        }if(SWITCH__D_A_T_A_fall(&switch__d_a_t_a,

#define DEFAULT     switch__d_a_t_a.strPtrThrSw,0)){

#define END         }};


#endif // defined __cplusplus / not defined __cplusplus

#endif // __SWITCH__H__