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

//Basic unlock test. Just makes sure that it literally unlocks the held lock
// DOES NOT interact with the lockQueue or map or naything.

int counter = 0;
int looper = 5;

int goodbye();
//Testing thread_libinit

int goodbye2(){

  if(thread_lock(1)){
    cout << "Failed Lock" << endl;
   }
  //thread_unlock(0);

  //cout << "goodbye2" << endl;
  if(counter != looper){
    cout << counter << endl;
    cout << looper << endl;
    cout << "there was an error" << endl;
  }

}

int hello(){
	//thread_unlock(0);
  if(thread_lock(0)){
    cout << "Failed Lock" << endl;
  }
  // thread_unlock(0);
  // thread_lock(0);
  for(int i  = 0; i < looper; i++){
    thread_create((thread_startfunc_t)goodbye, (void*)0);
  }
  //thread_yield();
  //thread_unlock(0);
  thread_create((thread_startfunc_t)goodbye2, (void*)0);
  //thread_create((thread_startfunc_t)goodbye, (void*)0);

//  cout << "hello" << endl;

  thread_unlock(0);

  return 0;
}

int goodbye(){
//  cout << "Trying to acquire lock in goodbye1" << endl;
  if(thread_lock(0)){
    cout << "Failed Lock" << endl;
  }
//  cout << "goodbye" << endl;
  counter++;
  if(thread_unlock(0)){
    cout << "Failed unlock" << endl;

  }
}

int main(int argc, char *argv[]) {
//  cout << "started running" << endl;

  thread_libinit((thread_startfunc_t)hello, (void*)0);

}
