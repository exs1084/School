d(X, X, 1).
d(C, X, 0) :- atomic(C), C \= X.
d(U + V, X, DU + DV) :- d(U, X, DU), d(V, X, DV).
d(U * V, X, U * DV + V * DU) :- d(U, X, DU), d(V, X, DV).

simp_sum(0, E2, E2).
simpsum(E1, 0, E1).
simpsum(E1, E2, E3):-
	E3 is E1 + E2.
simpsum(E1, E2, E3):-
	E1 + E2 = E3.

simpprod(0, E2, 0).
simpprod(E1, 0, 0).
simpprod(1, E2, E2).
simpprod(E1, 1, E1).
simpprod(E1, E2, E3):-
	E3 is E1 * E2.
simpprod(E1, E2, E3):-
	E1 * E2 = E3.
simp(E1, E2):-
	atomic(E1),
	E1 = E2.
simp(E1, E2):-
	H*T=E1,
	simp(H, S1),
	simp(T, S2),
	simpprod(S1, S2, E2).
simp(E1, E2):-
	H+T=E1,
	simp(H, S1),
	simp(T, S2),
	simpsum(S1, S2, E2).