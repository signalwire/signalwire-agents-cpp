// MCP gateway skill tests
#include "signalwire/skills/skill_registry.hpp"
namespace sw_skills = signalwire::skills;
using json = nlohmann::json;

TEST(skill_mcp_name) {
    auto skill = sw_skills::SkillRegistry::instance().create("mcp_gateway");
    ASSERT_EQ(skill->skill_name(), "mcp_gateway");
    return true;
}

TEST(skill_mcp_setup_requires_url) {
    auto skill = sw_skills::SkillRegistry::instance().create("mcp_gateway");
    ASSERT_FALSE(skill->setup(json::object()));
    return true;
}

TEST(skill_mcp_setup_with_url) {
    auto skill = sw_skills::SkillRegistry::instance().create("mcp_gateway");
    ASSERT_TRUE(skill->setup(json::object({{"gateway_url", "https://mcp.example.com"}})));
    return true;
}

TEST(skill_mcp_registers_tools) {
    auto skill = sw_skills::SkillRegistry::instance().create("mcp_gateway");
    skill->setup(json::object({{"gateway_url", "https://mcp.example.com"}}));
    auto tools = skill->register_tools();
    // May or may not have tools depending on implementation
    // Just verify it doesn't crash
    (void)tools;
    return true;
}

TEST(skill_mcp_with_services) {
    auto skill = sw_skills::SkillRegistry::instance().create("mcp_gateway");
    skill->setup(json::object({
        {"gateway_url", "https://mcp.example.com"},
        {"services", json::array({
            json::object({{"name", "calendar"}}),
            json::object({{"name", "email"}})
        })}
    }));
    auto tools = skill->register_tools();
    // Tools should exist
    ASSERT_TRUE(tools.size() >= 1u);
    return true;
}

TEST(skill_mcp_tool_handler_works) {
    auto skill = sw_skills::SkillRegistry::instance().create("mcp_gateway");
    skill->setup(json::object({
        {"gateway_url", "https://mcp.example.com"},
        {"services", json::array({json::object({{"name", "svc1"}})})}
    }));
    auto tools = skill->register_tools();
    ASSERT_TRUE(tools.size() >= 1u);
    auto result = tools[0].handler(json::object({{"query", "test"}}), json::object());
    auto resp = result.to_json()["response"].get<std::string>();
    ASSERT_FALSE(resp.empty());
    return true;
}

TEST(skill_mcp_has_hints) {
    auto skill = sw_skills::SkillRegistry::instance().create("mcp_gateway");
    skill->setup(json::object({
        {"gateway_url", "x"},
        {"services", json::array({json::object({{"name", "search"}})})}
    }));
    auto hints = skill->get_hints();
    ASSERT_TRUE(hints.size() >= 1u);
    bool has_mcp = false;
    for (const auto& h : hints) {
        if (h == "MCP" || h == "gateway") has_mcp = true;
    }
    ASSERT_TRUE(has_mcp);
    return true;
}

TEST(skill_mcp_global_data_is_object) {
    auto skill = sw_skills::SkillRegistry::instance().create("mcp_gateway");
    skill->setup(json::object({
        {"gateway_url", "https://mcp.example.com"},
        {"services", json::array({json::object({{"name", "svc1"}})})}
    }));
    auto gd = skill->get_global_data();
    ASSERT_TRUE(gd.is_object());
    return true;
}
