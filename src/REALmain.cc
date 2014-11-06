/*

REALmain.cc -- implementation of main initialization routine
               for the iRRAM library
 
Copyright (C) 2003-2008 Norbert Mueller
 
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
#include <cmath>
#include <cstdlib>
#include <cstdarg>
#include <cstring>


#include <iRRAM/core.h>


namespace iRRAM {


/* for debugging (time measuring):*/
#if defined(_WIN64) || defined(_WIN32)
#include <windows.h>
#include <psapi.h>
void resources(double &time, unsigned int &memory)
{
	FILETIME creation_time, exit_time, kernel_time, user_time;
	if (GetProcessTimes(GetCurrentProcess(), &creation_time, &exit_time, &kernel_time, &user_time)) {
		time = ((kernel_time.dwLowDateTime + ((uint64_t)kernel_time.dwHighDateTime << 32))
		        + (user_time.dwLowDateTime + ((uint64_t)  user_time.dwHighDateTime << 32))
		       ) / 1e7;
	} else {
		time = 0;
	}

	PROCESS_MEMORY_COUNTERS pmc;
	if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
		memory = pmc.WorkingSetSize;
	} else {
		memory = 0;
	}
}
#else
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

void resources(double& time, unsigned int& memory){
 struct rusage r;
 getrusage (RUSAGE_SELF,&r);
 time = r.ru_utime.tv_sec+0.000001*r.ru_utime.tv_usec;
// the following is not yet evaluated by linux.... 
 memory = r.ru_ixrss+r.ru_idrss+r.ru_isrss;
}
#endif
/* end of debugging aids */


__thread int iRRAM_debug=0;
__thread int iRRAM_infinite=0;

const int iRRAM_prec_steps=512;
int iRRAM_prec_array[iRRAM_prec_steps];

__thread int iRRAM_prec_skip=5;
__thread int iRRAM_max_prec=1;
__thread int iRRAM_prec_start=1;

void show_statistics()
{
  cerr << "   MP-objects in use:  "<<MP_var_count<<"\n"; 
#ifdef	MP_space_count
  cerr << "   MP-memory in use:   "<<MP_space_count<<"\n"; 
  cerr << "   max MP-memory used: "<<MP_max_space_count<<"\n"; 
#endif
  double time;
  unsigned int memory;
  resources(time,memory);
  cerr << "   CPU-Time for ln2:   "<<ln2_time<<" s\n";
  cerr << "   CPU-Time for pi:    "<<pi_time<<" s\n";
  cerr << "   total CPU-Time:     "<<time<<" s\n";
  //cerr << "   total Memory:       "<<memory/1024<<" KB\n";
  if ( ACTUAL_STACK.prec_step != 1) 
    cerr << "   basic precision:    "<<ACTUAL_STACK.actual_prec
		<<"["<<ACTUAL_STACK.prec_step<<"]\n"; 
  else 
  cerr << "   basic precision:    double\n"; 
  if ( iRRAM_max_prec != 1) 
    cerr << "   maximal precision:  "<<iRRAM_prec_array[iRRAM_max_prec]
		<<"["<<iRRAM_max_prec<<"]\n"; 
  else 
    cerr << "   maximal precision:  double\n"; 
}

extern "C" void iRRAM_initialize(int argc,char **argv)
{
int iRRAM_starting_prec=-50;
int iRRAM_prec_inc=-20;
double iRRAM_prec_factor=1.25;

for (int i=0;i<argc;i+=1)
{
if (!strncmp(argv[i],"-d",2)) 
	{
	iRRAM_debug=1;
	iRRAM_DEBUG1(1,"Debugging Mode\n");
	}
if (!strncmp(argv[i],"--debug=",8)) 
	{
	iRRAM_debug=atoi(&(argv[i][8]));
	iRRAM_DEBUG2(1,"Debugging Level %d\n",iRRAM_debug);
	}
if (!strncmp(argv[i],"--prec_init=",12)) 
	{
	iRRAM_starting_prec=atoi(&(argv[i][12]));
	iRRAM_DEBUG2(1,"Initialising precision to 2^(%d)\n",iRRAM_starting_prec);
	}
if (!strncmp(argv[i],"--prec_inc=",11)) 
	{
	int hi;
	hi=atoi(&(argv[i][11]));
	if (hi>0) iRRAM_prec_inc=-hi;
	iRRAM_DEBUG2(1,"Initialising precision increment to %d bits\n",-iRRAM_prec_inc);
	}
if (!strncmp(argv[i],"--prec_factor=",14)) 
	{
	double hd;
	hd=atof(&(argv[i][14]));
	if (hd>1.0) iRRAM_prec_factor=hd;
	iRRAM_DEBUG2(1,"Initialising precision factor to %f\n",iRRAM_prec_factor);
	}
if (!strncmp(argv[i],"--prec_skip=",12)) 
	{
	int hi;
	hi=atoi(&(argv[i][12]));
	if (hi>0) iRRAM_prec_skip=hi;
	iRRAM_DEBUG2(1,"Changed heuristic for precision changes to skip at most %d steps\n",iRRAM_prec_skip);
	}

if (!strncmp(argv[i],"--prec_start=",13)) 
	{
	int hi;
	hi=atoi(&(argv[i][13]));
	if (hi>0) iRRAM_prec_start=hi;
	iRRAM_DEBUG2(1,"Changed inital precision step to %d \n",iRRAM_prec_start);
	}


if (!strncmp(argv[i],"-h",2) || !strncmp(argv[i],"--help",6)) 
	{
	cerr 	<< "Runtime parameters for the iRRAM library:\n"
		<< "--prec_init=n   ["<<iRRAM_starting_prec<<"]  starting precision\n"
		<< "--prec_inc=n    ["<<iRRAM_prec_inc<<"]  basic increment for precision changes\n"
		<< "--prec_factor=x ["<<iRRAM_prec_factor<<"] basic factor for precision changes\n"
		<< "--prec_skip=n   [5]    bound for precision increments skipped by heuristic\n"
		<< "--prec_start=n   [1]   inital precision level\n"
		<< "--debug=n       [1]    level of limits up which debugging should happen\n"
		<< "-d                      debug mode, with level 1\n"
		<< "-h / --help             this small help\n";
	}
}
MP_initialize;

iRRAM_prec_array[0]=2100000000;
iRRAM_prec_array[1]=iRRAM_starting_prec;
int prec_inc=iRRAM_prec_inc;
double factor=std::sqrt(std::sqrt(iRRAM_prec_factor));
if (iRRAM_debug) cerr << "Basic precision bounds: " << "double[1]"; 
for (int i=2;i<iRRAM_prec_steps;i++){
	iRRAM_prec_array[i]=iRRAM_starting_prec+prec_inc;
	prec_inc=int(prec_inc * factor)+iRRAM_prec_inc;
	if (iRRAM_prec_array[i] >= iRRAM_prec_array[i-1])
		iRRAM_prec_array[i]= iRRAM_prec_array[i-1];
	else if (iRRAM_debug && ( (i%5==0) || (i<10) ) ) cerr << " " << iRRAM_prec_array[i]<< "["<<i<<"]"; 
	}
if (iRRAM_debug) cerr << "\n"; 

}

} // namespace iRRAM

