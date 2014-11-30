#include "biglong.h"
#include "baseconvert.h"

#include <omp.h>
#include <cstdio>
#include <ctime>
#include <sstream>

using blong::biglong;

bool RabinMiller(const biglong& n, const biglong& base);
biglong RhoPollard(const biglong& n, const biglong& coeff, const biglong& seed);

int main()
{
	biglong n = biglong(baseconvert::dec_to_hex_str("8758181640058340640277655505359"));
	std::cout << "n = " << baseconvert::hex_to_dec_str(n.to_string()) << std::endl;

	/*std::string n_str;
	std::cout << "n=";
	std::cin >> n_str;
	biglong n = biglong(util::dec_to_hex_str(n_str));*/

	int num_procs = omp_get_num_procs();
	omp_set_num_threads(num_procs);

	srand(static_cast<unsigned int>(time(nullptr)));
	biglong* random = new biglong[3 * num_procs];
	for (int i = 0; i < 3 * (num_procs); ++i)
		random[i] = biglong::get_random(n.trunc_sub(biglong::one)) + 1;

#pragma omp parallel shared(random, n)
	{
		int thread_num = omp_get_thread_num();
		std::stringstream strstream;

		if (RabinMiller(n, random[thread_num]) == 0)
		{
			strstream << "thread " << thread_num << ": composite" << std::endl;
			std::cout << strstream.str();
			strstream.str(std::string());
			strstream.clear();

			double rho_start_time = omp_get_wtime();
			biglong divider = RhoPollard(n, random[thread_num + 1], random[thread_num + 2]);
			double rho_end_time = omp_get_wtime();

			strstream << "thread " << thread_num << ": divider = " << baseconvert::hex_to_dec_str(divider.to_string())
				<< ", time = " << rho_end_time - rho_start_time << " seconds" << std::endl;
		}
		else strstream << "thread " << thread_num << ": probably prime" << std::endl;

		std::cout << strstream.str();
	}

	delete[] random;
	return 0;
}

bool RabinMiller(const biglong& n, const biglong& base)
{
	if (n == biglong::one || !n.isOdd())	return 0;

	biglong n_1 = n.trunc_sub(biglong::one);
	biglong t = n_1;
	biglong s = biglong::zero;
	biglong q = biglong::zero;
	do
	{
		t.div(biglong::two, q);
		t = q;
		++s;
	} while (!t.isOdd());

	biglong a = base;
	biglong d = biglong::gcd(a, n);

	if (d > biglong::one)
		return 0;

	a = a.power_mod(t, n);
	if (a == biglong::one || a == n_1) return 1;

	biglong k = biglong::one;
	biglong s_1 = s.trunc_sub(biglong::one);
	while (k <= s_1)
	{
		a = a.power_mod(biglong::two, n);
		if (a == n_1) return 1;
		++k;
	}

	return 0;
}

biglong RhoPollard(const biglong& n, const biglong& coeff, const biglong& seed)
{
	if (!n.isOdd()) return biglong::two;

	biglong a = seed;
	biglong b = seed;

	biglong d;
	biglong q;
	do
	{
		a = (coeff + a.power_mod(biglong::two, n)).div(n, q);
		b = (coeff + b.power_mod(biglong::two, n)).div(n, q);
		b = (coeff + b.power_mod(biglong::two, n)).div(n, q);

		if (a > b)
			d = biglong::gcd(a.trunc_sub(b), n);
		else
			d = biglong::gcd(b.trunc_sub(a), n);

		if (d == n)
			return biglong::zero;
		if (d > biglong::one)
			return d;

	} while (d == biglong::one);

	return biglong::zero;
}