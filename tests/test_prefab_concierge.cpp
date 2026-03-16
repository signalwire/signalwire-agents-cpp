// Prefab Concierge tests
#include "signalwire/prefabs/prefabs.hpp"
using namespace signalwire::prefabs;
using json = nlohmann::json;

TEST(prefab_concierge_default) {
    ConciergeAgent agent;
    ASSERT_EQ(agent.name(), "concierge");
    return true;
}

TEST(prefab_concierge_named) {
    ConciergeAgent agent("hotel", "/hotel");
    ASSERT_EQ(agent.name(), "hotel");
    return true;
}

TEST(prefab_concierge_has_personality) {
    ConciergeAgent agent;
    ASSERT_TRUE(agent.prompt_has_section("Personality"));
    return true;
}

TEST(prefab_concierge_set_venue_name) {
    ConciergeAgent agent;
    agent.set_venue_name("The Grand Hotel");
    // Prompt should update
    return true;
}

TEST(prefab_concierge_set_amenities_full) {
    ConciergeAgent agent;
    agent.set_amenities({
        json::object({{"name", "Pool"}, {"location", "Floor 2"}, {"available", true}}),
        json::object({{"name", "Gym"}, {"location", "Floor 1"}, {"available", true}}),
        json::object({{"name", "Spa"}, {"location", "Floor 3"}, {"available", false}})
    });
    ASSERT_TRUE(agent.has_tool("check_amenity"));
    ASSERT_TRUE(agent.prompt_has_section("Available Amenities"));
    return true;
}

TEST(prefab_concierge_set_hours_section) {
    ConciergeAgent agent;
    agent.set_hours(json::object({
        {"Monday", "9:00 AM - 9:00 PM"},
        {"Tuesday", "9:00 AM - 9:00 PM"},
        {"Wednesday", "9:00 AM - 9:00 PM"}
    }));
    ASSERT_TRUE(agent.prompt_has_section("Venue Hours"));
    return true;
}

TEST(prefab_concierge_full_config) {
    ConciergeAgent agent;
    agent.set_venue_name("Resort");
    agent.set_amenities({
        json::object({{"name", "Beach"}, {"location", "Ground"}, {"available", true}})
    });
    agent.set_hours(json::object({{"Daily", "8 AM - 10 PM"}}));
    json swml = agent.render_swml();
    ASSERT_TRUE(swml.contains("version"));
    auto& main = swml["sections"]["main"];
    bool has_ai = false;
    for (const auto& v : main) {
        if (v.contains("ai")) has_ai = true;
    }
    ASSERT_TRUE(has_ai);
    return true;
}
