import pickle
import random
# import matplotlib.pyplot as plt
from ruagomesfreiregame2sol import *

def runagent(A, T, R, I = 1, learningphase=True, nlearn = 1000, ntest = 100):

  J = 0
  if learningphase:
    n = nlearn
  else:
    n = ntest

  st = I
  for ii in range(1,n):
    aa = T[st][0]
    if learningphase:
      a = A.selectactiontolearn(st,aa)
    else:
      a = A.selectactiontoexecute(st,aa)
    try:
      if random.uniform(0, 1) < 0.1:
        nst = st
      else:
        nst = T[st][0][a]

    except:
      print(st,a)
    r = R[st]
    J += r
    #print(st,nst,a,r)

    if learningphase:
      A.learn(st,nst,a,r)
    else:
      #print(st,nst,a,r)
      pass

    st = nst

    if not ii%15:
      st = I
  return J/n


# due to the randomness in the learning process, we will run everythin NREP times
# the final grades is based on the average on all of them

NREP = 5
val = [0,0,0,0]
print("exemplo 1")
for nrep in range(0,NREP):
	A = LearningAgent(114,15)
	# your solution will be tested with other environments
	with open("mapasgraph2.pickle", "rb") as fp:   #Unpickling
	    AA = pickle.load(fp)

	T = AA[0]
	R = [-1]*114
	R[7] = 1
	R[1] = 0
	R[2] = 0
	R[3] = 0
	R[4] = 0
	# T contains the list of possible next states
	# T[14][0] - contains the possible next states of state 14

	print("# learning phase")
	# in this phase your agent will learn about the world
	# after these steps the agent will be tested
	runagent(A, T, R, I = 1, learningphase=True, nlearn = 500)
	print("# testing phase")
	# in this phase your agent will execute what it learned in the world
	# the total reward obtained needs to be the optimal
	Jn = runagent(A, T, R, I = 1, learningphase=False, ntest = 10)
	val[0] += Jn
	print("average reward",Jn)
	print("# 2nd learning phase")
	runagent(A, T, R, I = 1, learningphase=True, nlearn = 10000)
	print("# testing phase")
	Jn = runagent(A, T, R, I = 1, learningphase=False, ntest = 10)
	val[1] += Jn
	print("average reward",Jn)

print("exemplo 2")
for nrep in range(0,NREP):

	A = LearningAgent(114,15)

	T = AA[0]
	R = [-1]*114
	R[10] = 1
	# T contains the list of possible next states
	# T[14][0] - contains the possible next states of state 14


	print("# learning phase")
	# in this phase your agent will learn about the world
	# after these steps the agent will be tested
	runagent(A, T, R, I = 1, learningphase=True, nlearn = 1000)
	print("# testing phase")
	# in this phase your agent will execute what it learned in the world
	# the total reward obtained needs to be the optimal
	Jn = runagent(A, T, R, I = 1, learningphase=False, ntest = 10)
	val[2] += Jn
	print("average reward",Jn)
	print("# 2nd learning phase")
	runagent(A, T, R, I = 1, learningphase=True, nlearn = 10000)
	print("# testing phase")
	Jn = runagent(A, T, R, I = 1, learningphase=False, ntest = 10)
	val[3] += Jn
	print("average reward",Jn)


val = list([ii/NREP for ii in val])
print("values  :", val)
cor = [(val[0]) >= 0.3, (val[1]) >= 0.3, (val[2]) >= -0.85, (val[3]) >= -0.6]
# these values are not the optimal, they include some slack
print(cor)

grade = 0
for correct,mark in zip(cor,[3,7,3,7]):
	if correct:
		grade += mark
print("Grade in these tests (the final will also include hidden tests) : ", grade)
