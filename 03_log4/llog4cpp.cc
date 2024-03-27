#include "log4cpp/Appender.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Category.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/PatternLayout.hh"
#include "log4cpp/Priority.hh"
#include "log4cpp/RollingFileAppender.hh"
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::string;
using namespace log4cpp;
void test0() {
  PatternLayout *ptn1 = new PatternLayout();
  ptn1->setConversionPattern("%d %c [%p] %m%n");

  PatternLayout *ptn2 = new PatternLayout();
  ptn2->setConversionPattern("%d %c [%p] %m%n");

  PatternLayout *ptn3 = new PatternLayout();
  ptn3->setConversionPattern("%d %c [%p] %m%n");

  log4cpp::Appender *appender1 =
      new log4cpp::OstreamAppender("控制台", &std::cout);
  appender1->setLayout(ptn1);

  FileAppender *appender2 = new FileAppender("文件", "wd.log");
  appender2->setLayout(ptn2);

  RollingFileAppender *appender3 =
      new RollingFileAppender("回卷文件", "回卷文件/回卷文件.log", 5 * 1024, 9);
  appender3->setLayout(ptn3);
  Category &salesDepart = Category::getInstance("salesDepart");

  salesDepart.addAppender(appender1);
  salesDepart.addAppender(appender2);
  salesDepart.addAppender(appender3);

  int cout = 100;
  while (cout-- > 0) {
    salesDepart.emerg("this is an emerg msg");
    salesDepart.fatal("this is an fatal msg");
    salesDepart.alert("this is an alert msg");
    salesDepart.crit("this is an crit msg");
    salesDepart.error("this is an error msg");
    salesDepart.warn("this is an warn msg");
    salesDepart.notice("this is an notice msg");
    salesDepart.info("this is an info msg");
  }
}

int main() {
  test0();

  return 0;
}
