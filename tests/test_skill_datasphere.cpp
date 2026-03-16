// Datasphere skill tests
#include "signalwire/skills/skill_registry.hpp"
namespace sw_skills = signalwire::skills;
using json = nlohmann::json;

TEST(skill_datasphere_name) {
    auto skill = sw_skills::SkillRegistry::instance().create("datasphere");
    ASSERT_EQ(skill->skill_name(), "datasphere");
    return true;
}

TEST(skill_datasphere_multi_instance) {
    auto skill = sw_skills::SkillRegistry::instance().create("datasphere");
    ASSERT_TRUE(skill->supports_multiple_instances());
    return true;
}

TEST(skill_datasphere_setup_with_creds) {
    auto skill = sw_skills::SkillRegistry::instance().create("datasphere");
    ASSERT_TRUE(skill->setup(json::object({
        {"space_name", "test.signalwire.com"},
        {"project_id", "proj-123"},
        {"token", "tok-456"}
    })));
    return true;
}

TEST(skill_datasphere_registers_tool) {
    auto skill = sw_skills::SkillRegistry::instance().create("datasphere");
    skill->setup(json::object({
        {"space_name", "s"}, {"project_id", "p"}, {"token", "t"},
        {"document_id", "doc-1"}
    }));
    auto tools = skill->register_tools();
    ASSERT_EQ(tools.size(), 1u);
    ASSERT_EQ(tools[0].name, "search_knowledge");
    return true;
}

TEST(skill_datasphere_custom_tool_name) {
    auto skill = sw_skills::SkillRegistry::instance().create("datasphere");
    skill->setup(json::object({
        {"space_name", "s"}, {"project_id", "p"}, {"token", "t"},
        {"tool_name", "search_docs"}
    }));
    auto tools = skill->register_tools();
    ASSERT_EQ(tools[0].name, "search_docs");
    return true;
}

TEST(skill_datasphere_global_data_contains_enabled) {
    auto skill = sw_skills::SkillRegistry::instance().create("datasphere");
    skill->setup(json::object({{"space_name", "s"}, {"project_id", "p"}, {"token", "t"}}));
    auto gd = skill->get_global_data();
    ASSERT_TRUE(gd.contains("datasphere_enabled"));
    return true;
}

TEST(skill_datasphere_handler_returns_response) {
    auto skill = sw_skills::SkillRegistry::instance().create("datasphere");
    skill->setup(json::object({
        {"space_name", "s"}, {"project_id", "p"}, {"token", "t"},
        {"document_id", "doc-1"}
    }));
    auto tools = skill->register_tools();
    auto result = tools[0].handler(json::object({{"query", "test"}}), json::object());
    ASSERT_FALSE(result.to_json()["response"].get<std::string>().empty());
    return true;
}
