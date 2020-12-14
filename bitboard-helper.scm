(import pstk
        (chicken bitwise)
        (format))

(tk-start)

(tk/wm 'attributes tk "-type" "utility")
(tk/wm 'title tk "Chess Magic Number Finder")

(define current-hexcode 0)

(define (rank-file->hexcode rank file)
  (arithmetic-shift 1 (+ (* rank 8) file)))

(define (toggle-hexcode rank file)
  (set! current-hexcode (bitwise-xor current-hexcode
                                     (rank-file->hexcode rank file))))

(define label (tk 'create-widget 'label 'text: "#x0000000000000000"))
(tk/pack label)

(define board (tk 'create-widget 'frame))
(tk/pack board)

(let loop ((rank 0)
           (file 0))
  (tk/grid (board 'create-widget 'checkbutton
                  'command:
                  (lambda ()
                    (toggle-hexcode rank file)
                    (let ((magic-number (format "#x~16,'0x" current-hexcode)))
                      (label 'configure 'text: magic-number)
                      (tk/clipboard 'clear)
                      (tk/clipboard 'append magic-number))))
           'row: (- 7 rank) 'column: file)
  (cond
   ((and (= rank 7) (= file 7)) '())
   ((= file 7) (loop (add1 rank) 0))
   (else (loop rank (add1 file)))))

(tk-event-loop)
