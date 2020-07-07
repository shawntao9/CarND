#Modify the code below so that the function sense, which 
#takes p and Z as inputs, will output the NON-normalized 
#probability distribution, q, after multiplying the entries 
#in p by pHit or pMiss according to the color in the 
#corresponding cell in world.


p=[0.2, 0.2, 0.2, 0.2, 0.2]
# p = [0, 1, 0, 0, 0]
world=['green', 'red', 'red', 'green', 'green']
measurements = ['red', 'red']
motions = [1, 1]
pHit = 0.6
pMiss = 0.2
pExact = 0.8
pOvershoot = 0.1
pUndershoot = 0.1


def sense(p, Z):
    #
    #ADD YOUR CODE HERE
	#
    q = []
    for i in range(len(p)):
        if world[i] == Z:
            q.append(p[i] * pHit)
        else:
            q.append(p[i] * pMiss)
    sumq = sum(q)
    q = [i / sumq for i in q]
    return q

# for i in range(len(measurements)):
#     p = sense(p, measurements[i])
#     print(p)

def move(p, U):
    q = [0] * len(p)
    for i in range(len(p)):
        q[(i+U-1)%len(p)] += p[i] * pUndershoot
        q[(i+U)%len(p)] += p[i] * pExact
        q[(i+U+1)%len(p)] += p[i] * pOvershoot
    return q

for i in range(len(motions)):
    p = sense(p, measurements[i])
    p = move(p, motions[i])

print(p)
