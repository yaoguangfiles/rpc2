/*
 * fork.cxx
 * 
 * 	Example Program for CSCI 330
 *  shows fork system call
 * 
 */
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
using namespace std;

int main()
{
	pid_t pid;

	// fork will make 2 processes
	pid = fork();

	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (pid == 0)
	{
		// Child process: fork() returned 0
		int j;
		for (j = 0; j < 10; j++)
		{
			cout << "child: " << j << endl;
			sleep(1);
		}
	}
	else
	{
		// Parent pocess: fork() returned a positive number
		int i;
		for (i = 0; i < 10; i++)
		{
			cout << "parent: " << i << endl;
			sleep(1);
		}
	}

	return 0;
}
