// Web search skill tests
#include "signalwire/skills/skill_registry.hpp"
#include "httplib.h"
#include <atomic>
#include <chrono>
#include <thread>
namespace sw_skills = signalwire::skills;
using json = nlohmann::json;

TEST(skill_websearch_name) {
    auto skill = sw_skills::SkillRegistry::instance().create("web_search");
    ASSERT_EQ(skill->skill_name(), "web_search");
    return true;
}

TEST(skill_websearch_multi_instance) {
    auto skill = sw_skills::SkillRegistry::instance().create("web_search");
    ASSERT_TRUE(skill->supports_multiple_instances());
    return true;
}

TEST(skill_websearch_version) {
    auto skill = sw_skills::SkillRegistry::instance().create("web_search");
    ASSERT_EQ(skill->skill_version(), "2.0.0");
    return true;
}

TEST(skill_websearch_setup_with_keys) {
    auto skill = sw_skills::SkillRegistry::instance().create("web_search");
    ASSERT_TRUE(skill->setup(json::object({
        {"api_key", "gkey"}, {"search_engine_id", "seid"}
    })));
    return true;
}

TEST(skill_websearch_registers_tool) {
    auto skill = sw_skills::SkillRegistry::instance().create("web_search");
    skill->setup(json::object({{"api_key", "k"}, {"search_engine_id", "s"}}));
    auto tools = skill->register_tools();
    ASSERT_EQ(tools.size(), 1u);
    ASSERT_EQ(tools[0].name, "web_search");
    return true;
}

TEST(skill_websearch_custom_tool_name) {
    auto skill = sw_skills::SkillRegistry::instance().create("web_search");
    skill->setup(json::object({{"api_key", "k"}, {"search_engine_id", "s"}, {"tool_name", "search"}}));
    auto tools = skill->register_tools();
    ASSERT_EQ(tools[0].name, "search");
    return true;
}

// Drive the handler against a local HTTP fixture: prove that the skill
// actually issues a real GET to its upstream and parses the items[]
// response, instead of returning canned data. The fixture serves on a
// kernel-assigned ephemeral port and answers the customsearch path with
// a minimal Google CSE-shaped body.
TEST(skill_websearch_handler_works) {
    httplib::Server srv;
    std::atomic<bool> got_request{false};
    std::string captured_path;
    srv.Get("/customsearch/v1", [&](const httplib::Request& req, httplib::Response& res) {
        got_request = true;
        captured_path = req.path + (req.params.empty() ? "" : "?...");
        res.set_content(R"({"items":[{"title":"Test Result","link":"https://t/1","snippet":"hit for test query"}]})",
                        "application/json");
    });

    int port = 0;
    std::thread th([&]{ port = srv.bind_to_any_port("127.0.0.1"); srv.listen_after_bind(); });
    // Spin until bound.
    auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(3);
    while (port == 0 && std::chrono::steady_clock::now() < deadline) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    ASSERT_TRUE(port > 0);

    ::setenv("WEB_SEARCH_BASE_URL", ("http://127.0.0.1:" + std::to_string(port)).c_str(), 1);
    auto skill = sw_skills::SkillRegistry::instance().create("web_search");
    skill->setup(json::object({{"api_key", "k"}, {"search_engine_id", "s"}}));
    auto tools = skill->register_tools();
    auto result = tools[0].handler(json::object({{"query", "test query"}}), json::object());
    auto resp = result.to_json()["response"].get<std::string>();

    srv.stop();
    th.join();
    ::unsetenv("WEB_SEARCH_BASE_URL");

    ASSERT_TRUE(got_request);  // proves the skill issued real HTTP
    ASSERT_TRUE(resp.find("test query") != std::string::npos);
    ASSERT_TRUE(resp.find("Test Result") != std::string::npos);  // proves real parse
    return true;
}

TEST(skill_websearch_has_prompt_sections) {
    auto skill = sw_skills::SkillRegistry::instance().create("web_search");
    skill->setup(json::object({{"api_key", "k"}, {"search_engine_id", "s"}}));
    auto sections = skill->get_prompt_sections();
    ASSERT_TRUE(sections.size() >= 1u);
    return true;
}

TEST(skill_websearch_global_data) {
    auto skill = sw_skills::SkillRegistry::instance().create("web_search");
    skill->setup(json::object({{"api_key", "k"}, {"search_engine_id", "s"}}));
    auto gd = skill->get_global_data();
    ASSERT_TRUE(gd.contains("web_search_enabled"));
    return true;
}
