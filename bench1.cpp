// bench1.cpp - originally written and placed in the public domain by Wei Dai
//              CryptoPP::Test namespace added by JW in February 2017

#include "cryptlib.h"
#include "bench.h"
#include "validate.h"

#include "aes.h"
#include "blumshub.h"
#include "files.h"
#include "filters.h"
#include "hex.h"
#include "modes.h"
#include "factory.h"
#include "smartptr.h"
#include "cpu.h"

#include <time.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include <iomanip>

NAMESPACE_BEGIN(CryptoPP)
NAMESPACE_BEGIN(Test)

double logTotal = 0.0, g_allocatedTime = 0, g_hertz = 0;
unsigned int logCount = 0;

void OutputResultBytes(const char *name, double length, double timeTaken)
{
	// Coverity finding, also see http://stackoverflow.com/a/34509163/608639.
	StreamState ss(std::cout);

	// Coverity finding
	if (length < 0.0000000001f) length = 0.000001f;
	if (timeTaken < 0.0000000001f) timeTaken = 0.000001f;

	double mbs = length / timeTaken / (1024*1024);
	std::cout << "\n<TR><TH>" << name;
//	std::cout << "<TD>" << std::setprecision(3) << length / (1024*1024);
	std::cout << std::setiosflags(std::ios::fixed);
//	std::cout << "<TD>" << std::setprecision(3) << timeTaken;
	std::cout << "<TD>" << std::setprecision(0) << std::setiosflags(std::ios::fixed) << mbs;
	if (g_hertz)
		std::cout << "<TD>" << std::setprecision(1) << std::setiosflags(std::ios::fixed) << timeTaken * g_hertz / length;
	logTotal += log(mbs);
	logCount++;
}

void OutputResultKeying(double iterations, double timeTaken)
{
	// Coverity finding, also see http://stackoverflow.com/a/34509163/608639.
	StreamState ss(std::cout);

	// Coverity finding
	if (iterations < 0.0000000001f) iterations = 0.000001f;
	if (timeTaken < 0.0000000001f) timeTaken = 0.000001f;

	std::cout << "<TD>" << std::setprecision(3) << std::setiosflags(std::ios::fixed) << (1000*1000*timeTaken/iterations);
	if (g_hertz)
		std::cout << "<TD>" << std::setprecision(0) << std::setiosflags(std::ios::fixed) << timeTaken * g_hertz / iterations;
}

void OutputResultOperations(const char *name, const char *operation, bool pc, unsigned long iterations, double timeTaken)
{
	// Coverity finding, also see http://stackoverflow.com/a/34509163/608639.
	StreamState ss(std::cout);

	// Coverity finding
	if (!iterations) iterations++;
	if (timeTaken < 0.0000000001f) timeTaken = 0.000001f;

	std::cout << "\n<TR><TH>" << name << " " << operation << (pc ? " with precomputation" : "");
	std::cout << "<TD>" << std::setprecision(2) << std::setiosflags(std::ios::fixed) << (1000*timeTaken/iterations);
	if (g_hertz)
		std::cout << "<TD>" << std::setprecision(2) << std::setiosflags(std::ios::fixed) << timeTaken * g_hertz / iterations / 1000000;

	logTotal += log(iterations/timeTaken);
	logCount++;
}

/*
void BenchMark(const char *name, BlockTransformation &cipher, double timeTotal)
{
	const int BUF_SIZE = RoundUpToMultipleOf(2048U, cipher.OptimalNumberOfParallelBlocks() * cipher.BlockSize());
	AlignedSecByteBlock buf(BUF_SIZE);
	const int nBlocks = BUF_SIZE / cipher.BlockSize();
	clock_t start = clock();

	unsigned long i=0, blocks=1;
	double timeTaken;
	do
	{
		blocks *= 2;
		for (; i<blocks; i++)
			cipher.ProcessAndXorMultipleBlocks(buf, NULLPTR, buf, nBlocks);
		timeTaken = double(clock() - start) / CLOCK_TICKS_PER_SECOND;
	}
	while (timeTaken < 2.0/3*timeTotal);

	OutputResultBytes(name, double(blocks) * BUF_SIZE, timeTaken);
}
*/

