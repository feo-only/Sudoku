import copy

class Grid:
    def __init__(self, box_size, grid_size):
        self.BOX_SIZE = box_size
        self.GRID_SIZE = grid_size
        self.GLOBAL_SIZE = box_size * grid_size
        self.grid = [[0 for _ in range(self.GLOBAL_SIZE)] for _ in range(self.GLOBAL_SIZE)]
    
    def getRow(self, row : int):
        if row <= 0 or row >=self.GLOBAL_SIZE: raise Exception('Out of Index.')
        
        return self.grid[row]
    
    def getColumn(self, column : int):
        if column <= 0 or column >=self.GLOBAL_SIZE: raise Exception('Out of Index.')

        return [data[column] for data in self.grid]
    
    def getBox(self, col, row):
        local_box = [[self.grid[row * self.BOX_SIZE + i][col * self.BOX_SIZE + j] for j in range(self.BOX_SIZE)] for i in range(self.BOX_SIZE)]
        return local_box
    
    def showGrid(self):
        """
        output the grid
        """
        for i in range(self.GLOBAL_SIZE):
            print(self.grid[i])
    

class Sudoku(Grid):
    def __init__(self, input):
        super().__init__(3, 3)

        if type(input) == str:
            # if a str type input accepted, transform it.
            input = self.__transform(input)
        
        if(len(input) != 81): raise Exception('Input size is invalid!');
        

        for i in range(self.GRID_SIZE * self.BOX_SIZE):
            for j in range(self.GRID_SIZE * self.BOX_SIZE):
                self.grid[i][j] = input[i * self.GRID_SIZE * self.BOX_SIZE + j]
        
        # if the problem is solved, the signal will be set to True to avoid repeating solving.
        self.is_solved = False
        print('init the grid...')
        self.showGrid()

    def __transform(self, input : str):
        """
        Transform a str type input into a Int type list
        """
        input_trasformed = [int(data) for data in input]
        return input_trasformed
    
    def __solver(self, box):
        """
        private solver
        """
        for i in range(9):
            for j in range(9):
                if (box[i][j] == 0):    # find a zero block
                    for num in range(1, 10):   # fill with 1~9
                        if self.__isValid(box, i, j, num):
                            box[i][j] = num;  
                            if self.__solver(box):  # solve the other zero block
                                return True  # if solved by a recursive way, return True
                            
                            box[i][j] = 0
                        
                    
                    return False
                
        return True

    def __isValid(self, board, row, col, num):
        # Check in row 
        for i in range(9): 
            if board[row][i] == num : return False
        

        # Check in column
        for i in range(9):
            if board[i][col] == num : return False
        

        # check a 3x3 grid
        boxRowStart = (row // 3) * 3
        boxColStart = (col // 3) * 3
        for i in range(3):
            for j in range(3):
                if board[boxRowStart + i][boxColStart + j] == num : 
                    return False


        return True


    def inference(self):
        print('solving...')
        if self.is_solved  == False:
            copy_grid = copy.deepcopy(self.grid)
            if self.__solver(copy_grid):
                self.grid = copy_grid
                self.showGrid()
            
            else : print('Failed to solve the problem.')
            self.is_solved = True
        else: self.showGrid()
        
  

if __name__ == '__main__':
    # In a input str, zero means the position to be filled.
    input_str = '017903600000080000900000507072010430000402070064370250701000065000030000005601720'

    solver = Sudoku(input_str)
    solver.inference()