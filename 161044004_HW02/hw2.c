/*
	OMER CEVIK
	161044004
	CSE 344 System Programming
	Homework 2
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <sys/wait.h> 
#include <unistd.h>

int postOrderApply (char *path, int pathfun (char *path1));
int sizepathfun (char *path);
int OptionZCalculator(char PathConcat[], FILE* filePointer);

// ZeroFlag is for to realize options of buNeDu.
int ZeroFlag = 0;

int main(int argc, char const *argv[])
{
	if (argc == 2)
	{
		//	If option is only writing directory name we use ZeroFlag = 0 for not adding subdirectories size.
		ZeroFlag = 0;
		remove("161044004sizes.txt");
		FILE* filePointer = fopen("161044004sizes.txt","a+");
		if(filePointer)
		{
			int lengthOfTitle = strlen("PID\tSIZE\t\tPATH\n");
			printf("PID\tSIZE\t\tPATH\n");
			fprintf(filePointer,"PID\tSIZE\t\tPATH\n");
			fflush(filePointer);
			pid_t newChildPID = fork();
			if (newChildPID == 0)
			{
				char* directory = (char*)malloc(sizeof(char)*(strlen(argv[1])+1));
				strcpy(directory,argv[1]);
				fprintf(filePointer,"%d\t%d\t\t%s\n",getpid(),postOrderApply(directory, sizepathfun),directory );
				free(directory);
				fclose(filePointer);
				exit(0);
			}
			else
			{
				wait(NULL);
				fseek(filePointer,lengthOfTitle,SEEK_SET);
				size_t size = 5000;
				char *InputFile = (char*)malloc(sizeof(char)*size);
				int lineSize = 0;
				while(getline(&InputFile,&size,filePointer) >= 0)
				{
					printf("%s",InputFile );
					++lineSize;
				}
				int pidArray[lineSize];
				fseek(filePointer,lengthOfTitle,SEEK_SET);
				int i = 0;
				while(getline(&InputFile,&size,filePointer) >= 0)
				{
					sscanf(InputFile,"%d",&pidArray[i]);
					++i;
				}
				int ProcessCounter = 0;
				for (i = 0; i < lineSize; ++i) 
				{ 
					int j = 0; 
					for (j = 0; j < i; ++j)
					{
						if (pidArray[i] == pidArray[j])
						{
							break; 
						}
					}
					if (i == j)
					{
						++ProcessCounter;
					}
				}
				fprintf(filePointer,"%d child processes created. Main process is %d.",ProcessCounter, getpid() );
				printf("%d child processes created. Main process is %d.\n",ProcessCounter, getpid() );
				free(InputFile);
			}
			fclose(filePointer);
		}
	}
	else if (argc == 3 && strcmp(argv[1],"-z") == 0)
	{
		//	If option is writing "-z" and directory name we use ZeroFlag = 1 for adding subdirectories size.
		ZeroFlag = 1;
		remove("161044004sizes.txt");
		FILE* filePointer = fopen("161044004sizes.txt","a+");
		if(filePointer)
		{
			fprintf(filePointer,"PID\tSIZE\t\tPATH\n");
			printf("PID\tSIZE\t\tPATH\n");
			fflush(filePointer);
			pid_t newChildPID = fork();
			if (newChildPID == 0)
			{
				char* directory = (char*)malloc(sizeof(char)*(strlen(argv[2])+1));
				strcpy(directory,argv[2]);
				int total = postOrderApply(directory, sizepathfun);
				fprintf(filePointer,"%d\t%d\t\t%s\n",getpid(),total+OptionZCalculator(directory,filePointer),directory );
				free(directory);
				fclose(filePointer);
				exit(0);
			}
			else
			{
				wait(NULL);
				int lengthOfTitle = strlen("PID\tSIZE\t\tPATH\n");
				fseek(filePointer,lengthOfTitle,SEEK_SET);
				size_t size = 5000;
				char *InputFile = (char*)malloc(sizeof(char)*size);
				int lineSize = 0;

				while(getline(&InputFile,&size,filePointer) >= 0)
				{
					printf("%s",InputFile );
					++lineSize;
				}

				int pidArray[lineSize];
				fseek(filePointer,lengthOfTitle,SEEK_SET);
				int i = 0;
				while(getline(&InputFile,&size,filePointer) >= 0)
				{
					sscanf(InputFile,"%d",&pidArray[i]);
					++i;
				}
				int ProcessCounter = 0;
				for (i = 0; i < lineSize; ++i) 
				{ 
					int j = 0; 
					for (j = 0; j < i; ++j)
					{
						if (pidArray[i] == pidArray[j])
						{
							break; 
						}
					}
					if (i == j)
					{
						++ProcessCounter;
					}
				}
				fprintf(filePointer,"%d child processes created. Main process is %d.",ProcessCounter, getpid() );
				printf("%d child processes created. Main process is %d.\n",ProcessCounter, getpid() );
				free(InputFile);
			}
			fclose(filePointer);
		}
	}
	else
	{
		printf("Usage : ./buNeDuFork <directory> | ./buNeDuFork -z <directory>\n");
	}

	return 0;
}

int postOrderApply (char *path, int pathfun (char *path1))
{
	//	Opening path file.
	DIR *d = opendir(path);
	FILE* filePointer = fopen("161044004sizes.txt","a+");
	int filelock = flock(fileno(filePointer),LOCK_SH);
	int total_size = 0;

	if(filePointer)
	{
		fseek(filePointer,0,SEEK_END);
		pid_t childPID;

		if (d == 0){ return 0; }

		struct dirent *dir;
		int dir_size = 0;
		char PathConcat[5000];

		// Creating loop to read directories.
		while ((dir = readdir(d)) != NULL)
		{
			// Ignoring current directory ( "." ) and previous directory ( ".." ) then continues.
			if ((strcmp(dir->d_name, ".") == 0) || (strcmp(dir->d_name, "..") == 0))
				continue;

			sprintf(PathConcat, "%s/%s", path, dir->d_name);

			// Checking directory type if it is another directory.
			if (dir->d_type == DT_DIR)
			{
				childPID = fork();
				if (childPID >= 0)
				{
					if (childPID == 0)
					{
						sprintf(PathConcat,"%s/%s", path, dir->d_name);
						closedir(d);
						dir_size = postOrderApply(PathConcat, pathfun);
						
						if(filelock == 0)
						{	
							filelock = flock(fileno(filePointer),LOCK_UN);
							if (ZeroFlag)
							{
								dir_size += OptionZCalculator(PathConcat,filePointer);
								fseek(filePointer,0,SEEK_END);
							}
							fprintf(filePointer,"%d\t%d\t\t%s\n",getpid(),dir_size,PathConcat );
							fflush(filePointer);
							filelock = flock(fileno(filePointer),LOCK_SH);
						}
						fclose(filePointer);
						exit(0);
					}
				}
				else
				{
					wait(NULL); /* parent process child process leri bekliyor */
				}				
			}
			else
			{
				total_size += pathfun(PathConcat);
				// Checking if we have pip or symbolink link as a file and printing them.

				if(dir->d_type == DT_FIFO || dir->d_type == DT_LNK)
				{
					if(filelock == 0)
					{	
						filelock = flock(fileno(filePointer),LOCK_UN);
						fprintf(filePointer,"%d\t\t\tSpecial file %s\n",getpid(),dir->d_name);
						fflush(filePointer);
						filelock = flock(fileno(filePointer),LOCK_SH);
					}
				}
			}
		}
		// Closing opened directory.
		int child = 0;
		while(wait(&child) > 0);

		fclose(filePointer);
		closedir(d);
	}
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

