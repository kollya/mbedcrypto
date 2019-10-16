/** @file helper.hpp
 *
 * @copyright (C) 2016
 * @date 2016.03.06
 * @author amir zamani <azadkuh@live.com>
 *
 */

#ifndef TESTS_HELPER_HPP
#define TESTS_HELPER_HPP

#include "mbedcrypto/binutils.hpp"
//-----------------------------------------------------------------------------
namespace mbedcrypto {
namespace test {
//-----------------------------------------------------------------------------

// following functions return some sample data for various test units.

const char* short_text() noexcept;
const char* long_text() noexcept;

bin_view_t short_binary() noexcept;
bin_view_t long_binary() noexcept;

/// dumps content of data as binary to filename
void dump_to_file(bin_view_t data, const char* filename);

/// a sample RSA, 2048bit key in pem format
/// generated by:
/// @code
/// $> openssl genrsa -out mprivate.pem 2048
/// @endcode
bin_view_t rsa_private_key() noexcept;

/// same as above key, encrypted by AES256. password is: mbedcrypto1234
/// generated by:
/// @code
/// $> openssl rsa -aes256 -in mprivate.pem -out mprivate_encrypted.pem
/// @endcode
bin_view_t rsa_private_key_password() noexcept;

/// the public pair of the above key
/// generated by:
/// @code
/// $> openssl rsa -in mprivate.pem -pubout -out mpublic.pem
/// @endcode
bin_view_t rsa_public_key() noexcept;

/// with the above private key and long text
/// $> openssl dgst -sha1 -sign private.pem -out signature.bin long.txt
/// verification:
/// $> openssl dgst -sha1 -verify public.pem -signature signature.bin long.txt
bin_view_t long_text_signature() noexcept;

//-----------------------------------------------------------------------------

/// utility function for reading a buffer in chunks
/// used by test units to test start()/update().../finish() sequences.
template <class Func, class... Args>
inline void
chunker(bin_view_t input, size_t chunk_size, Func&& func, Args&&... args) {
    for (size_t i = 0; (i + chunk_size) <= input.size; i += chunk_size) {
        func(input.data + i, chunk_size, std::forward<Args&&>(args)...);
    }
    const auto residue = input.size % chunk_size;
    if (residue) {
        func(
            input.data + input.size - residue,
            residue,
            std::forward<Args&&>(args)...);
    }
}

//-----------------------------------------------------------------------------
} // namespace test
//-----------------------------------------------------------------------------

#if defined(QT_CORE_LIB)
inline bool
operator==(const QByteArray& ba, const std::string& ss) {
    if ((size_t)ba.size() != ss.size())
        return false;
    return std::memcmp(ba.data(), ss.data(), ss.size()) == 0;
}
#endif // QT_CORE_LIB

//-----------------------------------------------------------------------------
} // namespace mbedcrypto
//-----------------------------------------------------------------------------
#endif // TESTS_HELPER_HPP
