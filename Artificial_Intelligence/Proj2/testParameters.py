from ruagomesfreiregame2sol import *
import pickle

#
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
#

with open("mapasgraph2.pickle", "rb") as fp:   #Unpickling
    AA = pickle.load(fp)

def runTest(alpha, gama):
    # due to the randomness in the learning process, we will run everythin NREP times
    # the final grades is based on the average on all of them

    NREP = 5
    val = [0,0,0,0]
    for nrep in range(0,NREP):       
            A = LearningAgent(114,15, alpha, gama)

            T = AA[0]
            R = [-1]*114
            R[7] = 1
            R[1] = 0
            R[2] = 0
            R[3] = 0
            R[4] = 0
            
            runagent(A, T, R, I = 1, learningphase=True, nlearn = 500)
            
            Jn = runagent(A, T, R, I = 1, learningphase=False, ntest = 10)
            val[0] += Jn
            
            runagent(A, T, R, I = 1, learningphase=True, nlearn = 10000)
            
            Jn = runagent(A, T, R, I = 1, learningphase=False, ntest = 10)
            val[1] += Jn

    for nrep in range(0,NREP):
            
            A = LearningAgent(114,15, alpha, gama)

            T = AA[0]
            R = [-1]*114
            R[10] = 1
            
            runagent(A, T, R, I = 1, learningphase=True, nlearn = 1000)
            
            Jn = runagent(A, T, R, I = 1, learningphase=False, ntest = 10)
            val[2] += Jn
         
            runagent(A, T, R, I = 1, learningphase=True, nlearn = 10000)
            Jn = runagent(A, T, R, I = 1, learningphase=False, ntest = 10)
            val[3] += Jn

    val = list([ii/NREP for ii in val])
    cor = [(val[0]) >= 0.3, (val[1]) >= 0.3, (val[2]) >= -0.85, (val[3]) >= -0.6]

    grade = 0
    for correct,mark in zip(cor,[3,7,3,7]):
            if correct:
                    grade += mark
    
    return grade

def runTest2(alpha, gama):
    NREP = 1
    val = [0,0]
    for nrep in range(0, NREP): # Resultado esperado: 1.3
        A = LearningAgent(114,15, alpha, gama)

        T = AA[0]
        R = [-1]*114
        R[9] = 10
        
        runagent(A, T, R, I = 1, learningphase=True, nlearn = 500)
        
        Jn = runagent(A, T, R, I = 1, learningphase=False, ntest = 10)
        val[0] += Jn/NREP
        
        runagent(A, T, R, I = 1, learningphase=True, nlearn = 10000)
        
        Jn = runagent(A, T, R, I = 1, learningphase=False, ntest = 10)
        val[1] += Jn/NREP
    return val


def writeToFile(file, data):
    f = open(file, "w")
    f.write( "Alpha; Gama; Grade\n" )
    for el in data:
        string = str(el[0]).replace(".", ",") + ";" + str(el[1]).replace(".", ",") + ";" + str(el[2]).replace(".", ",") + "\n"
        f.write( string )
    f.close()

def test(minAlpha, maxAlpha, stepAlpha, minGama, maxGama, stepGama, runs, scale):
    results = []
    alpha = minAlpha
    while (alpha <= maxAlpha):
        gama = minGama
        while (gama <= maxGama):
            res = 0
            for run in range(0, runs):
                res += runTest(alpha/scale, gama/scale)

            print("alpha: ", alpha/scale, "\tgama: ", gama/scale, "\tgrade: ", res/runs)
            results.append([alpha/scale, gama/scale, res/runs])
            gama += stepGama
            
        alpha += stepAlpha
        print()
    return results

# print(runTest2(1,0.005))   
res = test(500, 600, 5, 900, 1000, 5, 5, 1000)
writeToFile("data/res.csv", res)


