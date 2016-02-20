enum Color
{
	RED, BLUE, GREEN, YELLOW
};

//a test enum
enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

//structs
struct Screen
{
	SDL_Window* window;
	SDL_Surface* surface;
	SDL_Renderer* renderer;
	int width;
	int height;
};

struct FilledRowRegion
{
	int firstFilledRow;
	int lastFilledRow;
};

#define ROWS 16  //standard # of rows & columns for a Tetris board
#define COLUMNS 10


#define SQUARE_DIM 10 //raw pixel dimension of square
#define SCALE 3 //how much do we want to scale our texture by?
#define VELOCITY SCALE*SQUARE_DIM //how many pixels per update do blocks move?

#define SCREEN_WIDTH 300 //SQUARE_DIM*SCALE*COLUMNS
#define SCREEN_HEIGHT 480 //SQUARE_DIM*SCALE*ROWS