void BenchMark(const char *name, StreamTransformation &cipher, double timeTotal)
{
	const int BUF_SIZE=RoundUpToMultipleOf(2048U, cipher.OptimalBlockSize());
	AlignedSecByteBlock buf(BUF_SIZE);
	Test::GlobalRNG().GenerateBlock(buf, BUF_SIZE);
	clock_t start = clock();

	unsigned long i=0, blocks=1;
	double timeTaken;
	do
	{
		blocks *= 2;
		for (; i<blocks; i++)
			cipher.ProcessString(buf, BUF_SIZE);
		timeTaken = double(clock() - start) / CLOCK_TICKS_PER_SECOND;
	}
	while (timeTaken < 2.0/3*timeTotal);

	OutputResultBytes(name, double(blocks) * BUF_SIZE, timeTaken);
}

void BenchMark(const char *name, AuthenticatedSymmetricCipher &cipher, double timeTotal)
{
	if (cipher.NeedsPrespecifiedDataLengths())
		cipher.SpecifyDataLengths(0, cipher.MaxMessageLength(), 0);

	BenchMark(name, static_cast<StreamTransformation &>(cipher), timeTotal);
}

void BenchMark(const char *name, HashTransformation &ht, double timeTotal)
{
	const int BUF_SIZE=2048U;
	AlignedSecByteBlock buf(BUF_SIZE);
	Test::GlobalRNG().GenerateBlock(buf, BUF_SIZE);
	clock_t start = clock();

	unsigned long i=0, blocks=1;
	double timeTaken;
	do
	{
		blocks *= 2;
		for (; i<blocks; i++)
			ht.Update(buf, BUF_SIZE);
		timeTaken = double(clock() - start) / CLOCK_TICKS_PER_SECOND;
	}
	while (timeTaken < 2.0/3*timeTotal);

	OutputResultBytes(name, double(blocks) * BUF_SIZE, timeTaken);
}

void BenchMark(const char *name, BufferedTransformation &bt, double timeTotal)
{
	const int BUF_SIZE=2048U;
	AlignedSecByteBlock buf(BUF_SIZE);
	Test::GlobalRNG().GenerateBlock(buf, BUF_SIZE);
	clock_t start = clock();

	unsigned long i=0, blocks=1;
	double timeTaken;
	do
	{
		blocks *= 2;
		for (; i<blocks; i++)
			bt.Put(buf, BUF_SIZE);
		timeTaken = double(clock() - start) / CLOCK_TICKS_PER_SECOND;
	}
	while (timeTaken < 2.0/3*timeTotal);

	OutputResultBytes(name, double(blocks) * BUF_SIZE, timeTaken);
}

void BenchMarkKeying(SimpleKeyingInterface &c, size_t keyLength, const NameValuePairs &params)
{
	unsigned long iterations = 0;
	clock_t start = clock();
	double timeTaken;
	do
	{
		for (unsigned int i=0; i<1024; i++)
			c.SetKey(defaultKey, keyLength, params);
		timeTaken = double(clock() - start) / CLOCK_TICKS_PER_SECOND;
		iterations += 1024;
	}
	while (timeTaken < g_allocatedTime);

	OutputResultKeying(iterations, timeTaken);
}

template <class T_FactoryOutput, class T_Interface>
void BenchMarkByName2(const char *factoryName, size_t keyLength = 0, const char *displayName=NULLPTR, const NameValuePairs &params = g_nullNameValuePairs)
{
	CRYPTOPP_UNUSED(params);
	std::string name(factoryName ? factoryName : "");
	member_ptr<T_FactoryOutput> obj(ObjectFactoryRegistry<T_FactoryOutput>::Registry().CreateObject(name.c_str()));

	if (!keyLength)
		keyLength = obj->DefaultKeyLength();

	if (displayName)
		name = displayName;
	else if (keyLength)
		name += " (" + IntToString(keyLength * 8) + "-bit key)";

	obj->SetKey(defaultKey, keyLength, CombinedNameValuePairs(params, MakeParameters(Name::IV(), ConstByteArrayParameter(defaultKey, obj->IVSize()), false)));
	BenchMark(name.c_str(), *static_cast<T_Interface *>(obj.get()), g_allocatedTime);
	BenchMarkKeying(*obj, keyLength, CombinedNameValuePairs(params, MakeParameters(Name::IV(), ConstByteArrayParameter(defaultKey, obj->IVSize()), false)));
}

