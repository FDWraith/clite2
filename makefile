compile:
	gcc test.c process.c execute.c -o testrun
	gcc main.c process.c execute.c -o clite2
runtest:
	./testrun
clite2:
	./clite2 tempfile
clear:
	rm tempfile
