//these enums tell location of a square from the origin 
//point of a block
enum Location
{
	ORIGIN,
	LEFT_OF_ORIGIN,
	RIGHT_OF_ORIGIN,
	TOP_OF_ORIGIN,
	BOTTOM_OF_ORIGIN,
	TOP_RIGHT_OF_ORIGIN,
	TOP_LEFT_OF_ORIGIN,
	TWO_LEFT_OF_ORIGIN,
	TWO_RIGHT_OF_ORIGIN,
	TWO_TOP_OF_ORIGIN,
	TWO_BOTTOM_OF_ORIGIN	
};

class Block
{
	public:
		Block(Color col = RED, int x = 0, int y = 0);
		void setX();
		void setY();
		int getX();
		int getY();
		void setSquare(int square, Location loc);
		void rotateBlock(Square[], int[], int);
		void render();
		Square* getSquares(); //returns pointer to first element of array of squares
		bool update(Square[], int[]);
		void moveBlock(int offset, Square[], int[]);
		Square squares[4];
		void setLowestPoint(int[]);
		void setLowestBlockY(); 
		void setToRest();
		int blockLowY; //what is lowest point of block currently
		int finalY; //where can block jump to

	private:
		int xCenter; //center point of the block
		int yCenter;
		Color color;
		void translateToOrigin(int&, int&);
		void rotate(int&, int&, int);
		void translateBack(int&, int&);
		bool boardCollisionCheck(Square*, Square[], int x = 0, int y = 0);
};

Block::Block(Color col, int x, int y)
{
	xCenter = x;
	yCenter = y;
	color = col;	
}

void Block::setSquare(int square, Location loc)
{
	squares[square] = Square(color);
	int offsetX, offsetY;
	int trueSize = SQUARE_DIM*SCALE;

	switch(loc)
	{
		case ORIGIN: 
		{
			offsetX = 0*trueSize; 
			offsetY = 0*trueSize; 
		} break;

		case RIGHT_OF_ORIGIN: 
		{
			offsetX = 1*trueSize; 
			offsetY = 0*trueSize; 
		} break;

		case LEFT_OF_ORIGIN: 
		{
			offsetX = -1*trueSize; 
			offsetY = 0*trueSize; 
		} break;

		case TWO_RIGHT_OF_ORIGIN: 
		{
			offsetX = 2*trueSize; 
			offsetY = 0*trueSize; 
		} break;

		case TWO_LEFT_OF_ORIGIN: 
		{
			offsetX = -2*trueSize; 
			offsetY = 0*trueSize; 
		} break;

		case TOP_OF_ORIGIN: 
		{
			offsetX = 0*trueSize; 
			offsetY = -1*trueSize; 
		} break;

		case BOTTOM_OF_ORIGIN: 
		{
			offsetX = 0*trueSize; 
			offsetY = 1*trueSize; 
		} break;

		case TWO_TOP_OF_ORIGIN: 
		{
			offsetX = 0*trueSize; 
			offsetY = -2*trueSize; 
		} break;

		case TWO_BOTTOM_OF_ORIGIN: 
		{
			offsetX = 0*trueSize; 
			offsetY = 2*trueSize; 
		} break;

		case TOP_RIGHT_OF_ORIGIN: 
		{
			offsetX = 1*trueSize; 
			offsetY = -1*trueSize; 
		} break;

		case TOP_LEFT_OF_ORIGIN: 
		{
			offsetX = -1*trueSize; 
			offsetY = 1*trueSize; 
		} break;
	}

	squares[square].setX(xCenter + offsetX);
	squares[square].setY(yCenter + offsetY);
}

int Block::getX()
{
	return xCenter;
}

int Block::getY()
{
	return yCenter;
}

void Block::render()
{
	for(int i = 0; i < 4; ++i)
	{
		//int curY = squares[i].getY();
		//int offset = squares[i].getY() - blockLowY; //squares[0] always center square
		squares[i].render();
	}
}

bool Block::boardCollisionCheck(Square* sq, Square board_squares[], int offsetX, int offsetY)
{
	//on checkerboard, which column is square on?
	int sqColumn = sq->getX() / (SQUARE_DIM*SCALE); 

	//which row??
	int sqRow = sq->getY() / (SQUARE_DIM*SCALE);

	if(sqRow < 0 || sqColumn < 0) return false; //not even on board, ignore
	
	//add offset to see which row/ column the block will be in next
	sqColumn += offsetX / (SQUARE_DIM*SCALE);
   	sqRow += offsetY / (SQUARE_DIM*SCALE);	

	//now check and see if moving square by offsetX will make it collide with something already on the board
	if(!board_squares[sqRow*COLUMNS + sqColumn].concrete) return false;

	//how about offsetY?
	if(!board_squares[sqRow*COLUMNS + sqColumn].concrete) return false;

	return true;
}

void Block::setLowestBlockY()
{
	int lY = -1;
	for(int i = 0; i < 4; ++i)
	{
		int curY = squares[i].getY();
		if(curY > lY)
		{
			lY = curY;
		}
	}
	blockLowY = lY;
}

void Block::setLowestPoint(int columnCount[])
{
	int fY = 10000;
	int lowIndex = 0;
	for(int i = 0; i < 4; ++i)
	{
		//get this square's current column
		int curCol = (squares[i].getX()) / (SQUARE_DIM*SCALE);
		int currLowY = (SQUARE_DIM*SCALE*ROWS) - (columnCount[curCol]*SQUARE_DIM*SCALE) - (SQUARE_DIM*SCALE);
		//printf("Screen Height: %d, column: %d, columnCount: %d\n", SQUARE_DIM*SCALE*ROWS, curCol, columnCount[curCol]);
		int height = currLowY - squares[i].getY();
		if(height < fY)
		{
			fY = height;
			finalY= currLowY;
			lowIndex = i;
		}

	}	
	//finalY = fY;

	//now set final Ys for all squares
	int relCenter = squares[lowIndex].getY();	
	for(int i = 0; i < 4; ++i)
	{
		int offset = relCenter - squares[i].getY();
		int result = 0;		
		result = finalY - offset;	
		squares[i].setLowestY(result);
	}

}

