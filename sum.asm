option casemap:none

.data

	fmt 	byte "Sum of 123 and 234 is %llu\n", 0
	num0 	qword 123
	num1 	qword 234
	
.code

	externdef printf:proc
	
	main proc
		sub rsp, 56	
		
		; summation part
		mov r10, num0
		add r10, num1
		; summation end
		
		lea rcx, fmt
		mov rdx, r10
		call printf
		add rsp, 56
		ret
	main endp

end