// REST namespace completeness tests
#include "signalwire/rest/signalwire_client.hpp"
using namespace signalwire::rest;
using json = nlohmann::json;

TEST(rest_ns_datasphere) {
    SignalWireClient client("example.signalwire.com", "proj", "tok");
    auto& ds = client.datasphere();
    (void)ds.documents;
    return true;
}

TEST(rest_ns_video) {
    SignalWireClient client("example.signalwire.com", "proj", "tok");
    auto& v = client.video();
    (void)v.rooms;
    (void)v.room_sessions;
    (void)v.recordings;
    return true;
}

TEST(rest_ns_compat) {
    SignalWireClient client("example.signalwire.com", "proj", "tok");
    auto& c = client.compat();
    (void)c;
    return true;
}

TEST(rest_ns_addresses) {
    SignalWireClient client("example.signalwire.com", "proj", "tok");
    auto& a = client.addresses();
    (void)a;
    return true;
}

TEST(rest_ns_number_groups) {
    SignalWireClient client("example.signalwire.com", "proj", "tok");
    auto& ng = client.number_groups();
    (void)ng;
    return true;
}

TEST(rest_ns_verified_callers) {
    SignalWireClient client("example.signalwire.com", "proj", "tok");
    auto& vc = client.verified_callers();
    (void)vc;
    return true;
}

TEST(rest_ns_sip_profile) {
    SignalWireClient client("example.signalwire.com", "proj", "tok");
    auto& sp = client.sip_profile();
    (void)sp;
    return true;
}

TEST(rest_ns_lookup) {
    SignalWireClient client("example.signalwire.com", "proj", "tok");
    auto& l = client.lookup();
    (void)l;
    return true;
}

TEST(rest_ns_short_codes) {
    SignalWireClient client("example.signalwire.com", "proj", "tok");
    auto& sc = client.short_codes();
    (void)sc;
    return true;
}

TEST(rest_ns_imported_numbers) {
    SignalWireClient client("example.signalwire.com", "proj", "tok");
    auto& in = client.imported_numbers();
    (void)in;
    return true;
}

TEST(rest_ns_mfa) {
    SignalWireClient client("example.signalwire.com", "proj", "tok");
    auto& m = client.mfa();
    (void)m;
    return true;
}

TEST(rest_ns_registry) {
    SignalWireClient client("example.signalwire.com", "proj", "tok");
    auto& r = client.registry();
    (void)r;
    return true;
}

TEST(rest_ns_logs) {
    SignalWireClient client("example.signalwire.com", "proj", "tok");
    auto& l = client.logs();
    (void)l;
    return true;
}

TEST(rest_ns_project) {
    SignalWireClient client("example.signalwire.com", "proj", "tok");
    auto& p = client.project();
    (void)p;
    return true;
}

TEST(rest_ns_pubsub) {
    SignalWireClient client("example.signalwire.com", "proj", "tok");
    auto& ps = client.pubsub();
    (void)ps;
    return true;
}

TEST(rest_ns_chat) {
    SignalWireClient client("example.signalwire.com", "proj", "tok");
    auto& ch = client.chat();
    (void)ch;
    return true;
}

TEST(rest_ns_total_count_21) {
    SignalWireClient client("example.signalwire.com", "proj", "tok");
    // Access all 21 namespaces
    int count = 0;
    (void)client.fabric(); count++;
    (void)client.calling(); count++;
    (void)client.phone_numbers(); count++;
    (void)client.datasphere(); count++;
    (void)client.video(); count++;
    (void)client.compat(); count++;
    (void)client.addresses(); count++;
    (void)client.queues(); count++;
    (void)client.recordings(); count++;
    (void)client.number_groups(); count++;
    (void)client.verified_callers(); count++;
    (void)client.sip_profile(); count++;
    (void)client.lookup(); count++;
    (void)client.short_codes(); count++;
    (void)client.imported_numbers(); count++;
    (void)client.mfa(); count++;
    (void)client.registry(); count++;
    (void)client.logs(); count++;
    (void)client.project(); count++;
    (void)client.pubsub(); count++;
    (void)client.chat(); count++;
    ASSERT_EQ(count, 21);
    return true;
}

TEST(rest_error_class_details) {
    SignalWireRestError err(500, "Internal Server Error", "{\"detail\":\"crash\"}");
    ASSERT_EQ(err.status(), 500);
    ASSERT_EQ(err.body(), "{\"detail\":\"crash\"}");
    ASSERT_TRUE(std::string(err.what()).find("Internal Server Error") != std::string::npos);
    return true;
}

TEST(rest_crud_resource_construction) {
    HttpClient client("http://localhost:9999", "proj", "tok");
    CrudResource r(client, "/api/v1/items");
    // Construction alone should not crash
    return true;
}

TEST(rest_client_base_url_format) {
    SignalWireClient client("my-space.signalwire.com", "proj", "tok");
    ASSERT_EQ(client.http_client().base_url(), "https://my-space.signalwire.com");
    return true;
}
