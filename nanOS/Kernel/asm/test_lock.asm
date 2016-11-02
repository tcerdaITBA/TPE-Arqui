GLOBAL test_lock

test_lock:
	push rbp
  mov rax, 0
  xchg rax, [rdi]
  cmp rax, 1
  je unlocked
  mov rax, 1
  pop rbp
	ret
unlocked:
  mov rax, 0
	pop rbp
	ret
