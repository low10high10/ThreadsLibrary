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

// This tests to see if library blocks thread if lock is held


int goodbye2(){

  if(thread_lock(0)){
    cout << "Lock is broken" << endl;

  }
  //thread_unlock(0);
  cout << "This should not print! Error" << endl;
  exit(1);


}

int hello(){
  if(thread_lock(0)){
    cout << "Lock is broken" << endl;
  }
  thread_create((thread_startfunc_t)goodbye2, (void*)0);
  thread_yield();


  return 0;
}


int main(int argc, char *argv[]) {
  cout << "started running" << endl;

  thread_libinit((thread_startfunc_t)hello, (void*)0);

}
