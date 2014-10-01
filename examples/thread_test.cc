#include "iRRAM/core.h"
#include <unistd.h>

using namespace iRRAM;

using std::string;

REAL f (const REAL& x) { return x+1; }

class my_arg_type { public: string s; long long n; };

double compute (const my_arg_type& arg){
  REAL x = arg.s; // with an automatic conversion
  for (long long i=0 ; i < arg.n; i++ ){
	  x=f(x);
	}
      	return x.as_double(); //now with explicit conversion
}

int main (int argc,char **argv){
  iRRAM_initialize(argc,argv);
  cout << "Starting an example with two threads...\n";

  iRRAM_thread_data<my_arg_type,double> t_1, t_2;

  my_arg_type arg;

  arg.s="0.3456789"; arg.n=345678;
  iRRAM_thread_exec(compute,arg,t_1);

  arg.s="0.9876543"; arg.n=987654;
  iRRAM_thread_exec(compute,arg,t_2);

  //just do something else or just wait...
  //but don't touch the result variables!
  //Here we just print, whether the process are already finished...
  cout << "Threads are running now\n"
       << "They will finish in a few seconds...\n";
  for (int i=1; i<=10; i++){
    cout<<"Ready: "<< iRRAM_thread_finished(t_1)<<iRRAM_thread_finished(t_2)<<"\n";
    usleep(200000);
  }

  //now print the results
  cout << std::setprecision(17);

  double d_1 = iRRAM_thread_wait(t_1);
  cout << "First result:  " << d_1 << "\n";

  double d_2 = iRRAM_thread_wait(t_2);
  cout << "Second result: " << d_2 << "\n";
}
