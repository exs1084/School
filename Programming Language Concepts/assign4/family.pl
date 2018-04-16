male(tony).
male(ethan).
male(chris).
male(ben).
male(nathan).

female(sim).
female(helen).
female(sophie).
female(olivia).

parents(helen, sim, tony).
parents(chris, sim, tony).
parents(ben, sim, tony).
parents(sophie, helen, ethan).
parents(olivia, helen, ethan).
parents(nathan, helen, ethan).

sisterof(X, Y) :-
	female(X),
	parents(X, M, D),
	parents(Y, M, D),
	not(X = Y).

second(L, E) :-
	[H | T] = L,
	[E | F] = T.

one(L) :-
	not(second(L, N)).

index(E, L, 0) :-
	[E | T] = L.
index(E, L, N) :-
	[H | T] = L,
	index(E, T, N1),
	N is N1 + 1.

insertionsort([], S) :-
	[] = S.
insertionsort(L, S) :-
	[H | T] = L,
	/* write(T), */
	insertionsort(T, T2),
	/* write(T2), */
	insert(H, T2, S).

insert(E, L, S) :-
	[H | T] = L,
	H > E,
	[E | L] = S.
insert(E, L, S) :-
	[H | T] = L,
	H = E,
	[E | L] = S.
insert(E, L, S) :-
	[H | T] = L,
	H < E,
	[H | L2] = S,
	insert(E, T, L2).
insert(E, [], S) :-
	[E] = S.