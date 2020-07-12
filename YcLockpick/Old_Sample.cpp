#include "pch.h"

#ifdef SAMPLE


#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include <algorithm>
#include <iostream> 
#include <fstream>
#include <sstream>
#include <iterator>
#include <string.h>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <gmp.h>

using namespace std;

//#define SHA256
//#define SHA1
//#define MD5_32

#ifdef SHA256
//SHA256 from https://blog.csdn.net/ak47000gb/article/details/79596543
// picosha2:20140213

#ifndef PICOSHA2_BUFFER_SIZE_FOR_INPUT_ITERATOR
#define PICOSHA2_BUFFER_SIZE_FOR_INPUT_ITERATOR \
    1048576  //=1024*1024: default is 1MB memory
#endif

//#include <algorithm>
//#include <cassert>
//#include <iterator>
//#include <sstream>
//#include <vector>

namespace picosha2 {
	typedef unsigned long word_t;
	typedef unsigned char byte_t;

	static const size_t k_digest_size = 32;

	namespace detail {
		inline byte_t mask_8bit(byte_t x) { return x & 0xff; }

		inline word_t mask_32bit(word_t x) { return x & 0xffffffff; }

		const word_t add_constant[64] = {
			0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
			0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
			0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
			0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
			0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
			0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
			0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
			0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
			0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
			0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
			0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };

		const word_t initial_message_digest[8] = { 0x6a09e667, 0xbb67ae85, 0x3c6ef372,
			0xa54ff53a, 0x510e527f, 0x9b05688c,
			0x1f83d9ab, 0x5be0cd19 };

		inline word_t ch(word_t x, word_t y, word_t z) { return (x & y) ^ ((~x) & z); }

		inline word_t maj(word_t x, word_t y, word_t z) {
			return (x & y) ^ (x & z) ^ (y & z);
		}

		inline word_t rotr(word_t x, std::size_t n) {
			_ASSERT(n < 32);
			return mask_32bit((x >> n) | (x << (32 - n)));
		}

		inline word_t bsig0(word_t x) { return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22); }

