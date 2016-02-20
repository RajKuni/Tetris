#include <stdio.h>
#include <vector>
#include <ctime>

#include <SDL.h>
#include <SDL_Image.h>

#include "defines.h"
#define null NULL
#define uint32 uint32_t
#define uint8 uint8_t


//other cpps
#include "LoadImages.cpp"
#include "Texture.cpp"
#include "Square.cpp"
#include "Block.cpp"
#include "BlockBuilder.cpp"

struct Board
{
	int numSquares;
	Square squares[ROWS*COLUMNS];
	int blocksInRow[ROWS];
	int blocksInColumn[COLUMNS];
};

//some function definitions
bool init(Screen*);
SDL_Surface* loadSurface(char*);
bool loadImages(SDL_Surface*[], SDL_Surface*&);
void updateWindow(SDL_Surface*, Screen*);
bool updateBlocks(Block*, Board*);
FilledRowRegion checkBoard(Board*);
void clearBoard(int firstRow, int lastRow, Board*);
void renderGrid(SDL_Renderer*);
void addSquaresToBoard(Block*, Board*);

//several global variables used because of laziness :)
Block* blocks[100]; //this will keep track of all old blocks that need to be cleared
int blockIndex = 0;


//keeps track of how many blocks occupy a row. Used to clear filled rows
int blocksInRow[ROWS] = {0}; 

//keeps track of all old squares. When limit reached, memory occupied by
//these defunct squares will be deleted
Square* squaresToDelete[ROWS*COLUMNS] = {0};
int squaresDeleteIndex = 0;

int main(int argc, char* args[])
{
	Screen screen = {};
	screen.width = SQUARE_DIM*SCALE*COLUMNS;
	screen.height = SQUARE_DIM*SCALE*ROWS;
	
	//init everything (window, main surface, renderer, SDL_Image)
	//NOTE: This is probably a bad way to do this since I am changing data inside function instead of returning something
	if(!init(&screen))
	{
		printf("Something could not be initialized! Error: %s\n", SDL_GetError());
	}

	//set Texture class render pointer
	Texture::setRenderer(screen.renderer);

	//set Square's main texture
	Square::setTexture("tiles.png");

	//create a blockbuilder
	BlockBuilder builder;

	//get a block pointer
	Block* block = null;

	//create an array for the Tetris board that will hold all inanimate squares
	Board board = {0};
	//int numGrid = ROWS*COLUMNS;
	//Square board_squares[ROWS*COLUMNS];
	
	//set up locations for board squares
	board.numSquares = COLUMNS*ROWS;	
	for(int i = 0; i < board.numSquares; ++i)
	{
		int x = (i%COLUMNS)*SQUARE_DIM*SCALE;
		int y = (i/COLUMNS)*SQUARE_DIM*SCALE;

		board.squares[i].setX(x);
		board.squares[i].setY(y);
		board.squares[i].concrete = 0;
	}

	//create vector for blocks
	//std::vector<Block*> blocks;
	//blocks.push_back(builder.buildBlock(L_BLOCK, RED, 0, 0));

	bool quit = false;
	SDL_Event event;
	
	//timer variables
	uint32 currentTime = SDL_GetTicks();
	uint32 previousTime = 0;

	//init RNG
	srand(time(0));

	Color colors[4];
	colors[0] = RED;
	colors[1] = BLUE;
	colors[2] = GREEN;
	colors[3] = YELLOW;

	BlockType blockTypes[5];

	blockTypes[0] = T_BLOCK;
	blockTypes[1] = I_BLOCK;
	blockTypes[2] = O_BLOCK;
	blockTypes[3] = S_BLOCK;
	blockTypes[4] = L_BLOCK;

	while(!quit)
	{
		//check to see if there is no active block
		if(!block)
		{
			int color = rand()%4;
			int blockType = rand()%5;
			block = builder.buildBlock(blockTypes[blockType], colors[color], 210, 30);
			block->setLowestPoint(board.blocksInColumn);
			//block->setLowestBlockY();
		}

		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
			{
				quit = true;
				printf("QUITTING! Another successful run! :)\n");
			}	
			else if(event.type == SDL_KEYDOWN)
			{
				switch(event.key.keysym.sym)
				{
					case SDLK_UP:
					{
						//rotate block counter - clockwise
						block->rotateBlock(board.squares, board.blocksInColumn, -1);
					} break;

					case SDLK_DOWN:
					{
					} break;

					case SDLK_RIGHT:
					{
						block->moveBlock(VELOCITY, board.squares, board.blocksInColumn);
					} break;

					case SDLK_LEFT:
					{
						block->moveBlock(-VELOCITY, board.squares, board.blocksInColumn);
					} break;
					case SDLK_SPACE:
					{
						//first set block's squares to resting place height
						block->setToRest();
					} break;

					default:
					{
					}
				}
			
			}
			
		}
		
		//clear screen set to light gray	
		SDL_SetRenderDrawColor(screen.renderer, 0, 0, 0, 0);
		SDL_RenderClear(screen.renderer);
	
		//render the grid
		renderGrid(screen.renderer);
		//the block
		block->render();	
		//printf("Final Y: %d, LowY: %d\n", block->finalY, block->blockLowY);
		//render board
		for(int i = 0; i < board.numSquares; ++i)
		{
			if(!board.squares[i].concrete) //check if null
			{
				continue;
			}
			board.squares[i].renderBoardSquare();
		}

		//printf("x: %d y: %d\n", block->getX(), block->getY());
		SDL_RenderPresent(screen.renderer);
		
		//bool makeStatic = false; // has block reached its final resting place?

		//check to see if 1/2 of a second has passed. 
		//if so, update locationsj
 		currentTime = SDL_GetTicks();	
		if((currentTime - previousTime) > 500)
		{
			//printf("currentTime - previousTime = %d\n", currentTime - previousTime);
			//updateBlocks(&blocks, board_squares);
			if(updateBlocks(block, &board))
			{
				block = null;

				//check to see if any rows are full
				FilledRowRegion check = checkBoard(&board);

				if(check.firstFilledRow > -1) //then there is a row that is filled!
				{
					clearBoard(check.firstFilledRow, check.lastFilledRow, &board);
				}
			}
			previousTime = currentTime;
		}
		
	}
	
	SDL_DestroyRenderer(screen.renderer);
	SDL_DestroyWindow(screen.window);
	screen.window = null;

	SDL_Quit();
	IMG_Quit();
	return 0;
}

