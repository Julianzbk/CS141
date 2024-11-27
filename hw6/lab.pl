meal(pasta).
meal(burger).
meal(taco).
drink(water).
drink(lemonade).
dessert(cake).
dessert(brownie).
combo(X, Y) :- meal(X), drink(Y).
dinner(X, Y, Z) :- combo(X, Y), dessert(Z).

my_sum([], 0).
my_sum([Car|Cdr], S) :- my_sum(Cdr, A), S is A + Car.