		inline word_t bsig1(word_t x) { return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25); }

		inline word_t shr(word_t x, std::size_t n) {
			_ASSERT(n < 32);
			return x >> n;
		}

		inline word_t ssig0(word_t x) { return rotr(x, 7) ^ rotr(x, 18) ^ shr(x, 3); }

		inline word_t ssig1(word_t x) { return rotr(x, 17) ^ rotr(x, 19) ^ shr(x, 10); }

		template <typename RaIter1, typename RaIter2>
		void hash256_block(RaIter1 message_digest, RaIter2 first, RaIter2 last) {
			assert(first + 64 == last);
			static_cast<void>(last);  // for avoiding unused-variable warning
			word_t w[64];
			std::fill(w, w + 64, 0);
			for (std::size_t i = 0; i < 16; ++i) {
				w[i] = (static_cast<word_t>(mask_8bit(*(first + i * 4))) << 24) |
					(static_cast<word_t>(mask_8bit(*(first + i * 4 + 1))) << 16) |
					(static_cast<word_t>(mask_8bit(*(first + i * 4 + 2))) << 8) |
					(static_cast<word_t>(mask_8bit(*(first + i * 4 + 3))));
			}
			for (std::size_t i = 16; i < 64; ++i) {
				w[i] = mask_32bit(ssig1(w[i - 2]) + w[i - 7] + ssig0(w[i - 15]) +
					w[i - 16]);
			}

			word_t a = *message_digest;
			word_t b = *(message_digest + 1);
			word_t c = *(message_digest + 2);
			word_t d = *(message_digest + 3);
			word_t e = *(message_digest + 4);
			word_t f = *(message_digest + 5);
			word_t g = *(message_digest + 6);
			word_t h = *(message_digest + 7);

			for (std::size_t i = 0; i < 64; ++i) {
				word_t temp1 = h + bsig1(e) + ch(e, f, g) + add_constant[i] + w[i];
				word_t temp2 = bsig0(a) + maj(a, b, c);
				h = g;
				g = f;
				f = e;
				e = mask_32bit(d + temp1);
				d = c;
				c = b;
				b = a;
				a = mask_32bit(temp1 + temp2);
			}
			*message_digest += a;
			*(message_digest + 1) += b;
			*(message_digest + 2) += c;
			*(message_digest + 3) += d;
			*(message_digest + 4) += e;
			*(message_digest + 5) += f;
			*(message_digest + 6) += g;
			*(message_digest + 7) += h;
			for (std::size_t i = 0; i < 8; ++i) {
				*(message_digest + i) = mask_32bit(*(message_digest + i));
			}
		}

	}  // namespace detail

	template <typename InIter>
	void output_hex(InIter first, InIter last, std::ostream& os) {
		os.setf(std::ios::hex, std::ios::basefield);
		while (first != last) {
			os.width(2);
			os.fill('0');
			os << static_cast<unsigned int>(*first);
			++first;
		}
		os.setf(std::ios::dec, std::ios::basefield);
	}

	template <typename InIter>
	void bytes_to_hex_string(InIter first, InIter last, std::string& hex_str) {
		std::ostringstream oss;
		output_hex(first, last, oss);
		hex_str.assign(oss.str());
	}

	template <typename InContainer>
	void bytes_to_hex_string(const InContainer& bytes, std::string& hex_str) {
		bytes_to_hex_string(bytes.begin(), bytes.end(), hex_str);
	}

	template <typename InIter>
	std::string bytes_to_hex_string(InIter first, InIter last) {
		std::string hex_str;
		bytes_to_hex_string(first, last, hex_str);
		return hex_str;
	}

	template <typename InContainer>
	std::string bytes_to_hex_string(const InContainer& bytes) {
		std::string hex_str;
		bytes_to_hex_string(bytes, hex_str);
		return hex_str;
	}

	class hash256_one_by_one {
	public:
		hash256_one_by_one() { init(); }

		void init() {
			buffer_.clear();
			std::fill(data_length_digits_, data_length_digits_ + 4, 0);
			std::copy(detail::initial_message_digest,
				detail::initial_message_digest + 8, h_);
		}

		template <typename RaIter>
		void process(RaIter first, RaIter last) {
			add_to_data_length(std::distance(first, last));
			std::copy(first, last, std::back_inserter(buffer_));
			std::size_t i = 0;
			for (; i + 64 <= buffer_.size(); i += 64) {
				detail::hash256_block(h_, buffer_.begin() + i,
					buffer_.begin() + i + 64);
			}
			buffer_.erase(buffer_.begin(), buffer_.begin() + i);
		}

		void finish() {
			byte_t temp[64];
			std::fill(temp, temp + 64, 0);
			std::size_t remains = buffer_.size();
			std::copy(buffer_.begin(), buffer_.end(), temp);
			temp[remains] = 0x80;

			if (remains > 55) {
				std::fill(temp + remains + 1, temp + 64, 0);
				detail::hash256_block(h_, temp, temp + 64);
				std::fill(temp, temp + 64 - 4, 0);
			}
			else {
				std::fill(temp + remains + 1, temp + 64 - 4, 0);
			}

			write_data_bit_length(&(temp[56]));
			detail::hash256_block(h_, temp, temp + 64);
		}

		template <typename OutIter>
		void get_hash_bytes(OutIter first, OutIter last) const {
			for (const word_t* iter = h_; iter != h_ + 8; ++iter) {
				for (std::size_t i = 0; i < 4 && first != last; ++i) {
					*(first++) = detail::mask_8bit(
						static_cast<byte_t>((*iter >> (24 - 8 * i))));
				}
			}
		}

	private:
		void add_to_data_length(word_t n) {
			word_t carry = 0;
			data_length_digits_[0] += n;
			for (std::size_t i = 0; i < 4; ++i) {
				data_length_digits_[i] += carry;
				if (data_length_digits_[i] >= 65536u) {
					carry = data_length_digits_[i] >> 16;
					data_length_digits_[i] &= 65535u;
				}
				else {
					break;
				}
			}
		}
		void write_data_bit_length(byte_t* begin) {
			word_t data_bit_length_digits[4];
			std::copy(data_length_digits_, data_length_digits_ + 4,
				data_bit_length_digits);

			// convert byte length to bit length (multiply 8 or shift 3 times left)
			word_t carry = 0;
			for (std::size_t i = 0; i < 4; ++i) {
				word_t before_val = data_bit_length_digits[i];
				data_bit_length_digits[i] <<= 3;
				data_bit_length_digits[i] |= carry;
				data_bit_length_digits[i] &= 65535u;
				carry = (before_val >> (16 - 3)) & 65535u;
			}

			// write data_bit_length
			for (int i = 3; i >= 0; --i) {
				(*begin++) = static_cast<byte_t>(data_bit_length_digits[i] >> 8);
				(*begin++) = static_cast<byte_t>(data_bit_length_digits[i]);
			}
		}
		std::vector<byte_t> buffer_;
		word_t data_length_digits_[4];  // as 64bit integer (16bit x 4 integer)
		word_t h_[8];
	};

	inline void get_hash_hex_string(const hash256_one_by_one& hasher,
		std::string& hex_str) {
		byte_t hash[k_digest_size];
		hasher.get_hash_bytes(hash, hash + k_digest_size);
		return bytes_to_hex_string(hash, hash + k_digest_size, hex_str);
	}

	inline std::string get_hash_hex_string(const hash256_one_by_one& hasher) {
		std::string hex_str;
		get_hash_hex_string(hasher, hex_str);
		return hex_str;
	}

	namespace impl {
		template <typename RaIter, typename OutIter>
		void hash256_impl(RaIter first, RaIter last, OutIter first2, OutIter last2, int,
			std::random_access_iterator_tag) {
			hash256_one_by_one hasher;
			// hasher.init();
			hasher.process(first, last);
			hasher.finish();
			hasher.get_hash_bytes(first2, last2);
		}

		template <typename InputIter, typename OutIter>
		void hash256_impl(InputIter first, InputIter last, OutIter first2,
			OutIter last2, int buffer_size, std::input_iterator_tag) {
			std::vector<byte_t> buffer(buffer_size);
			hash256_one_by_one hasher;
			// hasher.init();
			while (first != last) {
				int size = buffer_size;
				for (int i = 0; i != buffer_size; ++i, ++first) {
					if (first == last) {
						size = i;
						break;
					}
					buffer[i] = *first;
				}
				hasher.process(buffer.begin(), buffer.begin() + size);
			}
			hasher.finish();
			hasher.get_hash_bytes(first2, last2);
		}
	}

	template <typename InIter, typename OutIter>
	void hash256(InIter first, InIter last, OutIter first2, OutIter last2,
		int buffer_size = PICOSHA2_BUFFER_SIZE_FOR_INPUT_ITERATOR) {
		picosha2::impl::hash256_impl(
			first, last, first2, last2, buffer_size,
			typename std::iterator_traits<InIter>::iterator_category());
	}

	template <typename InIter, typename OutContainer>
	void hash256(InIter first, InIter last, OutContainer& dst) {
		hash256(first, last, dst.begin(), dst.end());
	}

	template <typename InContainer, typename OutIter>
	void hash256(const InContainer& src, OutIter first, OutIter last) {
		hash256(src.begin(), src.end(), first, last);
	}

	template <typename InContainer, typename OutContainer>
	void hash256(const InContainer& src, OutContainer& dst) {
		hash256(src.begin(), src.end(), dst.begin(), dst.end());
	}

	template <typename InIter>
	void hash256_hex_string(InIter first, InIter last, std::string& hex_str) {
		byte_t hashed[k_digest_size];
		hash256(first, last, hashed, hashed + k_digest_size);
		std::ostringstream oss;
		output_hex(hashed, hashed + k_digest_size, oss);
		hex_str.assign(oss.str());
	}

	template <typename InIter>
	std::string hash256_hex_string(InIter first, InIter last) {
		std::string hex_str;
		hash256_hex_string(first, last, hex_str);
		return hex_str;
	}

	inline void hash256_hex_string(const std::string& src, std::string& hex_str) {
		hash256_hex_string(src.begin(), src.end(), hex_str);
	}

	template <typename InContainer>
	void hash256_hex_string(const InContainer& src, std::string& hex_str) {
		hash256_hex_string(src.begin(), src.end(), hex_str);
	}

	template <typename InContainer>
	std::string hash256_hex_string(const InContainer& src) {
		return hash256_hex_string(src.begin(), src.end());
	}
}  // namespace picosha2

