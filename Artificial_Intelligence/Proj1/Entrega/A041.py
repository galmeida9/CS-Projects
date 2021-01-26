# Grupo 41, Daniel Gonçalves 91004, Gabriel Almeida 89446

import math
import pickle
import time
import itertools
import heapq

  
class SearchProblem:

	def __init__(self, goal, model, auxheur = []):
		self._goal = goal
		self._map = model
		self._coord = auxheur
		self._agents = len(goal) # Number of agents
		self._costs = dict()

		# calculate heuristic
		for goal in self._goal:
			self.calculateH(goal)

	def search(self, init, limitexp = 2000, limitdepth = 10, tickets = [math.inf,math.inf,math.inf], anyorder = False):

		# create queue
		queue = Heap(lambda x: max(x[3]))

		# push the first path into the queue
		if anyorder:
			for comb in list(itertools.permutations(self._goal, 3)):
				first = [[[[],init]], tickets, 0, [0], comb]
				queue.push(first)
		else:
			first = [[[[],init]], tickets, 0, [0], self._goal]
			queue.push(first)

		exp=0
		while not queue.isEmpty():

			# get the first path from the queue
			pathWT = queue.pop()
			path = pathWT[0]
	
			# get the last node from the path
			node = path[-1]

			#Check expansion and depth
			depth = pathWT[2]
			if depth > limitdepth: continue
			if (exp > limitexp): return path
			exp+=1
			
			# Solution found
			goal = list(pathWT[4])
			if node[1] == goal: return path
			
			# enumerate all adjacent nodes
			nextPoints = list()
			for i in range(0, self._agents):
				nodeList = list()
				for adjacent in self._map[node[1][i]]:
					changedtickets = list(pathWT[1])
					if changedtickets[adjacent[0]] > 0:
						nodeList.append(adjacent)
				nextPoints += [nodeList]
			
			# construct path for each move
			for comb in list(itertools.product(*nextPoints)):
				# check if next move does not intersect
				if (checkIfHasDup(comb)): continue

				newPath = list(path)
				newPath.append([[],[]])
				changedtickets = list(pathWT[1]).copy()
				heuristics = []

				# check if has the tickets to make this move
				canAdd = True
				for i in range(0, self._agents):
					currComb = comb[i]
					newPath[len(path)][0] += [currComb[0]]
					newPath[len(path)][1] += [currComb[1]]
					if changedtickets[currComb[0]] <= 0:
						canAdd = False
						break
					changedtickets[currComb[0]] -= 1
					heuristics += [ (depth + 1) + self.heuristic(goal[i], currComb[1])]
				if (depth > limitdepth or not canAdd): continue
				
				# add to queue next move
				newPath = [newPath] + [changedtickets, depth + 1, heuristics, goal]
				queue.push(newPath)

		return []

	def heuristic(self, goal, node):
		if node not in self._costs[goal]: return math.inf
		return self._costs[goal][node]


	def calculateH(self, goal):
		# construct dictionary with costs
		self._costs[goal] = dict()
		self._costs[goal][goal] = 0

		# create queue and visited set
		queue = Heap(lambda x: max(x[2]))
		queue.push([goal, 0, [0]])
		visited = set()

		while not queue.isEmpty():
			# get node
			nodeWT = queue.pop()
			node = nodeWT[0]
			depth = nodeWT[1]

			# mark as visited
			visited.add(node)

			for el in self._map[node]:
				depthNew = depth+1
		
				# update cost of node
				oldGoal = -1
				if el[1] in self._costs[goal]: oldGoal = self._costs[goal][el[1]]
				if el[1] not in self._costs[goal] or self._costs[goal][el[1]] > depthNew: self._costs[goal][el[1]] = depthNew

				# add new node if not visited or if cost updated
				newNode = [el[1], depthNew, [depthNew]]
				if ( el[1] not in visited or oldGoal > depthNew): queue.push(newNode)

class Heap(object):
	def __init__(self, key ):
		self._len = 0
		self.key = key
		self._data = []

	def push(self, item):
		self._len += 1
		heapq.heappush(self._data, (self.key(item), item))

	def pop(self):
		self._len -= 1
		return heapq.heappop(self._data)[1]
	
	def isEmpty(self):
		return True if self._len == 0 else False

def checkIfHasDup(lst):
	seen = set()
	for el in lst:
		el2 = tuple(el)[1]
		if el2 in seen:
			return True
		seen.add(el2)
	return False