(defun PRINTL (L)
    (cond
        ((null L) 1)
        (T 
            (FORMAT t "~A " (CAR L))
            (PRINTL (CDR L))
        )
    )
)

(defun MY-LENGTH (L)
    (cond 
        ((null L) 0)
        (T (+ (MY-LENGTH (cdr L)) 1))
    )
)

(defun MY-MEMQ (A L)
    (cond
        ((null L)           NIL)
        ((equal A (car L))  L  )
        (T (MY-MEMQ A (cdr L)) )
    )
)

(defun MY-APPEND (L1 L2)
    (cond
        ((null L1)  L2)
        ((null L2)  L1)
        (T          (cons (car L1) (MY-APPEND (cdr L1) L2)))
    )
    
)

(defun MY-CONCAT (L1 A)
    (cond
        ((null L1)  (cons A nil))
        (T          (cons (car L1) (MY-CONCAT (cdr L1) A)))
    )
)

(defun printem (&rest args)
    (format t "~{~a~^ ~}" args)
)

(defun REV2 (L R) ;cheeky accumulator variable
    (printem L R)
    (cond
        ((null L) R)
        (T (REV2 (CDR L) (CONS (CAR L) R))) 
    )
)

(defun MY-REVERSE (L)
    (REV2 L NIL)
)

(defun MY-MAPCAR (F L)
    (cond
        ((null L)   NIL)
        (T          (CONS (apply F (CONS (CAR L) NIL)) (MY-MAPCAR F (CDR L))))
    )
)

(defun MY-COPY (L)
    (cond
        ((null L)   NIL)
        ((atom L)   L)
        (T          (CONS (CAR L) (MY-COPY (CDR L))))
    )
)

(defun MY-NTH (L N)
    (cond
        ((equal N 1)    L)
        ((equal N 0)    (CONS NIL L))
        (T  (MY-NTH (CDR L) (- N 1)))
    )
)

(defun MY-REMOVE (X L)
    (cond
        ((null L)   NIL)
        ((equal (CAR L) X)   
            (MY-REMOVE X (CDR L)))
        (T  (CONS (CAR L) (MY-REMOVE X (CDR L))))
    )
)

(defun MY-SUBST (X Y Z)
    (cond
        ((null Z)   NIL)
        ((LISTP (CAR Z)) 
            (CONS (MY-SUBST X Y (CAR Z)) (MY-SUBST X Y (CDR Z))))
        ((equal (CAR Z) X)
            (CONS Y (MY-SUBST X Y (CDR Z))))
        (T  (CONS (CAR Z) (MY-SUBST X Y (CDR Z))))
    )
)

(defun MY-SUBSET (FN L)
    (cond
        ((null L)   NIL)
        ((not (null (apply FN (CONS (CAR L) NIL))))   
            (CONS (CAR L) (MY-SUBSET FN (CDR L))))
        (T  (MY-SUBSET FN (CDR L)))
    )
)

(defun REAL-ADD (N1 N2)
    (cond
        ((null N1)
            (cond
                ((null N2) NIL)
                (T (CONS (CAR N2) (REAL-ADD (CDR N1) (CDR N2))))
            )
        )
        ((null N2)
            (CONS (CAR N1) (REAL-ADD (CDR N1) (CDR N2))))
        (T (CONS (+ (CAR N1) (CAR N2)) (REAL-ADD (CDR N1) (CDR N2))))
    )
)

(defun CARRY (L FLAG)
    (cond
        ((null L)
            (cond
                ((null FLAG) nil)
                (T (CONS '1 NIL))
            )
        )
        ((not (null FLAG))
            (cond
                ((>= (1+ (CAR L)) 10)
                    (CONS (MOD (1+ (CAR L)) 10) (CARRY (CDR L) T)))
                (T (CONS (MOD (1+ (CAR L)) 10) (CARRY (CDR L) NIL)))
            )
        )
        ((>= (CAR L) 10)
            (CONS (MOD (CAR L) 10) (CARRY (CDR L) T)))
        (T  (CONS (MOD (CAR L) 10) (CARRY (CDR L) NIL)))
    )
)

; (CARRY '(11 4) NIL)
; (CARRY '(11 9 0) NIL)

(defun MY-ADD (N1 N2)
    (CARRY (REAL-ADD (MY-REVERSE N1) (MY-REVERSE N2)) NIL)
)

; (MY-ADD '(1 2) '(3 4))
; (MY-ADD '(1 7) '(3 4))

(defun MY-MERGE (L1 L2)
    (cond
        ((null L1)   L2)
        ((null L2)   L1)
        ((< (CAR L1) (CAR L2))   
            (CONS (CAR L1) (MY-MERGE (CDR L1) L2)))
        (T  (CONS (CAR L2) (MY-MERGE L1 (CDR L2))))
    )
)

(defun LISTCMP (L1 L2)
    (cond
        ((null L1) T)
        ((equal (CAR L1) (CAR L2)) (LISTCMP (CDR L1) (CDR L2)))
        (T NIL)
    )
)

(defun MY-SUBLIST (L1 L2)
    (cond
        ((null L2) NIL)
        ((LISTCMP L1 L2) T)
        (T (MY-SUBLIST L1 (CDR L2))) ;; TODO must be strict subset
    )
)

(defun MY-REPLACE (ALIST L)
    (cond
        ((null L) NIL)
        ((not (null (MY-ASSOC (CAR L) ALIST)))
            (CONS (CDR (MY-ASSOC (CAR L) ALIST)) (MY-REPLACE ALIST (CDR L))))
        (T (CONS (CAR L) (MY-REPLACE ALIST (CDR L))))
    )
)
