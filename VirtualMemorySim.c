// Steven Xiong
// CSC 139
// Assignment#4

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

struct page
{
	int pageID;
	int pagePast;
	int pageFuture;
};

void fifo(struct page pageArray[], int pageNum, int frameNum, int arraySize, int pageCount);
void opt(struct page pageArray[], int pageNum, int frameNum, int arraySize, int pageCount);
void lru(struct page pageArray[], int pageNum, int frameNum, int arraySize, int pageCount);

int main(int argc, char *argv[])
{

	char var0[10];	// will hold the input file to read from 
	char var1[10];	// will hold the mode that the program will be running in

	int mode = 0;
	int pageMode = 0;
	
	char fifoMode[4] = "FIFO";
	char lruMode[3] = "LRU";
	char optMode[3] = "OPT";
	
	char fifoModeLow[4] = "fifo";
	char lruModeLow[3] = "lru";
	char optModeLow[3] = "opt";
	
	int x = 0;
	int i = 0;

	FILE *fp;
	char c;
	
	char inputfile;
	int matchChar = 0;

	int pageNum = 0;
	int frameNum = 0;
	int pageAcessNum = 0;
		
	char holderChar = c;
	char doubleDigit[1];
	int convertedInt = 0;
	
	int pageCount = 0;
	struct page pageInfo;
	struct page pageArray[20];
	
	int inputChar;

	char input[10] = "input1.txt";
	int avgLen = 0;

	// PUTS THE USER INPUT INTO 'var0' and 'var1'
	if(argv[1] != NULL)
	{
		sscanf(argv[1], "%s", &var0);
	}
	
	if(argv[2] != NULL)
	{
		sscanf(argv[2], "%s", &var1);
	}
	
	fp = fopen(var0, "r");


	if(fp == NULL)
	{
		printf("\nERROR! input file could not be open\n");
	}
	
	// find out which mode the program will run in either FIFO, LRU, or OPT
	if(argv[2] != NULL)
	{
		for(x = 0; x < 4; x++)
		{	// for loop to find out if program will go into FIFO mode
			if(var1[x] == fifoMode[x] || var1[x] == fifoModeLow[x])
			{
				matchChar++;
			}	
		}	
	
		if(matchChar == 4)
		{
			mode = 1;
		}
		
		if(mode == 0)
		{
			matchChar = 0;
			for(x = 0; x < 3; x++)
			{ // for loop to find out if program will go into LRU mode
				if(var1[x] == lruMode[x] || var1[x] == lruModeLow[x])
				{
					matchChar++;
				}
			}	

			if(matchChar == 3)
			{
				mode = 2;
			}
		}

		if(mode == 0)
		{
			matchChar = 0;
			
			for(x = 0; x < 3; x++)
			{ // for loop to find out if program will go into OPT mode
				if(var1[x] == optMode[x] || var1[x] == optModeLow[x])
				{
					matchChar++;
				}

				if(matchChar == 3)
				{
					mode = 3;
				}
			}
		}
		
		//printf("station#1\n");

	}
	
	if(fp != NULL)
	{
		c = getc(fp);
	}
	
	x = 0;
		
	while(c != EOF && fp != NULL)
	{
		if(c == input[i])
		{
			i++;
			avgLen++;
			if(avgLen == 10)
			{
				c = getc(fp);
				c = getc(fp);
			}
		}
			
		if(avgLen == 10)
		{
			if(c > 47 && c < 58)
			{
				holderChar = c;
				c = getc(fp);		
			}
		

			// CHECK FOR DOUBLE DIGITS
			if( c == 9 || c == 10)
			{	// next char is a newline
				doubleDigit[0] = holderChar;
				doubleDigit[1] = '\0';
				convertedInt = atoi(doubleDigit);
			}
			else
			{
				doubleDigit[0] = holderChar;
				doubleDigit[1] = c;
				convertedInt = atoi(doubleDigit);
				c = getc(fp);
			}

			if(pageMode == 0)
			{
				pageNum = convertedInt;
				pageMode = 1;
			}
			else if(pageMode == 1)
			{
				frameNum = convertedInt;
				pageMode = 2;
			}
			else if(pageMode == 2)
			{
				pageAcessNum = convertedInt;
				pageMode = 3;
			}
			else if(pageMode == 3)
			{
				pageInfo.pageID = convertedInt;
				pageInfo.pagePast = 0;
				pageInfo.pageFuture = 0;
				
				pageCount++;
				pageArray[x] = pageInfo;
				x++;
			}
		}
		
		if(c > 47 && c < 58)
		{
			if (avgLen < 10)
			{
				c = getc(fp);
			}	
		}
		else 
		{
			c = getc(fp);
		}
	}

	if(mode == 1)
	{	// if mode == 1 then program will run in FIFO MODE
		fifo(pageArray, pageNum, frameNum, pageCount, pageAcessNum);
	}
	else if(mode == 2)
	{	// if mode == 2 then program will run in LRU mode
		lru(pageArray, pageNum, frameNum, pageCount, pageAcessNum);
	}
	else if(mode == 3)
	{	// if mode == 3 then program will run in OPT mode
		opt(pageArray, pageNum, frameNum, pageCount, pageAcessNum);
	}	
	
};

