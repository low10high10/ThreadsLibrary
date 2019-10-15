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

int goodbye();
//Tests if thread_wait works correctly.

int goodbye2(){
  thread_lock(0);
  //thread_unlock(0);
  cout << "SHOULD NOT PRINT THIS BC OF DEADLOCK" << endl;

}

int hello(){
	//thread_unlock(0);
  thread_lock(0);
  //thread_unlock(0);
  thread_create((thread_startfunc_t)goodbye, (void*)0);
  //thread_yield();
  thread_wait(0, 1);
  //thread_unlock(0);
  thread_create((thread_startfunc_t)goodbye2, (void*)0);
  //thread_create((thread_startfunc_t)goodbye, (void*)0);
  thread_yield();

  //cout << "HELLO" << endl;
  thread_unlock(0);

  return 0;
}

int goodbye(){
  thread_lock(1);
  thread_wait(0, 1);
}

int main(int argc, char *argv[]) {

  thread_libinit((thread_startfunc_t)hello, (void*)0);

}
