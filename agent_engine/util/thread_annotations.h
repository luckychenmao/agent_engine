#pragma once

// ported from tensorflow tensorflow/core/platform/default/thread_annotations.h
// reference: https://clang.llvm.org/docs/ThreadSafetyAnalysis.html

/* #if defined(__clang__) */
/* // #warning thread_annotations enabled */
/* #define UTIL_THREAD_ANNOTATION_ATTRIBUTE__(x) __attribute__((x)) */
/* #else */
/* // #warning thread_annotations disabled */
/* #define UTIL_THREAD_ANNOTATION_ATTRIBUTE__(x) */
/* #endif */

// Document if a shared variable/field needs to be protected by a mutex.
// GUARDED_BY allows the user to specify a particular mutex that should be
// held when accessing the annotated variable.  GUARDED_VAR indicates that
// a shared variable is guarded by some unspecified mutex, for use in rare
// cases where a valid mutex expression cannot be specified.
#if defined(__clang__)
#define UTIL_GUARDED_BY(x) __attribute__((guarded_by(x)))
#else
#define UTIL_GUARDED_BY(x)
#endif

// Document if the memory location pointed to by a pointer should be guarded
// by a mutex when dereferencing the pointer.  PT_GUARDED_VAR is analogous to
// GUARDED_VAR.   Note that a pointer variable to a shared memory location
// could itself be a shared variable. For example, if a shared global pointer
// q, which is guarded by mu1, points to a shared memory location that is
// guarded by mu2, q should be annotated as follows:
//     int *q GUARDED_BY(mu1) PT_GUARDED_BY(mu2);
#if defined(__clang__)
#define UTIL_PT_GUARDED_BY(x) __attribute__((pt_guarded_by(x)))
#else
#define UTIL_PT_GUARDED_BY(x)
#endif

// Document the acquisition order between locks that can be held
// simultaneously by a thread. For any two locks that need to be annotated
// to establish an acquisition order, only one of them needs the annotation.
// (i.e. You don't have to annotate both locks with both ACQUIRED_AFTER
// and ACQUIRED_BEFORE.)
#if defined(__clang__)

#define UTIL_ACQUIRED_AFTER(...) __attribute__((acquired_after(__VA_ARGS__)))
#define UTIL_ACQUIRED_BEFORE(...) __attribute__((acquired_before(__VA_ARGS__)))
#define UTIL_ACQUIRE(...) __attribute__((acquire_capability(__VA_ARGS__)))
#define UTIL_ACQUIRE_SHARED(...) __attribute__((acquire_shared_capability(__VA_ARGS__)))
#define UTIL_RELEASE(...) __attribute__((release_capability(__VA_ARGS__)))

#else

#define UTIL_ACQUIRED_AFTER(...)
#define UTIL_ACQUIRED_BEFORE(...)
#define UTIL_ACQUIRE(...)
#define UTIL_ACQUIRE_SHARED(...)
#define UTIL_RELEASE(...)

#endif

// Document a function that expects a mutex to be held prior to entry.
// The mutex is expected to be held both on entry to and exit from the
// function.
#if defined(__clang__)

#define UTIL_EXCLUSIVE_LOCKS_REQUIRED(...) __attribute__((exclusive_locks_required(__VA_ARGS__)))
#define UTIL_SHARED_LOCKS_REQUIRED(...) __attribute__((shared_locks_required(__VA_ARGS__)))

#else

#define UTIL_EXCLUSIVE_LOCKS_REQUIRED(...)
#define UTIL_SHARED_LOCKS_REQUIRED(...)

#endif

// Document the locks acquired in the body of the function. These locks
// cannot be held when calling this function (for instance, when the
// mutex implementation is non-reentrant).
#if defined(__clang__)
#define UTIL_LOCKS_EXCLUDED(...) __attribute__((locks_excluded(__VA_ARGS__)))
#else
#define UTIL_LOCKS_EXCLUDED(...)
#endif

