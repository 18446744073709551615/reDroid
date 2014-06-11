
// FIRST_UNUSED_BIT(x) -- first power-of-2 greater than x

#if 1

#define __OR_RSHIFT__(n,x) ((x)|(x)>>n)
#define FIRST_UNUSED_BIT(x) (1+__OR_RSHIFT__(16,__OR_RSHIFT__(8,__OR_RSHIFT__(4,__OR_RSHIFT__(2,__OR_RSHIFT__(1,x))))))

#else

#define FIRST_UNUSED_BIT(x) (1<<0x20-__builtin_clz(x))

#endif
