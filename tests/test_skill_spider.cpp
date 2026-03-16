// Spider skill tests
#include "signalwire/skills/skill_registry.hpp"
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

TEST(skill_spider_handler_works) {
    auto skill = sw_skills::SkillRegistry::instance().create("spider");
    skill->setup(json::object());
    auto tools = skill->register_tools();
    ASSERT_TRUE(tools.size() >= 1u);
    auto result = tools[0].handler(json::object({{"url", "https://example.com"}}), json::object());
    auto resp = result.to_json()["response"].get<std::string>();
    ASSERT_FALSE(resp.empty());
    return true;
}

TEST(skill_spider_has_hints) {
    auto skill = sw_skills::SkillRegistry::instance().create("spider");
    skill->setup(json::object());
    auto hints = skill->get_hints();
    ASSERT_TRUE(hints.size() >= 1u);
    return true;
}