// Document a function that returns a mutex without acquiring it.  For example,
// a public getter method that returns a pointer to a private mutex should
// be annotated with LOCK_RETURNED.
#if defined(__clang__)
#define UTIL_LOCK_RETURNED(x) __attribute__((lock_returned(x)))
#else
#define UTIL_LOCK_RETURNED(...)
#endif

// Document if a class/type is a lockable type (such as the Mutex class).
#if defined(__clang__)
#define UTIL_LOCKABLE __attribute__((lockable))
#else
#define UTIL_LOCKABLE
#endif

// Document if a class does RAII locking (such as the MutexLock class).
// The constructor should use LOCK_FUNCTION to specify the mutex that is
// acquired, and the destructor should use UNLOCK_FUNCTION with no arguments;
// the analysis will assume that the destructor unlocks whatever the
// constructor locked.
#if defined(__clang__)
#define UTIL_SCOPED_LOCKABLE __attribute__((scoped_lockable))
#else
#define UTIL_SCOPED_LOCKABLE
#endif

// Document functions that acquire a lock in the body of a function, and do
// not release it.
#if defined(__clang__)
#define UTIL_EXCLUSIVE_LOCK_FUNCTION(...) __attribute__((exclusive_lock_function(__VA_ARGS__)))
#else
#define UTIL_EXCLUSIVE_LOCK_FUNCTION(...)
#endif

#if defined(__clang__)
#define UTIL_SHARED_LOCK_FUNCTION(...) __attribute__((shared_lock_function(__VA_ARGS__)))
#else
#define UTIL_SHARED_LOCK_FUNCTION(...)
#endif

// Document functions that expect a lock to be held on entry to the function,
// and release it in the body of the function.
#if defined(__clang__)
#define UTIL_UNLOCK_FUNCTION(...) __attribute__((unlock_function(__VA_ARGS__)))
#else
#define UTIL_UNLOCK_FUNCTION(...)
#endif

// Document functions that try to acquire a lock, and return success or failure
// (or a non-boolean value that can be interpreted as a boolean).
// The first argument should be true for functions that return true on success,
// or false for functions that return false on success. The second argument
// specifies the mutex that is locked on success. If unspecified, it is assumed
// to be 'this'.
#if defined(__clang__)
#define UTIL_EXCLUSIVE_TRYLOCK_FUNCTION(...) __attribute__((exclusive_trylock_function(__VA_ARGS__)))
#else
#define UTIL_EXCLUSIVE_TRYLOCK_FUNCTION(...)
#endif

#if defined(__clang__)
#define UTIL_SHARED_TRYLOCK_FUNCTION(...) __attribute__((shared_trylock_function(__VA_ARGS__)))
#else
#define UTIL_SHARED_TRYLOCK_FUNCTION(...)
#endif

// Document functions that dynamically check to see if a lock is held, and fail
// if it is not held.
#if defined(__clang__)
#define UTIL_ASSERT_EXCLUSIVE_LOCK(...) __attribute__((assert_exclusive_lock(__VA_ARGS__)))
#define UTIL_ASSERT_SHARED_LOCK(...) __attribute__((assert_shared_lock(__VA_ARGS__)))
#else
#define UTIL_ASSERT_EXCLUSIVE_LOCK(...)
#define UTIL_ASSERT_SHARED_LOCK(...)
#endif

// Turns off thread safety checking within the body of a particular function.
// This is used as an escape hatch for cases where either (a) the function
// is correct, but the locking is more complicated than the analyzer can handle,
// or (b) the function contains race conditions that are known to be benign.
#if defined(__clang__)
#define UTIL_NO_THREAD_SAFETY_ANALYSIS __attribute__((no_thread_safety_analysis))
#else
#define UTIL_NO_THREAD_SAFETY_ANALYSIS
#endif

// TS_UNCHECKED should be placed around lock expressions that are not valid
// C++ syntax, but which are present for documentation purposes.  These
// annotations will be ignored by the analysis.
#if defined(__clang__)
#define UTIL_TS_UNCHECKED(x) ""
#else
#define UTIL_TS_UNCHECKED(x)
#endif

/* #undef UTIL_THREAD_ANNOTATION_ATTRIBUTE__ */
