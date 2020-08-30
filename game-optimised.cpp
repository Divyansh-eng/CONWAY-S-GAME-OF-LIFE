#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <ctime>

using namespace std;
//0x00 corresponds to black color
#define OFF_COLOUR 0x00
//0xFF corresponds to white color
#define ON_COLOUR 0xFF
// number of rows
#define ROW 300
// number of columns
#define COL 300
// size of cell in pixel
#define cell_size 4
// Limit loop rate for visibility
#define LIMIT_RATE 0
// Tick-rate in milliseconds (if LIMIT_RATE == 1)
#define TICK_RATE 100

// Created a pointer to the object of type SDL_Window
SDL_Window *window = NULL;
// Created a pointer to the object of type SDL_Surface
SDL_Surface *window_surface = NULL;
// Defined the screen_width
unsigned int screen_width = COL*cell_size;
// Defined the screen_height
unsigned int screen_height = ROW*cell_size;

// Defined the Grid class with functions to implement the rules of game on class variables
class Grid
{
    // Kept the class variables as private
    private :
    
    // rows shows about the height of grid
    unsigned int rows;
    
    // cols shows about the width of grid
    unsigned int cols;
    
    // Defined a pointer for 2-D array implementation which indicates dead or live cells LIVE = White  DEAD = Black & gives a count of neighbours in next state
    unsigned char** cell_current;

    uint8_t** cell_new_neighbour;
    
    public :
    
    // Constructor declaration for setting the class variables
    Grid(unsigned int height, unsigned int width);
    
    // destructor declaration for deleting the object
    ~Grid();
    
    // The Grid_Initialize() function is used for Initializing the class variables by giving random input using rand() function
    void Grid_Initialize(); // declaration
    
    // The Grid_New_State() function is used for updating the cell_current with new state according to the rules of game
    void Grid_New_State();  // declaration
    
    // The Grid_Update_Neighbour() function is used for updating the cell_new_neighbour in case of state transition according to the rules of game
    void Grid_Update_Neighbour(unsigned int status, unsigned int y, unsigned int x); //declaration
    
    // The fill_color() function is used for setting the pixel color for the new state transition
    void fill_color(uint8_t cell_color, unsigned int y, unsigned int x); //declaration
    
    // The handle_events() function is used for tracking the mouse and keyboard
    void handle_events(bool &window_status); //declaration
    
};

// Created a Grid object
Grid Current_Grid(ROW,COL);

int main()
{
    // SDL Code for setting SDL variables
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Failed to initialize the SDL2 library\n";
        return -1;
    }
     
    window = SDL_CreateWindow("Conway's Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN);
    if(!window)
    {
        std::cout << "Failed to create window\n";
        return -1;
    }

    window_surface = SDL_GetWindowSurface(window);
    
    if(!window_surface)
    {
        std::cout << "Failed to get the surface from the window\n";
        return -1;
    }
    
    // update frame buffer
    SDL_UpdateWindowSurface(window);

    // SDL_Delay(x) function does a delay of x ms
    SDL_Delay(50);
        
    bool keep_window_open = true;
    
    // Initialize the Grid object
    Current_Grid.Grid_Initialize();
    
    // Used for slowing down for proper visibility
    #if LIMIT_RATE
        SDL_Delay(1000);
    #endif
    
    while(keep_window_open)
    {
        // SDL Event Handler
        Current_Grid.handle_events(keep_window_open);
        
        // Calling Grid_New_State() function for change to new state
        Current_Grid.Grid_New_State();
        
        // update frame buffer
        SDL_UpdateWindowSurface(window);

        // Used for slowing down for proper visibility
        #if LIMIT_RATE
            SDL_Delay(TICK_RATE);
        #endif
    }

    // Destroy window
    SDL_DestroyWindow(window);
    
    // Quit SDL subsystems
    SDL_Quit();

    system("pause");

    return 0;
}

// Constructor Definition
Grid::Grid(unsigned int height, unsigned int width)
{
    // Setting rows and cols
    rows = height;
    cols = width;
    
    // Assigning memory to class variables on heap
    cell_current = new unsigned char*[rows];
    cell_new_neighbour  = new uint8_t*[rows];
    for (unsigned int i=0; i<rows; i++)
    {
        cell_current[i] = new unsigned char[cols];
        cell_new_neighbour[i]  = new uint8_t[cols];
    }
    
    // Setting the class variables to zero
    for(unsigned int i=0;i<rows;i++)
    {
        for(unsigned int j=0; j<cols; j++)
        {
            cell_current[i][j] = (unsigned char) 0;
            cell_new_neighbour[i][j] = 0;
        }
    }
}

Grid::~Grid()
{
    // Releasing the memory used by class variables
    delete [] cell_current;
    delete [] cell_new_neighbour;
    
    cout<<"Destroyed the Object"<<endl;
}

