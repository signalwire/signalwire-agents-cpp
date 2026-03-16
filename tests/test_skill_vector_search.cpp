// Native vector search skill tests
#include "signalwire/skills/skill_registry.hpp"
namespace sw_skills = signalwire::skills;
using json = nlohmann::json;

TEST(skill_vectorsearch_name) {
    auto skill = sw_skills::SkillRegistry::instance().create("native_vector_search");
    ASSERT_EQ(skill->skill_name(), "native_vector_search");
    return true;
}

TEST(skill_vectorsearch_multi_instance) {
    auto skill = sw_skills::SkillRegistry::instance().create("native_vector_search");
    ASSERT_TRUE(skill->supports_multiple_instances());
    return true;
}

TEST(skill_vectorsearch_setup_requires_source) {
    auto skill = sw_skills::SkillRegistry::instance().create("native_vector_search");
    ASSERT_FALSE(skill->setup(json::object()));
    return true;
}

TEST(skill_vectorsearch_setup_with_remote) {
    auto skill = sw_skills::SkillRegistry::instance().create("native_vector_search");
    ASSERT_TRUE(skill->setup(json::object({{"remote_url", "https://search.example.com"}})));
    return true;
}

TEST(skill_vectorsearch_setup_with_index_file) {
    auto skill = sw_skills::SkillRegistry::instance().create("native_vector_search");
    ASSERT_TRUE(skill->setup(json::object({{"index_file", "/path/to/index.swsearch"}})));
    return true;
}

TEST(skill_vectorsearch_registers_tool) {
    auto skill = sw_skills::SkillRegistry::instance().create("native_vector_search");
    skill->setup(json::object({{"remote_url", "https://search.example.com"}}));
    auto tools = skill->register_tools();
    ASSERT_EQ(tools.size(), 1u);
    ASSERT_EQ(tools[0].name, "search_knowledge");
    return true;
}

TEST(skill_vectorsearch_custom_tool_name) {
    auto skill = sw_skills::SkillRegistry::instance().create("native_vector_search");
    skill->setup(json::object({{"remote_url", "x"}, {"tool_name", "search_docs"}}));
    auto tools = skill->register_tools();
    ASSERT_EQ(tools[0].name, "search_docs");
    return true;
}

TEST(skill_vectorsearch_handler) {
    auto skill = sw_skills::SkillRegistry::instance().create("native_vector_search");
    skill->setup(json::object({{"remote_url", "https://search.example.com"}}));
    auto tools = skill->register_tools();
    auto result = tools[0].handler(json::object({{"query", "test"}}), json::object());
    auto resp = result.to_json()["response"].get<std::string>();
    ASSERT_TRUE(resp.find("test") != std::string::npos);
    return true;
}

TEST(skill_vectorsearch_get_hints) {
    auto skill = sw_skills::SkillRegistry::instance().create("native_vector_search");
    skill->setup(json::object({{"remote_url", "x"}}));
    auto hints = skill->get_hints();
    // May return empty or some hints depending on implementation
    // Just verify it doesn't crash and returns a vector
    (void)hints;
    return true;
}
