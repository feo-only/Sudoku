#include <iostream>
#include <exception>
#include <string>
#include <cstring>

class Grid {
	public:
	    int BOX_SIZE;
	    int GRID_SIZE;
	    int GLOBAL_SIZE;
	    int** grid;
	
	    Grid(int boxSize, int gridSize) : BOX_SIZE(boxSize), GRID_SIZE(gridSize), GLOBAL_SIZE(boxSize * gridSize) {
	        grid = new int*[GLOBAL_SIZE];
	        
			// init grid.
	        for (int i = 0; i < GLOBAL_SIZE; ++i) {
	            grid[i] = new int[GLOBAL_SIZE];
	            
	            for (int j = 0; j < GLOBAL_SIZE; ++j) {
	                grid[i][j] = 0;
	            }
	        }
	    }
	
	    ~Grid() {
	        for (int i = 0; i < GLOBAL_SIZE; ++i) delete[] grid[i];
	        
	        delete[] grid;
	    }
	
	    int* getRow(int row) {
			// row could be in [1, GLOBAL_SIZE]
	        if (row <= 0 || row > GLOBAL_SIZE) throw std::exception();
	        
	        return grid[row];
	    }
	
	    int* getColumn(int column) {
			// column could be in [1, GLOBAL_SIZE]
	        if (column <= 0 || column >= GLOBAL_SIZE) throw std::exception();
	        
	        int* colData = new int[GLOBAL_SIZE];
	        for (int i = 0; i < GLOBAL_SIZE; ++i) colData[i] = grid[i][column];
	        
	        return colData;
	    }
	
	    int** getBox(int col, int row) {
			if (col <= 0 || row <= 0) throw std::exception();
			if (col > GLOBAL_SIZE || row > GLOBAL_SIZE) throw std::exception();

	        int** localBox = new int*[BOX_SIZE];
	        for (int i = 0; i < BOX_SIZE; ++i) {
	            localBox[i] = new int[BOX_SIZE];
	            
	            for (int j = 0; j < BOX_SIZE; ++j) 
	                localBox[i][j] = grid[row * BOX_SIZE + i][col * BOX_SIZE + j];
	            
	        }
	        
	        return localBox;
	    }
	
	    void showGrid() {
			// display the grid

	        for (int i = 0; i < GLOBAL_SIZE; ++i) {
	        	if (i != 0 && i % BOX_SIZE == 0) std::cout<<'\n';
	        	
	            for (int j = 0; j < GLOBAL_SIZE; ++j) { 
	                if (j == GLOBAL_SIZE - 1) {
	                	
	                    std::cout << grid[i][j] << '\n';
	                    continue;
	                }
	                if ((j + 1)% BOX_SIZE == 0 && j != 0) {
	                	
	                    std::cout << grid[i][j] << "  ";
	                    continue;
	                }
	                
	                std::cout << grid[i][j] << " ";
	            }
	        }
	        std::cout<<'\n';
	    }
};

class Sudoku : public Grid {
	// if the problem has been solved yet, it could be True.
	// anyway, I hope the user won't inference twice...
	bool is_solved = false;

	public:
	    Sudoku(const std::string input_): Grid(3, 3) {
	    	if (input_.length() != 81) throw std::exception();
	    	
			int* input = transform(input_);
			
			std::cout<<"Creating grid..."<<'\n';
	        for (int i = 0; i < GLOBAL_SIZE; ++i)
	            for (int j = 0; j < GLOBAL_SIZE; ++j)
	                grid[i][j] = input[i * GLOBAL_SIZE + j];
	        
	        showGrid();
	    }
		
		int* transform(const std::string input_) {
			// the function is to transform the string like input to a int vector.

			int n = input_.length();
			int* input = new int[n];
			
			for(int i = 0; i < n; i ++) input[i] = input_[i] - '0';
			
			return input;
		}
		
	    bool solver(int**& box) {
	        for (int i = 0; i < 9; ++i) {
	            for (int j = 0; j < 9; ++j) {
	                if (box[i][j] == 0) {
	                    for (int num = 1; num <= 9; ++num) {
	                        if (isValid(box, i, j, num)) {

	                            box[i][j] = num;

	                            if (solver(box)) return true;

	                            box[i][j] = 0;
	                        }
	                    }
	                    return false;
	                }
	            }
	        }
	        return true;
	    }
	
	    bool isValid(int**& board, int row, int col, int num) {

			// check in row
	        for (int i = 0; i < 9; ++i) 
	            if (board[row][i] == num) return false;
	        
			// check in column
	        for (int i = 0; i < 9; ++i) 
	            if (board[i][col] == num) return false;
	        
	
	        int boxRowStart = (row / 3) * 3;
	        int boxColStart = (col / 3) * 3;
			
			// check in boxes
	        for (int i = 0; i < 3; ++i) 
	            for (int j = 0; j < 3; ++j) 
	                if (board[boxRowStart + i][boxColStart + j] == num) return false;
	            
	        
	        return true;
	    }
	
	    void inference() {
	    	std::cout<<"Inferencing..."<<'\n';
	    	if (is_solved == false){
				
		        int** copy_grid = new int*[GLOBAL_SIZE];

		        for (int i = 0; i < GLOBAL_SIZE; ++i) {
		            copy_grid[i] = new int[GLOBAL_SIZE];

		            for (int j = 0; j < GLOBAL_SIZE; ++j) 
		                copy_grid[i][j] = grid[i][j];
		            
		        }
		        
		        solver(copy_grid);
		        for (int i = 0; i < GLOBAL_SIZE; ++i) {
		            delete[] grid[i];
		        	grid[i] = copy_grid[i];
		        }

		        showGrid();

		        is_solved = true;
	   		}
		else{
			showGrid();
		}
	    }
};

int main(){
	std::string input_ = "017903600000080000900000507072010430000402070064370250701000065000030000005601720";
	Sudoku solver = Sudoku(input_);
	solver.inference();
}
