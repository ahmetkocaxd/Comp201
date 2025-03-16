// Write your full name: Ahmet Koca, write your KU ID: 0076779

#include <stdio.h>
#include <stdlib.h>

#define EMPTY '-'
#define HERBIVORE 'H'
#define CARNIVORE 'C'
#define PLANT 'P'

// Function Prototypes
char** read_map_from_txt(const char* filename, int *width, int *height);
void print_grid(char** grid, int width, int height);
char** create_temp_grid(char** grid, int width, int height);
void cleanup_grid(char** grid, int height);
void simulate(char** grid, int width, int height, int numSteps);


int main(int argc, char **argv) {
    int width, height;
    char *filename = argv[1];
    int numSteps = atoi(argv[2]);
    char **grid = read_map_from_txt(filename, &width, &height);

    printf("Initial Grid:\n");
    print_grid(grid, width, height);
    printf("\n\n");


    simulate(grid, width, height,numSteps);

    
    cleanup_grid(grid, height);

    return 0;
}



char** read_map_from_txt(const char* filename, int *width, int *height) {
    
   // Reads the map from the file and returns a 2D array of characters.
    
    char **grid;
    FILE *file = fopen(filename, "r"); //opening the file
	
    if(file == NULL){
	fprintf(stderr, "error\n");
	exit(EXIT_FAILURE);
    }

    fscanf(file, "%d %d", width, height); //scanning the two digits which represents width and height respectively.

    grid = malloc(*height * sizeof(char*)); //allocating memory for pointers to pointers.

    for(int i = 0; i < *height; i++)
    {
	    grid[i] = malloc(*width * sizeof(char)); //allocating memory for pointers to strings. grid[i] is a line (a string)
	    
	    for(int j = 0; j < *width; j++)
	    {
		    fscanf(file, " %c", &grid[i][j]); //reading the map and assigning it to the grid one by one.
	    }
    }

    fclose(file); //closing the file

    return grid;
}

void print_grid(char** grid, int width, int height) {
    
   // Print grid to the console with one extra new line at the end for readability.
    
    for(int i = 0; i < height; i++)
    {
	for(int j = 0; j < width; j++)
	{
		printf("%c ", grid[i][j]);
	}
	printf("\n"); //newline after each row is completed
    }
    printf("\n"); //newline after printing the whole grid (fore readability)
}

void cleanup_grid(char** grid, int height) {
    
   // Free the memory allocated for the grid.
    
    for(int i = 0; i < height; i++)
    {
	    free(grid[i]); //freeing memory allocated for each row, which was allocated during read_map_from_txt
    }

    free(grid);	//freeing the memory allocated for the whole grid, which was pointers to pointers. Allocated during read_map_from_txt.

}
char** create_temp_grid(char** grid, int width, int height) {
    
   // Create a temporary grid to store the state of the grid.
    
    char **tempGrid;
    tempGrid = malloc(height * sizeof(char*));

    for(int i = 0; i < height; i ++)
    {
	    tempGrid[i] = malloc(width * sizeof(char));
	    for(int j = 0; j < width; j++)
	    {
		    tempGrid[i][j] = grid[i][j];
	    }
    }

    return tempGrid;
}

void grow_plants(char** grid, char** tempGrid, int width, int height){
	
	for(int i = 0; i < height ; i++)
	{
		for(int j = 0; j < width; j++)
		{
			if(tempGrid[i][j] == PLANT)
			{
				if( i-1 >= 0 && j-1 >= 0 && tempGrid[i-1][j-1] == EMPTY){
					grid[i-1][j-1] = PLANT; //case of top left corner
				}
				if( i-1 >= 0 && j+1 < width && tempGrid[i-1][j+1] == EMPTY){
					grid[i-1][j+1] = PLANT; //case of top right corner
				}
				if( i+1 < height && j-1 >= 0 && tempGrid[i+1][j-1] == EMPTY){
					grid[i+1][j-1] = PLANT; //case of bottom left corner
				}
				if( i+1 < height && j+1 < width && tempGrid[i+1][j+1] == EMPTY){
					grid[i+1][j+1] = PLANT; //case of bottom right corner
				}
			}
		}
	}

}

