# Grupo 41, Daniel Goncalves 91004, Gabriel Almeida 89446
import random

# LearningAgent to implement
# no knowledeg about the environment can be used
# the code should work even with another environment
class LearningAgent:

        # init
        # nS maximum number of states
        # nA maximum number of action per state
        def __init__(self,nS,nA, learningRate = 0.9, discountRate = 0.05):

                # define this function
                self.nS = nS
                self.nA = nA
                self.initial = -nS
                # define this function

                self.qTable = list()
                self.qTableActions = list()
                self.frequencyTable = list()
                for i in range(0, nS):
                        self.qTable.append(list())
                        self.qTableActions.append(list())
                        self.frequencyTable.append(list())
                        for j in range(0, nA):
                                self.qTable[i].append(self.initial)
                                self.qTableActions[i].append(self.initial)
                                self.frequencyTable[i].append(0)

                self.learningRate = learningRate        
                self.learningDiscount = discountRate    

                self.epsilon = 1
                self.epsilonSTART = 1
                self.epsilonEND = 100000
                self.epsilonDECAY = self.epsilon / ( self.epsilonEND - self.epsilonSTART )
                self.episode = 0
              
        
        # Select one action, used when learning  
        # st - is the current state        
        # aa - is the set of possible actions
        # for a given state they are always given in the same order
        # returns
        # a - the index to the action in aa
        def selectactiontolearn(self,st,aa):
                numActions = len(aa)

                if ( self.qTableActions[st] != list(aa) ):
                        self.qTableActions[st] = list(aa)
                        self.qTable[st] = [self.initial] * numActions
                        self.frequencyTable[st] = [0] * numActions

                minfrequency = min(self.frequencyTable[st])
                minIndex = list()
                for i in range(0, numActions):
                        if self.frequencyTable[st][i] == minfrequency:
                                minIndex.append(i)

                actionToLearn = minIndex[round(random.random() * (len(minIndex) - 1))]
                self.frequencyTable[st][actionToLearn] += 1
                return actionToLearn


        # Select one action, used when evaluating
        # st - is the current state        
        # aa - is the set of possible actions
        # for a given state they are always given in the same order
        # returns
        # a - the index to the action in aa
        def selectactiontoexecute(self,st,aa):
                numActions = len(aa)

                a, maxA = 0, self.qTable[st][0]
                for j in range(1,numActions):
                        if self.qTable[st][j] > maxA: a = j
                
                return a


        # this function is called after every action
        # st - original state
        # nst - next state
        # a - the index to the action taken
        # r - reward obtained
        def learn(self,ost,nst,a,r):
                # define this function
                #print("learn something from this data")
                self.episode += 1


                nstIndex = a

                qValue = self.qTable[ost][nstIndex]
                maxNextValue = max( self.qTable[nst] )

                newQValue = (1- self.learningRate) * qValue + self.learningRate * ( r +  self.learningDiscount * maxNextValue )

                self.qTable[ost][nstIndex] = newQValue

                if self.epsilonEND >= self.episode >= self.epsilonSTART:
                        self.epsilon -= self.epsilonDECAY

        def qValue(self, ost):
                print(ost, self.qTableActions[ost], self.qTable[ost])
                pass
