(import pstk
	(chicken process)
	(chicken port)
	(chicken io))

(define-values (fin fout id)
  (process "./othello"))

(tk-start)

(tk/wm 'attributes tk "-type" "utility")
(tk/wm 'title tk "Othello GUI")

(define *size* 20)

(define board (tk 'create-widget 'frame))
(tk/pack board)

(define canvas (board 'create-widget 'canvas))
(tk/pack canvas)

(let loop ([i 1])
  (when (<= i 8) 
    (let inner-loop ([j 1])
      (when (<= j 8)
	(let ([space (canvas 'create 'rectangle (* i *size*) (* j *size*)
			     (* (+ i 1) *size*) (* (+ j 1) *size*)
			     'fill: 'green)])
	  (canvas 'bind space '<Button> (lambda () (display (number->string (+ (* (- 8 j) 8) (- i 1))) fout) (newline fout) (flush-output fout) (io-queue))))
	(inner-loop (+ j 1))))
    (loop (+ i 1))))

(define (draw-disk space color)
  (let ([x (* (+ 1 (remainder space 8)) *size*)]
	[y (* (+ 1 (- 7 (quotient space 8))) *size*)])
    (canvas 'create 'oval x y (+ x *size*) (+ y *size*) 'fill: color 'tags: 'disk)))

(draw-disk 0 'white)

(define (clear)
  (canvas 'delete 'disk))

(define (parse-fen fen)
  (clear)
  (let loop ([i 0]
	     [p 0])
    (when (< i (string-length fen))
      (let ([c (string-ref fen i)])
	(cond
	 [(<= 1 (- (char->integer c) (char->integer #\0)) 8)
	  (loop (+ i 1) (+ p (- (char->integer c) (char->integer #\0))))]
	 [(eq? #\W c)
	  (draw-disk p 'white)
	  (loop (+ i 1) (+ p 1))]
	 [(eq? #\B c)
	  (draw-disk p 'black)
	  (loop (+ i 1) (+ p 1))]
	 [(eq? #\space c) '()]
	 [else (loop (+ i 1) p)])))))

(define (io-queue)
  (tk/after 100
	    (lambda ()
	      (let redo ([fen (read-line fin)])
		(display fen)
		(newline)
		(if (string=? fen "Illegal move!")
		    (redo (read-line fin))
		    (parse-fen fen))))))
(io-queue)

(tk-event-loop)
