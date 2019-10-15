//condMap[make_pair(lock, cond)].push(current_thread);

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

//Testing Signal!

int counter = 1;
int postCounter = 0;

int goodbye();

int postGoodbye();

int goodbye2(){
  printf("g2 turn :)\n" );
  thread_lock(0);
  //cout << "GOODBYE2" << endl;
  if(thread_signal(0, 1)){
    cout << "SIGNAL FAILED" << endl;
  }
  if(thread_signal(1, 1)){
    cout << "SIGNAL FAILED" << endl;
  }
  thread_yield();

  thread_unlock(0); //Actually should NOT work (deadlock) because there is no unlock

}

int hello(){
  //thread_unlock(0);
  thread_lock(0);
  thread_create((thread_startfunc_t)goodbye, (void*)0);
  thread_create((thread_startfunc_t)goodbye2, (void*)0);
  if(thread_wait(1,5) == -1){
    cout << "ERROR: Trying to wait on a lock it does not hold" << endl;
  }
  thread_wait(0, 1);
  cout << "its hello here" << endl;
  thread_yield();
  cout << "HELLO" << endl;
  thread_unlock(0);

  return 0;
}

int goodbye(){
  cout << "Trying to acquire lock in goodbye1" << endl;
  thread_lock(1);
  thread_wait(1, 1);
  //counter++;

  cout << "GOODBYE" << endl;
  thread_unlock(1);
}


int main(int argc, char *argv[]) {
  cout << "started running" << endl;

  thread_libinit((thread_startfunc_t)hello, (void*)0);

}