/*---------------------------------------------------------------------->使用例main.cpp*/
/*
//#include "picosha2.h"

// 从string生成
void from_string()
{
	std::string src_str = "Hello";
	std::string hash_hex_str;
	std::vector<unsigned char> hash(picosha2::k_digest_size);
	picosha2::hash256(src_str.begin(), src_str.end(), hash.begin(), hash.end());
	hash_hex_str = picosha2::bytes_to_hex_string(hash.begin(), hash.end());
	std::cout << hash_hex_str << std::endl;
	//this output is "185f8db32271fe25f561a6fc938b2e264306ec304eda518007d1764826381969"

	picosha2::hash256_hex_string(src_str, hash_hex_str);
	std::cout << hash_hex_str << std::endl;
	//this output is "185f8db32271fe25f561a6fc938b2e264306ec304eda518007d1764826381969"

	hash_hex_str = picosha2::hash256_hex_string(src_str);
	std::cout << hash_hex_str << std::endl;
	//this output is "185f8db32271fe25f561a6fc938b2e264306ec304eda518007d1764826381969"

	src_str = "Hello!";//add '!'
	hash_hex_str = picosha2::hash256_hex_string(src_str.begin(), src_str.end());
	std::cout << hash_hex_str << std::endl;
	//this output is "334d016f755cd6dc58c53a86e183882f8ec14f52fb05345887c8a5edd42c87b7"
}

//从字节数组读取
void from_bytes()
{
	std::vector<unsigned char> src_vect(5, '\0');
	src_vect.at(0) = 'H';
	src_vect.at(1) = 'e';
	src_vect.at(2) = 'l';
	src_vect.at(3) = 'l';
	src_vect.at(4) = 'o';
	std::string hash_hex_str;
	picosha2::hash256_hex_string(src_vect, hash_hex_str);
	std::cout << hash_hex_str << std::endl;
	//this output is "185f8db32271fe25f561a6fc938b2e264306ec304eda518007d1764826381969"

	std::vector<unsigned char> _src_vect(picosha2::k_digest_size, '\0');
	_src_vect.at(0) = 'H';
	_src_vect.at(1) = 'e';
	_src_vect.at(2) = 'l';
	_src_vect.at(3) = 'l';
	_src_vect.at(4) = 'o';
	hash_hex_str = picosha2::hash256_hex_string(_src_vect);
	std::cout << hash_hex_str << std::endl;
	//this output is "cd4895ecd6438e664b91e74f0e632438cd092e27da585f77854e5c28239d2f76"

	unsigned char src_c_array[5] = { 'H','e','l','l','o' };
	picosha2::hash256_hex_string(src_c_array, src_c_array + 5, hash_hex_str);
	std::cout << hash_hex_str << std::endl;
	//this output is "185f8db32271fe25f561a6fc938b2e264306ec304eda518007d1764826381969"

	unsigned char _src_c_array[picosha2::k_digest_size] = { 'H','e','l','l','o' };
	hash_hex_str = picosha2::hash256_hex_string(_src_c_array, _src_c_array + picosha2::k_digest_size);
	std::cout << hash_hex_str << std::endl;
	//this output is "cd4895ecd6438e664b91e74f0e632438cd092e27da585f77854e5c28239d2f76"
}

//从文件读取
void from_file()
{
	std::ifstream ifs;
	ifs.open("file.txt", std::ios::binary); //file data is "Hello"

	std::vector<unsigned char> hash(picosha2::k_digest_size);
	picosha2::hash256(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>(), hash.begin(), hash.end());
	std::string hex_str = picosha2::bytes_to_hex_string(hash.begin(), hash.end());
	std::cout << hex_str << std::endl;
	//this output is "185f8db32271fe25f561a6fc938b2e264306ec304eda518007d1764826381969"
}

//从标准输入读取(这个好像最有用)
void from_input()
{
	std::cout << "Input freely. To get hash, input \"hash!\". " << std::endl;
	picosha2::hash256_one_by_one hasher;
	while (true) {
		hasher.init(); //reset hasher state
		while (true) {
			std::string line;
			std::getline(std::cin, line);
			if (line == "hash!") {
				break;
			}
			hasher.process(line.begin(), line.end());
		}
		hasher.finish();
		std::string hex_str;
		picosha2::get_hash_hex_string(hasher, hex_str);
		std::cout << hex_str << std::endl;
	}
}

int main(int argc, char* argv[])
{
	from_string();

	from_bytes();

	from_file();

	from_input();

	system("pause");
	return 0;
}
*/
/*使用例main.cpp 结束*/
#endif //SHA256

