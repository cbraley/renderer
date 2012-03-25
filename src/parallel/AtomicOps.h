#ifndef ATOMIC_OPS_H
#define ATOMIC_OPS_H

//https://github.com/mmp/pbrt-v2/blob/master/src/core/parallel.h

namespace AtomicOps{

// Parallel Declarations
    typedef volatile int32_t AtomicInt32;

    inline int32_t AtomicCompareAndSwap(AtomicInt32 *v, int32_t newValue, int32_t oldValue) {
        int32_t result;
        __asm__ __volatile__("lock\ncmpxchgl %2,%1"
                            : "=a"(result), "=m"(*v)
                            : "q"(newValue), "0"(oldValue)
                            : "memory");
        return result;
    }

    /**
     *   Credit for this atomic float add code goes to Matt Pharr.
     */
    inline float AtomicAdd(volatile float *val, float delta) {
        union bits { float f; int32_t i; };
        bits oldVal, newVal;
        do {
            // On IA32/x64, adding a PAUSE instruction in compare/exchange loops
            // is recommended to improve performance.  (And it does!)
#if (defined(__i386__) || defined(__amd64__))
            __asm__ __volatile__ ("pause\n");
#endif
            oldVal.f = *val;
            newVal.f = oldVal.f + delta;
        } while (AtomicCompareAndSwap(((AtomicInt32 *)val),
                                    newVal.i, oldVal.i) != oldVal.i);
        return newVal.f;
    }


}

#endif //ATOMIC_OPS_H
