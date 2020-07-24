
grid = [[0, 0, 1, 0, 0, 0],
        [0, 0, 1, 0, 0, 0],
        [0, 0, 0, 0, 1, 0],
        [0, 0, 1, 1, 1, 0],
        [0, 0, 0, 0, 1, 0]]

init = [0, 0]
goal = [len(grid)-1, len(grid[0])-1]

delta = [[-1, 0], #go up
         [0, -1], #go left
         [1, 0], #go down
         [0, 1]] #go right

delta_name = ['^', '<', 'v', '>']

cost = 1

def searchOptimalPath(grid, init, goal):
    