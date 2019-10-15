#include "thread.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <queue>
using namespace std;

int postedItems;
int sandLock;
//unsigned int hasPosted;
int boardSize;
int numberCashiers;
int MaxNumberCashiers;
int cashNum = 0;
unsigned int isFull = 10;
unsigned int notFull = 15;
int chefSand = -1;
vector<char*> inFiles;
vector<bool> isAlive;
vector<bool> hasPosted;
vector< queue<int> >  hardCode;
map<int, int> corkboard;
queue<int> readyQueue;
int max_size;



// void showq(queue <int> gq)
// {
//     queue <int> g = gq;
//     while (!g.empty())
//     {
//         cout << '\t' << g.front();
//         g.pop();
//     }
//     cout << '\n';

// }


int* holder[5];
// Enumerate function signatures
int main();
void start(void *args);

int main() {

    char *argv[100];
    // int swin0[2] = {53, 785};
    // int swin1[2] = {914, 350};
    // int swin2[2] = {827, 567};
    // int swin3[2] = {302, 230};
    // int swin4[2] = {631, 11};

    queue<int> swin0;
        swin0.push(53);
        swin0.push(785);

    queue<int> swin1;
        swin0.push(914);
        swin0.push(350);

    queue<int> swin2;
        swin0.push(827);
        swin0.push(567);

    queue<int> swin3;
        swin0.push(302);
        swin0.push(230);

    queue<int> swin4;
        swin0.push(631);
        swin0.push(11); 

    // holder[0] = swin0;
    // holder[1] = swin1;
    // holder[2] = swin2;
    // holder[3] = swin3;
    // holder[4] = swin4;

    hardCode.push_back(swin0);
        hasPosted.push_back(0);
        isAlive.push_back(1);
    hardCode.push_back(swin1);
        hasPosted.push_back(0);
        isAlive.push_back(1);
    hardCode.push_back(swin2);
        hasPosted.push_back(0);
        isAlive.push_back(1);
    hardCode.push_back(swin3);
        hasPosted.push_back(0);
        isAlive.push_back(1);
    hardCode.push_back(swin4);
        hasPosted.push_back(0);
        isAlive.push_back(1);

    // cout << hardCode[0].front() << endl;
    // hardCode[0].pop();
    // cout << hardCode[0].front() << endl;
    //     hardCode[0].pop();
    // cout << hardCode[0].front() << endl;


    //holder[5] ={swin0, swin1, swin2, swin3, swin4};

    //cout << *holder[1] << endl;

    // if (argc < 2) {
    //     cout << "Not enough inputs" << endl;
    //     return (0);
    // }

    // //vector<char*> inFiles;
    // for (int i = 2; i < argc; i++){
    //     inFiles.push_back(argv[i]);
    //     isAlive.push_back(1);
    //     hasPosted.push_back(0);
    // }



    //mkay, so inFiles contains the file names of all the cashier's stuff
    //printf("THIS IS IT>>>>> %s\n",inFiles[1]);
    // for (auto i = g1.begin(); i != g1.end(); ++i){
    //     cout << *i << " ";
    // }

    boardSize = 3;
    max_size = boardSize;
    numberCashiers = 5;
    MaxNumberCashiers = numberCashiers;

    if(boardSize > numberCashiers){
        boardSize = numberCashiers; //Edge case when board starts off much larger than num cashiers
        max_size = numberCashiers;
    }
    //printf("%d\n", boardSize );



    if(thread_libinit(start, argv)){
      cout << "FAILURE" << endl;
    }
}


// void start_thread(void *args) {
//     // Code goes here
//     return;
// }


void cashierPost(void *args) {
    //printf("I'm in cashierPost \n"); // IN thread.cc some issue with unlocking
    if(thread_lock(sandLock)){
      cout << "FAILURE" << endl;
    }
    //printf("I'm in cashierPost with lock\n");
    int k = cashNum;
    //printf("CashNum == %d\n", cashNum);
    //cashNum++; Why cant I put cashNum here?
    //char *argv[10] = argv[10] args;
    //printf("%s\n", argv[2]);
    // Code goes here
    // IF the board is not full, AND you havent got anything on the board yet, then post your stuff
    // lock
    // wait if board is full
      // post item on board
    // add item to board
    //cout << "POSTED: cashier " << cashNum << " sandwich " << sandNum << endl;// PRINTF();
    // notify chef if board is full
    // unlock

    int orders[2];

    for(int p = 0; p < 2; p++){
        orders[p] = hardCode[0].front();
        hardCode[0].pop();
    }

    //Here's my issue, a yield here causes some issues
    //orders = holder[cashNum];
    //cout << "aj;fsldfja;sdfj;" << k << endl;

    cashNum++;
    //printf("%d\n",cashNum);
    int z = 0;

    if(thread_unlock(sandLock)){
      cout << "FAILURE" << endl;
    }
    //int hasPosted = 0; //WIll need to be removed/changed
    //printf("%d\n", (int) orders.size());
    for(int p; p < 2; p++){
        if(thread_lock(sandLock)){
          cout << "FAILURE" << endl;
        }

       // printf("my orders \n");
       // printf("%d\n", orders[z] );
        //readyQueue.push(k);             //Need to fix ready queue
        //showq(readyQueue);


    while (postedItems == max_size || hasPosted[k] == 1){ //replace hasPosted with corkboard.find(k) and if it's true, pass.
        //printf("waiting for space or has posted\n");
        //printf("cashNum %d, and order %d\n", k, orders[z]);
        if(thread_wait(sandLock, notFull)){
          cout << "FAILURE" << endl;
        }    //This is important, passes onto chef when it's full REMOVE IF I WAN TO SEE ALL WITCHES
        if(thread_lock(sandLock) != -1){
            cout << "FAILURE: Note, this shows up in cashierPost right after the wait. Tries to reaquire lock" << endl;
        };
        //thread_yield();
        //printf("A cashier thread has woken up %d\n", k);
        //thread_signal(sandLock, isFull);
    }

    // while(hasPosted == 1){ //Needs to be replaced with something like "if you're in the map"
    //     printf("waiting because cashier has already posted\n");
    //     thread_wait(sandLock, hasPosted );
    // }

    if(thread_yield()){
        cout << "FAILURE" << endl;;
    }

    int sandNum = orders[z];
    cout << "POSTED: cashier " << k << " sandwich " << sandNum << endl;

    corkboard.insert(pair<int, int>(k, sandNum));
    //printf("%d\n", (int) corkboard.size());
    //cashNum++;
    postedItems++;
    z++;
    //hasPosted++;
    hasPosted[k] = 1;

    if(postedItems == max_size){
        //printf("Signal for full, cashier is: %d\n", k);
        if(thread_signal(sandLock, isFull)){
          cout << "FAILURE" << endl;
        }

    }


    if(thread_unlock(sandLock)){
      cout << "FAILURE" << endl;
    }

                                            }
    //printf("%d\n", z );
    //printf("%d\n", k );

    //k++;
    //printf("HERE IS BOARDSIZE: %d\n", boardSize);
    //printf("is getting here\n");
    //numberCashiers--; //A cashier thread is done, and thus the number must be deincrimented
    isAlive[k] = 0;

    // for(int p; p < (int) isAlive.size(); p++){
    //     printf("%d\n", (int) isAlive[p] );
    // }
    //thread_unlock(sandLock);
    //printf("%d\n", numberCashiers);
   // printf("is cashier is completing\n");
                                                        //    } //REMOVE THE FOR LOOP TO WORK
}