void move_herbivores(char** grid, char** tempGrid, int width, int height){

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			if(tempGrid[i][j] == HERBIVORE)
			{
				int plantNotFound = 1;
				if( i-1 >= 0 && j-1 >= 0 && tempGrid[i-1][j-1] == PLANT){
					grid[i-1][j-1] = HERBIVORE; //case of top left corner
					plantNotFound = 0;
				}
				if( i-1 >= 0 && tempGrid[i-1][j] == PLANT){
					grid[i-1][j] = HERBIVORE; // upper neighbor
					plantNotFound = 0;
				}
				if( i-1 >= 0 && j+1 < width && tempGrid[i-1][j+1] == PLANT){
					grid[i-1][j+1] = HERBIVORE; //case of top right corner
					plantNotFound = 0;
				}
				if( j-1 >= 0 && tempGrid[i][j-1] == PLANT){
					grid[i][j-1] = HERBIVORE; // left neighbor
					plantNotFound = 0;
				}
				if( j+1 < width && tempGrid[i][j+1] == PLANT){
					grid[i][j+1] = HERBIVORE; // right neighbor
					plantNotFound = 0;
				}
				if( i+1 < height && j-1 >= 0 && tempGrid[i+1][j-1] == PLANT){
					grid[i+1][j-1] = HERBIVORE; //case of bottom left corner
					plantNotFound = 0;
				}
				if( i+1 < height && tempGrid[i+1][j] == PLANT){
					grid[i+1][j] = HERBIVORE; // lower neighbor
					plantNotFound = 0;
				}
				if( i+1 < height && j+1 < width && tempGrid[i+1][j+1] == PLANT){
					grid[i+1][j+1] = HERBIVORE; //case of bottom right corner
					plantNotFound = 0;
				}
				if(plantNotFound == 1){
					grid[i][j] = EMPTY; // if there are no plants nearby
				}

			}	
		}
	}

}

void move_carnivores(char** grid, char** tempGrid, int width, int height){
	
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			//if the element is a carnivore
			if(tempGrid[i][j] == CARNIVORE)
			{
				int herbivoreNotFound = 1;
				if( i-1 >= 0 && j-1 >= 0 && tempGrid[i-1][j-1] == HERBIVORE){
					grid[i-1][j-1] = CARNIVORE; //case of top left corner
					herbivoreNotFound = 0;
				}
				if( i-1 >= 0 && tempGrid[i-1][j] == PLANT){
					grid[i-1][j] = PLANT; // upper neighbor
					herbivoreNotFound = 0;
				}
				if( i-1 >= 0 && j+1 < width && tempGrid[i-1][j+1] == HERBIVORE){
					grid[i-1][j+1] = CARNIVORE; //case of top right corner
					herbivoreNotFound = 0;
				}
				if( j-1 >= 0 && tempGrid[i][j-1] == HERBIVORE){
					grid[i][j-1] = PLANT; // left neighbor
					herbivoreNotFound = 0;
				}
				if( j+1 < width && tempGrid[i][j+1] == HERBIVORE){
					grid[i][j+1] = PLANT; // right neighbor
					herbivoreNotFound = 0;
				}
				if( i+1 < height && j-1 >= 0 && tempGrid[i+1][j-1] == HERBIVORE){
					grid[i+1][j-1] = CARNIVORE; //case of bottom left corner
					herbivoreNotFound = 0;
				}
				if( i+1 < height && tempGrid[i+1][j] == HERBIVORE){
					grid[i+1][j] = PLANT; // lower neighbor
					herbivoreNotFound = 0;
				}
				if( i+1 < height && j+1 < width && tempGrid[i+1][j+1] == HERBIVORE){
					grid[i+1][j+1] = CARNIVORE; //case of bottom right corner
					herbivoreNotFound = 0;
				}
				//if there isnt herbivore around, dies
				if(herbivoreNotFound == 1){
					grid[i][j] = EMPTY; // if there are no plants nearby
				}

			}	
		}
	}

}

void update_tempGrid(char** grid, char** tempGrid, int width, int height){
	
	//copying grid to tempGrid
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			tempGrid[i][j] = grid[i][j];
		}
	}

}

void simulate(char** grid, int width, int height, int numSteps) {  
    /*
    Simulate the grid for numSteps.
    At each step, grow plants, move herbivores, and move carnivores in the order. 
    You are encouraged to write seperate functions for each of these tasks.
    You can copy the grid to a temporary grid and then update the grid for each movement.
    You can write helper functions to use common in the movement of herbivores and carnivores.
    Make sure you print the grid 3 times (after each movement) for each step.
    Make sure you free all the allocated memory. (Valgrind can be useful for this)
    Try to code step by step and clearly understandable which will make both the task easier for you and grading easier for us.
    Good Luck :) */
    
    char **tempGrid = create_temp_grid(grid, width, height);

    for(int step = 1; step <= numSteps; step++){
		
	    //after every move, I am printing the grid and updating the tempGrid
		grow_plants(grid, tempGrid, width, height);
		print_grid(grid, width, height);
		update_tempGrid(grid, tempGrid, width, height);

		move_herbivores(grid, tempGrid, width, height);
		print_grid(grid, width, height);
		update_tempGrid(grid, tempGrid, width, height);

		move_carnivores(grid, tempGrid, width, height);
		print_grid(grid, width, height);
		update_tempGrid(grid, tempGrid, width, height);

		printf("-------------------------------------\n");

    }
	
    //freeing tempGrid
    cleanup_grid(tempGrid, height);

}
