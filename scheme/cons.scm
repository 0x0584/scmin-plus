;; expression                           ; expected
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(cons 1 2)                              ; (1 . 2)
(cons 1 (cons 2 nil))                   ; (1 2)
(cons nil '())                          ; (nil)
(cons (cons 'nil nil) (cons 'a '()))    ; ((nil) a)
(list 1 2 3 (+ 2 5))                    ; (1 2 3 7)
(car '(3 4 2))                          ; 3
(cdr '((3 5) (4 5) 3 5))                ; ((4 5) 3 5)
(define lst '(2 3 4 5 a b))             ; lst
(define lst '())                        ; lst
(set-car! lst '(3 4))                   ; (3 4)
(print lst)                             ; ((3 4) 3 4 5 a b)
(set-cdr! lst '(1))                     ; ((3 4) 1)
(set-caar! lst 'foo)					; ((foo 4) 1)
(cadar '((1 (2 b c) 3) foo) bar)		; 2
(car (cdr (car '((1 2 3)))))            ; 2
(set! lst 4)                            ; 4
