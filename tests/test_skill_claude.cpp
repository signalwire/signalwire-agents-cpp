// Claude skills skill tests
#include "signalwire/skills/skill_registry.hpp"
namespace sw_skills = signalwire::skills;
using json = nlohmann::json;

TEST(skill_claude_name) {
    auto skill = sw_skills::SkillRegistry::instance().create("claude_skills");
    ASSERT_EQ(skill->skill_name(), "claude_skills");
    return true;
}

TEST(skill_claude_multi_instance) {
    auto skill = sw_skills::SkillRegistry::instance().create("claude_skills");
    ASSERT_TRUE(skill->supports_multiple_instances());
    return true;
}

TEST(skill_claude_setup_requires_path) {
    auto skill = sw_skills::SkillRegistry::instance().create("claude_skills");
    ASSERT_FALSE(skill->setup(json::object()));
    return true;
}

TEST(skill_claude_setup_with_path) {
    auto skill = sw_skills::SkillRegistry::instance().create("claude_skills");
    ASSERT_TRUE(skill->setup(json::object({{"skills_path", "/tmp/skills"}})));
    return true;
}

TEST(skill_claude_registers_tool) {
    auto skill = sw_skills::SkillRegistry::instance().create("claude_skills");
    skill->setup(json::object({{"skills_path", "/tmp/skills"}}));
    auto tools = skill->register_tools();
    ASSERT_EQ(tools.size(), 1u);
    // Tool name includes the prefix
    ASSERT_TRUE(tools[0].name.find("skill") != std::string::npos);
    return true;
}

TEST(skill_claude_handler) {
    auto skill = sw_skills::SkillRegistry::instance().create("claude_skills");
    skill->setup(json::object({{"skills_path", "/tmp"}}));
    auto tools = skill->register_tools();
    auto result = tools[0].handler(json::object({{"arguments", "test args"}}), json::object());
    auto resp = result.to_json()["response"].get<std::string>();
    ASSERT_TRUE(resp.find("test args") != std::string::npos);
    return true;
}

TEST(skill_claude_default_prefix) {
    auto skill = sw_skills::SkillRegistry::instance().create("claude_skills");
    skill->setup(json::object({{"skills_path", "/tmp"}}));
    auto tools = skill->register_tools();
    ASSERT_EQ(tools[0].name, "claude_skill");
    return true;
}
