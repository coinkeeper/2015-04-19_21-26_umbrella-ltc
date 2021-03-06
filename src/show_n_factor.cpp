/* -*-coding: mule-utf-8-unix; fill-column: 58; -*-

  Copyright (C) 2014 Sergei Lodyagin 
 
  The utility for print Vertcoin GetNfactor.
*/

#include <iostream>
#include <cmath>
#include <tuple>
#include "types/time.h"
#include "btc_time.h"

using namespace std;
using namespace std::chrono;
using namespace curr;

std::tuple<size_t, unsigned, unsigned> 
GetNfactor(coin::time::block::time_point block_time);

using namespace coin::time::block;

using days = duration<clock::rep, ratio<3600 * 24>>;
using common_years = 
  duration<clock::rep, ratio<3600 * 24 * 365>>;

//! shows n_factor for the specified time point
void show(const coin::time::block::time_point now)
{
  size_t N = 0;
  unsigned r = 0, p = 0;
  tie(N, r, p) = GetNfactor(now);
  std::cout << put_time(clock::to_system_clock(now), "%c")
    << "\t<" << N << ", " << r << ", " << p << ">\t"
    << (size_t) 128 * N * r * p / (1024 * 1024) << "MiB"
    << std::endl;
}

//! change n_factor for the specified period and call fun
void iterate(
  const coin::time::block::time_point from, 
  const coin::time::block::time_point to,
  void (*fun)(const coin::time::block::time_point)
)
{
  auto now = from;
  (*fun)(now);
  auto last_factor = GetNfactor(now);
  while (now <= to) {
    while (GetNfactor(now) == last_factor) 
      now += hours(24);
    last_factor = GetNfactor(now);

    (*fun)(now);
  }
}

void 
get_scratchpad(const coin::time::block::time_point now)
{
  size_t N; unsigned r, p;
  std::tie(N, r, p) = GetNfactor(now);
  std::cout
  << "if (n_factor == n_factor_t(" 
  << N << ", " << r << ", " << p << "))\n"
  << "    // " 
  << put_time(clock::to_system_clock(now), "%c") << '\n'
  << "    return scratchpad_ptr(scratchpad<"
  << N << ", " << r << ", " << p << ">::allocate());\n"
  << "  else ";
}

void 
get_scratchpad(
  const coin::time::block::time_point from,
  const coin::time::block::time_point to
)
{
  std::cout 
  << "scratchpad_ptr get_scratchpad(n_factor_t n_factor)\n"
  << "{\n"
  << "  using namespace std;\n\n  ";

  iterate(from, to, get_scratchpad);

  std::cout
  << "\n    throw runtime_error(\"invalid n_factor\");\n"
  << "}\n\n";
}

void hash1(const coin::time::block::time_point now)
{
  size_t N; unsigned r, p;
  std::tie(N, r, p) = GetNfactor(now);
  std::cout
  << "if (n_factor == n_factor_t(" 
  << N << ", " << r << ", " << p << "))\n"
  << "    // " 
  << put_time(clock::to_system_clock(now), "%c") << '\n'
  << "    scrypt_256_sp_templ<"
  << N << ", " << r << ", " << p << ">(\n"
  << "      in, in, hash,\n"
  << "      dynamic_cast<scratchpad<"
  << N << ", " << r << ", " << p << ">&>(*scr).pad\n"
  << "    );\n"
  << "  else ";
}

void hash1(
  const coin::time::block::time_point from,
  const coin::time::block::time_point to
)
{
  std::cout
  << "uint256 hash(\n"
  << "  const CBlock& blk,\n"
  << "  n_factor_t n_factor,\n"
  << "  scratchpad_ptr scr\n"
  << ")\n"
  << "{\n"
  << "  using namespace std;\n\n"
  << "  const std::string in(BEGIN(blk.nVersion), 80);\n"
  << "  uint256 hash;\n\n  ";

  iterate(from, to, hash1);

  std::cout
  << "\n    throw runtime_error(\"invalid n_factor\");\n\n"
  << "  return hash;\n"
  << "}\n\n";
}

int main(int argc, char* argv[])
{
  auto now = clock::now();
#if 0
  // general print
  iterate(now, now + common_years(21), show);
#endif

  std::cout 
  << "// Autogenerated by show_n_factor.cpp\n\n"
  << "#include \"hash/scrypt.hpp\"\n"
  << "#include \"main.h\"\n\n"
  << "namespace scrypt {\n\n";
  get_scratchpad(now, now + common_years(21));
  hash1(now, now + common_years(21));
  std::cout
  << "} // scrypt\n";
}