template <class T_FactoryOutput>
void BenchMarkByName(const char *factoryName, size_t keyLength = 0, const char *displayName=NULLPTR, const NameValuePairs &params = g_nullNameValuePairs)
{
	CRYPTOPP_UNUSED(params);
	BenchMarkByName2<T_FactoryOutput, T_FactoryOutput>(factoryName, keyLength, displayName, params);
}

template <class T>
void BenchMarkByNameKeyLess(const char *factoryName, const char *displayName=NULLPTR, const NameValuePairs &params = g_nullNameValuePairs)
{
	CRYPTOPP_UNUSED(params);
	std::string name = factoryName;
	if (displayName)
		name = displayName;

	member_ptr<T> obj(ObjectFactoryRegistry<T>::Registry().CreateObject(factoryName));
	BenchMark(name.c_str(), *obj, g_allocatedTime);
}

void BenchmarkAll(double t, double hertz)
{
#if 1
	logTotal = 0;
	logCount = 0;
	g_allocatedTime = t;
	g_hertz = hertz;

	const char *cpb, *cpk;
	if (g_hertz)
	{
		cpb = "<TH>Cycles Per Byte";
		cpk = "<TH>Cycles to<br>Setup Key and IV";
		std::cout << "CPU frequency of the test platform is " << g_hertz << " Hz.\n";
	}
	else
	{
		cpb = cpk = "";
		std::cout << "CPU frequency of the test platform was not provided.\n";
	}

	std::cout << "<TABLE border=1><COLGROUP><COL align=left><COL align=right><COL align=right><COL align=right><COL align=right>" << std::endl;
	std::cout << "<THEAD><TR><TH>Algorithm<TH>MiB/Second" << cpb << "<TH>Microseconds to<br>Setup Key and IV" << cpk << std::endl;

	std::cout << "\n<TBODY style=\"background: yellow\">";
#if CRYPTOPP_BOOL_AESNI_INTRINSICS_AVAILABLE
	if (HasCLMUL())
		BenchMarkByName2<AuthenticatedSymmetricCipher, AuthenticatedSymmetricCipher>("AES/GCM", 0, "AES/GCM");
	else
#elif CRYPTOPP_BOOL_ARM_PMULL_AVAILABLE
	if (HasPMULL())
		BenchMarkByName2<AuthenticatedSymmetricCipher, AuthenticatedSymmetricCipher>("AES/GCM", 0, "AES/GCM");
	else
#endif
	{
		BenchMarkByName2<AuthenticatedSymmetricCipher, AuthenticatedSymmetricCipher>("AES/GCM", 0, "AES/GCM (2K tables)", MakeParameters(Name::TableSize(), 2048));
		BenchMarkByName2<AuthenticatedSymmetricCipher, AuthenticatedSymmetricCipher>("AES/GCM", 0, "AES/GCM (64K tables)", MakeParameters(Name::TableSize(), 64*1024));
	}
	BenchMarkByName2<AuthenticatedSymmetricCipher, AuthenticatedSymmetricCipher>("AES/CCM");
	BenchMarkByName2<AuthenticatedSymmetricCipher, AuthenticatedSymmetricCipher>("AES/EAX");

	std::cout << "\n<TBODY style=\"background: white\">";
#if CRYPTOPP_BOOL_AESNI_INTRINSICS_AVAILABLE
	if (HasCLMUL())
		BenchMarkByName2<AuthenticatedSymmetricCipher, MessageAuthenticationCode>("AES/GCM", 0, "GMAC(AES)");
	else
#endif
	{
		BenchMarkByName2<AuthenticatedSymmetricCipher, MessageAuthenticationCode>("AES/GCM", 0, "GMAC(AES) (2K tables)", MakeParameters(Name::TableSize(), 2048));
		BenchMarkByName2<AuthenticatedSymmetricCipher, MessageAuthenticationCode>("AES/GCM", 0, "GMAC(AES) (64K tables)", MakeParameters(Name::TableSize(), 64*1024));
	}
	BenchMarkByName<MessageAuthenticationCode>("VMAC(AES)-64");
	BenchMarkByName<MessageAuthenticationCode>("VMAC(AES)-128");
	BenchMarkByName<MessageAuthenticationCode>("HMAC(SHA-1)");
	BenchMarkByName<MessageAuthenticationCode>("Two-Track-MAC");
	BenchMarkByName<MessageAuthenticationCode>("CMAC(AES)");
	BenchMarkByName<MessageAuthenticationCode>("DMAC(AES)");
	BenchMarkByName<MessageAuthenticationCode>("Poly1305(AES)");
	BenchMarkByName<MessageAuthenticationCode>("BLAKE2s");
	BenchMarkByName<MessageAuthenticationCode>("BLAKE2b");
	BenchMarkByName<MessageAuthenticationCode>("SipHash-2-4");
	BenchMarkByName<MessageAuthenticationCode>("SipHash-4-8");

	std::cout << "\n<TBODY style=\"background: yellow\">";
	BenchMarkByNameKeyLess<HashTransformation>("CRC32");
	BenchMarkByNameKeyLess<HashTransformation>("CRC32C");
	BenchMarkByNameKeyLess<HashTransformation>("Adler32");
	BenchMarkByNameKeyLess<HashTransformation>("MD5");
	BenchMarkByNameKeyLess<HashTransformation>("SHA-1");
	BenchMarkByNameKeyLess<HashTransformation>("SHA-256");
	BenchMarkByNameKeyLess<HashTransformation>("SHA-512");
	BenchMarkByNameKeyLess<HashTransformation>("Keccak-224");
	BenchMarkByNameKeyLess<HashTransformation>("Keccak-256");
	BenchMarkByNameKeyLess<HashTransformation>("Keccak-384");
	BenchMarkByNameKeyLess<HashTransformation>("Keccak-512");
	BenchMarkByNameKeyLess<HashTransformation>("SHA3-224");
	BenchMarkByNameKeyLess<HashTransformation>("SHA3-256");
	BenchMarkByNameKeyLess<HashTransformation>("SHA3-384");
	BenchMarkByNameKeyLess<HashTransformation>("SHA3-512");
	BenchMarkByNameKeyLess<HashTransformation>("Tiger");
	BenchMarkByNameKeyLess<HashTransformation>("Whirlpool");
	BenchMarkByNameKeyLess<HashTransformation>("RIPEMD-160");
	BenchMarkByNameKeyLess<HashTransformation>("RIPEMD-320");
	BenchMarkByNameKeyLess<HashTransformation>("RIPEMD-128");
	BenchMarkByNameKeyLess<HashTransformation>("RIPEMD-256");
	BenchMarkByNameKeyLess<HashTransformation>("BLAKE2s");
	BenchMarkByNameKeyLess<HashTransformation>("BLAKE2b");

	std::cout << "\n<TBODY style=\"background: white\">";
	BenchMarkByName<SymmetricCipher>("Panama-LE");
	BenchMarkByName<SymmetricCipher>("Panama-BE");
	BenchMarkByName<SymmetricCipher>("Salsa20");
	BenchMarkByName<SymmetricCipher>("Salsa20", 0, "Salsa20/12", MakeParameters(Name::Rounds(), 12));
	BenchMarkByName<SymmetricCipher>("Salsa20", 0, "Salsa20/8", MakeParameters(Name::Rounds(), 8));
	BenchMarkByName<SymmetricCipher>("ChaCha20");
	BenchMarkByName<SymmetricCipher>("ChaCha12");
	BenchMarkByName<SymmetricCipher>("ChaCha8");
	BenchMarkByName<SymmetricCipher>("Sosemanuk");
	BenchMarkByName<SymmetricCipher>("MARC4");
	BenchMarkByName<SymmetricCipher>("SEAL-3.0-LE");
	BenchMarkByName<SymmetricCipher>("WAKE-OFB-LE");

	std::cout << "\n<TBODY style=\"background: yellow\">";
	BenchMarkByName<SymmetricCipher>("AES/CTR", 16);
	BenchMarkByName<SymmetricCipher>("AES/CTR", 24);
	BenchMarkByName<SymmetricCipher>("AES/CTR", 32);
	BenchMarkByName<SymmetricCipher>("AES/CBC", 16);
	BenchMarkByName<SymmetricCipher>("AES/CBC", 24);
	BenchMarkByName<SymmetricCipher>("AES/CBC", 32);
	BenchMarkByName<SymmetricCipher>("AES/OFB", 16);
	BenchMarkByName<SymmetricCipher>("AES/CFB", 16);
	BenchMarkByName<SymmetricCipher>("AES/ECB", 16);
	BenchMarkByName<SymmetricCipher>("Camellia/CTR", 16);
	BenchMarkByName<SymmetricCipher>("Camellia/CTR", 32);
	BenchMarkByName<SymmetricCipher>("Twofish/CTR");
	BenchMarkByName<SymmetricCipher>("Serpent/CTR");
	BenchMarkByName<SymmetricCipher>("CAST-256/CTR");
	BenchMarkByName<SymmetricCipher>("RC6/CTR");
	BenchMarkByName<SymmetricCipher>("MARS/CTR");
	BenchMarkByName<SymmetricCipher>("SHACAL-2/CTR", 16);
	BenchMarkByName<SymmetricCipher>("SHACAL-2/CTR", 64);
	BenchMarkByName<SymmetricCipher>("DES/CTR");
	BenchMarkByName<SymmetricCipher>("DES-XEX3/CTR");
	BenchMarkByName<SymmetricCipher>("DES-EDE3/CTR");
	BenchMarkByName<SymmetricCipher>("IDEA/CTR");
	BenchMarkByName<SymmetricCipher>("RC5/CTR", 0, "RC5 (r=16)");
	BenchMarkByName<SymmetricCipher>("Blowfish/CTR");
	BenchMarkByName<SymmetricCipher>("TEA/CTR");
	BenchMarkByName<SymmetricCipher>("XTEA/CTR");
	BenchMarkByName<SymmetricCipher>("CAST-128/CTR");
	BenchMarkByName<SymmetricCipher>("SKIPJACK/CTR");
	BenchMarkByName<SymmetricCipher>("SEED/CTR", 0, "SEED/CTR (1/2 K table)");
	std::cout << "</TABLE>" << std::endl;

	BenchmarkAll2(t, hertz);
	std::cout << "Throughput Geometric Average: " << std::setiosflags(std::ios::fixed) << exp(logTotal/(logCount ? logCount : 1)) << std::endl;

// Safer functions on Windows for C&A, https://github.com/weidai11/cryptopp/issues/55
#if (CRYPTOPP_MSC_VERSION >= 1400)
	tm localTime = {};
	char timeBuf[64];
	errno_t err;

	const time_t endTime = time(NULLPTR);
	err = localtime_s(&localTime, &endTime);
	CRYPTOPP_ASSERT(err == 0);
	err = asctime_s(timeBuf, sizeof(timeBuf), &localTime);
	CRYPTOPP_ASSERT(err == 0);

	std::cout << "\nTest ended at " << timeBuf;
#else
	const time_t endTime = time(NULLPTR);
	std::cout << "\nTest ended at " << asctime(localtime(&endTime));
#endif
#endif
}

NAMESPACE_END  // Test
NAMESPACE_END  // CryptoPP
