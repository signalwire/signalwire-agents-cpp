// Wikipedia search skill tests
#include "signalwire/skills/skill_registry.hpp"
namespace sw_skills = signalwire::skills;
using json = nlohmann::json;

TEST(skill_wikipedia_name) {
    auto skill = sw_skills::SkillRegistry::instance().create("wikipedia_search");
    ASSERT_EQ(skill->skill_name(), "wikipedia_search");
    return true;
}

TEST(skill_wikipedia_setup_no_params) {
    auto skill = sw_skills::SkillRegistry::instance().create("wikipedia_search");
    ASSERT_TRUE(skill->setup(json::object()));
    return true;
}

TEST(skill_wikipedia_registers_tool) {
    auto skill = sw_skills::SkillRegistry::instance().create("wikipedia_search");
    skill->setup(json::object());
    auto tools = skill->register_tools();
    ASSERT_EQ(tools.size(), 1u);
    ASSERT_EQ(tools[0].name, "search_wiki");
    return true;
}

TEST(skill_wikipedia_handler_with_query) {
    auto skill = sw_skills::SkillRegistry::instance().create("wikipedia_search");
    skill->setup(json::object());
    auto tools = skill->register_tools();
    auto result = tools[0].handler(json::object({{"query", "Albert Einstein"}}), json::object());
    auto resp = result.to_json()["response"].get<std::string>();
    ASSERT_TRUE(resp.find("Albert Einstein") != std::string::npos);
    return true;
}

TEST(skill_wikipedia_empty_query_returns_response) {
    auto skill = sw_skills::SkillRegistry::instance().create("wikipedia_search");
    skill->setup(json::object());
    auto tools = skill->register_tools();
    auto result = tools[0].handler(json::object({{"query", ""}}), json::object());
    auto resp = result.to_json()["response"].get<std::string>();
    // Just verify it returns some response without crashing
    ASSERT_FALSE(resp.empty());
    return true;
}

TEST(skill_wikipedia_prompt_sections) {
    auto skill = sw_skills::SkillRegistry::instance().create("wikipedia_search");
    skill->setup(json::object());
    auto sections = skill->get_prompt_sections();
    ASSERT_TRUE(sections.size() >= 1u);
    return true;
}