// Only shared data is the corkboard, so only one lock needed.

void chef(void *args) {

    //thread_lock(sandLock);
    //printf("I'm in chef, no lock\n");
    //printf("HELLO?\n");
   // thread_lock(sandLock);
    //printf("I'm in chef w/ lock \n");
    int magDif = 1000;
    int cash = 0;
    int sandNum = -1;
    // Code goes here
    // lock
    // IF the board is full then make then item that is the smallest magnitude diff.
    // ...From your current sandwitch
    // Remove the item from your board
    // PRINTF();
    // unlock
    // Gotta throw an atoi(); in here
     // while (boardSize != 0){
     //    thread_wait (sandLock, isFull);
     // }

while(numberCashiers > 0){
    if(thread_lock(sandLock)){
      cout << "FAILURE" << endl;
    }
    while (postedItems != max_size){    //check the or part, should be for when cashier start dying
       //printf("chef is going to sleep\n");
       if(thread_wait (sandLock, isFull)){
         cout << "FAILURE" << endl;
       }
       //printf("chef is about to acquire lockS\n");
       if(thread_lock(sandLock) != -1){
         cout << "FAILURE to recognize this tries to hold lock twice" << endl;
       }
       //printf("Chef has woken up\n");

       //return;
       //My issue is somewhere here. Why does chef go to sleep and also wake itself up?
    }

    //printf("slipping through\n" );
    int LastSand = chefSand;
     map<int, int>::iterator itr;
     magDif = 1000;
     for(itr = corkboard.begin(); itr != corkboard.end(); itr++){
        //printf("Order on board: %d\n", itr->second);
        //printf("Magnitude least: %d\n", chefSand);
        //printf("OLD Sand: %d\n", LastSand);
        if (abs(LastSand - itr->second) <= magDif){
            magDif = abs(LastSand - itr->second); //LastSand used to be chefSand before
            chefSand = itr->second;
            cash = itr->first;
            sandNum = itr->second;
            //corkboard.erase(cash);
        }

     }

     //printf("Current Smallest Sand %d\n", sandNum );
     corkboard.erase(cash);

    cout << "READY: cashier " << cash << " sandwich " << sandNum << endl;
    postedItems--;
    hasPosted[cash] = 0;

    //printf("%d\n", MaxNumberCashiers);
    //printf("ak;lsdfjak;lsfjk;ladsjfkl;asdj\n");


    // for(int p; p < (int) isAlive.size(); p++){
    //     printf("%d\n", (int) isAlive[p] );
    // }
    //cout << "HEREHERHEHR" << endl;

    for(int j = 0; j < 5; j++){
        //printf("dfj;askldf;k;lajfk;ldsj\n");
        //printf("%d\n", (int)isAlive[j]);
        if(((int)isAlive[j]) == 0 && cash == j){
            numberCashiers--;
        }
    }

    if(numberCashiers < max_size){
        max_size = numberCashiers;
    }

    //printf("Number of cashiers: %d\n", numberCashiers);
    if(thread_broadcast(sandLock, notFull)){
      cout << "FAILURE" << endl;
    }

    if(thread_unlock(sandLock)){
      cout << "FAILURE" << endl;
    }
    //printf("jkhlhlkh\n");
}
    //printf("is chef completing\n");

}

void start(void *args) {

  //  start_preemptions(1, 1, 100);

    if(thread_create(chef, args)){
      cout << "FAILURE" << endl;
    }
    for(int i = 0; i < numberCashiers; i++ ){
        if(thread_create(cashierPost, args)){
          cout << "FAILURE" << endl;
        }
        //printf("%s\n", args);
        //thread_create(chef, args);
    }




        //thread_create(chef, args);


}

//NOTE: MAPS AND VECTORS WILL BE USEFULL HERE (AND QUEUES)


//MAPS: https://www.geeksforgeeks.org/map-associative-containers-the-c-standard-template-library-stl/