#ifdef SHA1
//SHA-1 from https://www.jianshu.com/p/2587a0273a35

//#include <iostream> 
//#include<string>
//#include <cstdio>
//#include <cstdlib>
//#include<vector>

using namespace std;

vector<int> X;//8*64=512，每个下标存放8位
int W[80];//32位为一组
int A, B, C, D, E;
int A1, B1, C1, D1, E1;//缓冲区寄存器,产生最后结果
int Turn;//加密分组数量
void printX() {//输出填充后的文本
	for (int i = 0; i < X.size(); i++) {
		printf("%02x", X[i]);
		if ((i + 1) % 4 == 0)
			printf(" ");
		if ((i + 1) % 16 == 0)
			printf("\n");
	}
}
int S(unsigned int x, int n) {//循环左移
	return x >> (32 - n) | (x << n);
}
void WidenL(string m) {//文本的填充处理
	Turn = (m.size() + 8) / 64 + 1;
	X.resize(Turn * 64);
	int i = 0;
	for (; i < m.size(); i++) {
		X[i] = m[i];
	}
	X[i++] = 0x80;
	while (i < X.size() - 8) {
		X[i] = 0;
		i++;
	}
	long long int a = m.size() * 8;
	for (i = X.size() - 1; i >= X.size() - 8; i--) {
		X[i] = a % 256;
		a /= 256;
	}
}
void setW(vector<int> m, int n) {//W数组的生成
	n *= 64;
	for (int i = 0; i < 16; i++) {
		W[i] = (m[n + 4 * i] << 24) + (m[n + 4 * i + 1] << 16)
			+ (m[n + 4 * i + 2] << 8) + m[n + 4 * i + 3];
	}
	for (int i = 16; i < 80; i++) {
		W[i] = S(W[i - 16] ^ W[i - 14] ^ W[i - 8] ^ W[i - 3], 1);
	}
}
int ft(int t) {//ft(B,C,D)函数
	if (t < 20)
		return (B & C) | ((~B) & D);
	else if (t < 40)
		return B ^ C ^ D;
	else if (t < 60)
		return (B & C) | (B & D) | (C & D);
	else
		return B ^ C ^ D;
}
int Kt(int t) {//常量K
	if (t < 20)
		return 0x5a827999;
	else if (t < 40)
		return 0x6ed9eba1;
	else if (t < 60)
		return 0x8f1bbcdc;
	else
		return 0xca62c1d6;
}
void sha1(string text) {
	A1 = A = 0x67452301;
	B1 = B = 0xefcdab89;
	C1 = C = 0x98badcfe;
	D1 = D = 0x10325476;
	E1 = E = 0xc3d2e1f0;
	WidenL(text);
	printX();
	for (int i = 0; i < Turn; i++) {
		setW(X, i);
		for (int t = 0; t < 80; t++) {
			int temp = E + ft(t) + S(A, 5) + W[t] + Kt(t);
			E = D;
			D = C;
			C = S(B, 30);
			B = A;
			A = temp;
		}
		A1 = A = A + A1;
		B1 = B = B + B1;
		C1 = C = C + C1;
		D1 = D = D + D1;
		E1 = E = E + E1;
	}
	printf("%08x%08x%08x%08x%08x\n\n", A1, B1, C1, D1, E1);
}

