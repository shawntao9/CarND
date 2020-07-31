
grid = [[0, 0, 1, 0, 0, 0],
        [0, 0, 1, 0, 1, 0],
        [0, 0, 0, 0, 1, 0],
        [0, 0, 1, 1, 1, 0],
        [0, 0, 0, 0, 1, 0]]

init = [0, 0]
goal = [len(grid)-1, len(grid[0])-1]

delta = [[-1, 0], #go up
         [0, -1], #go left
         [1, 0], #go down
         [0, 1]] #go right

# delta_name = ['^', '<', 'v', '>']

# cost = 1

def searchOptimalPath(grid, init, goal):
        m = len(grid) # row number
        n = len(grid[0]) # column number
        visited = [[False for i in range(n)] for i in range(m)]
        reach = [[]]
        reach[0].append(init)            

        def helper(reach):
                reach.append([])
                for pos in reach[-2]:
                        visited[pos[0]][pos[1]] = True
                        if pos[0] == goal[0] and pos[1] == goal[1]:
                                return
                        for i in range(len(delta)):
                                if pos[0] + delta[i][0] >= 0 and pos[0] + delta[i][0] < m \
                                and pos[1] + delta[i][1] >= 0 and pos[1] + delta[i][1] < n:
                                
                                        newPos = [0] * 2
                                        newPos[0] = pos[0] + delta[i][0]
                                        newPos[1] = pos[1] + delta[i][1]
                                        
                                        if grid[newPos[0]][newPos[1]] == 0 and \
                                                not visited[newPos[0]][newPos[1]] \
                                                and newPos not in reach[-1]:
                                                reach[-1].append(newPos)
                if reach[-1] != []:
                        helper(reach)
                return     
        
        helper(reach)
        if visited[goal[0]][goal[1]]:
                return len(reach)-2
        else:
                return -1

a = searchOptimalPath(grid, init, goal)
print(a)