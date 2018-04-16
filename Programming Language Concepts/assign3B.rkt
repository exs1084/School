;;; Author: Arthur Nunes-Harwitt
(require (lib "yacc.ss" "parser-tools")
         (lib "lex.ss" "parser-tools")
         (prefix : (lib "lex-sre.ss" "parser-tools")))
(require (lib "pretty.ss"))
(define-tokens value-tokens (NUM ID))
(define-empty-tokens op-tokens
  (BEGIN
   END
   SEMI
   EQ1
   EQ2
   IF
   THEN
   ELSE
   OP
   CP
   COMMA
   LET
   IN
   BSLASH
   ARROW
   PROCS
   +
   -
   *
   /
   EOF))
(define-lex-abbrevs
  (lower-letter (:/ "a" "z"))
  (upper-letter (:/ "A" "Z"))
  (letter (:or lower-letter upper-letter))
  (digit (:/ "0" "9"))
  (ident (:+ letter))
  (number (:+ digit)))
;get-token: inputPort -> token
(define get-token
  (lexer
   ((eof) 'EOF)
   ("if" 'IF)
   ("then" 'THEN)
   ("else" 'ELSE)
   ("let" 'LET)
   ("in" 'IN)
   ("\\" 'BSLASH) ;just one, it looks like two because \ is an escape character
   ("->" 'ARROW)
   ("procedures" 'PROCS)
   ("{" 'BEGIN)
   ("}" 'END)
   ("(" 'OP)
   (")" 'CP)
   (";" 'SEMI)
   ("," 'COMMA)
   ("=" 'EQ1)
   ("==" 'EQ2)
   ("+" '+)
   ("-" '-)
   ("*" '*)
   ("/" '/)
   (number (token-NUM (string->number lexeme)))
   (ident (token-ID (string->symbol lexeme)))
   (whitespace (get-token input-port))))
;; An expression (Exp) is one of the following.
;; a Number n
;; an Identifier x
;; a sum with parts e1 and e2,
;;   where e1 and e2 are expressions
;; a let with parts x, e1, and e2,
;;   where x is an identifier
;;   and e1 and e2 are expressions
;; an if with parts e1, e2, and e3,
;;   where e1, e2, and e3 are expressions
;; a proc with parts x and e
;;   where x is an identifier and e is an expression
;; a funcall with parts e1 and e2
;;   where e1 and e2 are expressions
;; an assignment with parts x and e
;;   where x is an identifier and e is an expression

;; functions for associated with each part: predicate, constructor, selectors.
;; Number is a Scheme number
;; Identifier is a Scheme symbol
; ident?: Any -> Bool
(define ident? symbol?)
; (ident? 'x)
; sum?: Any -> Bool
(define (sum? a) (and (pair? a) (eq? (car a) '+)))
; (eq? (sum? '(+ 2 3)) #t)
; (eq? (sum? '3) #f)
; make-sum: Exp * Exp -> SumExp
(define (make-sum exp1 exp2)
  (list '+ exp1 exp2))
; (equal? (make-sum 2 3) '(+ 2 3))
; arg1: SumExp -> Exp
(define (arg1 e) (car (cdr e)))
; (= (arg1 (make-sum 2 3)) 2)
; arg2: SumExp -> Exp
(define (arg2 e) (car (cdr (cdr e))))
; (= (arg2 (make-sum 2 3)) 3)
; let?: Any -> Bool
(define (let? a) (and (pair? a) (eq? (car a) 'let)))
; (eq? (let? '(let x 1 3)) #t)
; (eq? (let? '3) #f)
; make-let: Identifier*Exp * Exp -> LetExp
; Identifier*Exp is represented as a two element list
(define (make-let var exp1 exp2)
  (list 'let var exp1 exp2))
; (equal? (make-let 'x 1 3) '(let x 1 3))
; let-var: LetExp -> Identifier
(define let-var cadr)
; (eq? (let-var (make-let 'x 1 3)) 'x)
; let-exp: LetExp -> Exp
(define let-exp caddr)
; (= (let-exp (make-let 'x 1 3)) 1)
; let-body: LetExp -> Exp
(define let-body cadddr)
; (= (let-body (make-let 'x 1 3)) 3)
; if?: Any -> Bool
(define (if? a) (and (pair? a) (eq? (car a) 'if)))
; (if? (make-if 1 2 3))
; (not (if? 5))
; make-if: Exp * Exp * Exp -> IfExp
(define (make-if exp1 exp2 exp3)
  (list 'if exp1 exp2 exp3))
; (equal? (make-if 1 2 3) '(if 1 2 3))
; if-exp1: IfExp -> Exp
(define if-exp1 cadr)
; (= (if-exp1 (make-if 1 2 3)) 1)
; if-exp2: IfExp -> Exp
(define if-exp2 caddr)
; (= (if-exp2 (make-if 1 2 3)) 2)
; if-exp3: IfExp -> Exp
(define if-exp3 cadddr)
; (= (if-exp3 (make-if 1 2 3)) 3)
; proc?: Any -> Bool
(define (proc? a) (and (pair? a) (eq? (car a) 'proc)))
; (proc? '(proc x 1))
; (not (proc? 3))
; make-proc: Identifier * Exp -> ProcExp
(define (make-proc var exp)
  (list 'proc var exp))
; (equal? (make-proc 'x 1) '(proc x 1))
; proc-var: ProcExp -> Identifier
(define proc-var cadr)
; (eq? (proc-var (make-proc 'x 1)) 'x)
; proc-exp: ProcExp -> Exp
(define proc-exp caddr)
; (= (proc-exp (make-proc 'x 1)) 1)
; funcall? Any -> Bool
(define (funcall? a) (and (pair? a) (eq? (car a) 'funcall)))
; (funcall? '(funcall f 1))
; (not (funcall? 3))
; make-funcall: Exp * Exp -> FuncallExp
(define (make-funcall exp1 exp2)
  (list 'funcall exp1 exp2))
; (equal? (make-funcall 'f 1) '(funcall f 1))
; funcall-rator: FuncallExp -> Exp
(define funcall-rator cadr)
; (eq? (funcall-rator (make-funcall 'f 1)) 'f)
; funcall-rand: FuncallExp -> Exp
(define funcall-rand caddr)
; (= (funcall-rand (make-funcall 'f 1)) 1)


;; assign?: Any -> Bool
;(define (assign? a) (and (pair? a) (eq? (car a) 'assign!)))
;; (eq? (assign? '(assign! x 3)) #t)
;; (eq? (assign? '3) #f)
;; make-assign: Identifier * LangExp -> AssignExp
;(define (make-assign var exp)
;  (list 'assign! var exp))
;; (equal? (make-assign 'x 3) (assign! x 3))
;; assign-var: AssignExp -> Identifier
;(define assign-var cadr)
;; (eq? (assign-var (make-assign 'x 3)) 'x)
;; assign-exp: AssignExp -> LangExp
;(define assign-exp caddr)
;; (= (assign-exp (make-assign 'x 3)) 3)
;

; parse-lang: (() -> token) -> SmallLangExp
(define parse-lang
  (parser
   (start exp)
   (end EOF)
   (tokens value-tokens op-tokens)
   (error (lambda (a b c) (error 'parse-lang "error occurred, ˜v ˜v ˜v" a b c)))
   (grammar
    (exp ((BEGIN exps END) (make-seq $2))
         ((LET let-defs IN exp) (make-letstar $2 $4))
         ((PROCS rec-defs IN exp) (make-procs $2 $4))
         ((IF exp THEN exp ELSE exp) (make-if $2 $4 $6))
         ((BSLASH formals BSLASH ARROW exp ) (make-curried-proc $2 $5))
         ((comp-exp) $1))
    (exps ((exp) (list $1))
          ((exp SEMI exps) (cons $1 $3)))
    (let-def ((ID EQ1 exp) (list $1 $3)))
    (let-defs ((let-def) (list $1))
              ((let-def COMMA let-defs) (cons $1 $3)))
    (rec-def ((ID OP formals CP EQ1 exp) (list $1 (make-curried-proc $3 $6))))
    (rec-defs ((rec-def) (list $1))
              ((rec-def COMMA rec-defs) (cons $1 $3)))
    (comp-exp ((math-exp EQ2 math-exp) (make-equality $1 $3))
              ((math-exp) $1))
    (math-exp ((math-exp + term) (make-sum $1 $3))
              ((math-exp - term) (make-diff $1 $3))
              ((term) $1))
    (term ((term * factor) (make-prod $1 $3))
          ((term / factor) (make-quo $1 $3))
          ((factor) $1))
    (factor ((simple) $1)
            ((NUM) $1)
            ((- factor) (make-neg $2))
            ((simple OP actuals CP) (make-curried-funcall $1 $3)))
    (simple ((ID) $1)
            ((OP exp CP) $2))
    (actuals (() null)
             ((actualsNE) (reverse $1)))
    (actualsNE ((exp) (list $1))
               ((actualsNE COMMA exp) (cons $3 $1)))
    (formals (() null)
             ((formalsNE) (reverse $1)))
    (formalsNE ((ID) (list $1))
               ((formalsNE COMMA ID) (cons $3 $1))))))
; lexer/parser test
;(let* ((example "2+3+4")
;       (i (open-input-string example)))
;  (equal? (parse-lang (lambda () (get-token i)))
;          '(+ (+ 2 3) 4)))
;; An environment (Env) is a Scheme function from Identifier to Value.
(define empty-env
  (lambda (var) (error 'empty-env "variable undefined")))
; apply-env: Env * Identifier -> Val
(define (apply-env env var) (env var))
; extend-env: Env * Identifier * Val -> Env
(define (extend-env env var val)
  (lambda (var2)
    (if (eq? var var2)
        val
        (env var2))))





;; continuations
(define init-k (lambda (v) v))
;; denotation
(define (boolify num)
  (if num
      #t
      #f))
; meaning: Exp * Env * (Val-> A) -> A
(define (meaning exp env k)
  (cond ((number? exp) (k exp))
        ((ident? exp)((apply-env env exp) k))
        ((sum? exp)
         (meaning (arg1 exp)
                  env
                  (lambda (v1)
                    (meaning (arg2 exp)
                             env
                             (lambda (v2)
                               (k (+ v1 v2)))))))
        ((diff? exp)
         (meaning (arg1 exp)
                  env
                  (lambda (v1)
                    (meaning (arg2 exp)
                             env
                             (lambda (v2)
                               (k (- v1 v2)))))))
        ((prod? exp)
         (meaning (arg1 exp)
                  env
                  (lambda (v1)
                    (meaning (arg2 exp)
                             env
                             (lambda (v2)
                               (k (* v1 v2)))))))
        ((quo? exp)
         (meaning (arg1 exp)
                  env
                  (lambda (v1)
                    (meaning (arg2 exp)
                             env
                             (lambda (v2)
                               (k (/ v1 v2)))))))
        ((neg? exp)
         (meaning (neg-exp exp)
                  env
                  (lambda (v1)
                    (k (- 0 v1)))))
        ((equality? exp)
         (meaning (arg1 exp)
                  env
                  (lambda (v1)
                    (meaning (arg2 exp)
                             env
                             (lambda (v2)
                               (k (equal? v1 v2)))))))
        
;        ((let? exp)
;         (meaning
;          (let-exp exp)
;           env
;          (lambda (v)
;            (meaning (let-body exp)
;                     (extend-env env (let-var exp) v)
;                     k))))



        ((let? exp)
         (meaning
          (let-body exp)
           (extend-env env (let-var exp) (lambda (k1) (meaning (let-exp exp) env k1)))
           k))



        
        ((if? exp)
         (meaning (if-exp1 exp)
                  env
                  (lambda (v)
                    (if (boolify v)
                        (meaning (if-exp2 exp) env k)
                        (meaning (if-exp3 exp) env k)))))
        ((proc? exp)
         (k (lambda (v k1)
              (meaning (proc-exp exp)
                       (extend-env env (proc-var exp) v)
                       k1))))


        
;        ((funcall? exp)
;         (meaning (funcall-rator exp)
;                  env
;                  (lambda (f)
;                    (meaning (funcall-rand exp)
;                             env
;                             (lambda (v) (f v k))))))
        ((funcall? exp)
         (meaning (funcall-rator exp)
                  env
                  (lambda (v1)
                    (v1
                     (lambda (k1)
                       (meaning (funcall-rand exp) env k1))
                     k))))

        
        (else (error 'meaning "Unknown expression"))))
; lexer/parser/meaning test
;(let* ((example "2+3+4")
;       (i (open-input-string example)))
;  (= (meaning (parse-lang (lambda () (get-token i))) empty-env init-k) 9))




;#########################3A-1#########################
;#########################predicates#########################
;diff? : Any -> Bool
(define (diff? a)
  (and (pair? a) (eq? (car a) '-)))
;(diff? (make-diff 2 3))
;prod? : Any -> Booll
(define (prod? a)
  (and (pair? a) (eq? (car a) '*)))
;(prod? (make-prod 2 3))
;quo? : Any -> Bool
(define (quo? a)
  (and (pair? a) (eq? (car a) '/)))
;(quo? (make-quo 2 3))
;neg? : Any -> Bool
(define (neg? a)
  (and (pair? a) (eq? (car a) 'neg)))
;(neg? (make-neg 2))
;equality? : Any -> Bool
(define (equality? a)
  (and (pair? a) (eq? (car a) '=)))
;(equality? (make-equality 2 3))

;#########################constructors#########################
;make-diff : Exp * Exp -> DiffExp
(define (make-diff exp1 exp2)
(list '- exp1 exp2))
;(equal? (make-diff 2 3) '(- 2 3))
;make-prod : Exp * Exp -> ProdExp
(define (make-prod exp1 exp2)
(list '* exp1 exp2))
;(equal? (make-prod 2 3) '(* 2 3))
;make-quo : Exp * Exp -> QuoExp
(define (make-quo exp1 exp2)
(list '/ exp1 exp2))
;(equal? (make-quo 2 3) '(/ 2 3))
;make-neg : Exp -> NegExp
(define (make-neg exp)
(list 'neg exp))
;(equal? (make-neg 2) '(neg 2))
;make-equality : Exp * Exp -> EQExp
(define (make-equality exp1 exp2)
(list '= exp1 exp2))
;(equal? (make-equality 2 3) '(= 2 3))

;#########################selector#########################
;neg-exp : NegExp -> Exp
(define (neg-exp e)
  (cadr e))
;(neg-exp (make-neg 2))

;#########################3A-2#########################
;make-letstar : List(pair) exp -> List
(define (make-letstar lis e)
  (if (null? lis)
      e
      (make-let (car (car lis)) (car (cdr (car lis))) (make-letstar (cdr lis) e))))
;(make-letstar '((2 3)) '())
;(make-letstar '((2 3) (4 5)) '())
;#########################3A-3#########################
;make-seq : List -> List
(define (make-seq lis)
  (if (null? (cdr lis))
      (car lis)
      (make-let '*temp* (car lis) (make-seq (cdr lis)))))
;(make-seq '((sum 2 3)))
;(make-seq '((sum 2 3) (sum 4 5)))

;#########################3A-4#########################

;all rewritten code above. test here.
;(let* ((my-env (extend-env empty-env 'x 5)))
;  (meaning '(neg x) my-env init-k))


;#########################3A-5#########################
;make-curried-proc : List Exp -> Exp
(define (make-curried-proc lis e)
  (if (null? lis)
      (make-proc '*temp* e)
      (if (null? (cdr lis))
          (make-proc (car lis) e)
          (make-proc (car lis) (make-curried-proc (cdr lis) e)))))
;(make-curried-proc '() '(sum 2 3))
;(make-curried-proc '(a b) '(sum 2 3))

;make-curried-funcall : fun List -> Exp
(define (make-curried-funcall f lis)
  (if (null? lis)
      (make-funcall f 0)
      (if (null? (cdr lis))
          (make-funcall f (car lis))
          (make-curried-funcall (make-funcall f (car lis)) (cdr lis)))))
;(make-curried-funcall 'f '())
;(make-curried-funcall 'f '(a b))

;#########################3B#########################
;store removed
;new semantics implemented
;working on fixing the second pair of tests
(let* ((example "let x = 1/0 in 3")
       (i (open-input-string example)))
  (= (meaning (parse-lang (lambda () (get-token i))) empty-env init-k) 3))

(let* ((example "let x = z in 3")
       (i (open-input-string example)))
  (= (meaning (parse-lang (lambda () (get-token i))) empty-env init-k) 3))

(let* ((example "(\\x,y\\->x)(1,1/0)")
       (i (open-input-string example)))
  (= (meaning (parse-lang (lambda () (get-token i))) empty-env init-k) 1))

(let* ((example "(\\x,y\\->x)(1,z)")
       (i (open-input-string example)))
  (= (meaning (parse-lang (lambda () (get-token i))) empty-env init-k) 1))