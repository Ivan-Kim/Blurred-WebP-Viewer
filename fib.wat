(func (export "fibonacci_nth") (param $n i32) (result i32)
    ;; placeholder local variables (Effectively assembly registers)
    (local $i i32) ;; counter
    (local $t i32) ;; tmp
    (local $a i32) ;; prev fib1
    (local $b i32) ;; prev fib2

    ;; base case (n <= 0)
    (if (i32.le_u (get_local $n) (i32.const 0)) (return (i32.const 0)))

    (set_local $i (i32.const 1))
    (set_local $t (i32.const 0))
    ;; Base case fib0 and fib1
    (set_local $a (i32.const 0))
    (set_local $b (i32.const 1))

    ;; while loop
    (block 
        (loop 
            (set_local $t (i32.add (get_local $a) (get_local $b))) ;; tmp = a + b
            (set_local $a (get_local $b)) ;; a = b
            (set_local $b (get_local $t)) ;; b = tmp
            (set_local $i (i32.add (get_local $i) (i32.const 1))) ;; counter increment

            ;; check terminating condition (i >= n)
            (br_if 1 (i32.ge_u (get_local $i) (get_local $n)))
            (br 0)
        )
    )

    ;; push result to stack
    get_local $b
)