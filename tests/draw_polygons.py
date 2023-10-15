import matplotlib.pyplot as plt

coord = [[16, -3], [28, 16], [16, 32], [0, 22], [3, 10]]
triangle = [[5, 5], [10, 10], [0, 5]]


ans_coord = [
[5.33333, 7.66667], 
[6.5, 6.5], 
[10, 10]]

coord.append(coord[0]) #repeat the first point to create a 'closed loop'
triangle.append(triangle[0])
ans_coord.append(ans_coord[0])

xs, ys = zip(*coord) #create lists of x and y values
ansX, ansY = zip(*ans_coord)
trX, trY = zip(*triangle)

plt.figure()

plt.plot(xs, ys, color='b') 
plt.plot(trX, trY, color='y') 
plt.plot(ansX, ansY, color='r') 

plt.show() 