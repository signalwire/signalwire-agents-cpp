// Datasphere serverless skill tests
#include "signalwire/skills/skill_registry.hpp"
namespace sw_skills = signalwire::skills;
using json = nlohmann::json;

TEST(skill_ds_serverless_name) {
    auto skill = sw_skills::SkillRegistry::instance().create("datasphere_serverless");
    ASSERT_EQ(skill->skill_name(), "datasphere_serverless");
    return true;
}

TEST(skill_ds_serverless_multi_instance) {
    auto skill = sw_skills::SkillRegistry::instance().create("datasphere_serverless");
    ASSERT_TRUE(skill->supports_multiple_instances());
    return true;
}

TEST(skill_ds_serverless_setup) {
    auto skill = sw_skills::SkillRegistry::instance().create("datasphere_serverless");
    ASSERT_TRUE(skill->setup(json::object({
        {"space_name", "test.signalwire.com"},
        {"project_id", "proj-123"},
        {"token", "tok-456"}
    })));
    return true;
}

TEST(skill_ds_serverless_no_webhook_tools) {
    auto skill = sw_skills::SkillRegistry::instance().create("datasphere_serverless");
    skill->setup(json::object({{"space_name", "s"}, {"project_id", "p"}, {"token", "t"}}));
    ASSERT_EQ(skill->register_tools().size(), 0u);
    return true;
}

TEST(skill_ds_serverless_returns_datamap) {
    auto skill = sw_skills::SkillRegistry::instance().create("datasphere_serverless");
    skill->setup(json::object({
        {"space_name", "test.signalwire.com"},
        {"project_id", "proj-123"},
        {"token", "tok-456"},
        {"document_id", "doc-1"}
    }));
    auto dm = skill->get_datamap_functions();
    ASSERT_EQ(dm.size(), 1u);
    ASSERT_EQ(dm[0]["function"].get<std::string>(), "search_knowledge");
    ASSERT_TRUE(dm[0].contains("data_map"));
    return true;
}

TEST(skill_ds_serverless_datamap_has_webhook) {
    auto skill = sw_skills::SkillRegistry::instance().create("datasphere_serverless");
    skill->setup(json::object({
        {"space_name", "test.signalwire.com"},
        {"project_id", "proj-123"},
        {"token", "tok-456"}
    }));
    auto dm = skill->get_datamap_functions();
    ASSERT_TRUE(dm[0]["data_map"].contains("webhooks"));
    auto url = dm[0]["data_map"]["webhooks"][0]["url"].get<std::string>();
    ASSERT_TRUE(url.find("test.signalwire.com") != std::string::npos);
    return true;
}

TEST(skill_ds_serverless_global_data) {
    auto skill = sw_skills::SkillRegistry::instance().create("datasphere_serverless");
    skill->setup(json::object({{"space_name", "s"}, {"project_id", "p"}, {"token", "t"}}));
    auto gd = skill->get_global_data();
    // get_global_data() may return empty or populated depending on impl
    ASSERT_TRUE(gd.is_object());
    return true;
}
