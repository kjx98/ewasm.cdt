/*
    Copyright 2020 Jesse Kuang <jkuang@21cn.com>

    This file is part of ewasm.cdt.

    ewasm.cdt is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ewasm.cdt is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ewasm.cdt.  If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once

#ifndef __EWASM_HPP__
#define __EWASM_HPP__

#include <ewasm/ewasm.h>
typedef decltype(nullptr) nullptr_t;

// EWASM C++ API - wrappers and bindings for C++
// @ingroup cpp
namespace ewasm {

struct address : ewasm_address {
  /// Default and converting constructor.
  ///
  /// Initializes bytes to zeros if not other @p init value provided.
  constexpr address(ewasm_address init = {}) noexcept : ewasm_address{init} {}

  /// Converting constructor from unsigned integer value.
  ///
  /// This constructor assigns the @p v value to the last 8 bytes [12:19]
  /// in big-endian order.
  constexpr explicit address(uint64_t v) noexcept
      : ewasm_address{{0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       static_cast<uint8_t>(v >> 56),
                       static_cast<uint8_t>(v >> 48),
                       static_cast<uint8_t>(v >> 40),
                       static_cast<uint8_t>(v >> 32),
                       static_cast<uint8_t>(v >> 24),
                       static_cast<uint8_t>(v >> 16),
                       static_cast<uint8_t>(v >> 8),
                       static_cast<uint8_t>(v >> 0)}} {
    static_assert(sizeof(address) == 20, "size of address MUST be 20");
  }

  /// Explicit operator converting to bool.
  constexpr inline explicit operator bool() const noexcept;
};

/// The fixed size array of 32 bytes for storing 256-bit EVM values.
///
/// This type wraps C ::ewasm_bytes32 to make sure objects of this type are
/// always initialized.
struct bytes32 : ewasm_bytes32 {
  /// Default and converting constructor.
  ///
  /// Initializes bytes to zeros if not other @p init value provided.
  constexpr bytes32(ewasm_bytes32 init = {}) noexcept : ewasm_bytes32{init} {}

  /// Converting constructor from address
  ///
  /// This constructor assigns the @p v value to the last 8 bytes [24:31]
  /// in big-endian order.
  constexpr bytes32(const address &addr) noexcept
      : ewasm_bytes32{{0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       addr.bytes[0],
                       addr.bytes[1],
                       addr.bytes[2],
                       addr.bytes[3],
                       addr.bytes[4],
                       addr.bytes[5],
                       addr.bytes[6],
                       addr.bytes[7],
                       addr.bytes[8],
                       addr.bytes[9],
                       addr.bytes[10],
                       addr.bytes[11],
                       addr.bytes[12],
                       addr.bytes[13],
                       addr.bytes[14],
                       addr.bytes[15],
                       addr.bytes[16],
                       addr.bytes[17],
                       addr.bytes[18],
                       addr.bytes[19]}} {
    static_assert(sizeof(bytes32) == 32, "size of bytes32 MUST be 32");
  }
  void from128u(const u128 *u128p) noexcept {
    memset(bytes, 0, 16);
    memrcpy(bytes + 16, (void *)u128p, 16);
  }
  template <typename T> void fromUint(const T v) noexcept {
    static_assert(__is_unsigned(T) && __is_integral(T), "unsigned integral");
    constexpr uint32_t off = 32 - sizeof(T);
    memrcpy(bytes + off, (void *)&v, sizeof(T));
  }
  template <typename T> void fromInt(const T v) noexcept {
    static_assert(__is_signed(T) && __is_integral(T), "unsigned integral");
    constexpr uint32_t off = 32 - sizeof(T);
    memrcpy(bytes + off, (void *)&v, sizeof(T));
  }
#ifdef ommit
  void from64u(const uint64_t v) noexcept {
    memset(bytes, 0, 24);
    *(uint64_t *)(bytes + 24) = __builtin_bswap64(v);
  }
  void from64s(const int64_t v) noexcept {
    memset(bytes, 0, 24);
    *(uint64_t *)(bytes + 24) = __builtin_bswap64(v);
  }
  int128_t to128s() noexcept {
    int128_t ret;
    memrcpy((void *)&ret, bytes + 16, 16);
    return ret;
  }
  uint128_t to128u() noexcept {
    int128_t ret;
    memrcpy((void *)&ret, bytes + 16, 16);
    return ret;
  }
#endif

  /// Converting constructor from unsigned integer value.
  ///
  /// This constructor assigns the @p v value to the last 8 bytes [24:31]
  /// in big-endian order.
  constexpr explicit bytes32(uint64_t v) noexcept
      : ewasm_bytes32{{0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       static_cast<uint8_t>(v >> 56),
                       static_cast<uint8_t>(v >> 48),
                       static_cast<uint8_t>(v >> 40),
                       static_cast<uint8_t>(v >> 32),
                       static_cast<uint8_t>(v >> 24),
                       static_cast<uint8_t>(v >> 16),
                       static_cast<uint8_t>(v >> 8),
                       static_cast<uint8_t>(v >> 0)}} {}

  /// Explicit operator converting to bool.
  constexpr inline explicit operator bool() const noexcept;
};

/// The alias for evmc::bytes32 to represent a big-endian 256-bit integer.
using uint256be = bytes32;

/// Loads 64 bits / 8 bytes of data from the given @p bytes array in big-endian
/// order.
constexpr inline uint64_t load64be(const uint8_t *bytes) noexcept {
  return __builtin_bswap64(*(uint64_t *)bytes);
}

/// Loads 32 bits / 4 bytes of data from the given @p bytes array in big-endian
/// order.
constexpr inline uint32_t load32be(const uint8_t *bytes) noexcept {
  return __builtin_bswap32(*(uint32_t *)bytes);
}

/// The "equal to" comparison operator for the evmc::address type.
constexpr bool operator==(const address &a, const address &b) noexcept {
  // TODO: Report bug in clang keeping unnecessary bswap.
  return load64be(&a.bytes[0]) == load64be(&b.bytes[0]) &&
         load64be(&a.bytes[8]) == load64be(&b.bytes[8]) &&
         load32be(&a.bytes[16]) == load32be(&b.bytes[16]);
}

/// The "not equal to" comparison operator for the evmc::address type.
constexpr bool operator!=(const address &a, const address &b) noexcept {
  return !(a == b);
}

/// The "less than" comparison operator for the evmc::address type.
constexpr bool operator<(const address &a, const address &b) noexcept {
  return load64be(&a.bytes[0]) < load64be(&b.bytes[0]) ||
         (load64be(&a.bytes[0]) == load64be(&b.bytes[0]) &&
          (load64be(&a.bytes[8]) < load64be(&b.bytes[8]) ||
           (load64be(&a.bytes[8]) == load64be(&b.bytes[8]) &&
            load32be(&a.bytes[16]) < load32be(&b.bytes[16]))));
}

/// The "greater than" comparison operator for the evmc::address type.
constexpr bool operator>(const address &a, const address &b) noexcept {
  return b < a;
}

/// The "less than or equal to" comparison operator for the evmc::address type.
constexpr bool operator<=(const address &a, const address &b) noexcept {
  return !(b < a);
}

/// The "greater than or equal to" comparison operator for the evmc::address
/// type.
constexpr bool operator>=(const address &a, const address &b) noexcept {
  return !(a < b);
}

/// The "equal to" comparison operator for the evmc::bytes32 type.
constexpr bool operator==(const bytes32 &a, const bytes32 &b) noexcept {
  return load64be(&a.bytes[0]) == load64be(&b.bytes[0]) &&
         load64be(&a.bytes[8]) == load64be(&b.bytes[8]) &&
         load64be(&a.bytes[16]) == load64be(&b.bytes[16]) &&
         load64be(&a.bytes[24]) == load64be(&b.bytes[24]);
}

/// The "not equal to" comparison operator for the evmc::bytes32 type.
constexpr bool operator!=(const bytes32 &a, const bytes32 &b) noexcept {
  return !(a == b);
}

/// The "less than" comparison operator for the evmc::bytes32 type.
constexpr bool operator<(const bytes32 &a, const bytes32 &b) noexcept {
  return load64be(&a.bytes[0]) < load64be(&b.bytes[0]) ||
         (load64be(&a.bytes[0]) == load64be(&b.bytes[0]) &&
          (load64be(&a.bytes[8]) < load64be(&b.bytes[8]) ||
           (load64be(&a.bytes[8]) == load64be(&b.bytes[8]) &&
            (load64be(&a.bytes[16]) < load64be(&b.bytes[16]) ||
             (load64be(&a.bytes[16]) == load64be(&b.bytes[16]) &&
              load64be(&a.bytes[24]) < load64be(&b.bytes[24]))))));
}

/// The "greater than" comparison operator for the evmc::bytes32 type.
constexpr bool operator>(const bytes32 &a, const bytes32 &b) noexcept {
  return b < a;
}

/// The "less than or equal to" comparison operator for the evmc::bytes32 type.
constexpr bool operator<=(const bytes32 &a, const bytes32 &b) noexcept {
  return !(b < a);
}

/// The "greater than or equal to" comparison operator for the evmc::bytes32
/// type.
constexpr bool operator>=(const bytes32 &a, const bytes32 &b) noexcept {
  return !(a < b);
}

/// Checks if the given address is the zero address.
constexpr inline bool is_zero(const address &a) noexcept {
  return a == address{};
}

constexpr address::operator bool() const noexcept { return !is_zero(*this); }

/// Checks if the given bytes32 object has all zero bytes.
constexpr inline bool is_zero(const bytes32 &a) noexcept {
  return a == bytes32{};
}

constexpr bytes32::operator bool() const noexcept { return !is_zero(*this); }

struct bytes : ewasm_bytes {
  constexpr bytes(ewasm_bytes init) noexcept : ewasm_bytes{init} {}
  constexpr bytes() noexcept : ewasm_bytes{nullptr, 0} {}
  template <size_t N>
  constexpr bytes(const char (&s)[N]) noexcept
      : ewasm_bytes{(void *)s, (uint32_t)N} {}
  constexpr bytes(const char *str, const size_t n) noexcept
      : ewasm_bytes{(void *)str, (uint32_t)n} {}
  constexpr inline explicit operator bool() const noexcept;
  char *data() { return (char *)_data; }
  size_t size() const { return _size; }
};

constexpr bool operator==(const bytes &a, const bytes &b) noexcept {
  if (a._size != b._size)
    return false;
  if (a._size == 0)
    return true;
  return memcmp(a._data, b._data, a._size) == 0;
}

constexpr bool operator!=(const bytes &a, const bytes &b) noexcept {
  return !(a == b);
}

constexpr inline bytes::operator bool() const noexcept {
  return this->_size != 0 && this->_data != nullptr;
}

forceinline uint128_t u128From256(const byte *src) {
#ifdef ommit
  uint128_t *rp = (uint128_t *)src;
  return bswap128(rp[1]);
#else
  uint128_t ret;
  memrcpy(&ret, src + 16, 16);
  return ret;
#endif
}

forceinline uint64_t u64From256(const byte *src) {
  uint64_t *rp = (uint64_t *)src;
  return __builtin_bswap64(rp[3]);
}

forceinline uint32_t u32From256(const byte *src) {
  uint32_t *rp = (uint32_t *)src;
  return __builtin_bswap32(rp[7]);
}

forceinline void u128To256(const byte *dst, uint128_t val) {
#ifdef ommit
  uint128_t *rp = (uint128_t *)dst;
  rp[0] = 0;
  rp[1] = bswap128(val);
#else
  memrcpy((void *)(dst + 16), (void *)&val, 16);
#endif
}

forceinline void u64To256(const byte *dst, uint64_t val) {
#ifndef ommit
  uint64_t *rp = (uint64_t *)dst;
  rp[0] = 0;
  rp[1] = 0;
  rp[2] = 0;
  rp[3] = __builtin_bswap64(val);
#else
  bytes32 *rp = static_cast<bytes32 *>((void *)dst);
  rp->fromUint(val);
#endif
}

forceinline void u32To256(const byte *dst, uint32_t val) {
#ifndef ommit
  uint32_t *rp = (uint32_t *)dst;
  rp[0] = 0;
  rp[1] = 0;
  rp[2] = 0;
  rp[3] = 0;
  rp[4] = 0;
  rp[5] = 0;
  rp[6] = 0;
  rp[7] = __builtin_bswap32(val);
#else
  bytes32 *rp = static_cast<bytes32 *>((void *)dst);
  rp->fromUint(val);
#endif
}

forceinline void i128To256(const byte *dst, int128_t val) {
#ifdef ommit
  int128_t *rp = (int128_t *)dst;
  if (val < 0)
    rp[0] = -1;
  else
    rp[0] = 0;
  rp[1] = bswap128(val);
#else
  memrcpy((void *)(dst + 16), (void *)&val, 16);
#endif
}

forceinline void i64To256(const byte *dst, int64_t val) {
#ifdef ommit
  int64_t *rp = (int64_t *)dst;
  if (val < 0) {
    rp[0] = -1;
    rp[1] = -1;
    rp[2] = -1;
  } else {
    rp[0] = 0;
    rp[1] = 0;
    rp[2] = 0;
  }
  rp[3] = __builtin_bswap64(val);
#else
  bytes32 *rp = static_cast<bytes32 *>((void *)dst);
  rp->fromInt(val);
#endif
}

// not work yet with ewasm_main.c
struct method : ewasm_method {
  constexpr method(ewasm_method init = {}) : ewasm_method{init} {}
  template <size_t nLen, size_t iLen, size_t oLen>
  constexpr method(const char (&s)[nLen], const u32 id,
                   const ewasm_argument (&ins)[iLen],
                   const ewasm_argument (&outs)[oLen])
      : ewasm_method{(char *)s,
                     id,
                     iLen,
                     oLen,
                     (ewasm_argument *)&ins[0],
                     (ewasm_argument *)&outs[0]} {}
  template <size_t nLen, size_t iLen>
  constexpr method(const char (&s)[nLen], const u32 id,
                   const ewasm_argument (&ins)[iLen])
      : ewasm_method{(char *)s, id, iLen, 0, (ewasm_argument *)&ins[0]} {}
  template <size_t N>
  constexpr method(const char (&s)[N], const u32 id)
      : ewasm_method{(char *)s, id, 0, 0, nullptr, nullptr} {}
  // ignore inputs, as nIn == 0
  template <size_t nLen, size_t oLen>
  constexpr method(const char (&s)[nLen], const u32 id, const int nIn,
                   const ewasm_argument (&outs)[oLen])
      : ewasm_method{(char *)s, id,      0,
                     oLen,      nullptr, (ewasm_argument *)&outs[0]} {}
};

struct ABI : ewasm_ABI {
  constexpr ABI(ewasm_ABI init = {}) : ewasm_ABI{init} {}
  template <size_t N>
  constexpr ABI(const ewasm_method (&s)[N]) : ewasm_ABI{N, &s[0]} {}
  template <size_t N>
  constexpr ABI(const method (&s)[N]) : ewasm_ABI{N, &s[0]} {}
};

} // namespace ewasm
#endif
