
SDL_Surface* loadBMP(char* mediaName)
{
	SDL_Surface* surface = SDL_LoadBMP(mediaName);
	if(!surface)
	{
		printf("Unable to load the image %s. Error: %s\n", mediaName, surface);
		return null;
	}

	return surface;
}

SDL_Surface* loadIMG(char* mediaName)
{	
	SDL_Surface* surface = IMG_Load(mediaName);
	if(!surface)
	{
		printf("Unable to load the image %s. Error: %s\n", mediaName, IMG_GetError());
		return null;
	}

	return surface;
}

SDL_Surface* loadOptimizedIMG(char* mediaName, SDL_Surface*& screenSurface)
{
	SDL_Surface* surface = IMG_Load(mediaName);
	SDL_Surface* optimizedSurface = SDL_ConvertSurface(surface, screenSurface->format, null);
		
	if(optimizedSurface == null)
	{
		printf("Unable to optimize image: $. Error: %s\n", mediaName, SDL_GetError());
	}	
	SDL_FreeSurface(surface);

	return optimizedSurface;
}

SDL_Surface* loadOptimizedBMP(char* mediaName, SDL_Surface*& screenSurface)
{
	SDL_Surface* surface = loadBMP(mediaName);
	SDL_Surface* optimizedSurface = SDL_ConvertSurface(surface, screenSurface->format, null);
	
	if(optimizedSurface == null)
	{
		printf("Unable to optimize image: $. Error: %s\n", mediaName, SDL_GetError());
	}	
	SDL_FreeSurface(surface);

	return optimizedSurface;
}

SDL_Texture* loadTexture(char* mediaName, SDL_Renderer*& screenRenderer)
{
	SDL_Surface* surface = loadIMG(mediaName);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(screenRenderer, surface);

	if(!texture)
	{
		printf("Unable to create texture. Error: %s\n", SDL_GetError());
		return null;
	}
	SDL_FreeSurface(surface);

	return texture;
}
