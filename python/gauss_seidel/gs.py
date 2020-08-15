#!/usr/bin/python3
#-*- coding:utf-8 -*-
class Gauss_Seidel():
	def calculation(self):
		coefficient_matrix = [[3,2,1],[1,4,1],[2,2,5]]
		y = [10,12,21]
		convergence_value = 10 ** -16
		length = len(y)
		x = [0] * length
		x_before = [0] * length
		counter = 1;

		while True:
			for i in range(length):
				x_before[i] = x[i]
				x[i] = (
					y[i]
					- coefficient_matrix[i][(i+1)%length]*x[(i+1)%length]
					- coefficient_matrix[i][(i+2)%length]*x[(i+2)%length]
					) / coefficient_matrix[i][i]

			judge_counter = 0;
			for i in range(length):
				if abs(x_before[i] - x[i]) < convergence_value:
					judge_counter = judge_counter + 1
			if length == judge_counter:
				break
				
			print("------" + str(counter) + "cnt" + "------")
			counter = counter + 1

			for i in range(length):
				print(x[i])

if __name__ == "__main__":
	instance = Gauss_Seidel()
	instance.calculation()
