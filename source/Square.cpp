class Square
{
	public:
		Square(); //a default constructor
		Square(int offset);
		Square(Square* sq); //to copy a square to new instance
		Square(Color col);
		int getX();
		int getY();
		void setX(int);
		void setY(int);
		void setColor(Color);
		void render();
		void renderBoardSquare();
		static void setTexture(char* path);
		static Texture getTexture();
		SDL_Rect getTextureRegion();
		Color getColor();
		~Square();
		void setLowestY(int);
		int getLowestY();
		uint8 concrete; //has the square actually been given pertinent properties (e.g. location, texture) 

	private:
		SDL_Rect textureRegion; //set once to let square know which part of main texture it uses
		Color color;
		int xPos;
		int yPos;
		int lowestY; //lowest resting point of the square
		static Texture mainTexture; //all squares will get their texture from this
		void setTextureRegion(Color);
};

//define the main texture
Texture Square::mainTexture;

Square::Square()
{
	concrete = 0; //because nothing has been set
}

Square::Square(Color col)
{
	color = col;
	setTextureRegion(col);
	concrete = 1;
}

Square::Square(int offset)
{
	textureRegion = {offset*SQUARE_DIM, 0, SQUARE_DIM, SQUARE_DIM};	
}

Square::Square(Square* sq)
{
	textureRegion = sq->getTextureRegion();
	xPos = sq->getX();
	yPos = sq->getY();
	color = sq->getColor();
	concrete = 1; //things have been set. This instance of square is now concrete
}

Square::~Square()
{
}

void Square::setLowestY(int y)
{
	lowestY = y;
}

void Square::setTexture(char* path)
{
	mainTexture.loadFromFile(path);	
}

void Square::setX(int x)
{
	xPos = x;
}

void Square::setY(int y)
{
	yPos = y;
}

void Square::setColor(Color col)
{
	color = col;
	//now setTextureRegion
	setTextureRegion(col);
}

void Square::setTextureRegion(Color col)
{	
	/*if(col == color)
	{
		return; //color is the same. RETURN
	}*/

	int offset;
	switch(col)
	{
		case RED: offset = 0; break;
		case BLUE: offset = 1; break;
		case GREEN: offset = 2; break;
		case YELLOW: offset = 3; break;
		default: offset = 0; //default red
	}

	textureRegion.x = offset*SQUARE_DIM;
    textureRegion.y = 0;
	textureRegion.w = SQUARE_DIM;
	textureRegion.h = SQUARE_DIM;	
}


int Square::getX()
{
	return xPos;
}

int Square::getY()
{
	return yPos;
}

int Square::getLowestY()
{
	return lowestY;
}

void Square::render()
{
	mainTexture.render(xPos, yPos, SCALE, &textureRegion);	

	//then render it at low point
	//changing alpha
	SDL_SetTextureBlendMode(mainTexture.getTexture(), SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(mainTexture.getTexture(), 128);
	mainTexture.render(xPos, lowestY, SCALE, &textureRegion);
	SDL_SetTextureBlendMode(mainTexture.getTexture(), SDL_BLENDMODE_NONE);
}

void Square::renderBoardSquare()
{	
	mainTexture.render(xPos, yPos, SCALE, &textureRegion);	
}

SDL_Rect Square::getTextureRegion()
{
	return textureRegion;
}

Color Square::getColor()
{
	return color;
}
