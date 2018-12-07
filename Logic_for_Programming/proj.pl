:- [exemplos_puzzles].

% propaga(Puz, Pos, Posicoes), afirma que Posicoes e a lista de posicoes que devem ser preenchidas
%desde a cabeca do termometro ate Pos, os termometros estao guardados no primeiro elemento de Puz
propaga([P | _], Pos, Pos_ord):- member(Pos, Posicoes), member(Posicoes, P), 
	enche(Posicoes, Pos, Posicoes_novo), sort(Posicoes_novo, Pos_ord).

% enche(Termometros, Pos, Res), afirma que Res e a lista desde a cabeca do termometro ate Pos
enche([P | _], P, [P]).
enche([P | R], Pos, Res):- P \= Pos, append([P], R2, Res), enche(R, Pos, R2).

% nao_altera_linhas_anteriores(Posicoes, L, Ja_Preenchidas), afirma que dada uma possibilidade para uma
% linha, Posicoes, todos os seus elementos pertencentes a linhas anteriores a L, estao contidas em Ja_Preenchidas
nao_altera_linhas_anteriores([],_,_).
nao_altera_linhas_anteriores([P | R], L, Ja_Preenchidas):- linha(P, X), X =:= L, 
	nao_altera_linhas_anteriores(R, L, Ja_Preenchidas); 
	linha(P, X), X \= L, X < L, member(P, Ja_Preenchidas),
	nao_altera_linhas_anteriores(R, L, Ja_Preenchidas);
	linha(P, X), X \= L, X > L,nao_altera_linhas_anteriores(R, L, Ja_Preenchidas). 

% linha(Pos, Res), afirma que Res e a linha a que Pos pertence
linha(Pos, X):- (X,_)=Pos.

% coluna(Pos, Res), afirma que Res e a coluna a que Pos pertence
coluna(Pos, Y):- (_, Y)=Pos.

% verifica_parcial(Puz, Ja_Preenchidas, Dim, Poss), afirma que dada uma possibilidade de preenchimento da linha,
% Poss, esta nao viola o numero maximo de elementos em cada coluna
verifica_parcial([_, _, P2 | _], Ja_Preenchidas, Dim, Poss):- union(Ja_Preenchidas, Poss, Total),
	verifica_coluna(P2, Dim, Total, 1).

% verifica_coluna(Max_el, Dim, Poss, Coluna), afirma que Poss nao ultrapassa o numero maximo de 
% elementos (Max_el) para a coluna dada, Coluna 
verifica_coluna([], _, _, _).
verifica_coluna([P2 | R2], Dim, Poss, Coluna):- Coluna =< Dim, Novo_Coluna is Coluna + 1,
	mesma_coluna(Poss, Coluna, Conjunto), length(Conjunto, N_el), N_el =< P2, 
	verifica_coluna(R2, Dim, Poss, Novo_Coluna).

% mesma_coluna(Poss, Coluna, Conjunto), afirma que Conjunto e uma lista de elementos de Poss, combina
% a mesma coluna que Coluna
mesma_coluna([], _, []).
mesma_coluna([P | R], X, Conjunto):- coluna(P, C), C =:= X, append([P], R1, Conjunto), 
	mesma_coluna(R, X, R1).
mesma_coluna([P | R], X, Conjunto):- coluna(P, C), C \= X, mesma_coluna(R, X, Conjunto).

% possibilidades_linha(Puz, Posicoes_linha, Total, Ja_Preenchidas, Possibilidades_L), afirma que
% Possibilidades_L e a lista com todas as possibilidades de uma linha (Posicoes_linha) de o puzzle (Puz)
possibilidades_linha(_, _, 0, _, [[]]):-!.
possibilidades_linha(Puz, Linhas, Total, Ja_Preenchidas, Possibilidades_L):-
	length(Linhas, Dim),
	findall(X, combina(Total, Linhas, X), Lista), !,
	propagar_lista(Puz, Lista, Conjunto, L), !,
	verificar_propaga(Conjunto, Ja_Preenchidas, Dim, Puz, Res, L), !,
	apaga_repetidos(Res, Res2, Total, L), !,
	contidos_linha(Ja_Preenchidas, L, Verifica), !, 
	length(Verifica, Tamanho), !,
	apagar_nao_contidos(Res2, Verifica, Res3, Tamanho), !,
	sort(Res3, Possibilidades_L).

% propagar_lista(Puz, Lista, Res, L), afirma que Res e a lista de listas que resulta de propagar todos os elementos
% de todas as listas de Lista
propagar_lista(_, [], [], _).
propagar_lista(Puz, [P | R], Lista, L) :- propaga_todos_el(Puz, P, Conjunto, L, []), 
	append([Conjunto], Resto, Lista), propagar_lista(Puz, R, Resto, L).

% propagar_todos_el(Puz, Lista, Res, L, Aux), afirma que Res e a lista que resulta de aplicar a funcao
% propaga a todos os elementos de Lista
propaga_todos_el(_, [], Aux, _, Aux).
propaga_todos_el(Puz, [P| R], Conjunto, L, Aux):- linha(P,L), propaga(Puz, P, Aux2),
	union(Aux, Aux2, Aux3),
	sort(Aux3, Aux4),
	propaga_todos_el(Puz, R, Conjunto, L, Aux4).

