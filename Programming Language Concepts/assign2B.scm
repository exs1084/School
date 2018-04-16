;; Import the parser and lexer generators.
(require (lib "yacc.ss" "parser-tools")
(lib "lex.ss" "parser-tools")
(prefix : (lib "lex-sre.ss" "parser-tools")))
(require (lib "pretty.ss"))
(define-tokens value-tokens (NUM ID))
(define-empty-tokens op-tokens
(OP
CP
COMMA
EQ1
LET
IN
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
(idfirst (:or letter "_" "$"))
(idrest (:or idfirst digit))
(ident (:: idfirst (:* idrest)))
(digits (:+ digit))
(number (:: digits (:? (:: "." digits)) (:? (:: (:or "E" "e") (:? (:or "+" "-")) digits)))))

;get-token: inputPort -> token
(define get-token
(lexer
((eof) 'EOF)
("let" 'LET)
("in" 'IN)
("(" 'OP)
(")" 'CP)
("," 'COMMA)
("=" 'EQ1)
("+" '+)
("-" '-)
("*" '*)
("/" '/)
(number (token-NUM (string->number lexeme)))
(ident (token-ID (string->symbol lexeme)))
(whitespace (get-token input-port))))

;;; data definitions
;; A small language expression (SmallLangExp) is one of the following.
;; a number n
;; an identifier x
;; a sum with parts e1 and e2,
;; where e1 and e2 are small language expressions
;; a difference with parts e1 and e2,
;; where e1 and e2 are small language expressions
;; a product with parts e1 and e2,
;; where e1 and e2 are small language expressions
;; a quotient with parts e1 and e2,
;; where e1 and e2 are small language expressions
;; a negation with part e,
;; where e is an small language expression
;; a bindings with parts defs and e,
;; where defs is a list of identifiers * SmallLangExp
;; and e is an small language expression
;; functions for associated with each part: predicate, constructor, selectors.
;; Number is a Scheme number
;; Identifier is a Scheme symbol

; make-sum: SmallLangExp * SmallLangExp -> SumExp
(define (make-sum exp1 exp2)
(list 'sum exp1 exp2))
; (equal? (make-sum 2 3) '(sum 2 3))

; make-diff: SmallLangExp * SmallLangExp -> DiffExp
(define (make-diff exp1 exp2)
(list 'diff exp1 exp2))
; (equal? (make-diff 2 3) '(diff 2 3))

; make-prod: SmallLangExp * SmallLangExp -> ProdExp
(define (make-prod exp1 exp2)
(list 'prod exp1 exp2))
; (equal? (make-prod 2 3) '(prod 2 3))

; make-quo: SmallLangExp * SmallLangExp -> QuoExp
(define (make-quo exp1 exp2)
(list 'quo exp1 exp2))
; (equal? (make-quo 2 3) '(quo 2 3))

; make-neg: SmallLangExp -> NegExp
(define (make-neg exp)
(list 'neg exp))
; (equal? (make-neg 2) '(neg 2))

; make-let: Listof(Identifier*SmallLangExp) * SmallLangExp -> BindingExp
; Identifier*SmallLangExp is represented as a two element list
(define (make-let defs exp)
(list 'with-bindings defs exp))
; (equal? (make-let (list (list 'x 1) (list 'y 2)) 3) '(with-bindings ((x 1) (y 2)) 3))

; parse-small-lang: (() -> token) -> SmallLangExp
(define parse-small-lang
(parser
(start exp)
(end EOF)
(tokens value-tokens op-tokens)
(error (lambda (a b c) (error 'parse-small-lang "error occurred,  ̃v  ̃v  ̃v" a b c)))
(grammar
(exp ((LET let-defs IN exp) (make-let $2 $4))
((math-exp) $1))
(let-def ((ID EQ1 exp) (list $1 $3)))
(let-defs ((let-def) (list $1))
((let-def COMMA let-defs) (cons $1 $3)))
(math-exp ((math-exp + term) (make-sum $1 $3))
((math-exp - term) (make-diff $1 $3))
((term) $1))
(term ((term * factor) (make-prod $1 $3))
((term / factor) (make-quo $1 $3))
((factor) $1))
(factor ((ID) $1)
((NUM) $1)
((- factor) (make-neg $2))
((OP exp CP) $2)))))

; lexer/parser test
(let* ((example "let x = -2 + 3 * 4, y = 0 in -2+5*x+y")
(i (open-input-string example))) ; convert string to inputPort
(equal? (parse-small-lang (lambda () (get-token i)))
'(with-bindings ((x (sum (neg 2) (prod 3 4)))
(y 0))
(sum (sum (neg 2) (prod 5 x)) y))))




;################2B-1################
;sum? exp -> bool
(define (sum? e)
  (if (pair? e)
      (equal? (car e) 'sum)
      #f))
;(sum? (make-sum 2 3))

;difference? : exp -> bool
(define (difference? e)
  (if (pair? e)
      (equal? (car e) 'diff)
      #f))
;(difference? (make-diff 2 3))

;product? : exp -> bool
(define (product? e)
  (if (pair? e)
      (equal? (car e) 'prod)
      #f))
;(product? (make-prod 2 3))

;quotient? : exp -> bool
(define (quotient? e)
  (if (pair? e)
      (equal? (car e) 'quo)
      #f))
;(quotient? (make-quo 2 3))

;negate? : exp -> bool
(define (negate? e)
  (if (pair? e)
      (equal? (car e) 'neg)
      #f))
;(negate? (make-neg 2))

;let? : exp -> bool
(define (let? e)
  (if (pair? e)
      (equal? (car e) 'with-bindings)
      #f))
;(let? (make-let (list (list 'x 1) (list 'y 2)) 3))

;arg1,arg2 : exp -> exp
(define (arg1 e)
  (if (pair? e)
      (car (cdr e))
      '()))
(define (arg2 e)
  (if (pair? e)
      (car (cdr (cdr e)))))
;(arg1 (make-sum 2 3))
;(arg2 (make-sum 2 3))

;neg-exp : exp -> exp
(define (neg-exp e)
  (if (pair? e)
      (car (cdr e))
      '()))
;(neg-exp (make-neg 2))

;let-defs : exp -> defs
(define (let-defs e)
  (if (pair? e)
      (car (cdr e))
      '()))
;(let-defs (make-let (list (list 'x 1) (list 'y 2)) 3))

;let-exp : exp -> exp
(define (let-exp e)
  (if (pair? e)
      (car (cdr (cdr e)))
      '()))
;(let-exp (make-let (list (list 'x 1) (list 'y 2)) 3))


;################2B-2################

;;; data definitions
;; A large language expression is one of the following.
;; a number n        (simple)
;; an identifier x        (factor)
;; a sum with parts e1 and e2,        (mathexpr)
;;    where e1 and e2 are small language expressions
;; a difference with parts e1 and e2,        (mathexpr)
;;    where e1 and e2 are small language expressions
;; a product with parts e1 and e2,        (term)
;;    where e1 and e2 are small language expressions
;; a quotient with parts e1 and e2,        (term)
;;    where e1 and e2 are small language expressions
;; a negation with part e,        (factor)
;;    where e is an small language expression
;; a bindings with parts defs and e,
;;    where defs is a list of identifiers * SmallLangExp
;;    and e is an small language expression
;; a list of identifiers        (formals)
;; a list of expressions        (actuals)
;; a funcall with parts e1 and e2        (simple)
;;    where e1 is a simple and e2 is a set of actuals
;; an access with parts e1 and e2        (simple)
;;    where e1 is a simple and e2 is an identifier
;; an equality with aprts e1 and e2
;;    where e1 and e2 are langeuage expressions
;; a procdef consisting of parts id forms and e
;;    where id is an identifier, forms is a set of formals,
;;    and e is a language expression
;; an assignment with parts id and e
;;    where id is an identifier and e is a language expression
;; a supercall with parts id and acts
;;    where id is an identifier and acts is a set of actuals
;; a "new" decleration with parts id and acts
;;    where id is an identifier and acts is a set of actuals
;; a procedure with parts procdefs and e
;;    where procdefs is a set of procdef and e is a language expression
;; an if with parts e1 e2 and e3
;;    where e1/e2/e3 are language expressions
;; a sequence with part e
;;    where e is a set of language expressions
;; a procedures with parts procdefs and e
;;    where procdefs is a set of procdef and e is a language expression
;; a method with parts id forms e and methdecls
;;    where id is an identifier, forms is a set of formals,
;;    e is a language expression, and methdecls is any number of additional
;;    methods
;; a class with parts id1 id2 fielddecls and methdecls
;;    where id1 and id2 are identifiers, fielddecls is a list of identifiers
;;    adn methdecls is a list of methods
;; a program with parts classdecls and e
;;    where classdecls is a list of classes, and e is a language expression
;; functions for associated with each part: predicate, constructor, selectors.
;; Number is a Scheme number
;; Identifier is a Scheme symbol





;$@$@$@$@$@$@$predicates@$@$@$@$@$@$@

;program? : exp -> bool
(define (program? e)
  (if (pair? e)
      (equal? (car e) 'program)
      #f))
;(program? '(program apple))

;class-decl? : exp -> bool
(define (class-decl? e)
  (if (pair? e)
      (equal? (car e) 'class)
      #f))
;(class-decl? '(class apple))

;method? : exp -> bool
(define (method? e)
  (if (pair? e)
      (equal? (car (car e)) 'method)
      #f))
;(method? '((method apple) pear))

;new? : exp -> bool
(define (new? e)
  (if (pair? e)
      (equal? (car e) 'new)
      #f))
;(new? '(new apple))

;supercall? : exp -> bool
(define (supercall? e)
  (if (pair? e)
      (equal? (car e) 'super)
      #f))
;(supercall? '(super apple))

;seq? : exp -> bool
(define (seq? e)
  (if (pair? e)
      (equal? (car e) 'sequence)
      #f))
;(seq? '(sequence apple))

;procs? : exp -> bool
(define (procs? e)
  (if (pair? e)
      (equal? (car e) 'procedures)
      #f))
;(procs? '(procedures apple pear))

;if? : exp -> bool
(define (if? e)
  (if (pair? e)
      (equal? (car e) 'if)
      #f))
;(if? '(if apple))

;assign? : exp -> bool
(define (assign? e)
  (if (pair? e)
      (equal? (car e) 'assign!)
      #f))
;(assign? '(assign! apple))

;equality? : exp -> bool
(define (equality? e)
  (if (pair? e)
      (equal? (car e) 'equality?)
      #f))
;(equality? '(equality? apple))

;proc? : exp -> bool
(define (proc? e)
  (if (pair? e)
      (equal? (car e) 'proc)
      #f))
;(proc? '(proc apple))

;access? : exp -> bool
(define (access? e)
  (if (pair? e)
      (equal? (car e) 'send)
      #f))
;(access? '(send apple))

;funcall? : exp -> bool
(define (funcall? e)
  (if (pair? e)
      (equal? (car e) 'funcall)
      #f))
;(funcall? '(funcall apple))


;$@$@$@$@$@$@constructors$@$@$@$@$@$@

;make-program : classdecls exp -> program
(define (make-program decls e)
  (list 'program decls e))
;(program? (make-program 'apple 'pear))

;make-class : ident ident fielddecls methdecls -> classdecl
(define (make-class id1 id2 field meth)
  (list 'class id1 id2 field meth))
;(class-decl? (make-class 'apple 'pear 'orange 'kiwi))

;make-method : ident formals exp methdecls -> methdecls
(define (make-method ident form exp meth)
  (list (list 'method ident form exp) meth))
;(method? (make-method 'apple 'pear 'orange 'kiwi))

;make-new : ident actuals -> exp
(define (make-new ident act)
  (list 'new ident act))
;(new? (make-new 'apple 'pear))

;make-supercall : ident actuals -> exp
(define (make-supercall ident act)
  (list 'super ident act))
;(supercall? (make-supercall 'apple 'pear))

;make-seq : seq exprs -> seq
(define (make-seq seq exps)
  (if (seq? seq)
      (append '(sequence) (seq-exps seq) (list exps))
      (list 'sequence exps)))
;(seq? (make-seq 'apple))

;make-procs : procdefs exp -> exp
(define (make-procs procdefs exp)
  (list 'procedures procdefs exp))
;(procs? (make-procs 'apple 'pear))

;make-if : exp exp exp -> exp
(define (make-if e1 e2 e3)
  (list 'if e1 e2 e3))
;(if? (make-if 'apple 'pear 'orange))

;make-assign : ident exp -> exp
(define (make-assign ident exp)
  (list 'assign! ident exp))
;(assign? (make-assign 'apple 'pear))

;make-equal : mathexp mathexp -> compexp
(define (make-equal math1 math2)
  (list 'equality? math1 math2))
;(equality? (make-equal 'apple 'pear))

;make-proc : formals exp -> exp
(define (make-proc forms exp)
  (list 'proc forms exp))
;(proc? (make-proc 'apple 'pear))

;make-access : simple ident -> simple
(define (make-access simp ident)
  (list 'send simp ident))
;(access? (make-access 'apple 'pear))

;make-funcall : simple funcall actuals -> simple
(define (make-funcall simp fcall acts)
  (if (funcall? fcall)
      (append '(funcall) (list simp) (funcall-rands fcall) (list acts))
      (list 'funcall simp acts)))
;(funcall? (make-funcall 'apple 'pear))

;$@$@$@$@$@$@$selectors@$@$@$@$@$@$@

;program-decls : program -> classdecls
(define (program-decls prog)
  (arg1 prog))
;(program-decls (make-program 'apple 'pear))

;program-exp : program -> exp
(define (program-exp prog)
  (arg2 prog))
;(program-exp (make-program 'apple 'pear))

;class-name : classdecl -> ident
(define (class-name class)
  (car (cdr class)))
;(class-name (make-class 'apple 'pear 'orange 'kiwi))

;class-parent : classdecl -> ident
(define (class-parent class)
  (car (cdr (cdr class))))
;(class-parent (make-class 'apple 'pear 'orange 'kiwi))

;class-fields : classdecl -> fielddecls
(define (class-fields class)
  (car (cdr (cdr (cdr class)))))
;(class-fields (make-class 'apple 'pear 'orange 'kiwi))

;class-methods : classdecl -> methdecls
(define (class-methods class)
  (car (cdr (cdr (cdr (cdr class))))))
;(class-methods (make-class 'apple 'pear 'orange 'kiwi))

;method-name : methdecls -> ident
(define (method-name meth)
  (car (cdr (car meth))))
;(method-name (make-method 'apple 'pear 'orange 'kiwi))

;method-formals : methdecls -> formals
(define (method-formals meth)
  (car (cdr (cdr (car meth)))))
;(method-formals (make-method 'apple 'pear 'orange 'kiwi))

;method-exp : methdecls -> exp
(define (method-exp meth)
  (car (cdr (cdr (cdr (car meth))))))
;(method-exp (make-method 'apple 'pear 'orange 'kiwi))

;new-name : exp -> ident
(define (new-name exp)
  (arg1 exp))
;(new-name (make-new 'apple 'pear))

;new-rands : exp -> actuals
(define (new-rands exp)
  (arg2 exp))
;(new-rands (make-new 'apple 'pear))

;supercall-name : exp -> ident
(define (supercall-name exp)
  (arg1 exp))
;(supercall-name (make-supercall 'apple 'pear))

;supercall-rands : exp -> actuals
(define (supercall-rands exp)
  (arg2 exp))
;(supercall-rands (make-supercall 'apple 'pear))

;seq-exps : exp -> exps
(define (seq-exps exp)
  (cdr exp))
;(seq-exps (make-seq 'apple))

;procs-defs : exp -> procdefs
(define (procs-defs exp)
  (arg1 exp))
;(procs-defs (make-procs 'apple 'pear))

;procs-exp : exp -> exp
(define (procs-exp exp)
  (arg2 exp))
;(procs-exp (make-procs 'apple 'pear))

;if-exp1, if-exp2, if-exp3 : exp -> exp
(define (if-exp1 exp)
  (car (cdr exp)))
(define (if-exp2 exp)
  (car (cdr (cdr exp))))
(define (if-exp3 exp)
  (car (cdr (cdr (cdr exp)))))
;(if-exp1 (make-if 'apple 'pear 'orange))
;(if-exp2 (make-if 'apple 'pear 'orange))
;(if-exp3 (make-if 'apple 'pear 'orange))

;assign-var : exp -> ident
(define (assign-var exp)
  (arg1 exp))
;(assign-var (make-assign 'apple 'pear))

;assign-exp : exp -> exp
(define (assign-exp exp)
  (arg2 exp))
;(assign-exp (make-assign 'apple 'pear))

;proc-formals : exp -> formals
(define (proc-formals exp)
  (arg1 exp))
;(proc-formals (make-proc 'apple 'pear))

;proc-exp : exp -> exp
(define (proc-exp exp)
  (arg2 exp))
;(proc-exp (make-proc 'apple 'pear))

;access-exp : simple -> simple
(define (access-exp sim)
  (arg1 sim))
;(access-exp (make-access 'apple 'pear))

;access-message : simple -> ident
(define (access-message sim)
  (arg2 sim))
;(access-message (make-access 'apple 'pear))

;funcall-rator : simple -> simple
(define (funcall-rator sim)
  (arg1 sim))
;(funcall-rator (make-funcall 'apple 'pear))

;funcall-rands : simple -> actuals
(define (funcall-rands sim)
  (cdr (cdr sim)))
;(funcall-rands (make-funcall 'apple 'pear))

