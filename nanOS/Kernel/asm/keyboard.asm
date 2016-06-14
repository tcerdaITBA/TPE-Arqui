GLOBAL get_key

get_key:
	in al, 60h
	movzx rax, al
	ret
