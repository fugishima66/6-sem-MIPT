EXEC = trans
SRC = main.c
OUTDIR = build

CFLAGS = -std=c99 -g

all:
	mkdir -p ${OUTDIR}
	mpicc ${CFLAGS} ${SRC} -o ${OUTDIR}/${EXEC}
clean:
	rm ${OUTDIR}/*