void renderGrid(SDL_Renderer* renderer)
{
	//set line color
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);

	//horizontal lines
	for(int i = 0; i <= 480; i += 30)
	{
		SDL_RenderDrawLine(renderer, 0, i, 300, i);
	}

	//vertical lines
	for(int i = 0; i <= 300; i += 30)
	{
		SDL_RenderDrawLine(renderer, i, 0, i, 480);
	}
}

void clearBoard(int firstRow, int lastRow, Board* board)
{
	//now we move squares that were above delete region down by delete region height
	int numSquaresToClear = (lastRow - firstRow)*COLUMNS + COLUMNS;
	int offsetY = (lastRow - firstRow + 1) * (SQUARE_DIM*SCALE);
	
	//actually setting square y positions
	int firstSquareToMoveDown = firstRow*COLUMNS - COLUMNS;
	int lastSquareToMoveDown = firstSquareToMoveDown + numSquaresToClear;

	//now set row count array
	for(int i = firstRow; i <= lastRow; ++i)
	{
		//printf("Row: %d. Count: %d\n", i, board->blocksInRow[i]);
		board->blocksInRow[i] = 0;
	}
	

	//set board array
	int lastSquareDeleted = lastRow*COLUMNS + COLUMNS - 1;
	printf("lastSquare: %d. NumSquares: %d\n", lastSquareDeleted, numSquaresToClear);

	for(int i = lastSquareDeleted; i >= numSquaresToClear; --i)
	{
		if(board->squares[i - numSquaresToClear].concrete)
		{
			board->squares[i].setColor(board->squares[i - numSquaresToClear].getColor());
			board->squares[i].concrete = 1;
			board->squares[i - numSquaresToClear].concrete = 0;
			board->blocksInRow[i / COLUMNS] += 1; //a square has moved from above to here
			board->blocksInRow[(i - numSquaresToClear) / COLUMNS] -= 1;
		}
		else
		{
			board->squares[i].concrete = 0;
		}
	}

	//subtract 1 from each column if > 0
	for(int i = 0; i < COLUMNS; ++i)
	{
		if(board->blocksInColumn[i] > 0)
		{
			board->blocksInColumn[i] -= lastRow - firstRow + 1;
			//printf("lastRow: %d firstRow: %d\n", lastRow, firstRow);
		}
	}

}

