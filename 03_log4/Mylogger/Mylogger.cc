#include <iostream>
#include <log4cpp/Category.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <sstream>

#define LOG_INFO(msg)                                                          \
  Mylogger::getInstance()->info(msg, __FILE__, __FUNCTION__, __LINE__)
#define LOG_WARN(msg)                                                          \
  Mylogger::getInstance()->warn(msg, __FILE__, __FUNCTION__, __LINE__)
#define LOG_ERROR(msg)                                                         \
  Mylogger::getInstance()->error(msg, __FILE__, __FUNCTION__, __LINE__)
#define LOG_DEBUG(msg)                                                         \
  Mylogger::getInstance()->debug(msg, __FILE__, __FUNCTION__, __LINE__)

class Mylogger {
public:
  static Mylogger *getInstance() {
    if (instance == nullptr) {
      instance = new Mylogger();
    }
    return instance;
  }

  static void destroy() {
    if (instance != nullptr) {
      delete instance;
      instance = nullptr;
    }
  }

  void warn(const char *msg, const char *file, const char *function, int line) {
    std::ostringstream oss;
    oss << file << ":" << function << ":" << line << " - " << msg;
    category.warn(oss.str());
  }

  void error(const char *msg, const char *file, const char *function,
             int line) {
    std::ostringstream oss;
    oss << file << ":" << function << ":" << line << " - " << msg;
    category.error(oss.str());
  }

  void debug(const char *msg, const char *file, const char *function,
             int line) {
    std::ostringstream oss;
    oss << file << ":" << function << ":" << line << " - " << msg;
    category.debug(oss.str());
  }

  void info(const char *msg, const char *file, const char *function, int line) {
    std::ostringstream oss;
    oss << file << ":" << function << ":" << line << " - " << msg;
    category.info(oss.str());
  }

private:
  Mylogger() : category(log4cpp::Category::getRoot().getInstance("Mylogger")) {
    log4cpp::OstreamAppender *osAppender =
        new log4cpp::OstreamAppender("osAppender", &std::cout);
    log4cpp::PatternLayout *pLayout = new log4cpp::PatternLayout();
    pLayout->setConversionPattern("%d:[%p] %c %x: %m%n");
    osAppender->setLayout(pLayout);
    category.setAppender(osAppender);
    category.setPriority(log4cpp::Priority::DEBUG);
  }

  ~Mylogger() { log4cpp::Category::shutdown(); }

  Mylogger(const Mylogger &) = delete;
  Mylogger &operator=(const Mylogger &) = delete;

  static Mylogger *instance;
  log4cpp::Category &category;
};

Mylogger *Mylogger::instance = nullptr;

void test0() {
  LOG_INFO("This is an info message");
  LOG_WARN("This is a warning message");
  LOG_ERROR("This is an error message");
  LOG_DEBUG("This is a debug message");
}

int main() {
  test0();
  Mylogger::destroy();
  return 0;
}
