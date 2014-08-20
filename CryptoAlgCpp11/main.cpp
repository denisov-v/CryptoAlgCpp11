#include "biglong.h"
#include "baseconvert.h"

#include <omp.h>
#include <cstdio>
#include <ctime>

using blong::biglong;

bool RabinMiller(const biglong& n, const biglong& base)
{
	if(n==biglong::one || !n.isOdd())	return 0;

	biglong n_1 = n.trunc_sub(biglong::one);
	biglong t = n_1;
	biglong s = biglong::zero;
	biglong q = biglong::zero;
	do
	{
		t.div(biglong::two, q);
		t = q;
		++s;
	} while(!t.isOdd());

	biglong a = base;
	biglong d = biglong::gcd(a, n);

	if(d > biglong::one) 
		return 0;

	a = a.power_mod(t,n);
	if(a==biglong::one || a==n_1) return 1;	

	biglong k = biglong::one;
	biglong s_1 = s.trunc_sub(biglong::one);
	while (k<=s_1)
	{
		a = a.power_mod(biglong::two,n);
		if(a==n_1) return 1;
		++k;
	}

	return 0;
}

biglong RoPollard(const biglong& n, const biglong& coeff, const biglong& seed)
{
	if(!n.isOdd()) return biglong::two;

	biglong a = seed;
	biglong b = seed;

	biglong d;
	biglong q;
	do
	{
		a = (coeff+a.power_mod(biglong::two,n)).div(n,q);
		b = (coeff+b.power_mod(biglong::two,n)).div(n,q);
		b = (coeff+b.power_mod(biglong::two,n)).div(n,q);
		
		if(a > b)
			d = biglong::gcd(a.trunc_sub(b), n);
		else
			d = biglong::gcd(b.trunc_sub(a), n);

		if(d == n)
			return biglong::zero;
		if(d > biglong::one)		
			return d;

	} while (d == biglong::one);

	return biglong::zero;
}

int main()
{
	/*cout << hex_to_dec_str((biglong(dec_to_hex_str("11247384611"))
		*biglong(dec_to_hex_str("71318207813"))
		*biglong(dec_to_hex_str("10918474913"))).toString()) << endl;*/
	
	/*biglong n = biglong(dec_to_hex_str("11247384611"))
		*biglong(dec_to_hex_str("71318207813"))
		*biglong(dec_to_hex_str("10918474913"));*/

	biglong n = biglong(util::dec_to_hex_str("10850315432251565671"));
	std::cout << "n = " << util::hex_to_dec_str(n.to_string()) << std::endl;

	/*std::string n_str;
	std::cout << "n=";
	std::cin >> n_str;
	biglong n = biglong(util::dec_to_hex_str(n_str));*/

	int num_procs = omp_get_num_procs();
	omp_set_num_threads(num_procs);
		
	srand(static_cast<unsigned int>(time(nullptr)));
	biglong* random = new biglong[3*num_procs];
	for(int i=0; i < 3*(num_procs); ++i)
		random[i] = biglong::get_random(n.trunc_sub(biglong::one))+1;

#pragma omp parallel shared(random, n)
	{
		int thread_num = omp_get_thread_num();
		char buffer [10];
		sprintf_s(buffer, "%d", thread_num);
		std::string thread_num_str = std::string(buffer);

		if(RabinMiller(n, random[thread_num])==0)
		{			
			std::cout << "thread " + thread_num_str + ": composite\n";
			std::cout << "thread " + thread_num_str + ": divider = " + util::hex_to_dec_str(
				RoPollard(n, random[thread_num+1], random[thread_num+2]).to_string()) + "\n";
		}
		else std::cout << "thread " + thread_num_str + ": probably prime\n";
	}
	
	delete[] random; 
	return 0;
}