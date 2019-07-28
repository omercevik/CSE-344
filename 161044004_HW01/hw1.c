/*
	OMER CEVIK
	161044004
	CSE 344 System Programming
	Homework 1
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h> 
#include <sys/types.h>
#include <sys/stat.h> 
#include <fcntl.h>

int postOrderApply (char *path, int pathfun (char *path1));
int sizepathfun (char *path);

// ZeroFlag is for to realize options of buNeDu.
int ZeroFlag = 0;

int main(int argc, char const *argv[])
{
	if (argc == 2 )
	{
		//	If option is only writing directory name we use ZeroFlag = 0 for not adding subdirectories size.
		ZeroFlag = 0;
		char* directory = (char*)malloc(sizeof(char)*(strlen(argv[1])+1));
		strcpy(directory,argv[1]);
		printf("%d\t\t%s\n",postOrderApply(directory, sizepathfun),directory );
		free(directory);
	}
	else if (argc == 3 && strcmp(argv[1],"-z") == 0)
	{
		//	If option is writing "-z" and directory name we use ZeroFlag = 1 for adding subdirectories size.
		ZeroFlag = 1;
		char* directory = (char*)malloc(sizeof(char)*(strlen(argv[2])+1));
		strcpy(directory,argv[2]);
		printf("%d\t\t%s\n",postOrderApply(directory, sizepathfun),directory );
		free(directory);
	}
	else
	{
		printf("Wrong used type for buNeDu!\n");
	}

	return 0;
}

int postOrderApply (char *path, int pathfun (char *path1))
{
	//	Opening path file.
	DIR *d = opendir(path);
	
	if (d == 0)
	{
		return 0;
	}

	struct dirent *dir;
	int total_size = 0;
	char Concatenate[4096];

	// Creating loop to read directories.
	while ((dir = readdir(d)) != NULL)
	{
		// Ignoring current directory ( "." ) and previous directory ( ".." ) then continues.
		if ((strcmp(dir->d_name, ".") == 0) || (strcmp(dir->d_name, "..") == 0))
		{
			continue;
		}

		// Checking the path if it is current directory and ignoring it to print.

		sprintf(Concatenate, "%s/%s", path, dir->d_name);

		// Checking directory type if it is another directory.
		if (dir->d_type == DT_DIR)
		{
			// If it is a directory then adding path size and calling recursive function.
			int dir_size = postOrderApply(Concatenate, pathfun) + pathfun(path) - 4;
			printf("%d\t\t%s\n", dir_size, Concatenate);

			// Checking the option if called "-z" or not.
			if (!ZeroFlag)
			{
				total_size += dir_size;
			}
        }
        else
        {
        	// If not a directory we sum other files size.
			total_size += pathfun(Concatenate);

			// Checking if we have pip or symbolink link as a file and printing them.
			if(dir->d_type == DT_FIFO || dir->d_type == DT_LNK)
			{
				printf("Special file\t%s\n", dir->d_name);
			}
		}
	}

	// Closing opened directory.
	closedir(d);
	return total_size;
}

int sizepathfun (char *path)
{
	// Creating stat structure to calculate path size and return it.
	struct stat st;
	if (lstat(path, &st) != 0)
	{
		return -1;
	}
	int size = st.st_size/1024;
	return size;
}