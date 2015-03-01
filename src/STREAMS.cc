/*

STREAMS.cc -- implementation of the C++-style IO for the iRRAM library
 
Copyright (C) 2004 Norbert Mueller
 
This file is part of the iRRAM Library.
 
The iRRAM Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Library General Public License as published by
the Free Software Foundation; either version 2 of the License, or (at your
option) any later version.
 
The iRRAM Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
License for more details.
 
You should have received a copy of the GNU Library General Public License
along with the iRRAM Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
MA 02111-1307, USA. 
*/


#include <iRRAM/core.h>

#include <fstream>
#include <iostream>

namespace iRRAM {

orstream cerr(&std::cerr,false);
orstream clog(&std::clog,false);
orstream cout;
irstream cin;

__thread long long rstream::requests = 0;
__thread long long rstream::outputs = 0;

orstream::orstream(std::ostream *s,bool respect_iteration){
  target = s; 
  iRRAM_DEBUG1(2,"I/O-handler: Creating output stream \n");\
  _respect_iteration=respect_iteration;  
  real_w = 20;
  real_f = iRRAM_float_absolute;
}
orstream::orstream(){
  target = &std::cout;
  iRRAM_DEBUG1(2,"I/O-handler: Creating output stream cout\n");\
  _respect_iteration=true;
  real_w = 20;
  real_f = iRRAM_float_absolute;
}
orstream::orstream(std::string s, std::ios::openmode mod){
  if (ACTUAL_STACK.inlimit>0) {				
    iRRAM_DEBUG1(2,"I/O-handler: Operation illegal in continuous section!\n");	
    return;
  }
  _respect_iteration=true;
  real_w = 20;
  real_f = iRRAM_float_absolute;
 
  if (ACTUAL_STACK.inlimit==0){
    if (iRRAM_thread_data_address->cache_os.get(target)){
/*    iRRAM_thread_data_address->cache_ui.get(real_w);
    iRRAM_thread_data_address->cache_ui.get(real_f);
    iRRAM_thread_data_address->cache_b.get(_respect_iteration);*/
    iRRAM_DEBUG1(2,"I/O-handler: Recreating output stream '"<< s <<"'("<< real_w<<")\n");\
      return;
    }
   target = new std::ofstream(s.c_str(),mod);	
    iRRAM_DEBUG1(2,"I/O-handler: Creating new output stream '"<< s <<"'\n");\
    iRRAM_thread_data_address->cache_os.put(target);
/*    iRRAM_thread_data_address->cache_ui.put(real_w);
    iRRAM_thread_data_address->cache_ui.put(real_f);
    iRRAM_thread_data_address->cache_b.put(_respect_iteration);*/
  } else {
    target = new std::ofstream(s.c_str(),mod);
    iRRAM_DEBUG1(2,"I/O-handler: Creating new output stream '"<< s <<"'\n");\
  }
  return;
}

irstream::irstream(){
   iRRAM_DEBUG1(2,"I/O-handler: Creating input stream cin\n");\
   target = &std::cin;
}

irstream::irstream(std::string s, std::ios::openmode mod){
  if (ACTUAL_STACK.inlimit>0) {
    iRRAM_DEBUG1(2,"I/O-handler: Operation illegal in continuous section!\n");	
    return;
  }
  if (ACTUAL_STACK.inlimit==0){
    if (iRRAM_thread_data_address->cache_is.get(target)){
      return;
    }
    iRRAM_DEBUG1(2,"I/O-handler: Creating new input stream '"<< s << "'\n");\
    target = new std::ifstream(s.c_str(),mod);
    iRRAM_thread_data_address->cache_is.put(target);
  } else {
    iRRAM_DEBUG1(2,"I/O-handler: Creating new input stream '"<< s << "'\n");\
    target = new std::ifstream(s.c_str(),mod);
  }
  return;
}




void orstream::rewind(){ requests = 0; }

void orstream::reset() { requests = 0; outputs = 0; }

#define iRRAM_outexec(x) \
  if (inReiterate) {\
    iRRAM_DEBUG1(2,"I/O-Handler: In iteration, so stream will be closed later.\n");\
    return;\
  }\
  if (ACTUAL_STACK.inlimit>0) {\
    iRRAM_DEBUG1(2,"illegal output in continuous section!\n");\
    return;\
  }\
  if (ACTUAL_STACK.inlimit==0){\
  ACTUAL_STACK.inlimit++;\
  x;\
  ACTUAL_STACK.inlimit--;}else{ x;}

template <class PARAM>
orstream& iRRAM_out(orstream* s,const PARAM &x){
  if ( (ACTUAL_STACK.inlimit>0)  &&  s->_respect_iteration) {
    iRRAM_DEBUG1(2,"illegal output in continuous section!\n");
    return *s;
  }
  if ( (ACTUAL_STACK.inlimit==0) && s->_respect_iteration){
  ACTUAL_STACK.inlimit++;
  if ((++rstream::requests) > rstream::outputs){
    *(s->target) << x;
    (rstream::outputs)++;
  }
  ACTUAL_STACK.inlimit--;}else{ *(s->target) << x;}
  return *s;
}
  
#define iRRAM_out2(x) \
  if ( (ACTUAL_STACK.inlimit>0) && _respect_iteration) {\
    iRRAM_DEBUG1(2,"I/O-handler: Illegal output in continuous section!\n");\
    return *this;\
  }\
  x;\
  return *this;

typedef  const char* char_p;

orstream& orstream::operator<<(const bool c) {return iRRAM_out<bool>(this, c);}
orstream& orstream::operator<<(const char* c)  {return iRRAM_out<char_p>(this, c);}
orstream& orstream::operator<<(const short i)    {return iRRAM_out<short>(this, i);}
orstream& orstream::operator<<(const unsigned short i)    {return iRRAM_out<unsigned short>(this, i);}
orstream& orstream::operator<<(const int i)    {return iRRAM_out<int>(this, i);}
orstream& orstream::operator<<(const unsigned int ui)    {return iRRAM_out<unsigned int>(this, ui);}
orstream& orstream::operator<<(const long i)    {return iRRAM_out<long>(this, i);}
orstream& orstream::operator<<(const unsigned long ui)    {return iRRAM_out<unsigned long>(this, ui);}
orstream& orstream::operator<<(const double d) {return iRRAM_out<double>(this, d);}
orstream& orstream::operator<<(const float d) {return iRRAM_out<float>(this, d);}
orstream& orstream::operator<<(const std::string& s) {return iRRAM_out<std::string>(this, s);}
orstream& orstream::operator<<(const long long ll){return iRRAM_out<long long>(this, ll);}
orstream& orstream::operator<<(const unsigned long long ull){return iRRAM_out<unsigned long long>(this, ull);}

orstream& orstream::operator<<(const REAL& x) {return iRRAM_out<>(this, swrite(x,real_w,real_f));}
orstream& orstream::operator<<(const DYADIC& x) {return iRRAM_out<>(this, swrite(x,real_w));}
orstream& orstream::operator<<(const INTEGER& x) {return iRRAM_out<>(this, swrite(x,real_w));}
orstream& orstream::operator<<(const RATIONAL& x) {return iRRAM_out<>(this, swrite(x,real_w));}

orstream& orstream::operator<<(decltype(std::setw(0)) _f) {return  iRRAM_out<>(this, _f);}
orstream& orstream::operator<<(decltype(std::setprecision(0)) _f) {return  iRRAM_out<>(this, _f);}
orstream& orstream::operator<<(decltype(std::setfill('*')) _f) {return  iRRAM_out<>(this, _f);}
orstream& orstream::operator<<(decltype(std::setiosflags(std::ios_base::dec)) _f) {return  iRRAM_out<>(this, _f);}
orstream& orstream::operator<<(decltype(std::resetiosflags(std::ios_base::dec)) _f) { return iRRAM_out<>(this, _f);}
orstream& orstream::operator<<(decltype(std::setbase(0)) _f) {return  iRRAM_out<>(this, _f);}
orstream& orstream::operator<<( std::ostream& _f (std::ostream&) ) {return  iRRAM_out<>(this, _f);}

orstream& orstream::operator<<( _SetRwidth _f) { iRRAM_out2(real_w=_f._M_n);}
orstream& orstream::operator<<( _SetRflags _f) { iRRAM_out2(real_f=_f._M_n);}

orstream::~orstream(){
  if ((++requests) > outputs){
    if ( (target != &std::cout) && _respect_iteration){
      iRRAM_DEBUG1(2,"I/O-handler: Closing handler for output stream\n");
      iRRAM_outexec(delete target;target=0;); 
    }
    outputs++;
  }
}

irstream::~irstream(){
  if ((++requests) > outputs){
    if (target != &std::cin){
      iRRAM_DEBUG1(2,"I/O-Handler: Closing handler for input stream\n");
      iRRAM_outexec(delete target;target=0;);
    }
    outputs++;
  }  
}


#define iRRAM_in(VAR,CACHE) \
  if (ACTUAL_STACK.inlimit>0) {				\
    iRRAM_DEBUG1(2,"illegal input in continuous section!\n");	\
    return *this; \
  }							\
  if (ACTUAL_STACK.inlimit==0){\
  if (CACHE.get(VAR)){ return *this;}			\
  ACTUAL_STACK.inlimit++;				\
  *target >> VAR;						\
  ACTUAL_STACK.inlimit--;				\
  CACHE.put(VAR);}else{*target >> VAR;} \
  return *this;


irstream& irstream::operator>>(bool& b) {iRRAM_in(b,iRRAM_thread_data_address->cache_b);}
irstream& irstream::operator>>(short& i) {iRRAM_in(i,iRRAM_thread_data_address->cache_sh);}
irstream& irstream::operator>>(unsigned short& ui) {iRRAM_in(ui,iRRAM_thread_data_address->cache_ush);}
irstream& irstream::operator>>(int& i) {iRRAM_in(i,iRRAM_thread_data_address->cache_i);}
irstream& irstream::operator>>(unsigned int& ui) {iRRAM_in(ui,iRRAM_thread_data_address->cache_ui);}
irstream& irstream::operator>>(long& l) {iRRAM_in(l,iRRAM_thread_data_address->cache_l);}
irstream& irstream::operator>>(unsigned long& ul) {iRRAM_in(ul,iRRAM_thread_data_address->cache_ul);}
irstream& irstream::operator>>(float& d) {iRRAM_in(d,iRRAM_thread_data_address->cache_f);}
irstream& irstream::operator>>(double& d) {iRRAM_in(d,iRRAM_thread_data_address->cache_d);}
irstream& irstream::operator>>(long long& ll) {iRRAM_in(ll,iRRAM_thread_data_address->cache_ll);}
irstream& irstream::operator>>(unsigned long long& ull) {iRRAM_in(ull,iRRAM_thread_data_address->cache_ull);}
irstream& irstream::operator>>(std::string& s) {iRRAM_in(s,iRRAM_thread_data_address->cache_s);}

#define iRRAM_in2(VAR,DATA) 				\
  std::string s; 						\
  if (ACTUAL_STACK.inlimit>0) {				\
    iRRAM_DEBUG1(2,"I/O-handler: Illegal input in continuous section!\n");	\
    return *this;					\
  }							\
  if (ACTUAL_STACK.inlimit==0){				\
  if (iRRAM_thread_data_address->cache_s.get(s)){VAR=DATA(s); return *this;}	\
  ACTUAL_STACK.inlimit++;				\
  *target >> s ;					\
  VAR=DATA(s);						\
  ACTUAL_STACK.inlimit--;				\
  iRRAM_thread_data_address->cache_s.put(s);}else{*target >> s ;VAR=DATA(s);};			\
  return *this;

irstream&  irstream::operator>>(REAL& d){iRRAM_in2(d,REAL);}
irstream&  irstream::operator>>(INTEGER& d){iRRAM_in2(d,INTEGER);}
//irstream&  irstream::operator>>(DYADIC& d){iRRAM_in2(d,DYADIC);}

#define iRRAM_inexec(VAR,CACHE,STMNT) \
  if (ACTUAL_STACK.inlimit>0) {				\
    iRRAM_DEBUG1(2,"I/O-handler: Illegal input in continuous section!\n");	\
    return VAR; \
  }							\
  if (ACTUAL_STACK.inlimit==0){\
  if (CACHE.get(VAR)){return VAR;}			\
  ACTUAL_STACK.inlimit++;				\
  STMNT;						\
  ACTUAL_STACK.inlimit--;				\
  CACHE.put(VAR);}else{STMNT;}\
  return VAR;

bool orstream::eof(){
bool test=false; iRRAM_inexec(test,iRRAM_thread_data_address->cache_b,test=target->eof()) }

bool orstream::fail(){
bool test=false; iRRAM_inexec(test,iRRAM_thread_data_address->cache_b,test=target->fail()) }

bool orstream::good(){
bool test=false; iRRAM_inexec(test,iRRAM_thread_data_address->cache_b,test=target->good()) }

bool orstream::bad(){
bool test=false; iRRAM_inexec(test,iRRAM_thread_data_address->cache_b,test=target->bad()) }

bool operator !(orstream& x){
bool test=false; iRRAM_inexec(test,iRRAM_thread_data_address->cache_b,test=(x.target->fail())) }

orstream::operator bool(){
bool test=false; iRRAM_inexec(test,iRRAM_thread_data_address->cache_b,test=!(target->fail())) }

bool irstream::eof(){
bool test=false; iRRAM_inexec(test,iRRAM_thread_data_address->cache_b,test=target->eof()) }

bool irstream::fail(){
bool test=false; iRRAM_inexec(test,iRRAM_thread_data_address->cache_b,test=target->fail()) }

bool irstream::good(){
bool test=false; iRRAM_inexec(test,iRRAM_thread_data_address->cache_b,test=target->good()) }

bool irstream::bad(){
bool test=false; iRRAM_inexec(test,iRRAM_thread_data_address->cache_b,test=target->bad()) }

bool operator !(irstream& x){
bool test=false; iRRAM_inexec(test,iRRAM_thread_data_address->cache_b,test=(x.target->fail())) }

irstream::operator bool(){
bool test=false; iRRAM_inexec(test,iRRAM_thread_data_address->cache_b,test=!(target->fail())) }


} // namespace iRRAM
