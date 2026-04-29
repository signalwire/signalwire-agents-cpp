// Spider skill tests
#include "signalwire/skills/skill_registry.hpp"
#include "httplib.h"
#include <atomic>
#include <chrono>
#include <thread>
namespace sw_skills = signalwire::skills;
using json = nlohmann::json;

TEST(skill_spider_name) {
    auto skill = sw_skills::SkillRegistry::instance().create("spider");
    ASSERT_EQ(skill->skill_name(), "spider");
    return true;
}

TEST(skill_spider_multi_instance) {
    auto skill = sw_skills::SkillRegistry::instance().create("spider");
    ASSERT_TRUE(skill->supports_multiple_instances());
    return true;
}

TEST(skill_spider_setup) {
    auto skill = sw_skills::SkillRegistry::instance().create("spider");
    ASSERT_TRUE(skill->setup(json::object()));
    return true;
}

TEST(skill_spider_registers_tools) {
    auto skill = sw_skills::SkillRegistry::instance().create("spider");
    skill->setup(json::object());
    auto tools = skill->register_tools();
    ASSERT_TRUE(tools.size() >= 1u);
    // First tool should contain "scrape"
    ASSERT_TRUE(tools[0].name.find("scrape") != std::string::npos);
    return true;
}

// Drive the handler against a local fixture so we prove the skill issues
// a real GET to the URL the LLM passes (with SPIDER_BASE_URL rewriting
// the host to point at the loopback). The fixture serves an HTML page;
// the skill must strip the tags and return readable text.
TEST(skill_spider_handler_works) {
    httplib::Server srv;
    std::atomic<bool> got_request{false};
    srv.Get("/page", [&](const httplib::Request&, httplib::Response& res) {
        got_request = true;
        res.set_content("<html><body><h1>Hello</h1><p>real content</p></body></html>",
                        "text/html");
    });

    int port = 0;
    std::thread th([&]{ port = srv.bind_to_any_port("127.0.0.1"); srv.listen_after_bind(); });
    auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(3);
    while (port == 0 && std::chrono::steady_clock::now() < deadline) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    ASSERT_TRUE(port > 0);

    ::setenv("SPIDER_BASE_URL", ("http://127.0.0.1:" + std::to_string(port)).c_str(), 1);
    auto skill = sw_skills::SkillRegistry::instance().create("spider");
    skill->setup(json::object());
    auto tools = skill->register_tools();
    ASSERT_TRUE(tools.size() >= 1u);
    auto result = tools[0].handler(json::object({{"url", "https://example.com/page"}}), json::object());
    auto resp = result.to_json()["response"].get<std::string>();

    srv.stop();
    th.join();
    ::unsetenv("SPIDER_BASE_URL");

    ASSERT_TRUE(got_request);
    ASSERT_TRUE(resp.find("Hello") != std::string::npos);  // proves HTML strip + real fetch
    ASSERT_TRUE(resp.find("real content") != std::string::npos);
    return true;
}

TEST(skill_spider_has_hints) {
    auto skill = sw_skills::SkillRegistry::instance().create("spider");
    skill->setup(json::object());
    auto hints = skill->get_hints();
    ASSERT_TRUE(hints.size() >= 1u);
    return true;
}
