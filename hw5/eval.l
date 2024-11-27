;; A simple LISP interpreter written by Dr Klefstad for ICS 141 at UC Irvine.
;; Of course, I deleted lots of it to let you learn more about evaluation.
;; Note the new top-level function, my-top, is defined at the bottom of this file.

;; To run this, load this file into the lisp interpreter, then call
;; (my-top)
;; then type in expressions to be evaluated.

;; The homework assignment gives a list of good test cases
;; listed in order that you should probably implement thim in to ensure you
;; enough working to go on to the next test case.

;; We will use association lists, alists, for the stack of variables and
;; their values.  An alist is a list of this form:
;; ((var1 . val1) (var2 . val2) ... (varN . valN))
;; where each vari is a symbol representing a variable (or parameter) name
;; and each vali is the value of the variable.  assoc returns the association
;; of a given symbol, e.g,
;; (assoc 'myvar '((a . 10)(b a b c)(myvar d e f)))
;; returns (myvar d e f) and you take the cdr of that to get myvar's value
;; (d e f)

;; Assoc returns the association (binding) of a variable in the association
;; list. An association list may contain multiple definitions of a variable
;; with the same name, for example with parameters to a recursive function.
;; Assoc always finds the first association of a variable, and this is how we
;; implement dynamic scoping. 

;; As evaluation proceeds deeper into recursion, new variables are added onto
;; the front of the current association list.  New defintions of a variable 
;; will hide previously made definitions effectively hiding them from access.
;; The previously made definitions will come back into scope when
;; recursive evaluation unwinds.

;; We us a special global variable, called global-alist, for saving top-level
;; definitions made using defun or setq. Note the global-alist is passed in
;; to my-eval only in the call made by my-top defined below.


;; You need to write this one.

(defun my-assoc (v alist)
    (cond
        ((null ALIST) NIL)
        ((equal (car (car ALIST)) V) (car ALIST))
        (T (my-assoc V (cdr ALIST)))
    )
)

(defun assoc-get (v alist)
    (cdr (my-assoc v alist))
)

(defun except (ERR)
    (print "Error thrown: ")
    (print ERR)
)

(defun printem (&rest args)
    (format t "~{~a~^ ~}" args)
    (terpri)
)
;; This one is done

(defun my-eval (e alist)
    (cond ((atom e) (my-eval-atom e alist))
          (t (my-apply (car e) (cdr e) alist))
    )
)

;; You need to write this one.

(defun my-eval-atom (E ALIST)
;; how do you evaluate an atom???
;; Remember there are special cases: T, NIL, MY-SYMBOL, 10, "Hello"
    (cond
        ((eq E NIL) NIL)
        ((eq E T) T)
        ((numberp E) E)
        ((stringp E) E)
        ((not (null (my-assoc E ALIST))) (cdr (my-assoc E ALIST)))
        (T (except "variable not found")) ; just set to nil
    )
)

;; This one is done, but you must write the functions it calls

(defun my-apply (fn args alist)
    (cond 
        ((atom fn) (my-apply-atom fn args alist))
        (t (my-apply-lambda fn args alist))
    )
)

;; You need to write this one.
;; Utility function for eval-cond and apply-lambda.  Evaluates each expression
;; in l and returns the value of the last expression

(defun my-eval-list (l alist)
    (cond
        ((null (cdr l)) (my-eval (car l) alist))
        (T  (my-eval (car l) alist)
            (my-eval-list (cdr l) alist))
    )
)

;; You need to write this one.


(defun my-apply-lambda (fn args alist)
;; bind the formals to the evaluated actuals then evaluate the body in that
;; new scoping context (i.e., that becomes the new alist for recursive
;; evaluation of the function body.  Return the value of the last 
;; expression in the body (using eval-list)).
    (my-eval-list (cddr fn) (my-bind-formals (cadr fn) args alist))
)

;; You need to write this one.