/* //Useage
int main() {

	string text;
	while (true)
	{
		cin >> text;
		sha1(text);
	}
	return 0;
}
*/
#endif //SHA1

#ifdef RSA
#endif //RSA

#ifdef MD5_32
//MD5 from https://blog.csdn.net/wudishine/article/details/42466831?depth_1-utm_source=distribute.pc_relevant.none-task&utm_source=distribute.pc_relevant.none-task
//md5.h started
/* Type define */
typedef unsigned char byte;
typedef unsigned long ulong;

using std::string;
using std::ifstream;

/* MD5 declaration. */
class MD5 {
public:
	MD5();
	MD5(const void *input, size_t length);
	MD5(const string &str);
	MD5(ifstream &in);
	void update(const void *input, size_t length);
	void update(const string &str);
	void update(ifstream &in);
	const byte* digest();
	string toString();
	void reset();
private:
	void update(const byte *input, size_t length);
	void final();
	void transform(const byte block[64]);
	void encode(const ulong *input, byte *output, size_t length);
	void decode(const byte *input, ulong *output, size_t length);
	string bytesToHexString(const byte *input, size_t length);

	/* class uncopyable */
	MD5(const MD5&);
	MD5& operator=(const MD5&);
private:
	ulong _state[4];	/* state (ABCD) */
	ulong _count[2];	/* number of bits, modulo 2^64 (low-order word first) */
	byte _buffer[64];	/* input buffer */
	byte _digest[16];	/* message digest */
	bool _finished;		/* calculate finished ? */

