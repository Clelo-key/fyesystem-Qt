#include "regexValidate.h"

#include <iostream>

RegexValidate::RegexValidate() : m_pattern(Pattern::Custom) {
  initBuiltinPatterns();
}

void RegexValidate::initBuiltinPatterns() {
  // 手机号：1开头，第二位为3-9，共11位数字
  m_builtinPatterns[Pattern::Phone] = "^1[3-9]\\d{9}$";
  m_builtinErrorMessages[Pattern::Phone] = "请输入有效的手机号";

  // 邮箱：用户名@域名
  m_builtinPatterns[Pattern::Email] =
      "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$";
  m_builtinErrorMessages[Pattern::Email] = "请输入有效的邮箱地址";

  // 身份证：18位，最后一位可能是数字或X
  m_builtinPatterns[Pattern::IDCard] = "^(\\d{17}[0-9Xx])$";
  m_builtinErrorMessages[Pattern::IDCard] = "请输入有效的身份证号码";

  // URL：http/https开头
  m_builtinPatterns[Pattern::URL] =
      "^(https?://)?([\\da-z.-]+)\\.([a-z.]{2,6})([/\\w.-]*)*?$";
  m_builtinErrorMessages[Pattern::URL] = "请输入有效的URL";

  // IP地址：IPv4格式
  m_builtinPatterns[Pattern::IP] =
      "^(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25["
      "0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]"
      "\\d|25[0-5])$";
  m_builtinErrorMessages[Pattern::IP] = "请输入有效的IP地址";

  // 用户名 字母、数字、下划线，长度4-20
  m_builtinPatterns[Pattern::Username] = "^[a-zA-Z0-9_]{4,20}$";
  m_builtinErrorMessages[Pattern::Username] =
      "用户名必须由4-20位中文、字母、数字或下划线组成";
  m_builtinPatterns[Pattern::Username] = 
    "^[a-zA-Z0-9_]{4,20}$";

  // 密码：字母、数字、特殊字符，长度8-20
  m_builtinPatterns[Pattern::Password] =
      "^(?=.*[a-zA-Z])(?=.*\\d)(?=.*[@$!%*?&])[A-Za-z\\d@$!%*?&]{8,20}$";
  m_builtinErrorMessages[Pattern::Password] =
      "密码必须包含字母、数字和特殊字符，长度8-20位";

  // 纯数字
  m_builtinPatterns[Pattern::Number] = "^\\d+$";
  m_builtinErrorMessages[Pattern::Number] = "请输入纯数字";

  // 小数
  m_builtinPatterns[Pattern::Decimal] = "^\\d+(\\.\\d+)?$";
  m_builtinErrorMessages[Pattern::Decimal] = "请输入有效的数字";
}

RegexValidate& RegexValidate::setInput(const std::string& input) {
  m_input = input;
  m_validationError.reset();
  return *this;
}

RegexValidate& RegexValidate::setPattern(Pattern pattern) {
  m_pattern = pattern;
  return *this;
}

RegexValidate& RegexValidate::setCustomPattern(const std::string& pattern) {
  m_customPattern = pattern;
  m_pattern = Pattern::Custom;
  return *this;
}

RegexValidate& RegexValidate::setErrorMessage(const std::string& message) {
  m_errorMessage = message;
  return *this;
}

RegexValidate& RegexValidate::setCustomValidator(
    std::function<bool(const std::string&)> validator) {
  m_customValidator = validator;
  return *this;
}

bool RegexValidate::validate() {
  // 重置错误信息
  m_validationError.reset();

  try {
    // 自定义验证函数优先
    if (m_customValidator) {
      bool result = m_customValidator(m_input);
      if (!result) {
        m_validationError =
            m_errorMessage.empty() ? "自定义验证失败" : m_errorMessage;
      }
      return result;
    }

    // 获取对应的正则表达式
    std::string patternStr;
    if (m_pattern == Pattern::Custom && !m_customPattern.empty()) {
      patternStr = m_customPattern;
    } else {
      auto it = m_builtinPatterns.find(m_pattern);
      if (it != m_builtinPatterns.end()) {
        patternStr = it->second;
      } else {
        m_validationError = "未定义的验证模式";
        return false;
      }
    }

    // 执行正则匹配
    std::regex pattern(patternStr);
    bool result = std::regex_match(m_input, pattern);

    // 设置错误信息
    if (!result) {
      if (!m_errorMessage.empty()) {
        m_validationError = m_errorMessage;
      } else if (m_pattern != Pattern::Custom) {
        auto it = m_builtinErrorMessages.find(m_pattern);
        if (it != m_builtinErrorMessages.end()) {
          m_validationError = it->second;
        } else {
          m_validationError = "验证失败";
        }
      } else {
        m_validationError = "自定义验证失败";
      }
    }

    return result;
  } catch (const std::regex_error& e) {
    m_validationError = "正则表达式错误: " + std::string(e.what());
    std::cout << m_validationError.value();
    return false;
  }
}

std::optional<std::string> RegexValidate::getErrorMessage() const {
  return m_validationError;
}

// 链式调用示例
bool RegexValidate::validatePhone(const std::string& phone) {
  return setInput(phone).setPattern(Pattern::Phone).validate();
}

bool RegexValidate::validateUsername(const std::string& username) {
  return setInput(username).setPattern(Pattern::Username).validate();
}

bool RegexValidate::validateEmail(const std::string& email) {
  return setInput(email).setPattern(Pattern::Email).validate();
}

bool RegexValidate::validateIDCard(const std::string& idCard) {
  return setInput(idCard).setPattern(Pattern::IDCard).validate();
}

bool RegexValidate::validatePassword(const std::string& password) {
  return setInput(password).setPattern(Pattern::Password).validate();
}
