my_length([], 0).
my_length([_|Cdr], R1) :- my_length(Cdr, R2), R1 is R2 + 1.

my_member(H, [H|_]).
my_member(M, [_|T]) :- my_member(M, T).

my_append(A, [], A).
my_append(L1, [Car2|Cdr2], [Car2|R]) :- my_append(L1, Cdr2, R), L1 == [].
my_append([Car1|Cdr1], L2, [Car1|R]) :- my_append(Cdr1, L2, R).

my_reverse(L, R) :- rev(L, [], R).
rev([], Acc, Acc).
rev([H|T], Acc, R) :- rev(T, [H|Acc], R).

my_nth(L, 1, L).
my_nth([], _, []).
my_nth([_|T], N, R) :- N1 is N-1, my_nth(T, N1, R).

my_remove(_, [], []).
my_remove(X, [X|T], R) :- my_remove(X, T, R).
my_remove(X, [H|T], [H|R]) :- my_remove(X, T, R).

my_subst(_, _, [], []).
my_subst(X, Y, [X|T], [Y|R]) :- my_subst(X, Y, T, R).
my_subst(X, Y, [H|T], [H|R]) :- my_subst(X, Y, T, R).

my_subset(_, [], []).
my_subset(P, [H|T], [H|R]) :- E =.. [P, H], call(E), my_subset(P, T, R).
my_subset(P, [_|T], R) :- my_subset(P, T, R).

my_add(N1, N2, R) :- add(N1, N2, 0, R).
add([], [], 1, [1]).
add([], [], _, []).
add([H1|T1], [H2|T2], PrevCarry, [Digit|R]) :- Sum is H1 + H2 + PrevCarry, Digit is Sum mod 10, Carry is Sum div 10,
                                                add(T1, T2, Carry, R).
add([], [H|T], PrevCarry, [Digit|R]) :- Sum is H + PrevCarry, Digit is Sum mod 10, Carry is Sum div 10,
                                                add([], T, Carry, R).
add([H|T], [], PrevCarry, [Digit|R]) :- Sum is H + PrevCarry, Digit is Sum mod 10, Carry is Sum div 10,
                                                add(T, [], Carry, R).


my_merge([], L2, L2).
my_merge(L1, [], L1).
my_merge([H1|T1], [H2|T2], [H1|R]) :- H1 =< H2, my_merge(T1, [H2|T2], R).
my_merge([H1|T1], [H2|T2], [H2|R]) :- H1 > H2, my_merge([H1|T1], T2, R).

my_sublist([H1|T1], [H1|T2]) :- check(T1, T2).
my_sublist(L1, [H2|T2]) :- my_sublist(L1, T2).
check([], _).
check([H1|T1], [H1|T2]) :- check(T1, T2).

my_assoc(A, [A|[V|T]], V).
my_assoc(A, [K|[V|T]], R) :- my_assoc(A, T, R).

my_replace(_, [], []).
my_replace(ALIST, [H|T], [V|R]) :- my_assoc(H, ALIST, V), my_replace(ALIST, T, R).
my_replace(ALIST, [H|T], [H|R]) :- my_replace(ALIST, T, R).