	static const byte PADDING[64];	/* padding for calculate */
	static const char HEX[16];
	static const size_t BUFFER_SIZE = 1024;
};
//md5.h end

//md5.cpp started
/* Constants for MD5Transform routine. */
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21


/* F, G, H and I are basic MD5 functions.
*/
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits.
*/
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
Rotation is separate from addition to prevent recomputation.
*/
#define FF(a, b, c, d, x, s, ac) { \
	(a) += F ((b), (c), (d)) + (x) + ac; \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
}
#define GG(a, b, c, d, x, s, ac) { \
	(a) += G ((b), (c), (d)) + (x) + ac; \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
}
#define HH(a, b, c, d, x, s, ac) { \
	(a) += H ((b), (c), (d)) + (x) + ac; \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
}
#define II(a, b, c, d, x, s, ac) { \
	(a) += I ((b), (c), (d)) + (x) + ac; \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
}


const byte MD5::PADDING[64] = { 0x80 };
const char MD5::HEX[16] = {
	'0', '1', '2', '3',
	'4', '5', '6', '7',
	'8', '9', 'a', 'b',
	'c', 'd', 'e', 'f'
};

/* Default construct. */
MD5::MD5() {
	reset();
}

/* Construct a MD5 object with a input buffer. */
MD5::MD5(const void *input, size_t length) {
	reset();
	update(input, length);
}

/* Construct a MD5 object with a string. */
MD5::MD5(const string &str) {
	reset();
	update(str);
}

/* Construct a MD5 object with a file. */
MD5::MD5(ifstream &in) {
	reset();
	update(in);
}

/* Return the message-digest */
const byte* MD5::digest() {
	if (!_finished) {
		_finished = true;
		final();
	}
	return _digest;
}

/* Reset the calculate state */
void MD5::reset() {

	_finished = false;
	/* reset number of bits. */
	_count[0] = _count[1] = 0;
	/* Load magic initialization constants. */
	_state[0] = 0x67452301;
	_state[1] = 0xefcdab89;
	_state[2] = 0x98badcfe;
	_state[3] = 0x10325476;
}

/* Updating the context with a input buffer. */
void MD5::update(const void *input, size_t length) {
	update((const byte*)input, length);
}

/* Updating the context with a string. */
void MD5::update(const string &str) {
	update((const byte*)str.c_str(), str.length());
}

/* Updating the context with a file. */
void MD5::update(ifstream &in) {

	if (!in)
		return;

	std::streamsize length;
	char buffer[BUFFER_SIZE];
	while (!in.eof()) {
		in.read(buffer, BUFFER_SIZE);
		length = in.gcount();
		if (length > 0)
			update(buffer, length);
	}
	in.close();
}

/* MD5 block update operation. Continues an MD5 message-digest
operation, processing another message block, and updating the
context.
*/
void MD5::update(const byte *input, size_t length) {

	ulong i, index, partLen;

	_finished = false;

	/* Compute number of bytes mod 64 */
	index = (ulong)((_count[0] >> 3) & 0x3f);

	/* update number of bits */
	if ((_count[0] += ((ulong)length << 3)) < ((ulong)length << 3))
		_count[1]++;
	_count[1] += ((ulong)length >> 29);

	partLen = 64 - index;
	/* transform as many times as possible. */
	if (length >= partLen) {

		::memcpy(&_buffer[index], input, partLen);
		transform(_buffer);

		for (i = partLen; i + 63 < length; i += 64)
			transform(&input[i]);
		index = 0;

	}
	else {
		i = 0;
	}

	/* Buffer remaining input */
	::memcpy(&_buffer[index], &input[i], length - i);
}

