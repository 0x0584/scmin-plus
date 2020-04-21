(if t 4)                                   ; 4
(if nil 5)                                 ; nil
((if nil '+ '*) 2 10)                      ; 20
(if (and 4 5) 6)                           ; 6
(cond ((gt? 3 2) (+ 2 3)) ((eq? 2 2) '())) ; 5
(cond ((lt? 5 2) 1 2 3) (else 4 3 2))      ; 2
