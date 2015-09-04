/*Class for commands
Parent class compares and identifies command, exits and prints error message if command not valid
Then jumps to individual child class based on which command it is.
7 children:
echo

exit

cd

ls

more

etime

limits


*/

#include <iostream>
#include <cstdlib>


#ifndef _COMMAND_H
#define _COMMAND_H


class command 
{
//constructor for all commands

//identify function that then jumps to appropriate child class


};

class exit: public command
{
  // Exiting shell
exit() //constructor
{
printf("Exiting shell... \n\n");
exit(0);
}
// Exiting shell
};

class echo : public command
{
  
};

class cd : public command
{
  
};

class ls : public command
{
  
};

class more : public command
{
  
};

class etime : public command
{
  
};

class limits : public command
{
  
};




#endif
