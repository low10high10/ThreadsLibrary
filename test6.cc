#include "interrupt.h"
#include "thread.h"
#include <ucontext.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>
#include <map>
#include <vector>
using namespace std;


int goodbye(int i);
//Testing thread_yield
int hello(){
	if (thread_yield()) {
		cout << "thread_yield failed\n";
		exit(1);
	}

  for(int i=0;i<5;i++){

		if (thread_create((thread_startfunc_t)goodbye, (void*)i)) {
      cout << "thread_create failed\n";
      exit(1);
    }

  }
  cout << "hello" << endl;

  return 0;
}

int goodbye(int i){
  cout << "goodbye " << i << endl;
	if (thread_yield()) {
		cout << "thread_yield failed\n";
		exit(1);
	}
  cout << "thread " << i << " has gotten out of yield "<< endl;
}

int main(int argc, char *argv[]) {
  cout << "started running" << endl;

	if (thread_libinit((thread_startfunc_t)hello, (void*)0)) {
		cout << "thread_yield libinit\n";
		exit(1);
	}


}
