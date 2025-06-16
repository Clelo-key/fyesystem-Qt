#ifndef REGEX_VALIDATE_H
#define REGEX_VALIDATE_H

#include <functional>
#include <optional>
#include <regex>
#include <string>
#include <unordered_map>

class RegexValidate {
 public:
  // 内置的验证模式
  enum class Pattern {
    Phone,     // 手机号
    Email,     // 邮箱
    IDCard,    // 身份证
    URL,       // URL
    IP,        // IP地址
    Username,  // 用户名（中文、字母、数字、下划线，长度4-20）
    Password,  // 密码（字母、数字、特殊字符，长度8-20）
    Number,    // 纯数字
    Decimal,   // 小数
    Custom     // 自定义模式
  };

  RegexValidate();
  ~RegexValidate() = default;

  // 设置要验证的字符串
  RegexValidate& setInput(const std::string& input);

  // 设置验证模式
  RegexValidate& setPattern(Pattern pattern);

  // 设置自定义正则表达式
  RegexValidate& setCustomPattern(const std::string& pattern);

  // 设置自定义错误信息
  RegexValidate& setErrorMessage(const std::string& message);

  // 设置自定义验证函数
  RegexValidate& setCustomValidator(
      std::function<bool(const std::string&)> validator);

  // 执行验证
  bool validate();

  // 获取错误信息
  std::optional<std::string> getErrorMessage() const;

  // 链式调用示例
  bool validatePhone(const std::string& phone);
  bool validateEmail(const std::string& email);
  bool validateIDCard(const std::string& idCard);
  bool validatePassword(const std::string& password);
  bool validateUsername(const std::string& username);


 private:
  std::string m_input;
  Pattern m_pattern;
  std::string m_customPattern;
  std::string m_errorMessage;
  std::function<bool(const std::string&)> m_customValidator;
  std::optional<std::string> m_validationError;

  // 初始化内置模式和错误信息
  void initBuiltinPatterns();

  // 内置模式和对应正则表达式
  std::unordered_map<Pattern, std::string> m_builtinPatterns;

  // 内置错误信息
  std::unordered_map<Pattern, std::string> m_builtinErrorMessages;
};

#endif  // REGEX_VALIDATE_H