void fifo(struct page pageFifoArray[], int pageCount, int frameCount, int fifoArraySize, int pageAcessCount)
{
	struct page frameArray[frameCount];
	
	int currentFrame = 0;
	int clock = 0;
	int x = 0;	
	bool pageNotInFrame = false;
	int pageExchange = 0;
	int pageFaults = 0;

	while(clock != pageAcessCount)
	{
		
		// search the whole frameArray to find existing pages in frames
		for(x = 0; x < frameCount; x++)
		{
			if(frameArray[x].pageID == pageFifoArray[clock].pageID)
			{	// if ture, page already exist in a frame
				printf("Page %d already in frame %d\n", pageFifoArray[clock].pageID, x);
				pageNotInFrame = false;
				x = frameCount;
			}
			else
			{
				pageNotInFrame = true; // page isn't currently in any frames 	
			}
		}			
		
		// page wasn't found in frame session so program will go about adding the page into the frame session	
		if(pageNotInFrame)
		{
			if(frameArray[currentFrame].pageID > 0 && frameArray[currentFrame].pageID < 10)
			{	// if there is already a page currently in session then the program will unload it and place a new one in it
				printf("Page %d unloaded from Frame %d, ", frameArray[currentFrame].pageID, currentFrame);
				pageExchange++;
			}			
				
			printf("Page %d loaded into Frame %d\n", pageFifoArray[clock].pageID, currentFrame);
		
			frameArray[currentFrame] = pageFifoArray[clock];
			frameArray[currentFrame].pagePast++;
			currentFrame++;
			
			if(currentFrame == 4)
			{
				currentFrame = 0;
			}
		}		
		clock++;
	}	
	
	pageFaults = pageExchange + frameCount;
	printf("%d page faults\n", pageFaults);
	
};

void lru(struct page pageLruArray[], int pageCount, int frameCount, int lruArraySize, int pageAcessCount)
{
	struct page frameArray[frameCount];
	int pagePast[frameCount]; // will hold the past for each page in the frames

	int currentFrame = 0;
	int clock = 0;
	int x = 0;
	int y = 0;
	bool pageNotInFrame = false;
	int pageExchange = 0;
	int pageFaults = 0;
	struct page highPastPage;
	int frameToReplace = 0;
	int allFramesFilled = 0;	

	while(clock != pageAcessCount)
	{
		// FIND OUT IF THERE IS A PAGE ALREADY IN FRAME SESSION
		for(x = 0; x < frameCount; x++)
		{
			if(frameArray[x].pageID == pageLruArray[clock].pageID)
			{
				printf("Page %d already in frame %d\n", pageLruArray[clock].pageID,  x);
				pageNotInFrame = false;
				x = frameCount;
			}
			else
			{
				pageNotInFrame = true;
			}
		}

		
		if(pageNotInFrame)
		{
			if(allFramesFilled == frameCount)
			{
				//FIND PAST VALUE FOR EACH FRAME
				for(y = 0; y < frameCount; y++)
				{
					for(x = clock; x >= 0; x--)
					{
						if(frameArray[y].pageID == pageLruArray[x].pageID)
						{
							frameArray[y].pagePast = clock - x;
							x = -1;
						}
						else
						{
							frameArray[y].pagePast = 0;	
						}
					}	
				}	

				// FINDS THE FRAME WITH THE HIGHIEST PAST VALUE TO BE REPLACED	
				highPastPage = frameArray[0];
				
				for(x = 0; x < frameCount; x++)
				{
					if(frameArray[x].pagePast == 0)
					{
						frameToReplace = x;
						x = frameCount;
					}
					else if(highPastPage.pagePast < frameArray[x].pagePast)
					{
						frameToReplace = x;
						highPastPage = frameArray[x];
					}
					else
					{
						// do nothing
					}
				}

				// REPLACE FRAME IN SEASSION THAT HASN'T BEEN USED FOR AWHILE WITH A NEW PAGE
				pageExchange++;
				printf("page %d unloaded from Frame %d, Page %d loaded into Frame %d\n",frameArray[frameToReplace].pageID, frameToReplace, pageLruArray[clock].pageID, frameToReplace);
				frameArray[frameToReplace] = pageLruArray[clock];

		
			}
			else if(allFramesFilled < frameCount)
			{	// FILLS THE FRAME WITH NEW PAGES
				frameArray[currentFrame] = pageLruArray[clock];
				printf("Page %d loaded into Frame %d\n", pageLruArray[clock].pageID, currentFrame);

				currentFrame++;
				allFramesFilled++;
			}
		}
		clock++;
	}	
	
	pageFaults = pageExchange + frameCount;
	printf("%d page faults\n", pageFaults);

};

