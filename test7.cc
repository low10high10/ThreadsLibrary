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

//Tests yield case where first runs and creates others threads then yields

int goodbye(int i);
//Testing thread_libinit
int hello(){


  for(int i=0;i<5;i++){

    thread_create((thread_startfunc_t)goodbye, (void*)i);
  }

	thread_yield();

  cout << "hello" << endl;

  return 0;
}

int goodbye(int i){
  cout << "goodbye " << i << endl;
  //thread_yield();
  cout << "thread " << i << " has gotten out of yield "<< endl;
}

int main(int argc, char *argv[]) {
  cout << "started running" << endl;

  thread_libinit((thread_startfunc_t)hello, (void*)0);

}
