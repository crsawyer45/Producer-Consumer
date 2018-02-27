#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <cstdlib>

int main(){

    int num[2], received;
    pid_t person;

    pipe(num);
    person = fork();

    if(person == -1) {cout << "ERROR" << endl;}

    if(person == 0) {
      //Child process creates the random numbers
      close(num[0]);
      int sentCount = 0;
      int sentSum = 0;

      for(int i = 0; i < 5000; i++){
        int howMany = rand() % 4;

        for(int j = 0; j <= howMany; j++){

          unsigned int random = rand() % 1000000;
          random++;
          write(num[1], random, 2);
          sentCount++;
          sentSum = sentSum + random;
        }
      }

      cout << "Producer sent " << sentCount << "values ";
      cout << "with and average of " << sentSum/sentCount << endl;
      exit(0);
    }

    else {
      //Parent Process takes in the numbers
      close(num[1]);
      int tempCount = 0;
      int gotCount = 0;
      int tempSum = 0;
      int gotSum = 0;

      while((reading = read(num[1], received, 2)) > 0){

        gotCount++;
        tempCount++;
        gotSum = gotSum + received;
        tempSum = tempSum + received;

        if (tempCount == 50){

          cout << "Consumer received 50 values with an average";
          cout << " of " << tempSum/50 << endl;
          tempSum = 0; tempCount = 0;
        }
      }
      cout << "Consumer receieved " << tempCount << "values with an average of ";
      cout << tempSum/tempCount;
      cout << "Consumer received " << gotCount << "values ";
      cout << "and had an average of " << gotSum/gotCount << endl;
    }

    return 0;
}
