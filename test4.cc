#include <stdlib.h>
#include <iostream>
#include "thread.h"
#include <assert.h>

using namespace std;

// This tests if the library can correctly detect user error when they lock same lock twice within in the same thread

int goodbye();
bool correctDetect = false;
// Have to fix this **
int hello(){
  thread_lock(0);
  thread_create((thread_startfunc_t)goodbye, (void*)0);
  if(thread_lock(0)==-1){
    correctDetect = true;
    //cout << "Library is correctly detecting that user tries to lock same lock twice on same thread! " << endl;
  }




  //thread_unlock(0);

  return 0;
}

int goodbye(){
  //cout << "GOODBYE" << endl;
  if(correctDetect == false){
    cout << "ERROR: Library is not detecting lock is trying to be held twice! " << endl;
  }
}

int main(int argc, char *argv[]) {

  thread_libinit((thread_startfunc_t)hello, (void*)0);

}
