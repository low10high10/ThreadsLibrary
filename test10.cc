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

//Tests if library can signal properly without lock being held by thread

int goodbye(){
  thread_signal(1,1);
  return 0;
}
int hello(){

  thread_lock(1);
  if(thread_create((thread_startfunc_t)goodbye, (void*)0)){
    cout << "Thread create failed" << endl;
  }

  if(int x =thread_wait(1,1)){
    cout << "Thread wait failed " << x << endl;
  }

  thread_unlock(1);

  cout << "Should print if library working correctly" << endl;
  //thread_unlock(0);

  return 0;
}



int main(int argc, char *argv[]) {
  //cout << "started running" << endl;

  thread_libinit((thread_startfunc_t)hello, (void*)0);

}
