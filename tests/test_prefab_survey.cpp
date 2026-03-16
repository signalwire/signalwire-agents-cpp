// Prefab Survey tests
#include "signalwire/prefabs/prefabs.hpp"
using namespace signalwire::prefabs;
using json = nlohmann::json;

TEST(prefab_survey_default_construction) {
    SurveyAgent agent;
    ASSERT_EQ(agent.name(), "survey");
    return true;
}

TEST(prefab_survey_named) {
    SurveyAgent agent("csat", "/csat");
    ASSERT_EQ(agent.name(), "csat");
    return true;
}

TEST(prefab_survey_has_personality) {
    SurveyAgent agent;
    ASSERT_TRUE(agent.prompt_has_section("Personality"));
    return true;
}

TEST(prefab_survey_has_instructions) {
    SurveyAgent agent;
    ASSERT_TRUE(agent.prompt_has_section("Survey Instructions"));
    return true;
}

TEST(prefab_survey_set_questions_and_tool) {
    SurveyAgent agent;
    agent.set_questions({
        json::object({{"question", "Rate our service?"}, {"type", "rating"}}),
        json::object({{"question", "Comments?"}, {"type", "open_ended"}})
    });
    ASSERT_TRUE(agent.has_tool("submit_survey_answer"));
    return true;
}

TEST(prefab_survey_set_completion_message) {
    SurveyAgent agent;
    agent.set_completion_message("Thanks for your feedback!");
    return true;
}

TEST(prefab_survey_set_intro_message) {
    SurveyAgent agent;
    agent.set_intro_message("Welcome to our survey.");
    return true;
}

TEST(prefab_survey_renders_swml) {
    SurveyAgent agent;
    json swml = agent.render_swml();
    ASSERT_TRUE(swml.contains("version"));
    ASSERT_TRUE(swml.contains("sections"));
    return true;
}
