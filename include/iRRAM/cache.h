/*

iRRAM_cache.h -- caching routines for the iterative structure of the iRRAM library
 
Copyright (C) 2001-2003 Norbert Mueller
 
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

#ifndef iRRAM_cache_h
#define iRRAM_cache_h

#include <string>

namespace iRRAM {

inline void noclearfct(void*){};
inline void MPclear(void* x){MP_clear(*(MP_type*)(x));};
inline void MPIclear(void* x){MP_int_clear(*(MP_int_type*)(x));};

class iRRAM_cache_type{
public:
virtual void clear()=0;
virtual void rewind()=0;
};

class cachelist{public:
iRRAM_cache_type* id[50];
};

extern __thread cachelist * cache_active;
extern __thread int max_active;

template <class DATA,void clearfct(void*)> class iRRAM_cache : public iRRAM_cache_type
{
public:
DATA *data;
unsigned int current,end,size;
bool active;

iRRAM_cache(){active=false;data=NULL;size=0;end=0;current=0;};

void put(const DATA& x){
  if (unlikely(!active)){activate();}
  if ( unlikely(end>=size)){
    if ( unlikely(size> 1000000000) ) 
       throw iRRAM_Numerical_Exception(iRRAM_cacheerror_test);
    DATA* temp=data;
    data=new DATA[2*size];
    for (unsigned int i=0; i< size;i++) data[i]=temp[i];
    size=2*size;
    delete []temp;
  }
  data[end++]=x;
  current++;
};

bool get(DATA& x){
  if (current>=end)return false;
    x=data[current++];
  return true; 
}

void modify(DATA& x){
  data[current-1]=x;
}

void rewind(){
  current=0;
};

void clear(){
  if (clearfct!=noclearfct) {
    for (current=0;current<end; current++) clearfct(&(data[current]));
  }
  delete []data;
  active=false;
  size=0;
  current=0;
  end=0;
};

void activate(){
  size=1;
  data=new DATA[size];
  active=true;
  cache_active->id[max_active++]=this;
  current=0;
  end=0;
};

};

class iRRAM_thread_data_class { public:

iRRAM_cache<bool,noclearfct> cache_b;
iRRAM_cache<short,noclearfct> cache_sh;
iRRAM_cache<unsigned short,noclearfct> cache_ush;
iRRAM_cache<int,noclearfct> cache_i;
iRRAM_cache<long,noclearfct> cache_l;
iRRAM_cache<unsigned long,noclearfct> cache_ul;
iRRAM_cache<double,noclearfct> cache_d;
iRRAM_cache<long long,noclearfct> cache_ll;
iRRAM_cache<unsigned int,noclearfct> cache_ui;
iRRAM_cache<unsigned long long,noclearfct> cache_ull;
iRRAM_cache<std::string,noclearfct> cache_s;
iRRAM_cache<float,noclearfct> cache_f;
iRRAM_cache<void*,noclearfct> cache_v;
iRRAM_cache<MP_type,MPclear> cache_mp;
iRRAM_cache<MP_int_type,MPIclear> cache_mpi;
iRRAM_cache<std::ostream*,noclearfct> cache_os;
iRRAM_cache<std::istream*,noclearfct> cache_is;

};

extern __thread iRRAM_thread_data_class *iRRAM_thread_data_address; 


} // namespace iRRAM

#endif //define iRRAM_cache_h
