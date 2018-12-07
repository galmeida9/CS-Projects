As diretorias existentes no arquivo são:
	1- CircuitSolver-Parsolver
		contém a solução do projeto, ou seja, procurar os caminhos da grid paralelamente
	
	2- CircuitSolver-SeqSolver
		contém a solução do Exercicio 1 do projeto dado pelo corpo docente

        3- lib
                livraria de apoio
		
	4- inputs
		contém os inputs de texto standard 

	5- results
		contém os outputs criados pelo script doTest.sh

=====================================================================================================

	Para compilar o projeto basta estar na raiz do arquivo e executar o comando $make, que por sua vez 
vai compilar o ParSolver e o SeqSolver.
	Para correr o ParSolver normalmente é necessário estar na diretoria do mesmo e executar o comando
$./CircuitSolver-ParSolver (argumentos).
	Para executar o doTest.sh existem algumas opções (todas executáveis na raiz):

			1- $make doTestAll
				vai correr todos os testes com 4 threads exceto os de 512.

			2- $make doTest
				vai correr um teste default, que resolve um circuito de tamanho 256x256 com 4 threads, usado nomeadamente para testes.

			3- ./doTest.sh (nthreads) (nome do ficheiro)
				vai correr um input repetidamente até o número de threads indicado, começando pelo sequencial.
            
            4- ./doTestAll.sh (nthreads)
                vai correr todos os testes exceto os de 512 repetidamente até o número de threads indicado.


=====================================================================================================

    Para correr os testes usamos o Sigma:
        OS: Debian 4.9.110-3+deb9u4
        CPU: Intel(R) Xeon(R) CPU E5-2620 0 @ 2.00GHz
        Max Turbo Frequency: 2.50 GHz

=====================================================================================================
    
    Fizemos também um script que indica o speedup de cada ficheiro com 4 threads (que corresponde à 6ª linha), chamado echoSpeedup.sh .

=====================================================================================================
