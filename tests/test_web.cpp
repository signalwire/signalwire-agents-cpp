// Web mixin tests — webhook URLs, proxy detection, query params, dynamic config

#include "signalwire/agent/agent_base.hpp"

using namespace signalwire::agent;
using json = nlohmann::json;

// ========================================================================
// Proxy URL
// ========================================================================

TEST(web_manual_proxy_url) {
    AgentBase agent;
    agent.manual_set_proxy_url("https://proxy.example.com");
    agent.set_auth("u", "p");
    agent.define_tool("test_tool", "Test", json::object(),
        [](const json&, const json&) { return signalwire::swaig::FunctionResult("ok"); });

    json swml = agent.render_swml();
    auto& main = swml["sections"]["main"];
    for (const auto& verb : main) {
        if (verb.contains("ai") && verb["ai"].contains("SWAIG")) {
            auto url = verb["ai"]["SWAIG"]["functions"][0]["web_hook_url"].get<std::string>();
            ASSERT_TRUE(url.find("proxy.example.com") != std::string::npos);
            return true;
        }
    }
    ASSERT_TRUE(false);
    return true;
}

TEST(web_webhook_url_override) {
    AgentBase agent;
    agent.set_webhook_url("https://custom.webhook.com/swaig");
    agent.set_auth("u", "p");
    agent.define_tool("test_tool", "Test", json::object(), nullptr);

    json swml = agent.render_swml();
    auto& main = swml["sections"]["main"];
    for (const auto& verb : main) {
        if (verb.contains("ai") && verb["ai"].contains("SWAIG")) {
            auto url = verb["ai"]["SWAIG"]["functions"][0]["web_hook_url"].get<std::string>();
            ASSERT_EQ(url, "https://custom.webhook.com/swaig");
            return true;
        }
    }
    ASSERT_TRUE(false);
    return true;
}

// ========================================================================
// SWAIG query params
// ========================================================================

TEST(web_swaig_query_params) {
    AgentBase agent;
    agent.set_auth("u", "p");
    agent.add_swaig_query_param("tenant", "acme");
    agent.add_swaig_query_param("mode", "test");
    agent.define_tool("test_tool", "Test", json::object(), nullptr);

    json swml = agent.render_swml();
    auto& main = swml["sections"]["main"];
    for (const auto& verb : main) {
        if (verb.contains("ai") && verb["ai"].contains("SWAIG")) {
            auto url = verb["ai"]["SWAIG"]["functions"][0]["web_hook_url"].get<std::string>();
            ASSERT_TRUE(url.find("tenant=acme") != std::string::npos);
            ASSERT_TRUE(url.find("mode=test") != std::string::npos);
            return true;
        }
    }
    ASSERT_TRUE(false);
    return true;
}

TEST(web_clear_swaig_query_params) {
    AgentBase agent;
    agent.set_auth("u", "p");
    agent.add_swaig_query_param("key", "val");
    agent.clear_swaig_query_params();
    agent.define_tool("test_tool", "Test", json::object(), nullptr);

    json swml = agent.render_swml();
    auto& main = swml["sections"]["main"];
    for (const auto& verb : main) {
        if (verb.contains("ai") && verb["ai"].contains("SWAIG")) {
            auto url = verb["ai"]["SWAIG"]["functions"][0]["web_hook_url"].get<std::string>();
            ASSERT_TRUE(url.find("key=val") == std::string::npos);
            return true;
        }
    }
    ASSERT_TRUE(false);
    return true;
}

// ========================================================================
// Dynamic config
// ========================================================================

TEST(web_dynamic_config_modifies_copy) {
    AgentBase agent;
    agent.set_auth("u", "p");
    agent.set_prompt_text("Original prompt");

    agent.set_dynamic_config_callback(
        [](const std::map<std::string, std::string>& qp,
           const json&,
           const std::map<std::string, std::string>&,
           AgentBase& copy) {
            auto it = qp.find("tenant");
            if (it != qp.end()) {
                copy.set_prompt_text("Tenant: " + it->second);
            }
        });

    std::map<std::string, std::string> qp = {{"tenant", "acme"}};
    json swml = agent.render_swml_for_request(qp, json::object(), {});

    // Original agent should not change
    ASSERT_EQ(agent.get_prompt(), "Original prompt");

    // Rendered SWML should have the modified prompt
    auto& main = swml["sections"]["main"];
    for (const auto& verb : main) {
        if (verb.contains("ai") && verb["ai"].contains("prompt")) {
            auto text = verb["ai"]["prompt"]["text"].get<std::string>();
            ASSERT_EQ(text, "Tenant: acme");
            return true;
        }
    }
    ASSERT_TRUE(false);
    return true;
}

