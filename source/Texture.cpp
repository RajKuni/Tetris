class Texture
{
	public:
		Texture(); //constructer
		~Texture(); //destructor
		bool loadFromFile(char* path);
		void free(); //de-allocate texture
		void render(int x, int y, int scale = 1, SDL_Rect* region = null); //render at a certain point, can also render part of texture
		int getWidth(); //get width and height of texture
		int getHeight();
		static void setRenderer(SDL_Renderer*); //sets a global pointer to renderer for all texture instances
		SDL_Texture* getTexture();

	private:
		SDL_Texture* texture;
		static SDL_Renderer* renderer; //a constant pointer to main renderer
		int height;
		int width;
};

//set renderer. Need to define all static members here!!! JUST AFTER declaring class structure
SDL_Renderer* Texture::renderer = null;

Texture::Texture()
{
	texture = null;
	height = 0;
	width = 0;
}

Texture::~Texture()
{
	free();
}

SDL_Texture* Texture::getTexture()
{
	return texture;
}

void Texture::free()
{
	if(texture) //if there is a texture, we can destroy it
	{
		SDL_DestroyTexture(texture);
		texture = null;
		width = 0;
		height = 0;
	}
}

void Texture::setRenderer(SDL_Renderer* mRenderer)
{
	if(!renderer) //only set if it is null
	{
		renderer = mRenderer;
	}
}

bool Texture::loadFromFile(char* path)
{
	free(); //free old texture, if there

	SDL_Texture* newTexture = null;
	SDL_Surface* surface = IMG_Load(path);

	if(surface == null)
	{
		printf("Unable to load file: %s. Error: %s\n", path, IMG_GetError());
		return false;
	}
	else
	{
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0xff, 0xff));
		newTexture = SDL_CreateTextureFromSurface(renderer, surface);

		if(newTexture == null)
		{
			printf("Unable to create texture. Error: %s\n", SDL_GetError());
			return false;
		}
		else
		{
			height = surface->h;
			width = surface->w;
		}
		SDL_FreeSurface(surface);
	}

	texture = newTexture;
	return true;
}

void Texture::render(int x, int y, int scale, SDL_Rect* region)
{
	SDL_Rect renderRect = {x, y, width, height};
	
	if(region)
	{
		renderRect.w = region->w*scale;
		renderRect.h = region->h*scale;
	}	

	SDL_RenderCopy(renderer, texture, region, &renderRect);
}

int Texture::getWidth()
{
	return width;
}

int Texture::getHeight()
{
	return height;
}