int OptionZCalculator(char PathConcat[], FILE* filePointer)
{
	int lengthOfTitle = strlen("PID\tSIZE\t\tPATH\n");
	fseek(filePointer,lengthOfTitle,SEEK_SET);
	size_t size = 5000;
	char *InputFile = (char*)malloc(sizeof(char)*size);

	char OutputColumn1[size], OutputColumn2[size], OutputColumn3[size];
	
	int donek = 0 ; 
	
	while(getline(&InputFile,&size,filePointer) >= 0)
	{	
		int FlagOfEquality = 1; 
		sscanf(InputFile,"%s %s %s",OutputColumn1,OutputColumn2,OutputColumn3);
		if (!(strcmp(OutputColumn2,"Special") == 0 && 
			strcmp(OutputColumn3,"file") == 0) )
		{
			int i = 0;
			while(i < strlen(PathConcat))
			{
				if (!(PathConcat[i] == OutputColumn3[i]))
				{
					FlagOfEquality = 0;
				}
				++i;
			}

			int Saban = 1;
			for (int j = i; j < strlen(OutputColumn3); ++j)
			{
				if (OutputColumn3[j] == '/')
				{
					++Saban;
				}
			}

			if (FlagOfEquality && Saban == 2)
			{
				int SabanOgluSaban = 0;
				sscanf(OutputColumn2,"%d",&SabanOgluSaban);
				donek += SabanOgluSaban;
			}
		}
	} 
	free(InputFile);
	return donek;
}