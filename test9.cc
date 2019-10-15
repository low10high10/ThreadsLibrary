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

//Tests if library detects unlocking lock that thread doesnt hold onto.

int hello(){

  if(thread_unlock(1)!=-1){
    cout << "Error: Thread Library does not detect that user unlocked from lock it does not hold onto." << endl;
    exit(1);
  }

  //thread_unlock(0);

  return 0;
}



int main(int argc, char *argv[]) {
  //cout << "started running" << endl;

  thread_libinit((thread_startfunc_t)hello, (void*)0);

}
