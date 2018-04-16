;2A-1
;member? : List sym -> bool
(define (member? set sym)
  (if (equal? set '())
      #f
      (pair? (member sym set))))
;(member? '(1 2 3 4 5) 5)
;(member? '() 5)

;union : List List -> List
(define (union set1 set2)
  (if (equal? set1 '())
      (if (equal? set2 '())
          '()
          set2)
      (if (equal? set2 '())
          set1
          (if (member? set1 (car set2))
              (union set1 (cdr set2))
              (union (append set1 (list (car set2))) (cdr set2))))))
;(union '(1 2 3 4) '(5 2 6 3))
;(union '() '(9))

;2A-2





;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
; Terminals are quoted.
; A rule A -> X1 ... Xn is written (A (X1 ... Xn))
; A grammar is a list of rules.

(define *grammar*
  '((S (E 'eof))
    (E (T E2))
    (E2 ('+ T E2))
    (E2 ('- T E2))
    (E2 ())
    (T (F T2))
    (T2 ('* F T2))
    (T2 ('/ F T2))
    (T2 ())
    (F ('n))
    (F ('id))
    (F ('- F))
    (F ('OP E 'CP))))

; rule-lhs : Rule -> Variable
(define rule-lhs car)

; (eq? (rule-lhs '(E (T E2))) 'E)

; rule-rhs : Rule -> List(Variables or Terminals)
(define rule-rhs cadr)

; (equal? (rule-rhs '(E (T E2))) '(T E2))

; variable? : Any -> Boolean
(define variable? symbol?)

; (variable? 'E)
; (not (variable? ''+))

; terminal? : Any -> Boolean
(define (terminal? a) (and (pair? a) (eq? (car a) 'quote)))

; (terminal? ''+)
; (not (terminal? 'E))
;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@



;call first3, it calls first-var which uses first again.......

;findRules : grammar sym -> List
(define (findRules grammar sym)
  (if (equal? grammar '())
      '()
      (if (equal? (rule-lhs (car grammar)) sym)
          (append (list (car grammar)) (findRules (cdr grammar) sym))
          (findRules (cdr grammar) sym))))
;(findRules *grammar* 'E2)
;ruleEps : List -> List
(define (ruleEps rules)
  (if (equal? '() rules)
      '()
      (if (equal? '() (rule-rhs (car rules)))
          (car rules)
          (ruleEps (cdr rules)))))
;(ruleEps (findRules *grammar* 'E2))
;ruleOther : List-> List
(define (ruleOther rules)
  (if (equal? '() rules)
      '()
      (if (equal? '() (rule-rhs (car rules)))
          (ruleOther (cdr rules))
          (append (list (car rules)) (ruleOther (cdr rules))))))
;(ruleOther (findRules *grammar* 'E2))

;first3 : grammar alpha seen ->
(define (first3 grammar alpha seen)
  (if (equal? alpha '())
      '(());? it said epsilon but...?
      (if (terminal? (car alpha))
          (cdar alpha);return unquoted terminal
          (let ((fullRule (findRules grammar (car alpha))));find all rules
            (if (equal? '() fullRule);any rules at all?
                '();nope, no more firsts
                (if (not (equal? '() (ruleEps fullRule)));yes..any epsilons?
                    (union (first3 grammar (cdr alpha) seen);yup
                               (first-var3 grammar (ruleOther fullRule) seen))
                    (first-var3 grammar (ruleOther fullRule) seen)))))));nope
;(first3 *grammar* '() '())
;(first3 *grammar* '('+ E) '())
          
;if first in alpha is a terminal, return it. otherwise dont

;first-var3 : grammer rules seen ->
(define (first-var3 grammar rules seen)
  (if (equal? rules '())
      '()
      (if (member? seen (car rules))
          (first-var3 grammar (cdr rules) seen)
          (union (first3 grammar (rule-rhs (car rules))
                         (append (car rules) seen))
                 (first-var3 grammar (cdr rules) (append (car rules) seen))))))
;(first-var3 *grammar* '() '())
;(first-var3 *grammar* (findRules *grammar* 'E2) '())


;first-alpha : grammar alpha -> List
(define (first-alpha grammar alpha)
  (if (equal? alpha '())
      '(())
      (first3 grammar alpha '())))
;(first-alpha *grammar* '(T E2))
;(first-alpha *grammar* '())

;2A-3

;findFollow : grammar var -> List
(define (findFollow grammar var)
  (if (equal? grammar '())
      '()
      (if (member? (rule-rhs (car grammar)) var)
          (append (list (car grammar)) (findFollow (cdr grammar) var))
          (findFollow (cdr grammar) var))))
;(findFollow *grammar* 'E)
;(findFollow '() 'E)

;follow-rules4 : grammar var rules seen -> List
(define (follow-rules4 grammar var rules seen)
  (if (equal? '() rules)
      '()
      (if (member? seen (car rules))
          (follow-rules4 grammar var (cdr rules) seen)
          (let ((tail (cdr (member var (rule-rhs (car rules))))))
            (if (equal? tail '())
                (follow-rules4 grammar (rule-lhs (car rules)) (findFollow grammar (rule-lhs (car rules))) (append (list (car rules)) seen))
                (let ((set (first-alpha grammar tail)))
                  (if (member? set '())
                      (union (union set (follow-rules4 grammar var (cdr rules) (append seen (car rules)))) (follow-rules4 grammar (rule-lhs (car rules)) (findFollow grammar (rule-lhs (car rules))) (append (list (car rules)) seen)))
                      (union set (follow-rules4 grammar var (cdr rules) (append seen (car rules)))))))))))
;(follow-rules4 *grammar* 'E (findFollow *grammar* 'E) '())
;(follow-rules4 *grammar* 'E '() '())

;delete : elmnt List -> List
(define (delete elm lst)
  (if (equal? lst '())
      '()
      (if (equal? elm (car lst))
          (delete elm (cdr lst))
          (append (list (car lst)) (delete elm (cdr lst))))))
;(delete '5 '(5 7 5 2 5 3 5))
;(delete '5 '())

;follow-var : grammar var -> List
(define (follow-var grammar var)
  (delete '() (follow-rules4 grammar var (findFollow grammar var) '())))
;(follow-var *grammar* 'F)
;(follow-var *grammar* 'E2)