(defun my-bind-formals (formals actuals alist)
;; This takes a list of formals and unevaluated actuals.  It should evaluate
;; each actual and bind it to its corresponding formal placing them all on
;; the front of the alist.  It should return the alist with the new bindings
;; on the front.  This will be used to evaluate calls to functions defined
;; via defun.
;; e.g., (my-bind-formals '(a) '((add 1 b)) '((b . 10)))
;; will return ((a . 11) (b . 10))
;; Note there will be one actual parameter for each formal parameter.
    (cond
        ((null formals) alist)
        (T (cons (cons (car formals) (my-eval (car actuals) alist))
                 (my-bind-formals (cdr formals) (cdr actuals) alist))
        )
    )
)

;; You need to write this one.  Handle the primitives as special cases, then
;; handle user defined functions (defined via defun) in the default case.
;; Handle car, cdr, cons, eq, quote, cond, defun, setq, eval, print, atom, null,
;; listp, apply, equal, +, -, mod, floor and user defined functions (defined via defun).
;; This should allow you to interpret your functions from HW4.

(defun my-apply-atom (fn args alist) ;; ARGS is a list, take the car or cadr!
    (cond 
        ((eq fn 'eq)
            (eq (my-eval (car args) alist) (my-eval (cadr args) alist)))
        ;; I wrote the first one, eq, for you, you write the rest
        ((eq fn 'equal)
            (equal (my-eval (car args) alist) (my-eval (cadr args) alist)))
        ((eq fn 'car)
            (car (my-eval (car args) alist))
        )
        ((eq fn 'cdr)
            (cdr (my-eval (car args) alist))
        )
        ((eq fn 'cons)
            (cons (my-eval (car args) alist) (my-eval (cadr args) alist))
        )
        ((eq fn 'quote)
            (car args)
        )
        ((eq fn 'setq)
            (my-eval-setq (car args) (my-eval (cadr args) alist))
        )
        ((eq fn 'cond)
            (my-eval-cond args alist)
        )
        ((eq fn 'defun)
            (my-eval-defun args alist)
        )
        ((eq fn 'eval)
            (my-eval (my-eval (car args) alist) alist)
        )
        ((eq fn 'print)
            (print (my-eval (car args) alist))
        )
        ((eq fn 'atom)
            (atom (my-eval (car args) alist))
        )
        ((eq fn 'null)
            (null (my-eval (car args) alist))
        )
        ((eq fn 'listp)
            (listp (my-eval (car args) alist))
        )
        ((eq fn 'symbolp)
            (symbolp (my-eval (car args) alist))
        )
        ((eq fn 'stringp)
            (stringp (my-eval (car args) alist))
        )
        ((eq fn 'numberp)
            (numberp (my-eval (car args) alist))
        )
        ((eq fn 'and)
            (and (my-eval (car args) alist) (my-eval (cadr args) alist))
        )
        ((eq fn 'or)
            (and (my-eval (car args) alist) (my-eval (cadr args) alist))
        )
        ((eq fn 'not)
            (and (my-eval (car args) alist) (my-eval (cadr args) alist))
        )
        ((eq fn 'apply)
            (apply (my-eval (car args) alist) (my-eval (cadr args) alist))
        )
        ((eq fn '+)
            (+ (my-eval (car args) alist) (my-eval (cadr args) alist))
        )
        ((eq fn '-)
            (- (my-eval (car args) alist) (my-eval (cadr args) alist))
        )
        ((eq fn '*)
            (* (my-eval (car args) alist) (my-eval (cadr args) alist))
        )
        ((eq fn '/)
            (/ (my-eval (car args) alist) (my-eval (cadr args) alist))
        )
        ((eq fn '<)
            (< (my-eval (car args) alist) (my-eval (cadr args) alist))
        )
        ((eq fn '>)
            (> (my-eval (car args) alist) (my-eval (cadr args) alist))
        )
        ((eq fn '1+)
            (1+ (my-eval (car args) alist))
        )
        ((eq fn '1-)
            (1- (my-eval (car args) alist))
        )
        ((eq fn 'mod)
            (mod (my-eval (car args) alist) (my-eval (cadr args) alist))
        )
        ((eq fn 'floor)
            (floor (my-eval (car args) alist) (my-eval (cadr args) alist)) ;; only returns first value
        )
        (T
            (my-apply (my-assoc fn alist) args alist)
        );; get the lambda from the alist
    )
)


;; setq and defun will push a new association on the global-alist.
;; whenever we apply a function, we will bind the formals to the evaluated
;; actuals pushing these new bindings onto the local alist and then
;; evaluate the body of the function in that new scoping context.

;; You need to write this one.

(defun my-eval-setq (var val)
;; just push a new association of the var and its evaluated val onto the
;; global alist
    (setq GLOBAL-ALIST (cons (cons VAR VAL) GLOBAL-ALIST)) ;; TODO return cdr of the cons cell instead of the glob alist
    (assoc-get var GLOBAL-ALIST)
)

;; You need to write this one.  You should know how cond works at this point.
;; Remember, cond clauses have one or more expressions in each clause.

(defun my-eval-cond (clauses alist)
    (cond
        ((null clauses) nil)
        ((not (null (my-eval (car (car clauses)) alist))) (my-eval-list (cdr (car clauses)) alist))
        (T (my-eval-cond (cdr clauses) alist))
    )
)

;; You need to write this one.
;; Hint: just push the function body onto the global alist.  It is already an
;; association, e.g., (equal (L1 L2) (cond (...))) and (assoc 'equal in
;; the global alist will return this.  You can then take the cdr and you
;; have a list containing the formal parameters and the expressions in
;; the function body. defun returns the name of the function.

(defun my-eval-defun (body alist)
    (my-eval-setq (car BODY) (cdr BODY))
    (car body)
)

;; This one is done, it just initializes the global alist where global
;; settings, like those defined via setq and defun, go.

(defvar global-alist nil)

;; to push a new value, (setq global-alist (cons (cons 'newvar 'newval) global-alist))

;; This one is done, it will become the new top-level for LISP.  After you
;; load this file, call (my-top) and then you can type in expressions and
;; define and call functions to test your my-eval. Note it uses the prog which
;; allows defining local variables, labels and goto looping similar to features
;; found in imperative languages.

(defun my-top ()
    (prog ()
        top
            ;; read an s-expression, evaluate it using my-eval passing in the global-alist,
            ;; then print the result, functions and global variables will be on global-alist
            (print (my-eval (read) global-alist))
            (terpri) ;; prints a newline
            (go top) ;; loops forever
    )
)