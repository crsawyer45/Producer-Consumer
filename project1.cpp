// Colin Sawyer
// CPSC 3220 Operating Systems Section 2
// Program 1
// Demonstrating fork() and pipe() system calls, simulating a producer/consumer problem
 
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <cstdlib>
#include <ctime>
#define INTSIZE 4

using namespace std;

int main(){
    //seed RNG off time to get new answers on each run
    srand(time(NULL));
    long int random[1], received[1], reading; //need long to meet 1000000 math
    int num[2]; //declare fd
    pid_t person; // whether process is producer or consumer

    //create pipe and split process into parent and child processes
    pipe(num);
    person = fork();

    if(person == -1) {cout << "WHOOPS, THERE'S AN ERROR" << endl; exit(0);}

    //Child process creates the random numbers and sends them through pipe
    if(person == 0) {

      close(num[0]);
      //variables to store info for calculating average of numbers sent
      int sentCount = 0;
      long int sentSum = 0;

      // run for 5000 iterations
      for(int i = 0; i < 5000; i++){
        int howMany = rand() % 4;

        //create 1-4 random integers at each iteration to send through the pipe
        for(int j = 0; j <= howMany; j++){

          random[0] = rand() % 1000000;
          //increment random so values are 1-1000000 instead of 0-999999
          random[0]++;
          write(num[1], random, INTSIZE);
          sentCount++;
          sentSum = sentSum + random[0];
        }
      }

      cout << "Producer sent " << sentCount <<
        " values with an average of " << int(sentSum/sentCount) << endl;
      close(num[1]);
      exit(0);
    }

    //Parent Process takes in the numbers from the pipe and calculates averages
    else {

      close(num[1]);
      //variables to store info for calculating averages of numbers received
      int tempCount = 0;
      int gotCount = 0;
      long int tempSum = 0;
      long int gotSum = 0;

      while((reading = read(num[0], received, INTSIZE)) > 0){

        gotCount++;
        tempCount++;
        gotSum = gotSum + received[0];
        tempSum = tempSum + received[0];

        //Every fifty numbers reveived, report the partial average
        if (tempCount == 50){

          cout << "Consumer received 50 values with an average of " <<
          int(tempSum/50) << endl;
          tempSum = 0; tempCount = 0;
        }
      }
      cout << "Pipe was closed" << endl;
      //report final partial average even if there are less than fifty left
      if (tempCount != 0){
        cout << "Consumer received " << tempCount << " values with an average of " <<
          int(tempSum/tempCount) << endl;
      }
      //report total numbers received from pipe and the average of all of them
      cout << "Consumer received " << gotCount <<
        " values and had an average of " << int(gotSum/gotCount) << endl;
    }

    return 0;
}
