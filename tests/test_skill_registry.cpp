// Skill registry tests — enumeration, creation, validation
#include "signalwire/skills/skill_registry.hpp"
#include "signalwire/skills/skill_manager.hpp"
#include "signalwire/agent/agent_base.hpp"

namespace sw_skills = signalwire::skills;
using json = nlohmann::json;

TEST(skill_registry_singleton) {
    auto& r1 = sw_skills::SkillRegistry::instance();
    auto& r2 = sw_skills::SkillRegistry::instance();
    ASSERT_EQ(&r1, &r2);
    return true;
}

TEST(skill_registry_all_18_skills_present) {
    auto& reg = sw_skills::SkillRegistry::instance();
    auto all = reg.list_skills();
    ASSERT_TRUE(all.size() >= 18u);

    // Check each expected skill
    std::vector<std::string> expected = {
        "datetime", "math", "joke", "weather_api", "web_search",
        "wikipedia_search", "google_maps", "spider", "datasphere",
        "datasphere_serverless", "swml_transfer", "play_background_file",
        "api_ninjas_trivia", "native_vector_search", "info_gatherer",
        "claude_skills", "mcp_gateway", "custom_skills"
    };
    for (const auto& name : expected) {
        ASSERT_TRUE(reg.has_skill(name));
    }
    return true;
}

TEST(skill_registry_create_returns_null_for_unknown) {
    auto& reg = sw_skills::SkillRegistry::instance();
    auto skill = reg.create("totally_fake_skill");
    ASSERT_TRUE(skill == nullptr);
    return true;
}

TEST(skill_registry_create_returns_unique_instances) {
    auto& reg = sw_skills::SkillRegistry::instance();
    auto s1 = reg.create("datetime");
    auto s2 = reg.create("datetime");
    ASSERT_TRUE(s1 != nullptr);
    ASSERT_TRUE(s2 != nullptr);
    ASSERT_TRUE(s1.get() != s2.get());
    return true;
}

// ========================================================================
// SkillManager tests
// ========================================================================

TEST(skill_manager_load_and_check) {
    sw_skills::SkillManager mgr;
    signalwire::agent::AgentBase agent;
    bool ok = mgr.load_skill("datetime", json::object(), agent);
    ASSERT_TRUE(ok);
    ASSERT_TRUE(mgr.is_loaded("datetime"));
    return true;
}

TEST(skill_manager_unload_skill) {
    sw_skills::SkillManager mgr;
    signalwire::agent::AgentBase agent;
    mgr.load_skill("math", json::object(), agent);
    mgr.unload_skill("math");
    ASSERT_FALSE(mgr.is_loaded("math"));
    return true;
}

TEST(skill_manager_list_loaded_skills) {
    sw_skills::SkillManager mgr;
    signalwire::agent::AgentBase agent;
    mgr.load_skill("datetime", json::object(), agent);
    mgr.load_skill("math", json::object(), agent);
    auto loaded = mgr.list_loaded();
    ASSERT_EQ(loaded.size(), 2u);
    return true;
}

TEST(skill_manager_cleanup_all) {
    sw_skills::SkillManager mgr;
    signalwire::agent::AgentBase agent;
    mgr.load_skill("datetime", json::object(), agent);
    mgr.load_skill("math", json::object(), agent);
    mgr.cleanup_all();
    ASSERT_FALSE(mgr.is_loaded("datetime"));
    ASSERT_FALSE(mgr.is_loaded("math"));
    return true;
}

TEST(skill_manager_unknown_skill_returns_false) {
    sw_skills::SkillManager mgr;
    signalwire::agent::AgentBase agent;
    ASSERT_FALSE(mgr.load_skill("nonexistent_skill", json::object(), agent));
    return true;
}

TEST(skill_manager_duplicate_single_instance_rejected) {
    sw_skills::SkillManager mgr;
    signalwire::agent::AgentBase agent;
    mgr.load_skill("datetime", json::object(), agent);
    ASSERT_FALSE(mgr.load_skill("datetime", json::object(), agent));
    return true;
}
