/*

iRRAM_STREAMS.h -- interface to the C++-style IO for the iRRAM library
 
Copyright (C) 2004-2009 Norbert Mueller
 
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


#ifndef rstreams
#define rstreams
#include <iostream>
#include <istream>
#include <ostream>
#include <iomanip>
#include <string>

namespace iRRAM { 

struct _SetRwidth { int _M_n; };

inline _SetRwidth  setRwidth(int __n) {
  _SetRwidth __x;
  __x._M_n = __n;
  return __x;
}

struct _SetRflags { int _M_n; };

inline _SetRflags  setRflags(int __n) {
  _SetRflags __x;
  __x._M_n = __n;
  return __x;
}

class rstream { 
public:
  static __thread long long requests;
  static __thread long long outputs;
  // the two counters are used to determine whether 
  // output is actually produced.
};

class orstream :public rstream
{
public:
  bool fail();
  bool eof();
  bool bad();
  bool good(); 
  operator bool(); 
  friend  bool operator !(orstream&);
  unsigned int real_w;
  unsigned int real_f;

  orstream(std::string s, std::_Ios_Openmode mod=std::_S_out);
  ~orstream();

  bool is_open();
  void open(std::string s, std::_Ios_Openmode mod=std::_S_out);
  void close();

  orstream& operator<<(const bool b);
  orstream& operator<<(const char* ch);
  orstream& operator<<(const short i);
  orstream& operator<<(const unsigned short i);
  orstream& operator<<(const int i);
  orstream& operator<<(const unsigned int i);
  orstream& operator<<(const long i);
  orstream& operator<<(const unsigned long i);
  orstream& operator<<(const long long i);
  orstream& operator<<(const unsigned long long i);
  orstream& operator<<(const double d);
  orstream& operator<<(const float d);
  orstream& operator<<(const std::string& s);
  orstream& operator<<(const REAL& r);
  orstream& operator<<(const RATIONAL& r);
  orstream& operator<<(const INTEGER& r);
  orstream& operator<<(const DYADIC& d);

  orstream& operator<<(std::_Setfill<char>);
  orstream& operator<<(std::_Setiosflags);

  orstream& operator<<(std::_Resetiosflags);
  orstream& operator<<(std::_Setbase);
  orstream& operator<<(std::_Setprecision);
  orstream& operator<<(std::_Setw);
  orstream& operator<<(_SetRwidth);
  orstream& operator<<(_SetRflags);
  orstream& operator<<(std::ostream&(std::ostream&) );

// the following should be "private:"

  orstream();
  // Default constructor, only for "rout": Will select cout as target.

  orstream(std::ostream *target, bool respect_iteration);
  // Internal constructor, only for "cout", "cerr", and "clog".

  void rewind();
  // this will reset the requests counter.
  void reset();
  // this will reset both counters.

  std::ostream *target;
  bool _respect_iteration;



};

class irstream :public rstream
{
public:
  bool fail();
  bool eof();
  bool bad();
  bool good(); 
  operator bool(); 
  friend  bool operator !(irstream&);

  irstream(std::string s, std::_Ios_Openmode mod=std::_S_in);
  ~irstream();

  bool is_open();
  void open(std::string s, std::_Ios_Openmode mod=std::_S_out);
  void close();

  irstream& operator>>(bool& i);
  irstream& operator>>(short& i);
  irstream& operator>>(unsigned short& i);
  irstream& operator>>(int& i);
  irstream& operator>>(unsigned int& i);
  irstream& operator>>(long& i);
  irstream& operator>>(unsigned long& i);
  irstream& operator>>(double& d);
  irstream& operator>>(float& d);
  irstream& operator>>(long long& d);
  irstream& operator>>(unsigned long long& d);
  irstream& operator>>(std::string& s);
  irstream& operator>>(REAL& d);
  irstream& operator>>(INTEGER& d);
  irstream& operator>>(DYADIC& d);

  irstream& operator>>(std::_Setfill<char>);
  irstream& operator>>(std::_Setiosflags);
  irstream& operator>>(std::_Resetiosflags);
  irstream& operator>>(std::_Setbase);
  irstream& operator>>(std::_Setprecision);
  irstream& operator>>(std::_Setw);

// the following should be "private:"
  irstream();

private:
  std::istream *target;

};

extern irstream cin;
extern orstream cout;
extern orstream cerr;
extern orstream clog;

}

#endif
