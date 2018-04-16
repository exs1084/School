quicksort([], []).
quicksort([H | T], S) :-
   partition(T, H, Less, Same, Greater),
   quicksort(Less, SLess),
   quicksort(Greater, SGreater),
   append(SLess, [H | Same], SGreater, S).

append([], L, L).
append([H | T], L, [H | A]) :- append(T, L, A).

append(L1, L2, L3, A) :- append(L2, L3, L23), append(L1, L23, A).

partition([], H, [], [], []).
partition(T, H, Less, Same, Greater) :-
	[H1 | T1] = T,
	H1 > H,
	[H1 | G1] = Greater,
	partition(T1, H, Less, Same, G1).
partition(T, H, Less, Same, Greater) :-
	[H1 | T1] = T,
	H1 = H,
	[H1 | S1] = Same,
	partition(T1, H, Less,S1, Greater).
partition(T, H, Less, Same, Greater) :-
	[H1 | T1] = T,
	H1 < H,
	[H1 | L1] = Less,
	partition(T1, H, L1, Same, Greater).

merge([], O2, M) :-
	O2 = M.
merge(O1, [], M) :-
	O1 = M.
merge(O1, O2, M) :-
	[H1 | T1] = O1,
	[H2 | T2] = O2,
	H1 >= H2,
	[H2 | M1] = M,
	merge(O1, T2, M1).
merge(O1, O2, M) :-
	[H1 | T1] = O1,
	[H2 | T2] = O2,
	H1 < H2,
	[H1 | M1] = M,
	merge(T1, O2, M1).

mhelp([], [], [], Boo).
mhelp(L, S1, S2, 0) :-
	[H|T]=L,
	[H|R1]=S1,
	mhelp(T, R1, S2, 1).
mhelp(L, S1, S2, 1) :-
	[H|T]=L,
	[H|R2]=S2,
	mhelp(T, S1, R2, 0).
merge_sort([],[]).
merge_sort(L, S) :-
	[H1|T]=L,
	[]=T,
	[H1]=S.
merge_sort(L, S) :-
	[H1|T]=L,
	[H2]=T,
	merge([H1],[H2],S).
merge_sort(L, S) :-
	mhelp(L, L1, L2, 0),
	merge_sort(L1, S1),
	merge_sort(L2, S2),
	merge(S1, S2, S).