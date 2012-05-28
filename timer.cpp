unsigned long long timer() {
  __asm__ __volatile__(
	   "CPUID\n"
	   "rdtsc\n"
	   );
};