void Block::setToRest()
{
	for(int i = 0; i < 4; ++i)
	{
		int lowY = squares[i].getLowestY();
		squares[i].setY(lowY);
	}
}

bool Block::update(Square board_squares[], int columnCount[]) //moves block down
{

	//first check if move is valid
	for(int i = 0; i < 4; ++i)
	{
		//block is at ground
		if(squares[i].getY() + (SQUARE_DIM*SCALE) + VELOCITY > SCREEN_HEIGHT) return false;
		
		//check if board is already occupied below block's current position
		if(boardCollisionCheck(&squares[i], board_squares, 0, VELOCITY)) return false;
	}
	
	int newLowestY = -1; //initing lowest Y

	for(int i = 0; i < 4; ++i)
	{
		int oldY = squares[i].getY();
		squares[i].setY(oldY + VELOCITY); //blocks move down VELOCITY pixels at a time (right now)!

		//update the lowest point block can jump to.
		//get column of current square
		int column = squares[i].getX() / VELOCITY;

			
	}	
	
	//check what the lowest point of the board block can currently jump to.
	//setLowestPoint(columnCount);

	//finally update yCenter and lowestY of block and finalY
	yCenter += VELOCITY;
	setLowestBlockY();
	setLowestPoint(columnCount);
	return true;
}

void Block::moveBlock(int offset, Square board[], int columnCount[])
{

	//first check if move is valid
	for(int i = 0; i < 4; ++i)
	{
		int newX = squares[i].getX()+ offset;
		int squareY = squares[i].getY();	
		int areaToCheck = (squareY/(SQUARE_DIM*SCALE))*COLUMNS + (newX / (SQUARE_DIM*SCALE));

		if(squares[i].getX() + offset < 0 || 
				squares[i].getX() + (SQUARE_DIM*SCALE) + offset > SCREEN_WIDTH ||
				board[areaToCheck].concrete > 0)
		{
			//already at border of play screen. Do nothing
			//or colliding with occupied area
			return;
		}	
	}	

	//then update locations
	for(int i = 0; i < 4; ++i)
	{
		int oldX = squares[i].getX();
		squares[i].setX(oldX + offset);
	}	

	//finally update xCenter
	xCenter += offset;

	setLowestBlockY();
	//then set lowestY
	setLowestPoint(columnCount);
}

struct RotateStruct
{
	int x;
	int y;
};

void Block::translateToOrigin(int& oldX, int& oldY)
{
	//translate following square to origin OF SCREEN	
	oldX =  oldX - xCenter;
	oldY =  oldY - yCenter;

	//squares[index].setX(newX);
	//squares[index].setY(newY);	
}

void Block::rotate(int& originX, int& originY, int direction)
{
	//invert the y
	originY = -1*originY;
	//squares[index].setY(-1*squares[index].getY());	

	int oldX = originX;
	int oldY = originY;
	//do rotation
	 originX = direction*oldY;
	 originY = -1*direction*oldX;

	//invert y
	originY = -1*originY;
}

inline void Block::translateBack(int& x, int& y)
{
	x = x + xCenter;
	y = y + yCenter;
}	

void Block::rotateBlock(Square board[], int columnCount[], int direction)
{
	RotateStruct rotations[4];

	//we want to keep track of leftmost and rightmost point of block after rotation
	int leftMost = SCREEN_WIDTH;
	int rightMost = SCREEN_WIDTH;	

	for(int i = 0; i < 4; ++i)
	{
		int oldX = squares[i].getX();
		int oldY = squares[i].getY();


		rotations[i] = {oldX, oldY};
		translateToOrigin(rotations[i].x, rotations[i].y);
		rotate(rotations[i].x, rotations[i].y, direction);
		translateBack(rotations[i].x, rotations[i].y);

		//now to check for collision
		int gridX = rotations[i].x / 30;
		int gridY = rotations[i].y / 30;
		
		if(board[gridY*COLUMNS + gridX].concrete) //can't rotate because of collision return
		{
			//collision detected, make no rotation
			return; 
		}
		
		if(rotations[i].x < leftMost)
		{
			leftMost = rotations[i].x;
		}
		if(rotations[i].x > rightMost)
		{
			rightMost = rotations[i].x;
		}
	}	

	//if rotation causes block to move out of screen on left or right, we need to 
	//"push" block back into screen by amount block is out of screen
	if(leftMost <= 0)
	{
		for(int i = 0; i < 4; ++i)
		{
			rotations[i].x = rotations[i].x - leftMost;
		}
	}
	else if(rightMost > SCREEN_WIDTH - 30)
	{
		for(int i = 0; i < 4; ++i)
		{
			//last term accounts for fact that we render from top left of square. Therefore, also need to subtract last amount
			rotations[i].x = rotations[i].x - (rightMost - SCREEN_WIDTH) - (SCALE*SQUARE_DIM);
		}
	}

	//now set all squares to new locations
	for(int i = 0; i < 4; ++i)
	{
		squares[i].setX(rotations[i].x);
		squares[i].setY(rotations[i].y);
	}

	//set lowest point of block
	setLowestBlockY();
	//set finalY again
	setLowestPoint(columnCount);
}
