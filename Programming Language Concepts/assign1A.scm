;1A-1
;2nd : L -> element
(define (2nd L)
  (if(null? L)
     '()
     (if(null? (cdr L)); if null there is no second element
        '()
        (car (cdr L)))))
;(2nd '(x y z))
;(2nd '())

;1A-2
;one? : L -> bool
(define (one? L)
  (if(null? L); 0 isn't 1
     #f
     (if(null? (cdr L)); if the cdr exists, there is at least 2 elements
        #t
        #f)))
;(one? '())
;(one? '(a))
;(one? '(a b))

;1A-3
; insert : Real List(Real) -> List(Real)
(define (insert L Lis)
  (if (null? Lis)
      (list L);if the "sorted list" paramater is null, return only the element
      (if (< L (car Lis))
          (cons L Lis); if element is less than first element in the list, it goes first
          (cons (car Lis) (insert L (cdr Lis))))));otherwise find its place recursively
;(insert 3 '(4 5))
;(insert 3 '(2 4))

; insertion-sort : List(Real) -> List(Real)
(define (insertion-sort nums)
  (if(null? nums); if null, already sorted
     '()
     (insert (car nums) (insertion-sort (cdr nums)))))
        ;call subroutine with an element and a sorted result from recursion
;(insertion-sort '(5 1 4 3 2 6 5))
;(insertion-sort '(9 8 7 6 5 4 3 2 1))

;1A-4
;ind-num : List List symbol -> Real
(define (ind-num all left sym);returns index of the symbol
  (if (null? left)
      #f;if recursive call results in empty list, no occurence of sym
      (if (eq? sym (car left))
          (- (length all) (length left));return difference in length since start
          (ind-num all (cdr left) sym))))
;(ind-num '(x y) '(x y) 'x)
;(ind-num '(x y) '(x y) 'y)

;index : symbol List(symbol) -> #f or Real
(define (index sym syms)
  (if (null? syms)
      #f; empty list cannot have the target sym
      (ind-num syms syms sym)));call subroutine that can track the difference in
       ;length from the original to the final list
;(index 'x '(q r s x y z))
;(index 's '(q r s x y z))

;1A-5
;filter-by : predicate List -> List
(define (filter-by p L)
  (if (null? L)
      '()
      (if (p (car L))
          (cons (car L) (filter-by p (cdr L)))
          (filter-by p (cdr L)))))
;(filter-by number? '(() a 5 (1 2 3) (x y z) b 10))
;(filter-by symbol? '(() a 5 (1 2 3) (x y z) b 10))

;compK : function element -> predicate
(define (compK p k);can be any 2 input bool returning func
  (lambda (x) (p x k)))
;((compK < 4) 5)
;((compK < 4) 3)

;find-less : Real List(Real) -> List(Real)
(define (find-less n nums)
  (if (null? nums)
      '()
      (filter-by (compK < n) nums)))
;(find-less 5 '(1 2 3 4 5 6 7))
;(find-less 5 '(5 6 7))

;find-same : Real List(Real) -> List(Real)
(define (find-same n nums)
  (if (null? nums)
      '()
      (filter-by (compK = n) nums)))
;(find-same 5 '(1 2 3 4 5 6 7))
;(find-same 5 '(5 6 7))

;find-more : Real List(Real) -> List(Real)
(define (find-more n nums)
  (if (null? nums)
      '()
      (filter-by (compK > n) nums)))
;(find-more 5 '(1 2 3 4 5 6 7))
;(find-more 5 '(5 6 7))