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



int hello(){
	//thread_unlock(0);


  //if(thread_libinit((thread_startfunc_t)hello, (void*)0)!=-1){
  //  cout << "Error: Library does not detect user has ran lib_init twice" << endl;

  //}

  return 0;
}



int main(int argc, char *argv[]) {

  if(thread_create((thread_startfunc_t)hello, (void*)0)!=-1){
    cout << "Error: Library does not detect user has failed to run lib_init" << endl;
  }
  if(thread_lock(0)!=-1){
    cout << "Error: Library does not detect user has failed to run lib_init" << endl;
  }
  if(thread_unlock(0)!=-1){
    cout << "Error: Library does not detect user has failed to run lib_init" << endl;
  }
  if(thread_yield()!=-1){
    cout << "Error: Library does not detect user has failed to run lib_init" << endl;
  }
  if(thread_wait(0,1)!=-1){
    cout << "Error: Library does not detect user has failed to run lib_init" << endl;
  }
  if(thread_signal(0,1) !=-1){
    cout << "Error: Library does not detect user has failed to run lib_init" << endl;
  }
  if(thread_broadcast(0,1) !=-1){
    cout << "Error: Library does not detect user has failed to run lib_init" << endl;
  }
  

//  thread_libinit((thread_startfunc_t)hello, (void*)0);



}
