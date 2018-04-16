judgeType(Gamma, N, int) :- number(N).

judgeType(Gamma, N, list(T)) :-
	N = [].
judgeType(Gamma, N, list(T)) :-
	[H|R]=N,
	judgeType(Gamma, H, T),
	judgeType(Gamma, R, list(T)).

judgeType(Gamma, X, T) :- atom(X), lookUp(X, Gamma, T).

judgeType(Gamma, if(M, N1, N2), T) :-
        judgeType(Gamma, M, bool),
        judgeType(Gamma, N1, T),
        judgeType(Gamma, N2, T).

judgeType(Gamma, nil(T), list(T)). /* unsure, but works */

judgeType(Gamma, cons(M, N), T) :-
	judgeType(Gamma, M, T1),
	judgeType(Gamma, N, list(T1)),
	T = list(T1).

judgeType(Gamma, isNil(M), bool):- /* unsure, but works */
	judgeType(Gamma, nil(M), T).

judgeType(Gamma, head(M), T) :-
	judgeType(Gamma, M, list(T)).

judgeType(Gamma, tail(M), T) :-
	T = list(T1),
	judgeType(Gamma, M, list(T1)).

judgeType(Gamma, let(X, M, N), T) :-
        judgeType(Gamma, M, T1),
        extend(Gamma, X, T1, Gamma2),
        judgeType(Gamma2, N, T).

judgeType(Gamma, letRec(Y, X, TX, TM, M, N), T) :-
        extend(Gamma, X, TX, Gamma2),
        extend(Gamma2, Y, arrow(TX, TM), Gamma3),
        judgeType(Gamma3, M, TM),
        extend(Gamma, Y, arrow(TX, TM), Gamma4),
        judgeType(Gamma4, N, T).

judgeType(Gamma, proc(X, T, M), arrow(T, TM)) :-
        extend(Gamma, X, T, Gamma2),
        judgeType(Gamma2, M, TM).

judgeType(Gamma, funcall(M, N), T) :-
        judgeType(Gamma, N, TN),
        judgeType(Gamma, M, arrow(TN, T)).



lookUp(X, [[X, Type] | Tail], Type) :- !.
lookUp(X, [[Y, T] | Tail], Type) :- 
    lookUp(X, Tail, Type).

extend(Gamma, X, T, [[X, T] | Gamma]).

initGamma([
    [eq, arrow(int, arrow(int, bool))],
    [prod, arrow(int, arrow(int, int))],
    [sum, arrow(int, arrow(int, int))],
    [diff, arrow(int, arrow(int, int))]
          ]).
