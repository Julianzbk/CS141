eval(A ^ B, V) :- atom(A), atom(B), V is A ^ B.
eval(A * B, V) :- atom(A), atom(B), V is A * B.
eval(A / B, V) :- atom(A), atom(B), V is A * B.
eval(A + B, V) :- atom(A), atom(B), V is A + B.
eval(A - B, V) :- atom(A), atom(B), V is A - B.
eval(A ^ B, V) :- eval(A, VA), eval(B, VB), V is VA ^ VB.
eval(A * B, V) :- eval(A, VA), eval(B, VB), V is VA * VB.
eval(A / B, V) :- eval(A, VA), eval(B, VB), V is VA / VB.
eval(A + B, V) :- eval(A, VA), eval(B, VB), V is VA + VB.
eval(A - B, V) :- eval(A, VA), eval(B, VB), V is VA - VB.
eval(V, V).

prop(x - x, 0).
prop(x / x, 1).
prop(x + 0, x).
prop(0 + x, x).
prop(x - 0, x).
prop(0 - x, x).
prop(x * 1, x).
prop(1 * x, x).
prop(x ^ 0, 1).
prop(x ^ 1, x).
prop(N1 + N2, M) :- number(N1), number(N2), M is N1 + N2.
prop(N1 - N2, M) :- number(N1), number(N2), M is N1 - N2.
prop(N1 * N2, M) :- number(N1), number(N2), M is N1 * N2.
prop(N1 / N2, M) :- number(N1), number(N2), M is N1 / N2.
prop(N1*x + N2*x, M*x) :- number(N1), number(N2), M is N1 + N2.
prop(N1*x^E + N2*x^E, M*x^E) :- number(N1), number(N2), M is N1 + N2.
prop(N1*x - N2*x, M*x) :- number(N1), number(N2), M is N1 - N2.
prop(N1*x^E - N2*x^E, M*x^E) :- number(N1), number(N2), M is N1 - N2.
prop(N * x^E / x^E, N) :- number(N).
prop(N * x * x, N * x^2) :- number(N).
prop(N * x * x * x, N * x^3) :- number(N).
prop(E, E).

simplify(x, x).
simplify(N, N) :- number(N).
simplify(x * N, N * x) :- number(N).
simplify(A * B, S) :- atomic(A), atomic(B), A \== x, B \== x, S is A * B.
simplify(A * B, Y) :- simplify(A, D1), simplify(B, D2), prop(D1 * D2, Y).
simplify(A * B, D1 * D2) :- simplify(A, D1), simplify(B, D2).
simplify(A * B, A * B) :- A == x; B == x.
simplify(A / B, S) :- atomic(A), atomic(B), A \== x, B \== x, S is A / B.
simplify(A / B, Y) :- simplify(A, D1), simplify(B, D2), prop(D1 / D2, Y).
simplify(A / B, D1 / D2) :- simplify(A, D1), simplify(B, D2).
simplify(A / B, A / B) :- A == x; B == x.
simplify(A + B, S) :- atomic(A), atomic(B), A \== x, B \== x, S is A + B.
simplify(A + B, Y) :- simplify(A, D1), simplify(B, D2), prop(D1 + D2, Y).
simplify(A + B, D1 + D2) :- simplify(A, D1), simplify(B, D2).
simplify(A + B, A + B) :- A == x; B == x.
simplify(A - B, S) :- atomic(A), atomic(B), A \== x, B \== x, S is A - B.
simplify(A - B, Y) :- simplify(A, D1), simplify(B, D2), prop(D1 - D2, Y).
simplify(A - B, D1 - D2) :- simplify(A, D1), simplify(B, D2).
simplify(A - B, A - B) :- A == x; B == x.
simplify(A ^ B, S) :- atomic(A), atomic(B), A \== x, B \== x, S is A ^ B.
simplify(A ^ B, Y) :- simplify(A, D1), simplify(B, D2), prop(D1 ^ D2, Y).
simplify(A ^ B, D1 ^ D2) :- simplify(A, D1), simplify(B, D2).
simplify(A ^ B, A ^ B) :- A == x; B == x.

can(C, C) :- number(C).
can(x, x).
can(x^E, x^E) :- number(E).
can(C * x^E, C * x^E) :- number(C), number(E).
can(A + 0, A).
can(0 + A, A).
can(A - 0, A).
can(_ * 0, 0).
can(0 / _, 0).
can(C * x * x, C * x^2) :- atomic(C).
can(C * x * x * x, C * x^3) :- atomic(C).
can(x * C * x, C * x^2) :- number(C).
can(N * x^2 / x, N * x) :- number(N).
can(N * x^E / x, N * x^E1) :- number(N), E1 is E - 1.
can(E / x, Y) :- not(number(E)), can(E, E1), can(E1 / x, Y).
can(E, E).

neg(-[T], T).

handle_plus(D1, D2, E) :- neg(D2, D3), can(D1 - D3, E).
handle_plus(D1, D2, E) :- can(D1 + D2, E).

handle_minus(D1, D2, E) :- neg(D2, D3), can(D1 + D3, E).
handle_minus(D1, D2, E) :- can(D1 - D2, E).

deriv(C * x^2, C1 * x) :- C1 is C * 2.
deriv(x^2, C1 * x) :- C1 is 2.
deriv(C * x, C) :- number(C).
deriv(x, 1).
deriv(C, 0) :- number(C).
deriv(C * x^E, C1 * x^E1) :- number(C), C1 is C * E, E1 is E - 1.
deriv(x^E, C1 * x ^ E1) :- C1 is E, E1 is E - 1.
deriv(C / x^E, -[C2 / x ^ E1]) :- number(C), C1 is -1 * C * E, C1 < 0, C2 is -C1, E1 is E + 1.
deriv(C / x^E, C1 / x ^ E1) :- number(C), C1 is -1 * C * E, E1 is E + 1.
deriv(C / x, -[C / x ^ 2]) :- number(C), C1 is -1 * C, C1 < 0.
deriv(C / x, C1 / x ^ 2) :- number(C), C1 is -1 * C.
deriv(A + B, E) :- can(A, A1), can(B, B1), deriv(A1, D1), deriv(B1, D2), handle_plus(D1, D2, E).
deriv(A - B, E) :- can(A, A1), can(B, B1), deriv(A1, D1), deriv(B1, D2), handle_minus(D1, D2, E).
deriv(E, D) :- can(E, E1), deriv(E1, D).

guest(G) :- male(G); female(G).
party_seating([G1|[G2|T]]) :- guest(G1), guest(G2), not(member(G1, T)), not(member(G2, T)), same_language(G1, G2), no_females(G1, G2), party_seating(T).

same_language(G1, G2) :- speaks(G1, L), speaks(G2, L).
not_both_female(G1, G2) :- male(G1), female(G2).
not_both_female(G1, G2) :- male(G2), female(G1). 
not_both_female(G1, G2) :- male(G2), male(G1). 
