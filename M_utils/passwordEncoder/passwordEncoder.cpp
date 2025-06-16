#include "passwordEncoder.h"

std::string PasswordEncoder::encode(const std::string& plaintext,
                                    std::uint32_t opsLimit,
                                    std::uint32_t memLimit) {
  // 初始化 libsodium（线程安全，可多次调用但仅首次有效）
  if (sodium_init() < 0) {
    throw std::runtime_error("libsodium 初始化失败");
  }

  // 分配哈希缓冲区
  char hash[crypto_pwhash_STRBYTES];
  // 生成哈希
  int result = crypto_pwhash_str(hash, plaintext.c_str(), plaintext.length(),
                                 opsLimit, memLimit);  // 直接指定并行度为 1

  if (result != 0) {
    throw std::runtime_error("密码哈希失败");
  }

  return std::string(hash);
}

bool PasswordEncoder::matches(const std::string& plaintext,
                              const std::string& hashStr) {
  // 初始化 libsodium
  if (sodium_init() < 0) {
    throw std::runtime_error("libsodium 初始化失败");
  }

  // 验证哈希（常量时间比较）
  int result = crypto_pwhash_str_verify(hashStr.c_str(), plaintext.c_str(),
                                        plaintext.length());

  return result == 0;
}
