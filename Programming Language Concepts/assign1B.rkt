;copied derivative stuff
; variable?: Any -> Bool
(define variable? symbol?)

; (eq? (variable? 'x) #t)
; (eq? (variable? 3) #f)

; variable=?: VarExp VarExp -> Bool
(define variable=? eq?)

; (variable=? 'x 'x)
; (not (variable=? 'x 'y))

;; a sum is represented as a list with three elements: tag, e1, e2.
;; the tag is the symbol +

; sum?: Any -> Bool
(define (sum? a) (and (pair? a) (eq? (car a) '+)))

; (eq? (sum? '(+ 2 3)) #t)
; (eq? (sum? '3) #f)



; (equal? (make-sum 2 3) '(+ 2 3))

;; a product is represented as a list with three elements: tag, e1, e2.
;; the tag is the symbol *

; product?: Any -> Bool
(define (product? a) (and (pair? a) (eq? (car a) '*)))

; (eq? (product? '(* 2 3)) #t)
; (eq? (product? '3) #f)



; (equal? (make-product 2 3) '(* 2 3))

;; sums and products will use the same selectors

; arg1: SumExp or ProdExp -> ArithExp
(define (arg1 e) (car (cdr e)))

; (= (arg1 (make-sum 2 3)) 2)
; (= (arg1 (make-product 2 3)) 2)

; arg2: SumExp or ProdExp -> ArithExp
(define (arg2 e) (car (cdr (cdr e))))

; (= (arg2 (make-sum 2 3)) 3)
; (= (arg2 (make-product 2 3)) 3)




;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

;1B-1
; merge : List List -> List
(define (merge L1 L2)
  (if(null? L1)
     L2
     (if (null? L2)
         L1
         (if (< (car L1) (car L2))
             (cons (car L1) (merge (cdr L1) L2))
             (cons (car L2) (merge L1 (cdr L2)))))))
;(merge '(1 3 5 7) '(2 4 6 8))
;(merge '() '(1 2))

;1B-2

; ms-help : List List -> List
(define (ms-help L1 L2);function to split the list
  (if(< (length L1) (length L2));done splitting?
     (ms-help (append L1 (list (car L2))) (cdr L2))
     (if(< (+ (length L1) (length L2)) 3)
        (merge L1 L2)
        (merge (merge-sort L1) (merge-sort L2)))))
;(ms-help '(1) '(3 7))
;(ms-help '(1 5 2) '(99 42 0))

; merge-sort : List -> List
(define (merge-sort nums)
  (if(null? nums)
     '()
     (let ((L1 (list (car nums))))
       (if(< (length L1) (length (cdr nums)))
          (ms-help L1 (cdr nums))
          (merge L1 (cdr nums))))))
;(merge-sort '(3 4 1 2))
;(merge-sort '(0 1 7 2 8 9 12 7))

;1B-3
; make-expt: ArithExp ArithExp -> ExponentExp
(define (make-expt e n)
  (if (number? e)
      (if (number? n)
          (expt e n);both nums
          (list '^ e n));e num, n var
      (if (number? n)
          (if (= n 0);e var, n num
              1
              (if (= n 1)
                  e
                  (list '^ e n)))
          (list '^ e n))));both vars
; (list '^ e n))
;(make-expt 1 2))
;(make-expt 7 7))

;expt? : Any -> Bool
(define (expt? a)
  (and (pair? a) (eq? (car a) '^)))
;(expt? '(^ 1 4))
;(expt? '(+ 1 4))

;;; derivative code

;deriv: ArithExp VarExp -> ArithExp
(define (deriv exp var)
  (cond ((number? exp)
         0)
        ((variable? exp)
         (if (variable=? exp var)
             1
             0))
        ((sum? exp) 
         (make-sum (deriv (arg1 exp) var)
                   (deriv (arg2 exp) var)))
        ((product? exp)
         (make-sum (make-product (arg1 exp) (deriv (arg2 exp) var))
                   (make-product (arg2 exp) (deriv (arg1 exp) var))))
        ((expt? exp)
         (if(= (arg2 exp) 1)
         (deriv (arg1 exp) var)
         (make-product (deriv (arg2 exp) var) (deriv (make-expt (arg1 exp) (- (arg2 exp) 1)) var))
         ))
        (else (error 'deriv "Unexpected Input, not an ArithExp"))))

; (= (deriv 1 'x) 0)
; (= (deriv 'y 'x) 0)
; (= (deriv 'x 'x) 1)

;1B-4
; make-sum: ArithExp ArithExp -> SumExp
(define (make-sum e1 e2)
  (if (number? e1)
      (if (number? e2)
          (list (+ e1 e2));both nums
          (if (= e1 0);e1 num, e2 var
              e2
              (list '+ e1 e2)))
      (if (number? e2)
          (if (= e2 0);e1 var, e2 num
              e1
              (list '+e1 e2))
          (list '+ e1 e2))));both var
;(make-sum 1 1)
;(make-sum 0 'x)

; make-product: ArithExp ArithExp -> ProductExp
(define (make-product e1 e2)
  (if (number? e1)
      (if(number? e2)
         (* e1 e2);both nums
         (if (= e1 0);e1 num, e2 var
             0
             (if (= e1 1)
                 e2
                 (list '* e1 e2))))
      (if(number? e2)
         (if (= e2 0);e1 var, e2 num
             0
             (if (= e2 1)
                 e1
                 (list '* e1 e2)))
         
         (list '* e1 e2))));both var
;(make-product 0 5)
;(make-product 'x 1)

;1B-5

;zero-poly? : List -> bool
(define (zero-poly? poly)
  (eq? poly '()))
;(zero-poly? '())
;(zero-poly? '(1 6 3))

;poly<-const : const -> List
(define (poly<-const num)
  (if(= num 0)
     '()
     (list num)))
;(poly<-const 0)
;(poly<-const 55)

;poly<-var : none -> List
(define (poly<-var)
  '(0 1))
;(poly<-var)

;shift-left : List -> List
(define (shift-left pol)
  (if (zero-poly? pol)
      '()
      (append '(0) pol)));1+2x+3x^2 = = = = = 1x+2x^2+3x^3 / / / / (1 2 3)->(0 1 2 3)
;(shift-left '(1 2))
;(shift-left '(5 2 1))

;shift-right : List -> List
(define (shift-right pol)
  (if (zero-poly? pol)
      '()
      (cdr pol)))
;(shift-right '(0 2 3))
;(shift-right (shift-left '(1 2)))

;const-coeff : List -> Real
(define (const-coeff pol)
  (if (zero-poly? pol)
      0
      (car pol)))
;(const-coeff '(1 2))
;(const-coeff '(5))

;add-const-poly : List Real-> List
(define (add-const-poly pol con)
  (if (zero-poly? pol)
      (list con)
      (append (list (+ con (car pol))) (cdr pol))))
;(add-const-poly '() 5)
;(add-const-poly '(1 2 3) 5)

;scale-poly : List num -> List
(define (scale-poly pol num)
  (if (zero-poly? pol)
      '()
      (if (= num 0)
          '()
          (append (list (* num (car pol))) (scale-poly (cdr pol) num)))))
;(scale-poly '(0 2) 5)
;(scale-poly '(5 2 1) 0)

;add-poly : List List -> List
(define (add-poly pol1 pol2)
  (if (zero-poly? pol1)
      pol2
      (if (zero-poly? pol2)
          pol1
          (add-const-poly (shift-left (add-poly (shift-right pol1) (shift-right pol2)))
                          (+ (car pol1) (car pol2))))))
;(add-poly '(4 2) '(5 4))
;(add-poly '() '(4 2))

;mult-poly : List List -> List
(define (mult-poly pol1 pol2)
  (if (or (zero-poly? pol1) (zero-poly? pol2))
      '()
      (add-poly (mult-poly (shift-right pol1) (shift-left pol2))
                (scale-poly pol2 (const-coeff pol1)))))
;(mult-poly '(2) '(2))
;(mult-poly '(1 2) '(1 2))

;expt-poly : List Real -> List
(define (expt-poly pol num)
  (if (and (integer? num) (>= num 0))
      (if (or (= num 0) (zero-poly? pol))
          '()
          (if (= num 1)
              pol
              (mult-poly pol (expt-poly pol (- num 1)))))
      (error message "exponent not a real")))
;(expt-poly '(1 1 1) 2)
;(expt-poly '(1 2) 0)

;poly<-exp : ArithExp var -> List
(define (poly<-exp exp var)
  (if (number? exp)
      (poly<-const exp)
      (if (symbol? exp)
          (if (eq? exp var)
              (poly<-var)
              (error message "too many variables"))
          (if (sum? exp)
              (add-poly (poly<-exp (arg1 exp) var) (poly<-exp (arg2 exp) var))
              (if (product? exp)
                  (mult-poly (poly<-exp (arg1 exp) var) (poly<-exp (arg2 exp) var))
                  (if (expt? exp)
                      (expt-poly (poly<-exp (arg1 exp) var) (car (poly<-exp (arg2 exp) var)))
                      (error message "not a valid expression")))))))
;(poly<-exp 'y 'y)
;(poly<-exp '(^ x 2) 'x)

;exp<-poly : List var -> ArithExp
(define (exp<-poly pol var)
  (if (zero-poly? pol)
      '()
      (build-exp pol var 0)))
;(exp<-poly '(1 2 3) 'x)
;(exp<-poly '() 'x)

;build-exp : List var Real -> ArithExp
(define (build-exp pol var count)
  (if (zero-poly? pol)
      0
      (make-sum (make-product (car pol) (make-expt var count))
                (build-exp (cdr pol) var (+ count 1)))))
;(build-exp '(0 0 2) 'x 0)
;(build-exp '(1 1 1) 'x 0)

;integrate-poly : List -> List
(define (integrate-poly pol)
  (if (zero-poly? pol)
      pol
      (shift-left (integ-help pol 1))))
;(integrate-poly '(2 4 6))
;(integrate-poly '())

;integ-help List Real -> Real
(define (integ-help pol count)
  (if (zero-poly? pol)
      '()
      (append (list (/ (car pol) count)) (integ-help (cdr pol) (+ count 1)))))
;(integ-help '() 1)
;(integ-help '(1 2 3) 1)

;integral : ArithExp var -> ArithExp
(define (integral exp var)
  (exp<-poly (integrate-poly (poly<-exp exp var)) var))
;(integral 'x 'x)
;(integral '(* x x) 'x)