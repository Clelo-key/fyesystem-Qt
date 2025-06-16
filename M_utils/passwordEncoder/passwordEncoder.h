#ifndef PASSWORD_ENCODER_H
#define PASSWORD_ENCODER_H

#include <stdexcept>
#include <string>
#include <sodium.h>  // libsodium 头文件

/**
 * @brief 安全密码编码器，提供静态密码哈希和验证功能
 *
 * 使用 libsodium 实现，支持 Argon2id 算法，自动管理盐值和参数
 */
class PasswordEncoder {
 public:
  // 禁止实例化（仅通过静态方法使用）
  PasswordEncoder() = delete;
  ~PasswordEncoder() = delete;
  PasswordEncoder(const PasswordEncoder&) = delete;
  PasswordEncoder& operator=(const PasswordEncoder&) = delete;

  /**
   * @brief 对明文密码进行哈希
   * @param plaintext 明文密码
   * @param opsLimit 计算强度（操作次数）
   * @param memLimit 内存限制（字节数）
   * @return 包含算法、参数、盐值和哈希的字符串
   * @throw std::runtime_error 初始化失败或哈希过程出错
   */
  static std::string encode(
      const std::string& plaintext,
      std::uint32_t opsLimit = crypto_pwhash_OPSLIMIT_INTERACTIVE,
      std::uint32_t memLimit = crypto_pwhash_MEMLIMIT_INTERACTIVE);

  /**
   * @brief 验证明文密码与哈希值是否匹配
   * @param plaintext 明文密码
   * @param hashStr 哈希字符串（由 encode 生成）
   * @return 匹配成功返回 true，否则返回 false
   * @throw std::runtime_error 初始化失败或输入格式无效
   */
  static bool matches(const std::string& plaintext, const std::string& hashStr);
};

#endif  // PASSWORD_ENCODER_H
