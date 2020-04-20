(cons 1 2)								; (1 . 2)
(cons 1 (cons 2 nil))					; (1 2)
(cons nil '())							; (nil)
(cons (cons 'nil nil) (cons 'a '()))	; ((nil) a)
(list 1 2 3 (+ 2 5))					; (1 2 3 7)
(car '(3 4 2))							; 3
(cdr '((3 5) (4 5) 3 5))				; ((4 5) 3 5)
(define lst '(2 3 4 5 a b))				; t
(define lst '())						; nil
(set-car! lst '(3 4))					; t
(print lst)								; (((3 4) 3 4 5 a b))
(set-cdr! lst '(1))						; t
(print lst)								; (((3 4) 1))
(set! lst 4)							; t
(print lst)								; (4)
