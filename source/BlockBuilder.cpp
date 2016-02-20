enum BlockType
{
	T_BLOCK, 
	I_BLOCK, 
	O_BLOCK,
    S_BLOCK,
	L_BLOCK	
};

#define SQUARES_IN_BLOCK 4

class BlockBuilder
{
	public:
		BlockBuilder();
		Block* buildBlock(BlockType type, Color col, int x = 0, int y = 0);
	
	private:
		Location t_block[SQUARES_IN_BLOCK];
		Location i_block[SQUARES_IN_BLOCK];
		Location o_block[SQUARES_IN_BLOCK];
		Location s_block[SQUARES_IN_BLOCK];
		Location l_block[SQUARES_IN_BLOCK];	
		void setLocationArray(Location[], Location[]);
};

BlockBuilder::BlockBuilder()
{
	t_block[0] = ORIGIN; 
	t_block[1] = LEFT_OF_ORIGIN; 
	t_block[2] = RIGHT_OF_ORIGIN; 
	t_block[3] = TOP_OF_ORIGIN;

	i_block[0] = ORIGIN; 
	i_block[1] = TWO_LEFT_OF_ORIGIN; 
	i_block[2] = LEFT_OF_ORIGIN; 
	i_block[3] = RIGHT_OF_ORIGIN;

	o_block[0] = ORIGIN; 
	o_block[1] = TOP_OF_ORIGIN; 
	o_block[2] = TOP_LEFT_OF_ORIGIN; 
	o_block[3] = LEFT_OF_ORIGIN;

	s_block[0] = ORIGIN; 
	s_block[1] = LEFT_OF_ORIGIN; 
	s_block[2] = TOP_OF_ORIGIN; 
	s_block[3] = TOP_RIGHT_OF_ORIGIN;

	l_block[0] = ORIGIN; 
	l_block[1] = TOP_OF_ORIGIN; 
	l_block[2] = RIGHT_OF_ORIGIN; 
	l_block[3] = TWO_RIGHT_OF_ORIGIN;	
}

Block* BlockBuilder::buildBlock(BlockType type, Color col, int x, int y)
{
	//selecting correct template array	
	Location* locations;
	Block* block = new Block(col, x, y);	

	switch(type)
	{
		case T_BLOCK: locations = t_block; break;
		case O_BLOCK: locations = o_block; break;
		case I_BLOCK: locations = i_block; break;
		case S_BLOCK: locations = s_block; break;
		case L_BLOCK: locations = l_block; break;
		default: return null;
	}

	for(int i = 0; i < SQUARES_IN_BLOCK; ++i)
	{
		block->setSquare(i, locations[i]);
	}
	
	return block;
}

