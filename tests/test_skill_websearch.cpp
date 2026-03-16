// Web search skill tests
#include "signalwire/skills/skill_registry.hpp"
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

TEST(skill_websearch_handler_works) {
    auto skill = sw_skills::SkillRegistry::instance().create("web_search");
    skill->setup(json::object({{"api_key", "k"}, {"search_engine_id", "s"}}));
    auto tools = skill->register_tools();
    auto result = tools[0].handler(json::object({{"query", "test query"}}), json::object());
    auto resp = result.to_json()["response"].get<std::string>();
    ASSERT_TRUE(resp.find("test query") != std::string::npos);
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
