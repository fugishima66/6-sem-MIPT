EXEC = transeq
SRC = main.c
OUTDIR = build

CFLAGS = -lm -ldl -g

all:
	mkdir -p ${OUTDIR}
	mpicc ${SRC} -o ${OUTDIR}/${EXEC} ${CFLAGS}
clean:
	rm ${OUTDIR}/*