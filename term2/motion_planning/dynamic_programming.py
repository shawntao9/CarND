# ----------
# User Instructions:
# 
# Create a function compute_value which returns
# a grid of values. The value of a cell is the minimum
# number of moves required to get from the cell to the goal. 
#
# If a cell is a wall or it is impossible to reach the goal from a cell,
# assign that cell a value of 99.
# ----------
from collections import deque

grid = [[0, 0, 1, 0, 0, 0],
        [0, 0, 1, 0, 0, 0],
        [0, 0, 1, 0, 0, 0],
        [0, 0, 0, 0, 1, 0],
        [0, 0, 1, 1, 1, 0],
        [0, 0, 0, 0, 1, 0]]

for row in grid:
    print(row)
    
goal = [len(grid)-1, len(grid[0])-1]
cost = 1 # the cost associated with moving from a cell to an adjacent one

delta = [[-1, 0 ], # go up
         [ 0, -1], # go left
         [ 1, 0 ], # go down
         [ 0, 1 ]] # go right

delta_name = ['v', '>', '^', '<']

moves = [[' ' for col in range(len(grid[0]))] for row in range(len(grid))]

def compute_value(grid,goal,cost):
    # ----------------------------------------
    # insert code below
    # ----------------------------------------
    value = [[99 for col in range(len(grid[0]))] for row in range(len(grid))]
    visited = [[False for col in range(len(grid[0]))] for row in range(len(grid))]
    value[goal[0]][goal[1]] = 0
    positions = deque([goal])
    while positions:
        x = positions[0][0]
        y = positions[0][1]
        positions.popleft()
        for i in range(len(delta)):
            x2 = x + delta[i][0]
            y2 = y + delta[i][1]
            if x2 >= 0 and x2 < len(grid) and y2 >= 0 and y2 < len(grid[0]) and not visited[x2][y2]:
                visited[x2][y2] = True
                if grid[x2][y2] == 0:
                    value[x2][y2] = value[x][y] + cost
                    moves[x2][y2] = delta_name[i]
                    positions.append([x2, y2])
        visited[x][y] = True
    # make sure your function returns a grid of values as 
    # demonstrated in the previous video.
    return value 

res = compute_value(grid, goal, cost)
for row in res:
    print(row)
for row in moves:
    print(row)