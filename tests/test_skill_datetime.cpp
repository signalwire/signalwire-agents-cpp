// DateTime skill tests
#include "signalwire/skills/skill_registry.hpp"
namespace sw_skills = signalwire::skills;
using json = nlohmann::json;

TEST(skill_datetime_name) {
    auto& reg = sw_skills::SkillRegistry::instance();
    auto skill = reg.create("datetime");
    ASSERT_TRUE(skill != nullptr);
    ASSERT_EQ(skill->skill_name(), "datetime");
    return true;
}

TEST(skill_datetime_version) {
    auto skill = sw_skills::SkillRegistry::instance().create("datetime");
    ASSERT_EQ(skill->skill_version(), "1.0.0");
    return true;
}

TEST(skill_datetime_no_multi_instance) {
    auto skill = sw_skills::SkillRegistry::instance().create("datetime");
    ASSERT_FALSE(skill->supports_multiple_instances());
    return true;
}

TEST(skill_datetime_setup_ok) {
    auto skill = sw_skills::SkillRegistry::instance().create("datetime");
    ASSERT_TRUE(skill->setup(json::object()));
    return true;
}

TEST(skill_datetime_registers_two_tools) {
    auto skill = sw_skills::SkillRegistry::instance().create("datetime");
    skill->setup(json::object());
    auto tools = skill->register_tools();
    ASSERT_EQ(tools.size(), 2u);
    ASSERT_EQ(tools[0].name, "get_current_time");
    ASSERT_EQ(tools[1].name, "get_current_date");
    return true;
}

TEST(skill_datetime_tool_handlers_work) {
    auto skill = sw_skills::SkillRegistry::instance().create("datetime");
    skill->setup(json::object());
    auto tools = skill->register_tools();
    auto time_result = tools[0].handler(json::object(), json::object());
    auto date_result = tools[1].handler(json::object(), json::object());
    // Both should return non-empty responses
    ASSERT_FALSE(time_result.to_json()["response"].get<std::string>().empty());
    ASSERT_FALSE(date_result.to_json()["response"].get<std::string>().empty());
    return true;
}

TEST(skill_datetime_has_prompt_sections) {
    auto skill = sw_skills::SkillRegistry::instance().create("datetime");
    skill->setup(json::object());
    auto sections = skill->get_prompt_sections();
    ASSERT_EQ(sections.size(), 1u);
    ASSERT_EQ(sections[0].title, "Date and Time Information");
    return true;
}

TEST(skill_datetime_no_env_vars_required) {
    auto skill = sw_skills::SkillRegistry::instance().create("datetime");
    ASSERT_TRUE(skill->required_env_vars().empty());
    return true;
}
