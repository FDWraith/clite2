compile:
	gcc test.c process.c -o testrun
	gcc main.c process.c -o clite2
runtest:
	./testrun
clite2:
	./clite2 tempfile
clear:
	rm tempfile
