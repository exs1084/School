grandchild(X,Y) :-
    child(X,Z),
    child(Z,Y).
greatgrandparent(X, Y):-
    grandchild(Z,X),
    child(Y,Z).
ancestor(X,Y):-
    child(Y,X).
ancestor(X,Y):-
    grandchild(Y,X).
ancestor(X,Y):-
    greatgrandparent(X,Y).
brother(X,Y) :-
    son(X,Z),
    child(Y,Z),
    X\==Y.
sister(X,Y) :-
    daughter(X,Z),
    child(Y,Z),
    X\==Y.
daughter(X, Y):-
    child(X,Y),
    female(X).
son(X,Y):-
    child(X,Y),
    male(X).
cousin(X,Y):-
    child(Y,Z),
    child(X,Q),
    child(Z,W),
    child(Q,W).
brotherInLaw(X,Y):-
    brother(X,Z),
    spouse(Y,Z).
brotherInLaw(X,Y):-
    brother(Y,Z),
    spouse(X,Z).
sisterInLaw(X,Y):-
    sister(X,Z),
    spouse(Y,Z).
sisterInLaw(X,Y):-
    sister(Y,Z),
    spouse(X,Z).
aunt(X,Y):-
    child(Y,Z),
    sister(X,Z).
uncle(X,Y):-
    child(Y,Z),
    brother(X,Z).

male(william).
male(harry).
male(peter).
male(james).
male(charles).
male(mark).
male(andrew).
male(edward).
male(spencer).
male(philip).
male(george).

female(zara).
female(beatrice).
female(eugenie).
female(louise).
female(diana).
female(anne).
female(sarah).
female(sophie).
female(kydd).
female(elizabeth).
female(margaret).
female(mum).

spouse(diana, charles).
spouse(anne, mark).
spouse(andrew, sarah).
spouse(edward, sophie).
spouse(spencer, kydd).
spouse(elizabeth, philip).
spouse(george, mum).

spouse(charles, diana).
spouse(mark, anne).
spouse(sarah, andrew).
spouse(sophie, edward).
spouse(kydd, spencer).
spouse(philip, elizabeth).
spouse(mum, george).


child(william, diana).
child(william, charles).
child(harry, diana).
child(harry, charles).
child(peter, anne).
child(peter, mark).
child(zara, anne).
child(zara, mark).
child(beatrice, andrew).
child(beatrice, sarah).
child(eugenie, andrew).
child(eugenie, sarah).
child(louise, edward).
child(louise, sophie).
child(james, edward).
child(james, sophie).
child(diana, spencer).
child(diana, kydd).
child(charles, elizabeth).
child(charles, philip).
child(anne, elizabeth).
child(anne, philip).
child(andrew, elizabeth).
child(andrew, philip).
child(edward, elizabeth).
child(edward, philip).
child(elizabeth, george).
child(elizabeth, mum).
child(margaret, george).
child(margaret, mum).





