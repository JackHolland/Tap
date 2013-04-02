def append(ls, val):
	new_ls = list(ls)
	new_ls.append(val)
	return new_ls

sources = ['source/arrays.c', 'source/casting.c', 'source/constructors.c', 'source/dates.c', 'source/debug.c', 'source/engine.c', 'source/hashtable.c', 'source/memory.c', 'primitives/prim_arr.c', 'primitives/prim_dat.c', 'primitives/prim_exp.c', 'primitives/prim_flo.c', 'primitives/prim_fun.c', 'primitives/prim_int.c', 'primitives/prim_laz.c', 'primitives/prim_nil.c', 'primitives/prim_obj.c', 'primitives/prim_str.c', 'primitives/prim_typ.c', 'source/strings.c', 'source/types.c']

env = Environment(CC = 'gcc', CCFLAGS = ['-O2', '-Wall'], LINKFLAGS = '-lm')
env.Program('tap', append(sources, 'source/main.c'))

env.Append(LIBS = 'cspec', LIBPATH = 'testing/')
env.Program('source/tests/arrays_test', append(sources, 'source/tests/arrays_test.c'))