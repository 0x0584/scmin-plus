(let ((x 10)) x)						; 10
(let ((x 10) (y x)) y)					; x
(let* ((x 10) (y x)) y)					; 10
(letrec ((foo (lambda (n) (* n n))) (bar (lambda (x) (+ x (foo x))))) (bar 11)) ; 132
(let loop ((i 10)) (print i) (if (= i 0) i (loop (- i 1))))	; from 10 to 0 returns 0
