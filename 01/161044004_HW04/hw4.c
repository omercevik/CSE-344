/*
	OMER CEVIK
	161044004
	CSE 344 System Programming
	Homework 4
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
#define FIFO "161044004sizes"

int postOrderApply (char *path, int pathfun (char *path1));
int sizepathfun (char *path);

// ZeroFlag is for to realize options of buNeDu.
int ZeroFlag = 0;

int main(int argc, char const *argv[])
{
	if (mkfifo(FIFO, 0666) == -1)
	{
		printf("Can not make fifo!\n");
		exit(1);
	}
	if (argc == 2)
	{
		FILE* filePointer = fopen(FIFO,"r+");
		if(filePointer)
		{
			//	If option is only writing directory name we use ZeroFlag = 0 for not adding subdirectories size.
			ZeroFlag = 0;
			printf("PID\tSIZE\t\tPATH\n");
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
				fclose(filePointer);
				filePointer = fopen(FIFO,"r");
				if (!filePointer)
				{
					printf("File can not open!\n");
					exit(1);
				}
				size_t size = 5000;
				char *InputFile = (char*)malloc(sizeof(char)*size);
				int lineSize = 0;
				int pidArray[size];

				while(getline(&InputFile,&size,filePointer) >= 0)
				{
					printf("%s",InputFile);
					sscanf(InputFile,"%d",&pidArray[lineSize]);
					++lineSize;
				}				
				int ProcessCounter = 0;
				for (int i = 0; i < lineSize; ++i) 
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
				printf("%d child processes created. Main process is %d.\n",ProcessCounter, getpid() );
				free(InputFile);
			}
			fclose(filePointer);
		}
	}
	else if (argc == 3 && strcmp(argv[1],"-z") == 0)
	{
		FILE* filePointer = fopen(FIFO,"r+");
		if(filePointer)
		{
			//	If option is writing "-z" and directory name we use ZeroFlag = 1 for adding subdirectories size.
			ZeroFlag = 1;
			printf("PID\tSIZE\t\tPATH\n");
			fflush(filePointer);
			pid_t newChildPID = fork();
			if (newChildPID == 0)
			{
				char* directory = (char*)malloc(sizeof(char)*(strlen(argv[2])+1));
				strcpy(directory,argv[2]);
				int total = postOrderApply(directory, sizepathfun);
				fprintf(filePointer,"%d\t%d\t\t%s\n",getpid(),total,directory);
				free(directory);
				fclose(filePointer);
				exit(0);
			}
			else
			{
				fclose(filePointer);
				filePointer = fopen(FIFO,"r");
				if (!filePointer)
				{
					printf("File can not open!\n");
					exit(0);
				}
				size_t size = 5000;
				char *InputFile = (char*)malloc(sizeof(char)*size);
				int lineSize = 0;
				int pidArray[size];

				while(getline(&InputFile,&size,filePointer) >= 0)
				{
					printf("%s",InputFile);
					sscanf(InputFile,"%d",&pidArray[lineSize]);
					++lineSize;
				}				
				int ProcessCounter = 0;
				for (int i = 0; i < lineSize; ++i) 
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
				printf("%d child processes created. Main process is %d.\n",ProcessCounter, getpid() );
				free(InputFile);
			}
			fclose(filePointer);
		}
	}
	else
	{
		printf("Usage : ./buNeDuFPF <directory> | ./buNeDuFPF -z <directory>\n");
	}
	unlink(FIFO);
	return 0;
}

int postOrderApply (char *path, int pathfun (char *path1))
{
	int dir_size = 0;
	FILE* filePointer = fopen(FIFO,"r+");

	if(filePointer)
	{
		DIR *d = opendir(path);
		pid_t pid;
		if (d == 0){ return 0; }
		int CountOfPipes = 0;
		int MaxPipeSize = 5000;
		struct dirent *dir;
		int FileDescriptorOfPipe[MaxPipeSize][2]; 
	
		while ((dir = readdir(d)) != NULL)
		{
			if ((strcmp(dir->d_name, ".") == 0) || (strcmp(dir->d_name, "..") == 0))
			{
				continue;
			}
			char ConcatPath[MaxPipeSize];
			sprintf(ConcatPath, "%s/%s", path, dir->d_name);

			if (dir->d_type == DT_DIR)
			{ 
				if (pipe(FileDescriptorOfPipe[CountOfPipes++]) == -1)
				{
					printf("Pipe couldn't open.\n");
					exit(1);
				}

				pid = fork();

				if (pid >= 0)
				{
					if (pid == 0)
					{
						sprintf(ConcatPath,"%s/%s", path, dir->d_name);
						closedir(d);
						int SubDirectorySizes = postOrderApply(ConcatPath, pathfun);
						if (ZeroFlag)
						{  
							close(FileDescriptorOfPipe[CountOfPipes-1][0]);
							write(FileDescriptorOfPipe[CountOfPipes-1][1],&SubDirectorySizes,sizeof(int));
							close(FileDescriptorOfPipe[CountOfPipes-1][1]);
							fprintf(filePointer,"%d\t%d\t\t%s\n",getpid(),SubDirectorySizes,ConcatPath );
						}
						else
						{
							fprintf(filePointer,"%d\t%d\t\t%s\n",getpid(),SubDirectorySizes,ConcatPath );
						}

						fclose(filePointer);
						exit(0);
					}
				}
			}
			else
			{
				dir_size += pathfun(ConcatPath);
				if(dir->d_type == DT_FIFO || dir->d_type == DT_LNK)
				{
					fprintf(filePointer,"%d\t-1\t\tSpecial_file_%s\n",getpid(),dir->d_name);
					fflush(filePointer);
				}
			}
		}
		int child = 0;
		while(wait(&child) > 0);
		if (ZeroFlag)
		{
			int OptionZPipeSize = 0;
			close(FileDescriptorOfPipe[0][1]);
			for(int i = 0; i < CountOfPipes;)
			{
				read(FileDescriptorOfPipe[i][0], &OptionZPipeSize, sizeof(int));
				close(FileDescriptorOfPipe[i++][0]);
				close(FileDescriptorOfPipe[i][1]);
				dir_size += OptionZPipeSize;  
			}
		}
		fclose(filePointer);
		closedir(d);
	}
	return dir_size;
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