% verificar_propaga(Conjunto, Ja_Preenchidas, Dim, Puz, Res, L), afirma que Conjunto e uma possibilidade
% valida para preencher a linha L
verificar_propaga([], _, _, _, [],_).
verificar_propaga([P | R], Ja_Preenchidas, Dim, Puz, Res, L):-  
	nao_altera_linhas_anteriores(P, L, Ja_Preenchidas), verifica_parcial(Puz, Ja_Preenchidas, Dim, P),
	append([P], Resto, Res), !, verificar_propaga(R, Ja_Preenchidas, Dim, Puz, Resto, L);
	verificar_propaga(R, Ja_Preenchidas, Dim, Puz, Res, L).

% apaga_repetidos(Lista, Res, Total, L), afirma que Res, e o resultado de apagar os elementos 
% repetidos de Lista
apaga_repetidos([], [], _, _).
apaga_repetidos([P| R], Res, Total, L) :- apaga_repetidos_aux(P, 0, L, Aux), Aux =:= Total, 
	append([P], Res2, Res), apaga_repetidos(R, Res2, Total, L).
apaga_repetidos([P| R], Res, Total, L) :- apaga_repetidos_aux(P, 0, L, Aux), Aux \= Total,
	apaga_repetidos(R, Res, Total, L).

apaga_repetidos_aux([], Aux, _, Aux).
apaga_repetidos_aux([P | R], Total, L, Aux) :- linha(P, A), A =:= L, Novo_total is Total + 1,
	apaga_repetidos_aux(R,Novo_total, L, Aux). 
apaga_repetidos_aux([P | R], Total, L, Aux) :- 
	linha(P, A), A \= L, apaga_repetidos_aux(R, Total, L, Aux).

% combina(N, L, Lista), afirma que Lista e o resultado de uma combinacao de N elementos de L
combina(0, _, []).
combina(N, L, [E | R]) :- N > 0, append(_, [E | RdepoisE], L), NN is N-1, combina(NN, RdepoisE, R).

% contidos_linha(Lista, L, Res), afirma que Res e a lista resultante de todos os elementos com linha
% L de Lista
contidos_linha([], _, []).
contidos_linha([P | R], L, Res) :- linha(P, X), X =:= L, append([P], Nova_Res, Res), 
	contidos_linha(R, L, Nova_Res).
contidos_linha([P | R], L, Res) :- linha(P, X), X \= L, contidos_linha(R, L, Res).

% apagar_nao_contidos(Lista, Contidos, Res, Tamanho), afirma que Res e a lista resultante de apagar
% todas as listas de Lista que nao tenham os elementos contidos em Contidos 
apagar_nao_contidos(Res, _, Res, 0).
apagar_nao_contidos([], _, [], _).
apagar_nao_contidos([P | R], Contidos, Res, Tamanho) :- apagar_nao_contidos_aux(P, Contidos, Existe), 
	Existe =:= 1,
 	append([P], Nova_Res, Res), apagar_nao_contidos(R, Contidos, Nova_Res, Tamanho).
apagar_nao_contidos([P | R], Contidos, Res, Tamanho) :- apagar_nao_contidos_aux(P, Contidos, Existe),	
	Existe =:= 0,
	apagar_nao_contidos(R, Contidos, Res, Tamanho).

apagar_nao_contidos_aux(_, [], _).
apagar_nao_contidos_aux(List, [P | R], Existe) :- member(P, List), Existe = 1,
	apagar_nao_contidos_aux(List, R, Existe).
apagar_nao_contidos_aux(List, [P | R], Existe) :- not(member(P, List)), Existe = 0, 
	apagar_nao_contidos_aux(List, R, Existe).

% resolve(Puz, Solucao), afirma que Solucao e uma solucao para o puzzle Puz
resolve(Puz, Solucao) :- dimensao(Puz, Dim), Linha = 1, gera_linhas(Dim, Linha,1, Pos_Linha), 
	resolve(Puz, Dim, Linha, Pos_Linha, [], Solucao).
resolve(_, Dim, Linha, _, Ja_Preenchidas, Solucao) :- Linha > Dim, sort(Ja_Preenchidas, Solucao).
resolve(Puz, Dim, Linha, Pos_Linha, Ja_Preenchidas, Solucao) :- Linha =< Dim, 
	gera_total(Puz, Linha, Total),
	possibilidades_linha(Puz, Pos_Linha, Total, Ja_Preenchidas, Possibilidades_L), 
	length(Possibilidades_L, Len), Len > 0, combina(1, Possibilidades_L, S),
	primeiro_el(S, S2),
	union(S2, Ja_Preenchidas, Novo_Ja_Preenchidas), Nova_Linha is Linha+1, 
	gera_linhas(Dim, Nova_Linha, 1, Novo_Pos_Linha),
	resolve(Puz, Dim, Nova_Linha, Novo_Pos_Linha, Novo_Ja_Preenchidas, Solucao).

% dimensao(Lista, Dim), afirma que Dim e a dimensao de Lista
dimensao([_, P2 | _], Dim) :- length(P2, Dim).

% gera_linhas(Dim, Linha, Ac, Res), afirma que, Res e a lista com todos os elementos da linha Linha
% com dimensao Dim
gera_linhas(Dim, _, Ac, []):- Ac > Dim. 
gera_linhas(Dim, Linha, Ac, Res) :-  Ac =< Dim, Novo_Ac is Ac+1, append([(Linha,Ac)], Resto, Res),
	gera_linhas(Dim, Linha, Novo_Ac, Resto).

% gera_total(Lista, Linha, Res), afirma que Res e o elemento numero Linha da lista
gera_total([_, P2 | _], Linha, Res) :- nth1(Linha, P2, Res).

% primeiro_el(Lista, X), afirma que X e o primeiro elemento de Lista
primeiro_el([P | _], X) :- X = P.