foo                                     ; foo
bar-baz                                 ; bar
"this is a string"                      ; "this is a string"
10                                      ; 10
11.2                                    ; 11.2
-12.32                                  ; -12.32
(+ 1 2)                                 ; 3
(cons 'a 'b)                            ; (a . b)
(cons "ss" "tt")                        ; ("ss" . "tt")
(cons 'a (cons 'b (cons 'c '())))       ; (a b c)
(quote (a b c))                         ; (a b c)
(quote a)                               ; a
'a                                      ; a
(quote (cons 'a 'b))                    ; (cons (quote a) (quote b))
(quasiquote a)                          ; a
`a                                      ; a
'(1 2 3)                                ; (1 2 3)
`(1 2 ,(+ 1 2) ,@(list 'a 'b 'c))       ; (1 2 3 a b c)
(quote (cons 'a 'b))                    ; (cons (quote a) (quote b))
((lambda (x y) (+ x y)) 2 3)            ; 5
(print 'foo 'bar)                       ; foo bar
(print (lambda (x y) (cons x y)))       ; [args: (x y), body: (cons x y)]
(define x '(1 2 3))                     ; x
(print x)                               ; (1 2 3)