void opt(struct page pageOptArray[], int pageCount, int frameCount, int optArraySize, int pageAcessCount)
{

	struct page frameArray[frameCount];

	int currentFrame = 0;
	int clock = 0;
	int x = 0;
	int y = 0;
	bool pageNotInFrame = false;
	int pageExchange = 0;
	int pageFaults = 0;
	struct page highFuturePage;
	int frameToReplace = 0;	
	int allFramesFilled = 0;

	while(clock != pageAcessCount)
	{	
		// SEARCH THE WHOLE FRAMEARRAY TO FIND EXISTING PAGES IN FRAME
		for(x = 0; x < frameCount; x++)
		{
			if(frameArray[x].pageID == pageOptArray[clock].pageID)
			{	// if ture, page already exist in a frame
				printf("Page %d already in frame %d\n", pageOptArray[clock].pageID, x);
				pageNotInFrame = false;
				x = frameCount;
			}
			else
			{	// page isn't curently in any frames
				pageNotInFrame = true;
			}
		}

		
		if(pageNotInFrame)
		{
			// Need to figure out the page with the highiest future or no future at all(future == 0
			if(allFramesFilled == frameCount)
			{
				//FIND FUTURE VALUE FOR EACH FRAME
				for(y = 0; y < frameCount; y++)
				{
					for(x = clock; x < pageAcessCount; x++)
					{
						if(frameArray[y].pageID == pageOptArray[x].pageID)
						{
							frameArray[y].pageFuture = x - clock;
							x = pageAcessCount;
						}
						else
						{
							frameArray[y].pageFuture = 0;
						}
					}
				}
				
				// FINDS THE FRAME WITH THE HIGHIEST FUTURE VALUE TO BE REPLACED	
				
				highFuturePage = frameArray[0];

				for(x = 0; x < frameCount; x++)
				{
					
					if(frameArray[x].pageFuture == 0)
					{
						frameToReplace = x; // page doesn't have a future and should be repalced
						x = frameCount; // exits the for loop since good canditate to replace has already been found
					}
					else if(highFuturePage.pageFuture < frameArray[x].pageFuture)
					{
						frameToReplace = x;
						highFuturePage = frameArray[x];
					}
					else
					{
						// do nothing
						// most likely highFuturePage.pageFuture == frameArray[x].pageFuture
						// OR highFuturePage.pageFuture > frameArray[x].pageFuture	
					}
				}
				pageExchange++;
				printf("Page %d unloaded from Frame %d, Page %d loaded into2 Frame %d\n", frameArray[frameToReplace].pageID, frameToReplace, pageOptArray[clock].pageID, frameToReplace);
				frameArray[frameToReplace] = pageOptArray[clock];
				
			}
			else if(allFramesFilled < frameCount)
			{
				frameArray[currentFrame] = pageOptArray[clock];
				printf("Page %d loaded into Frame %d\n", pageOptArray[clock].pageID, currentFrame );
				currentFrame++;
				allFramesFilled++;
			}					
		}
				
		clock++;
	}
		pageFaults = pageExchange + frameCount;
		printf("%d page faults\n", pageFaults);
};