FilledRowRegion checkBoard(Board* board)
{
	int numRowsFilled = 0;
	int firstFilledRow = -1, lastFilledRow = -1;  
	
	for(int i = 0; i < ROWS; ++i)
	{
		if(board->blocksInRow[i] == COLUMNS && firstFilledRow == -1)
		{
			firstFilledRow = i;
			lastFilledRow = i;
		}
		else if(board->blocksInRow[i] == COLUMNS)
		{
			lastFilledRow += 1;
		}
	}
	FilledRowRegion result = {firstFilledRow, lastFilledRow};
	return result;
}

void wipeBoard(Board* board)
{
	for(int i = 0; i < ROWS*COLUMNS; ++i)
	{
		board->squares[i].concrete = 0;
		//printf("i: %d\n", i);
	}

	//clear column and row counts
	for(int i = 0; i < COLUMNS; ++i)
	{
		board->blocksInColumn[i] = 0;
	}
	
	for(int i = 0; i < ROWS; ++i)
	{
		board->blocksInRow[i] = 0;
	}
}

void addSquaresToBoard(Block* bk, Board* board)
{
	for(int i = 0; i < 4; ++i)
	{
		int xOfSquare = bk->squares[i].getX();
		int yOfSquare = bk->squares[i].getY();

		int xBoardPos = xOfSquare / (SQUARE_DIM*SCALE); //the column
		int yBoardPos = yOfSquare / (SQUARE_DIM*SCALE); //the row
	
		//Square sq = bk->squares[i];	
		board->squares[yBoardPos*COLUMNS + xBoardPos].setColor(bk->squares[i].getColor()); //place this in array
		board->squares[yBoardPos*COLUMNS + xBoardPos].concrete = 1;

		//update the array that tracks # blocks per row and column
		board->blocksInRow[yBoardPos] += 1;
		if(board->blocksInColumn[xBoardPos] < ROWS - yBoardPos)
		{
			board->blocksInColumn[xBoardPos] = ROWS - yBoardPos;
		}

		//check if board is overflowed. If so, clear whole board. Game starts over
		if(board->blocksInColumn[xBoardPos] >= ROWS)
		{
			wipeBoard(board); //completely clears board
		}
	}	
}

bool updateBlocks(Block* block, Board* board)
{
	if(!block->update(board->squares, board->blocksInColumn))
	{
		//block could not be updated. Means it is in final resting place. So update static board
		addSquaresToBoard(block, board);
		
		//move block to array of pointers. When this array is full, we clear memory
		//these old blocks used.
		blocks[blockIndex] = block;
		blockIndex++;
		if(blockIndex == 100)
		{
			//time to take out the trash
			for(int i = 0; i < blockIndex; ++i)
			{
				delete blocks[i];
				blocks[i] = null;
			}
			blockIndex = 0;
		}
		return true;
	}	
	return false;
}

inline void updateWindow(SDL_Surface* toShow, Screen* screen)
{
	SDL_Rect stretch;
	stretch.x = 0;
	stretch.y = 0;
	stretch.w = screen->width;
	stretch.h = screen->height;
	SDL_BlitScaled(toShow, null, screen->surface, &stretch);

	//SDL_BlitSurface(toShow, null, screen->surface, null);
	SDL_UpdateWindowSurface(screen->window);
}

bool init(Screen* screen)
{

	//initing the window and surface	
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not init window. SDL_ERROR: %s\n", SDL_GetError());
		return false;
	}
	else
	{
		screen->window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED, 
					SDL_WINDOWPOS_UNDEFINED, screen->width, screen->height, SDL_WINDOW_SHOWN);

		if(screen->window == null)
		{
			printf("Window could not be created. Error: %s\n", SDL_GetError());
			return false;
		}
	}
	screen->surface = SDL_GetWindowSurface(screen->window);

	//creating & initing renderer
	screen->renderer = SDL_CreateRenderer(screen->window, -1, SDL_RENDERER_ACCELERATED);

	if(!screen->renderer)
	{
		printf("Could not initialize the renderer. Error: %s\n", SDL_GetError());
		return false;
	}

	//initing SDL_Image library
	int imgFlags = IMG_INIT_PNG;

	if(!IMG_Init(imgFlags) & imgFlags)
	{
		printf("SDL_Image could not initialize. Error: %s\n", IMG_GetError());
		return false;
	}
	return true;
}