/* MD5 finalization. Ends an MD5 message-_digest operation, writing the
the message _digest and zeroizing the context.
*/
void MD5::final() {

	byte bits[8];
	ulong oldState[4];
	ulong oldCount[2];
	ulong index, padLen;

	/* Save current state and count. */
	::memcpy(oldState, _state, 16);
	::memcpy(oldCount, _count, 8);

	/* Save number of bits */
	encode(_count, bits, 8);

	/* Pad out to 56 mod 64. */
	index = (ulong)((_count[0] >> 3) & 0x3f);
	padLen = (index < 56) ? (56 - index) : (120 - index);
	update(PADDING, padLen);

	/* WidenL length (before padding) */
	update(bits, 8);

	/* Store state in digest */
	encode(_state, _digest, 16);

	/* Restore current state and count. */
	::memcpy(_state, oldState, 16);
	::memcpy(_count, oldCount, 8);
}

/* MD5 basic transformation. Transforms _state based on block. */
void MD5::transform(const byte block[64]) {

	ulong a = _state[0], b = _state[1], c = _state[2], d = _state[3], x[16];

	decode(block, x, 64);

	/* Round 1 */
	FF(a, b, c, d, x[0], S11, 0xd76aa478); /* 1 */
	FF(d, a, b, c, x[1], S12, 0xe8c7b756); /* 2 */
	FF(c, d, a, b, x[2], S13, 0x242070db); /* 3 */
	FF(b, c, d, a, x[3], S14, 0xc1bdceee); /* 4 */
	FF(a, b, c, d, x[4], S11, 0xf57c0faf); /* 5 */
	FF(d, a, b, c, x[5], S12, 0x4787c62a); /* 6 */
	FF(c, d, a, b, x[6], S13, 0xa8304613); /* 7 */
	FF(b, c, d, a, x[7], S14, 0xfd469501); /* 8 */
	FF(a, b, c, d, x[8], S11, 0x698098d8); /* 9 */
	FF(d, a, b, c, x[9], S12, 0x8b44f7af); /* 10 */
	FF(c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
	FF(b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
	FF(a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
	FF(d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
	FF(c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
	FF(b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

	/* Round 2 */
	GG(a, b, c, d, x[1], S21, 0xf61e2562); /* 17 */
	GG(d, a, b, c, x[6], S22, 0xc040b340); /* 18 */
	GG(c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
	GG(b, c, d, a, x[0], S24, 0xe9b6c7aa); /* 20 */
	GG(a, b, c, d, x[5], S21, 0xd62f105d); /* 21 */
	GG(d, a, b, c, x[10], S22, 0x2441453); /* 22 */
	GG(c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
	GG(b, c, d, a, x[4], S24, 0xe7d3fbc8); /* 24 */
	GG(a, b, c, d, x[9], S21, 0x21e1cde6); /* 25 */
	GG(d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
	GG(c, d, a, b, x[3], S23, 0xf4d50d87); /* 27 */
	GG(b, c, d, a, x[8], S24, 0x455a14ed); /* 28 */
	GG(a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
	GG(d, a, b, c, x[2], S22, 0xfcefa3f8); /* 30 */
	GG(c, d, a, b, x[7], S23, 0x676f02d9); /* 31 */
	GG(b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

	/* Round 3 */
	HH(a, b, c, d, x[5], S31, 0xfffa3942); /* 33 */
	HH(d, a, b, c, x[8], S32, 0x8771f681); /* 34 */
	HH(c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
	HH(b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
	HH(a, b, c, d, x[1], S31, 0xa4beea44); /* 37 */
	HH(d, a, b, c, x[4], S32, 0x4bdecfa9); /* 38 */
	HH(c, d, a, b, x[7], S33, 0xf6bb4b60); /* 39 */
	HH(b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
	HH(a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
	HH(d, a, b, c, x[0], S32, 0xeaa127fa); /* 42 */
	HH(c, d, a, b, x[3], S33, 0xd4ef3085); /* 43 */
	HH(b, c, d, a, x[6], S34, 0x4881d05); /* 44 */
	HH(a, b, c, d, x[9], S31, 0xd9d4d039); /* 45 */
	HH(d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
	HH(c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
	HH(b, c, d, a, x[2], S34, 0xc4ac5665); /* 48 */

	/* Round 4 */
	II(a, b, c, d, x[0], S41, 0xf4292244); /* 49 */
	II(d, a, b, c, x[7], S42, 0x432aff97); /* 50 */
	II(c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
	II(b, c, d, a, x[5], S44, 0xfc93a039); /* 52 */
	II(a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
	II(d, a, b, c, x[3], S42, 0x8f0ccc92); /* 54 */
	II(c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
	II(b, c, d, a, x[1], S44, 0x85845dd1); /* 56 */
	II(a, b, c, d, x[8], S41, 0x6fa87e4f); /* 57 */
	II(d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
	II(c, d, a, b, x[6], S43, 0xa3014314); /* 59 */
	II(b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
	II(a, b, c, d, x[4], S41, 0xf7537e82); /* 61 */
	II(d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
	II(c, d, a, b, x[2], S43, 0x2ad7d2bb); /* 63 */
	II(b, c, d, a, x[9], S44, 0xeb86d391); /* 64 */

	_state[0] += a;
	_state[1] += b;
	_state[2] += c;
	_state[3] += d;
}

/* Encodes input (ulong) into output (byte). Assumes length is
a multiple of 4.
*/
void MD5::encode(const ulong *input, byte *output, size_t length) {

	for (size_t i = 0, j = 0; j < length; i++, j += 4) {
		output[j] = (byte)(input[i] & 0xff);
		output[j + 1] = (byte)((input[i] >> 8) & 0xff);
		output[j + 2] = (byte)((input[i] >> 16) & 0xff);
		output[j + 3] = (byte)((input[i] >> 24) & 0xff);
	}
}

/* Decodes input (byte) into output (ulong). Assumes length is
a multiple of 4.
*/
void MD5::decode(const byte *input, ulong *output, size_t length) {

	for (size_t i = 0, j = 0; j < length; i++, j += 4) {
		output[i] = ((ulong)input[j]) | (((ulong)input[j + 1]) << 8) |
			(((ulong)input[j + 2]) << 16) | (((ulong)input[j + 3]) << 24);
	}
}

/* Convert byte array to hex string. */
string MD5::bytesToHexString(const byte *input, size_t length) {
	string str;
	str.reserve(length << 1);
	for (size_t i = 0; i < length; i++) {
		int t = input[i];
		int a = t / 16;
		int b = t % 16;
		str.WidenL(1, HEX[a]);
		str.WidenL(1, HEX[b]);
	}
	return str;
}

/* Convert digest to string value */
string MD5::toString() {
	return bytesToHexString(digest(), 16);
}
//md5.cpp end

//test.cpp started
//#include "md5.h"
void PrintMD5(const string &str, MD5 &md5) {
	cout << "MD5(\"" << str << "\") = " << md5.toString() << endl;
}

string FileDigest(const string &file) {

	ifstream in(file.c_str(), ios::binary);
	if (!in)
		return "";

	MD5 md5;
	std::streamsize length;
	char buffer[1024];
	while (!in.eof()) {
		in.read(buffer, 1024);
		length = in.gcount();
		if (length > 0)
			md5.update(buffer, length);
	}
	in.close();
	return md5.toString();
}

int main() {
	cout << MD5("wxtILoveU").toString() << endl;

	//cout << FileDigest("D:\\staticDll.dll") << endl;

	return 0;
}
//test.cpp end
#undef S11
#undef S12
#undef S13 
#undef S14
#undef S21
#undef S22
#undef S23 
#undef S24
#undef S31
#undef S32 
#undef S33 
#undef S34 
#undef S41 
#undef S42 
#undef S43
#undef S44 

#undef F
#undef G
#undef H
#undef I
#undef FF
#undef GG
#undef HH
#undef II

#endif //MD5_32
//----------------------------------------------------------------//

#endif
