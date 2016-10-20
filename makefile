LINK = link.exe

CC = cl.exe

parse_regular_expression.exe : parse_regular_expression.obj l_common.obj state.obj log.obj input.obj
	$(LINK) /OUT:parse_regular_expression.exe parse_regular_expression.obj l_common.obj state.obj log.obj input.obj

parse_regular_expression.obj : parse_regular_expression.c
	$(CC) /c parse_regular_expression.c parse_regular_expression.obj
l_common.obj : l_common.c
	$(CC) /c l_common.c l_common.obj
state.obj : state.c
	$(CC) /c state.c state.obj
log.obj : log.c
	$(CC) /c log.c log.obj
input.obj : input.c
	$(CC) /c input.c input.obj