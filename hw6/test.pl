fruit(mango).
fruit(pineapple).
fruit(grape).
vegetable(carrot).				
vegetable(zucchini).
vegetable(kale).

likes(alice, X) :- fruit(X).
likes(patricia, X) :- vegetable(X).
likes(ruby, kale).
likes(julia, X) :- likes(ruby, X).
