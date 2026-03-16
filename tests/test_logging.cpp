// Logging system tests
#include "signalwire/logging.hpp"
#include "signalwire/logging/logger.hpp"

using namespace signalwire;

TEST(logging_singleton) {
    auto& l1 = Logger::instance();
    auto& l2 = Logger::instance();
    ASSERT_EQ(&l1, &l2);
    return true;
}

TEST(logging_default_level_info) {
    auto& logger = Logger::instance();
    // Level should be Info by default (unless env overrides)
    LogLevel level = logger.level();
    // Just verify it's a valid level
    ASSERT_TRUE(level >= LogLevel::Debug && level <= LogLevel::Off);
    return true;
}

TEST(logging_set_level) {
    auto& logger = Logger::instance();
    auto original = logger.level();
    logger.set_level(LogLevel::Error);
    ASSERT_EQ(logger.level(), LogLevel::Error);
    logger.set_level(original); // Restore
    return true;
}

TEST(logging_suppress_unsuppress) {
    auto& logger = Logger::instance();
    bool was_suppressed = logger.is_suppressed();

    logger.suppress();
    ASSERT_TRUE(logger.is_suppressed());

    logger.unsuppress();
    ASSERT_FALSE(logger.is_suppressed());

    // Restore original state
    if (was_suppressed) logger.suppress();
    else logger.unsuppress();
    return true;
}

TEST(logging_log_methods_no_crash) {
    auto& logger = Logger::instance();
    bool was_suppressed = logger.is_suppressed();
    logger.suppress(); // Suppress during test

    logger.debug("test debug message");
    logger.info("test info message");
    logger.warn("test warn message");
    logger.error("test error message");

    // Restore
    if (!was_suppressed) logger.unsuppress();
    else logger.suppress();
    return true;
}

TEST(logging_named_logger) {
    auto logger = logging::Logger("TestModule");
    // Should not crash; output suppressed by main() in tests
    return true;
}

TEST(logging_get_logger_function) {
    auto& logger = signalwire::get_logger();
    // Should return the same singleton
    ASSERT_EQ(&logger, &Logger::instance());
    return true;
}

TEST(logging_named_get_logger) {
    auto logger = logging::get_logger("MyComponent");
    // Should not crash
    return true;
}