void Grid::Grid_Initialize()
{
    // Using the rand() function for assigning a random state of 0 or 1
    for (unsigned int i=0; i<ROW; i++)
    {
        for (unsigned int j=0; j<COL; j++)
        {
            if(rand()%2 == 1)
            {
                cell_current[i][j] |= 0x01;  // set LSB to 1
                Grid_Update_Neighbour(1,i,j);
            }
        }
    }

    // Setting the cell_current values equal to cell_new_neighbour
    for (unsigned int i=0; i<ROW; i++)
    {
        for (unsigned int j=0; j<COL; j++)
        {
            cell_new_neighbour[i][j] = cell_current[i][j]>>1 ;      // The bit value in cell_current is updated to new neighbour count
        }
    }
 }

void Grid::Grid_Update_Neighbour(unsigned int status, unsigned int y, unsigned int x)
{
    //updating cell_current according to the position of cell where transition occurred
    if(status == 1)
    {
        if(y!=0)
        {
            cell_current[y-1][x]+=0x02;
        }
        if(x!=0)
        {
            cell_current[y][x-1]+=0x02;
        }
        if(y!=ROW-1)
        {
            cell_current[y+1][x]+=0x02;
        }
        if(x!=COL-1)
        {
            cell_current[y][x+1]+=0x02;
        }
        if(x>0 && y>0)
        {
            cell_current[y-1][x-1]+=0x02;
        }
        if(x<COL-1 && y>0)
        {
            cell_current[y-1][x+1]+=0x02;
        }
        if(x>0 && y<ROW-1)
        {
            cell_current[y+1][x-1]+=0x02;
        }
        if(x<COL-1 && y<ROW-1)
        {
            cell_current[y+1][x+1]+=0x02;
        }
    }
    if(status == 0)
    {
        if(y!=0)
        {
            cell_current[y-1][x]-=0x02;
        }
        if(x!=0)
        {
            cell_current[y][x-1]-=0x02;
        }
        if(y!=ROW-1)
        {
            cell_current[y+1][x]-=0x02;
        }
        if(x!=COL-1)
        {
            cell_current[y][x+1]-=0x02;
        }
        if(x>0 && y>0)
        {
            cell_current[y-1][x-1]-=0x02;
        }
        if(x<COL-1 && y>0)
        {
            cell_current[y-1][x+1]-=0x02;
        }
        if(x>0 && y<ROW-1)
        {
            cell_current[y+1][x-1]-=0x02;
        }
        if(x<COL-1 && y<ROW-1)
        {
            cell_current[y+1][x+1]-=0x02;
        }
    }
}
void Grid::Grid_New_State()
{
    // Traversing the grid and checking for state transitions only i.e. 0->1 or 1->0
    for(unsigned int i=0;i<ROW;i++)
    {
        for (unsigned int j=0; j<COL; j++)
        {
            if((cell_current[i][j] & 0x01) == 0 && cell_new_neighbour[i][j] == 3)  // used the bitwise operator
            {
                cell_current[i][j] |= 0x01;                             // set the first bit to one
                Grid_Update_Neighbour(1,i,j); // Need a temporary to calculate properly
                fill_color(ON_COLOUR, i, j);
            }
            if((cell_current[i][j] & 0x01) == 1 && (cell_new_neighbour[i][j]>3 || cell_new_neighbour[i][j]<2)) // used the bitwise operator
            {
                cell_current[i][j] &= ~0x01;                            // set the first bit to zero
                Grid_Update_Neighbour(0,i,j);
                fill_color(OFF_COLOUR, i, j);
            }
        }
    }
    
    // Setting the cell_current values equal to cell_new_neighbour
    for (unsigned int i=0; i<ROW; i++)
    {
        for (unsigned int j=0; j<COL; j++)
        {
            cell_new_neighbour[i][j] = cell_current[i][j]>>1 ; // The bit value in cell_current is updated to new neighbour count
        }
    }
    
}

void Grid::fill_color(uint8_t cell_color,unsigned int y,unsigned int x)
{
    // Using pixel_ptr and pointing to the (x,y) coordinate on surface. pixels is a pointer pointing to array of size 4 with R,G,B and alpha in array.
    uint8_t* pixel_ptr = (uint8_t*)window_surface->pixels + (y * cell_size * screen_width + x * cell_size) * 4;
    
    // Assigning Color
    for (unsigned int i = 0; i < cell_size; i++)
    {
        for (unsigned int j = 0; j < cell_size; j++)
        {
            *(pixel_ptr + j * 4) = cell_color;            // Setting the red color
            *(pixel_ptr + j * 4 + 1) = cell_color;        // Setting the green color
            *(pixel_ptr + j * 4 + 2) = cell_color;        // Setting the blue color
            *(pixel_ptr + j * 4 + 3) = SDL_ALPHA_OPAQUE;  // Setting the alpha value
        }
        pixel_ptr += screen_width * 4;
    }
}

void Grid::handle_events(bool &window_status)
{
    SDL_Event e;
    while(SDL_PollEvent(&e) > 0)
    {
        switch(e.type)
        {
            case SDL_QUIT:
                window_status = false;
                break;
            
            case SDL_KEYDOWN:
                if (e.key.keysym.sym == SDLK_SPACE)
                {
                    window_status =false;
                }
                break;
        }
    }

}