TEST(web_dynamic_config_without_callback) {
    AgentBase agent;
    agent.set_prompt_text("Static");
    json swml = agent.render_swml_for_request({}, json::object(), {});
    auto& main = swml["sections"]["main"];
    for (const auto& verb : main) {
        if (verb.contains("ai") && verb["ai"].contains("prompt")) {
            ASSERT_EQ(verb["ai"]["prompt"]["text"].get<std::string>(), "Static");
            return true;
        }
    }
    ASSERT_TRUE(false);
    return true;
}

// ========================================================================
// Proxy detection from headers
// ========================================================================

TEST(web_proxy_from_forwarded_headers) {
    AgentBase agent;
    agent.set_auth("u", "p");
    agent.define_tool("test_tool", "Test", json::object(), nullptr);

    std::map<std::string, std::string> headers = {
        {"x-forwarded-proto", "https"},
        {"x-forwarded-host", "myapp.example.com"}
    };
    json swml = agent.render_swml_for_request({}, json::object(), headers);

    auto& main = swml["sections"]["main"];
    for (const auto& verb : main) {
        if (verb.contains("ai") && verb["ai"].contains("SWAIG")) {
            auto url = verb["ai"]["SWAIG"]["functions"][0]["web_hook_url"].get<std::string>();
            ASSERT_TRUE(url.find("myapp.example.com") != std::string::npos);
            ASSERT_TRUE(url.find("https://") != std::string::npos);
            return true;
        }
    }
    ASSERT_TRUE(false);
    return true;
}

// ========================================================================
// Post-prompt URL
// ========================================================================

TEST(web_post_prompt_url_auto_generated) {
    AgentBase agent;
    agent.set_auth("u", "p");
    json swml = agent.render_swml();
    auto& main = swml["sections"]["main"];
    for (const auto& verb : main) {
        if (verb.contains("ai") && verb["ai"].contains("post_prompt_url")) {
            auto url = verb["ai"]["post_prompt_url"].get<std::string>();
            ASSERT_TRUE(url.find("/post_prompt") != std::string::npos);
            return true;
        }
    }
    ASSERT_TRUE(false);
    return true;
}

TEST(web_post_prompt_url_direct_override) {
    AgentBase agent;
    agent.set_post_prompt_url_direct("https://custom.com/post_prompt");
    json swml = agent.render_swml();
    auto& main = swml["sections"]["main"];
    for (const auto& verb : main) {
        if (verb.contains("ai") && verb["ai"].contains("post_prompt_url")) {
            ASSERT_EQ(verb["ai"]["post_prompt_url"].get<std::string>(),
                       "https://custom.com/post_prompt");
            return true;
        }
    }
    ASSERT_TRUE(false);
    return true;
}

// ========================================================================
// Debug routes
// ========================================================================

TEST(web_enable_debug_routes) {
    AgentBase agent;
    agent.enable_debug_routes(true);
    // Just verify it doesn't crash; actual route testing requires server
    return true;
}

// ========================================================================
// SIP routing
// ========================================================================

TEST(web_sip_routing_enable) {
    AgentBase agent;
    agent.enable_sip_routing(true);
    // No crash
    return true;
}

TEST(web_sip_register_valid_username) {
    AgentBase agent;
    agent.enable_sip_routing(true);
    agent.register_sip_username("alice");
    agent.register_sip_username("bob_123");
    // No crash; valid usernames accepted
    return true;
}

TEST(web_auto_map_sip_usernames) {
    AgentBase agent;
    agent.auto_map_sip_usernames(true);
    // No crash
    return true;
}
