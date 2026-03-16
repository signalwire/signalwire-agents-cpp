// Prefab FAQBot tests
#include "signalwire/prefabs/prefabs.hpp"
using namespace signalwire::prefabs;
using json = nlohmann::json;

TEST(prefab_faqbot_default) {
    FAQBotAgent agent;
    ASSERT_EQ(agent.name(), "faq_bot");
    return true;
}

TEST(prefab_faqbot_named) {
    FAQBotAgent agent("helpdesk", "/help");
    ASSERT_EQ(agent.name(), "helpdesk");
    return true;
}

TEST(prefab_faqbot_has_personality) {
    FAQBotAgent agent;
    ASSERT_TRUE(agent.prompt_has_section("Personality"));
    return true;
}

TEST(prefab_faqbot_set_faqs) {
    FAQBotAgent agent;
    agent.set_faqs({
        json::object({
            {"question", "What are your hours?"},
            {"answer", "9-5 weekdays"},
            {"keywords", json::array({"hours", "open"})}
        }),
        json::object({
            {"question", "Where are you located?"},
            {"answer", "123 Main St"},
            {"keywords", json::array({"location", "address"})}
        })
    });
    ASSERT_TRUE(agent.has_tool("search_faq"));
    return true;
}

TEST(prefab_faqbot_set_no_match_message) {
    FAQBotAgent agent;
    agent.set_no_match_message("I don't have that info.");
    return true;
}

TEST(prefab_faqbot_set_suggest_related) {
    FAQBotAgent agent;
    agent.set_suggest_related(true);
    return true;
}

TEST(prefab_faqbot_renders_swml) {
    FAQBotAgent agent;
    json swml = agent.render_swml();
    ASSERT_TRUE(swml.contains("version"));
    return true;
}
